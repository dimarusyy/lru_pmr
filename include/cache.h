#include <memory_resource> 
#include <list>
#include <unordered_map>
#include <iostream>
#include <optional>

struct cache_allocator_t : std::pmr::memory_resource
{
    explicit cache_allocator_t(size_t sz)
        : _max_size(sz)
    {
    }

protected:
    virtual bool can_allocate(size_t bytes) const
    {
        return _max_size - _alloc_size >= bytes;
    }

private:
    void* do_allocate(size_t bytes, size_t alignment) override
    {
        if (!can_allocate(bytes))
            throw std::runtime_error("allocation failed");

        _alloc_size += bytes;
        return std::pmr::new_delete_resource()->allocate(bytes, alignment);
    }

    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override
    {
        _alloc_size -= bytes;
        std::pmr::new_delete_resource()->deallocate(ptr, bytes, alignment);
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return std::pmr::new_delete_resource()->is_equal(other);
    }

    size_t _max_size;
    size_t _alloc_size{ 0 };

};

template <typename Key, typename Value>
class cache_t
{
    struct aux_t
    {
        explicit aux_t(const Key& key,
                       const Value& value,
                       std::pmr::memory_resource* resource)
            : _key(key)
            , _value(value.begin(), value.end(), resource)
        {
        }

        Key _key;
        std::pmr::vector<char> _value;
    };

public:
    using iterator_t = typename std::list<aux_t>::iterator;

    explicit cache_t(size_t sz)
        : _allocator(sz)
        , _pool(&_allocator)
    {
        std::cout << "initialized with [" << sz << "] bytes\n";
    }

    void add(const Key& key,
             const Value& value)
    {
        auto it = _lru_asoc.find(key);

        if (it == _lru_asoc.end()) [[unlikely]]
        {
            // try to allocate if we can
repeat:
            try
            {
                aux_t entry(key, value, &_pool);
                _data.push_front(std::move(entry));
                _lru_asoc.insert({ key, _data.begin() });
            }
            catch (const std::exception& ex)
            {
                std::cerr << "exception: [" << ex.what() << "]\n";
                if (_data.empty())
                    throw;

                //erase the last element
                auto& last = _data.back();
                _lru_asoc.erase(last._key);
                _data.pop_back();

                goto repeat;
            }
        }
        else [[likely]]
        {
            // update order by putting to 1-st position
            _data.splice(_data.begin(), _data, it->second);

              // update lru
            it->second = _data.begin();
        }
    }

    std::optional<Value> get(const Key& key)
    {
        auto it = _lru_asoc.find(key);
        if (it == _lru_asoc.end()) [[unlikely]]
        {
            return std::nullopt;
        }

            // reorder
        _data.splice(_data.begin(), _data, it->second);
        _lru_asoc[key] = _data.begin();

        auto& rc = _data.front();
        return std::make_optional(Value(rc._value.begin(), rc._value.end()));
    }

    auto size() const
    {
        return _data.size();
    }

private:
    cache_allocator_t _allocator;
    std::pmr::unsynchronized_pool_resource _pool;

    std::list<aux_t> _data;
    std::unordered_map<Key, iterator_t> _lru_asoc;
};
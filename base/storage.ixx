module;
export module my.base:Storage;

import :Type;

export namespace myt::base {

class Storage {
public:
    explicit Storage(size_type size)
        : size_(size)
    {
        data_ = operator new(size_);
    }

    ~Storage() { operator delete(data_); }

    Storage(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage& operator=(Storage&&) = delete;

    size_type size() const { return size_; }

    template <typename T> T* data() { return static_cast<T*>(data_); }

private:
    void* data_ {};
    size_type size_ {};
};

} // namespace myt::base

#ifndef SHARED_VAR_H
#define SHARED_VAR_H

#include "v0/shared_void_inline.h"

/**
 * @brief Typed template wrapper around SharedVoid.
 *
 * @note Incompatible conversions are either compile-time errors where possible,
 *       or run-time errors where necessary.
 *
 * @note Since this is a template class, all methods are defined in the header file.
 */
template <typename T>
class SharedVar
{
private:
    SharedVoid _v;

public:
    SharedVar()
        : _v()
    {
        static_assert(std::is_default_constructible<T>::value, "T must be default constructible");
        _v.create<T>();
    }

    ~SharedVar() = default;
    SharedVar(const SharedVar&) = default;
    SharedVar(SharedVar&&) = default;
    SharedVar& operator=(const SharedVar&) = default;
    SharedVar& operator=(SharedVar&&) = default;

    explicit SharedVar(const SharedVoid& sv)
        : _v()
    {
        this->operator=(sv);
    }

    template <
        typename... Args, 
        typename = typename std::enable_if<std::is_constructible<T, Args...>::value, void>::type>
    explicit SharedVar(Args&&... args)
        : _v()
    {
        static_assert(std::is_constructible<T, Args...>::value, "T must be constructible from Args");
        _v.create<T>(std::forward<Args>(args)...);
    }

    T& operator*() const
    {
        return *static_cast<T*>(_v.get());
    }

    T* operator&() const
    {
        return static_cast<T*>(_v.get());
    }

    T* operator->() const
    {
        return static_cast<T*>(_v.get());
    }

    std::shared_ptr<T> to_shared_ptr() const
    {
        return _v.to_shared_ptr<T>();
    }

    std::shared_ptr<void> to_shared_ptr_void() const
    {
        return _v.to_shared_ptr_void();
    }

    SharedVoid to_shared_void() const
    {
        return _v;
    }

    SharedVar& operator= (const SharedVoid& sv)
    {
        if (!sv.has_type<T>())
        {
            throw std::invalid_argument("SharedVar cannot be assigned a SharedVoid of different type or a null pointer");
        }
        _v = sv;
        return *this;
    }

    SharedVar& operator= (const std::shared_ptr<T>& ptr)
    {
        if (!ptr)
        {
            throw std::invalid_argument("SharedVar cannot be assigned a null pointer");
        }
        _v = ptr;
        return *this;
    }

    template <typename U>
    SharedVar& operator= (const std::shared_ptr<U>& ptr)
    {
        static_assert(std::is_trivially_assignable<T&, U>::value, "T must be trivially assignable from U");
        if (!ptr)
        {
            throw std::invalid_argument("SharedVar cannot be assigned a null pointer");
        }
        /**
         * @note must go through static_pointer_cast twice; first directly 
         *       from U to T (here), then T to void (inside SharedVoid).
         */
        _v = std::static_pointer_cast<T>(ptr);
        return *this;
    }

    SharedVar& operator= (const T& value)
    {
        static_assert(std::is_trivially_assignable<T&, T>::value, "T must be trivially assignable");
        this->operator*() = value;
        return *this;
    }
};

#endif // SHARED_VAR_H

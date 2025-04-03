#ifndef SHARED_VOID_INLINE_H
#define SHARED_VOID_INLINE_H

#include "v0/data/shared_void.h"

template<typename T>
SharedVoid::SharedVoid(const std::shared_ptr<T>& ptr)
    : _ptr()
    , _type()
{
    _ptr = std::static_pointer_cast<void>(ptr);
    _type = _ptr ? typeid(T) : typeid(void);
}

template <typename T>
SharedVoid& SharedVoid::operator=(const std::shared_ptr<T>& ptr)
{
    _ptr = std::static_pointer_cast<void>(ptr);
    _type = _ptr ? typeid(T) : typeid(void);
    return *this;
}

template <typename T>
bool SharedVoid::has_type() const
{
    return has_type(typeid(T));
}

template <typename T>
std::shared_ptr<T> SharedVoid::to_shared_ptr() const
{
    return has_type<T>() ? std::static_pointer_cast<T>(_ptr) : nullptr;
}

template <typename T, typename... Args>
auto SharedVoid::create(Args&&... args)
    -> typename std::enable_if<std::is_constructible<T, Args...>::value, void>::type
{
    reset();
    _ptr = std::make_shared<T>(std::forward<Args>(args)...);
    _type = typeid(T);
}

#endif // SHARED_VOID_INLINE_H

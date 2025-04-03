// include guard for source file is only needed for single-translation-unit build mode.
#ifndef SHARED_VOID_CPP
#define SHARED_VOID_CPP

#include "v0/shared_void.h"

SharedVoid::SharedVoid()
    : _ptr()
    , _type(typeid(void))
{
}

SharedVoid::SharedVoid(const SharedVoid& src)
    : _ptr(src._ptr)
    , _type(src._type)
{
}

SharedVoid::SharedVoid(SharedVoid&& src)
    : _ptr(std::move(src._ptr))
    , _type(src._type)
{
}

SharedVoid& SharedVoid::operator=(const SharedVoid& src)
{
    if (this != &src)
    {
        _ptr = src._ptr;
        _type = src._type;
    }
    return *this;
}

SharedVoid& SharedVoid::operator=(SharedVoid&& src)
{
    if (this != &src)
    {
        _ptr = std::move(src._ptr);
        _type = src._type;
    }
    return *this;
}

SharedVoid::~SharedVoid()
{
}

void SharedVoid::reset()
{
    _ptr.reset();
    _type = typeid(void);
}

std::type_index SharedVoid::to_type_index() const
{
    return _type;
}

bool SharedVoid::has_type(const std::type_index& type) const
{
    return type == ((_ptr) ? _type : typeid(void));
}

std::shared_ptr<void> SharedVoid::to_shared_ptr_void() const
{
    return _ptr;
}

void* SharedVoid::get() const
{
    return _ptr.get();
}

SharedVoid::operator bool () const
{
    return (bool)_ptr;
}

bool SharedVoid::operator==(const SharedVoid& other) const
{
    return (this == &other) || (_type == other._type) && (_ptr == other._ptr);
}

bool SharedVoid::operator!=(const SharedVoid& other) const
{
    return !(*this == other);
}

size_t SharedVoid::hash() const
{
    if (!_ptr)
    {
        return 0;
    }
    return _type.hash_code() ^ std::hash<void*>{}(_ptr.get());
}

std::string SharedVoid::to_string() const
{
    if (!_ptr)
    {
        return "SharedVoid(nullptr)";
    }
    char buf[48u] = {};
    std::string s_typename{_type.name()};
    snprintf(buf, sizeof(buf), "%p", _ptr.get());
    std::string s_ptr{buf};
    size_t h = this->hash();
    std::string s_hash = std::to_string(h);
    return "SharedVoid(" + s_typename + " @ " + s_ptr + ", hash=" + s_hash + ")";
}

#endif // SHARED_VOID_CPP

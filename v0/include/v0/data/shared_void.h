#ifndef SHARED_VOID_H
#define SHARED_VOID_H

#include <memory>
#include <typeindex>
#include <string> // for debugging

/**
 * @brief A type-erased shared_ptr and an associated type_index.
 * 
 * @note Convenience methods, such as operator->, operator*, etc. are 
 *       not implemented by this class because these would require T
 *       to be known.
 * 
 * @note Instances of this class is not thread-safe. If an instance
 *       need to be used between classes, either: (1) create a copy
 *       in a thread-safe manner, or (2) use a mutex to protect
 *       access to the instance.
 * 
 * @note All template methods are implemented in "shared_void_inline.h"
 */
class SharedVoid
{
public:
    std::shared_ptr<void> _ptr;
    std::type_index _type;

    SharedVoid();
    SharedVoid(const SharedVoid& src);
    SharedVoid(SharedVoid&& src);
    SharedVoid& operator=(const SharedVoid& src);
    SharedVoid& operator=(SharedVoid&& src);
    ~SharedVoid();

    template<typename T> SharedVoid(const std::shared_ptr<T>& ptr);

    template <typename T> SharedVoid& operator=(const std::shared_ptr<T>& ptr);

    void reset();

    std::type_index to_type_index() const;
    bool has_type(const std::type_index& type) const;
    template <typename T> bool has_type() const;

    template <typename T> std::shared_ptr<T> to_shared_ptr() const;
    std::shared_ptr<void> to_shared_ptr_void() const;

    void* get() const;

    explicit operator bool () const;

    template <typename T, typename... Args>
    auto create(Args&&... args) -> typename std::enable_if<
        std::is_constructible<T, Args...>::value, void>::type;

    bool operator==(const SharedVoid& other) const;
    bool operator!=(const SharedVoid& other) const;
    size_t hash() const;
    std::string to_string() const;
};

#endif // SHARED_VOID_H

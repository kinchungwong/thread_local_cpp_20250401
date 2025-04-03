#ifndef TLS_THREAD_DATA_H
#define TLS_THREAD_DATA_H

#include <stdexcept>
#include <mutex>
#include <unordered_map>
#include "v0/shared_void.h"
#include "v0/shared_var.h"

/**
 * @brief Thread-local storage for shared data, for use by a single thread.
 * 
 * @note All template methods are implemented in "tls_thread_data_inline.h"
 */
class TlsThreadData
{
private:
    using MutexType = std::mutex;
    using LockType = std::unique_lock<MutexType>;
    mutable MutexType _mutex;
    std::unordered_map<size_t, SharedVoid> _data;

public:
    TlsThreadData();
    ~TlsThreadData();

    SharedVoid get(size_t data_key) const;
    void set(size_t data_key, SharedVoid value);

    template <typename T>
    SharedVar<T> get(size_t data_key) const;

    template <typename T>
    void set(size_t data_key, const SharedVar<T>& value);

    template <typename T>
    void set(size_t data_key, const std::shared_ptr<T>& value);

    template <typename T>
    void set(size_t data_key, const T& value);
};

#endif // TLS_THREAD_DATA_H

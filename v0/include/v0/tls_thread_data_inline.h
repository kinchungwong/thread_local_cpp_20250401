#ifndef TLS_THREAD_DATA_INLINE_H
#define TLS_THREAD_DATA_INLINE_H

#include "v0/tls_thread_data.h"

template <typename T>
SharedVar<T> TlsThreadData::get(size_t data_key) const
{
    auto sv = this->get(data_key);
    if (!sv.has_type<T>())
    {
        throw std::invalid_argument("TlsThreadData::get(): type mismatch");
    }
    return SharedVar<T>(sv);
}

template <typename T>
void TlsThreadData::set(size_t data_key, const SharedVar<T>& value)
{
    auto sv = value.to_shared_void();
    this->set(data_key, sv);
}

template <typename T>
void TlsThreadData::set(size_t data_key, const std::shared_ptr<T>& value)
{
    auto sv = SharedVoid(value);
    this->set(data_key, sv);
}

template <typename T>
void TlsThreadData::set(size_t data_key, const T& value)
{
    SharedVoid sv;
    sv.create<T>(value);
    this->set(data_key, sv);
}

#endif // TLS_THREAD_DATA_INLINE_H

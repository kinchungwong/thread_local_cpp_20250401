// include guard for source file is only needed for single-translation-unit build mode.
#ifndef TLS_THREAD_DATA_CPP
#define TLS_THREAD_DATA_CPP

#include "v0/tls_thread_data.h"

TlsThreadData::TlsThreadData()
    : _mutex()
    , _data()
{
}

TlsThreadData::~TlsThreadData()
{
    _data.clear();
}

SharedVoid TlsThreadData::get(size_t data_key) const
{
    LockType lock(_mutex);
    auto it = _data.find(data_key);
    if (it != _data.end())
    {
        return it->second;
    }
    return SharedVoid();
}

void TlsThreadData::set(size_t data_key, SharedVoid value)
{
    LockType lock(_mutex);
    auto it = _data.find(data_key);
    if (it != _data.end())
    {
        it->second = std::move(value);
    }
    else
    {
        _data[data_key] = std::move(value);
    }
}

#endif // TLS_THREAD_DATA_CPP

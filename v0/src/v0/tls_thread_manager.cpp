// include guard for source file is only needed for single-translation-unit build mode.
#ifndef TLS_THREAD_MANAGER_CPP
#define TLS_THREAD_MANAGER_CPP

#include "v0/tls_thread_manager.h"

TlsThreadManager::TlsThreadManager() = default;
TlsThreadManager::~TlsThreadManager() = default;

std::shared_ptr<TlsThreadData> TlsThreadManager::get_thread_data(const std::thread::id& thread_id)
{
    LockType lock(_mutex);
    std::shared_ptr<TlsThreadData> result;
    auto it = _thread_data.find(thread_id);
    if (it != _thread_data.end())
    {
        auto& swp = it->second;
        result = swp.first;
        if (result)
        {
            return result;
        }
        result = swp.second.lock();
        if (result)
        {
            swp.first = result;
            return result;
        }
        result = std::make_shared<TlsThreadData>();
        swp.first = result;
        return result;
    }
    else
    {
        result = std::make_shared<TlsThreadData>();
        _thread_data[thread_id] = { result, WeakType{}};
        return result;
    }
}

std::shared_ptr<TlsThreadData> TlsThreadManager::get_current_thread_data()
{
    return this->get_thread_data(std::this_thread::get_id());
}

void TlsThreadManager::weaken_thread_data(const std::thread::id& thread_id,
    TlsWeakenType wt)
{
    if (wt == TlsWeakenType::Keep)
    {
        return;
    }
    LockType lock(_mutex);
    auto it = _thread_data.find(thread_id);
    if (it != _thread_data.end())
    {
        this->_detail_weaken(it->second, wt);
    }
}

void TlsThreadManager::weaken_all_thread_data(TlsWeakenType wt)
{
    if (wt == TlsWeakenType::Keep)
    {
        return;
    }
    LockType lock(_mutex);
    for (auto& key_value : _thread_data)
    {
        this->_detail_weaken(key_value.second, wt);
    }
}

void TlsThreadManager::_detail_weaken(StrongWeakPair& swp, TlsWeakenType wt)
{
    /**
     * @note Caller must hold the mutex lock.
     */
    if (wt == TlsWeakenType::RemoveStrong)
    {
        if (swp.first)
        {
            swp.second = swp.first;
            swp.first.reset();
        }
    }
    else if (wt == TlsWeakenType::RemoveBoth)
    {
        swp.first.reset();
        swp.second.reset();
    }
}

#endif // TLS_THREAD_MANAGER_CPP

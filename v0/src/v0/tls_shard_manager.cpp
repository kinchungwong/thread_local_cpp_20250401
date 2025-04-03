// include guard for source file is only needed for single-translation-unit build mode.
#ifndef TLS_SHARD_MANAGER_CPP
#define TLS_SHARD_MANAGER_CPP

#include "v0/tls_shard_manager.h"
#include "v0/tls_thread_data.h"
#include "v0/tls_thread_manager.h"

TlsShardManager::TlsShardManager()
    : _mutex()
    , _shards()
{
    _shards.resize(_const_shard_count);
    for (size_t i = 0; i < _const_shard_count; ++i)
    {
        _shards[i] = std::make_shared<TlsThreadManager>();
    }
}

TlsShardManager::~TlsShardManager()
{
    // Explicit cleanup step to improve debugging experience.
    for (auto& shard : _shards)
    {
        shard->weaken_all_thread_data(TlsWeakenType::RemoveBoth);
    }
}

std::shared_ptr<TlsThreadData> TlsShardManager::get_thread_data(const std::thread::id& thread_id)
{
    size_t shard_index = get_shard_index(thread_id);
    if (shard_index >= _shards.size())
    {
        throw std::out_of_range("Shard index out of range");
    }
    return _shards[shard_index]->get_thread_data(thread_id);
}

std::shared_ptr<TlsThreadData> TlsShardManager::get_current_thread_data()
{
    return this->get_thread_data(std::this_thread::get_id());
}

void TlsShardManager::weaken_thread_data(const std::thread::id& thread_id, TlsWeakenType wt)
{
    if (wt == TlsWeakenType::Keep)
    {
        return;
    }
    size_t shard_index = get_shard_index(thread_id);
    if (shard_index >= _shards.size())
    {
        throw std::out_of_range("Shard index out of range");
    }
    _shards[shard_index]->weaken_thread_data(thread_id, wt);
}

void TlsShardManager::weaken_all_thread_data(TlsWeakenType wt)
{
    if (wt == TlsWeakenType::Keep)
    {
        return;
    }
    for (size_t i = 0; i < _shards.size(); ++i)
    {
        _shards[i]->weaken_all_thread_data(wt);
    }
}

size_t TlsShardManager::get_shard_index(const std::thread::id& thread_id)
{
    std::hash<std::thread::id> hasher;
    size_t hash_value = hasher(thread_id);
    return hash_value % _const_shard_count;
}

#endif // TLS_SHARD_MANAGER_CPP

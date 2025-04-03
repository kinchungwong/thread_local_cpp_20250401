#ifndef TLS_SHARD_MANAGER_H
#define TLS_SHARD_MANAGER_H

#include <vector>
#include <memory>
#include <mutex>
#include <thread>

class TlsThreadData;
class TlsThreadManager;

class TlsShardManager
{
private:
    using MutexType = std::mutex;
    using LockType = std::unique_lock<MutexType>;
    using ManagerType = std::shared_ptr<TlsThreadManager>;
    static constexpr size_t _const_shard_count = 16;

private:
    mutable MutexType _mutex;
    std::vector<ManagerType> _shards;

public:
    TlsShardManager();
    ~TlsShardManager();

    std::shared_ptr<TlsThreadData> get_thread_data(const std::thread::id& thread_id);
    std::shared_ptr<TlsThreadData> get_current_thread_data();

    void weaken_thread_data(const std::thread::id& thread_id, TlsWeakenType wt);
    void weaken_all_thread_data(TlsWeakenType wt);
    
private:
    static size_t get_shard_index(const std::thread::id& thread_id);
};

#endif // TLS_SHARD_MANAGER_H

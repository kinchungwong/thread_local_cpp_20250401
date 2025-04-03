#ifndef TLS_THREAD_MANAGER_H
#define TLS_THREAD_MANAGER_H

#include <unordered_map>
#include <mutex>
#include <thread>
#include "v0/tls_thread_data.h"

enum class TlsWeakenType
{
    /**
     * @brief Not making any changes.
     */
    Keep = 0,

    /**
     * @brief Convert strong reference to weak reference.
     * @note By converting to weak reference, if the thread data is still in use,
     *       it can potentially be brought back to life.
     */
    RemoveStrong = 1,

    /**
     * @brief Remove both strong and weak references.
     * @note By removing all references, the thread data is detached from this
     *       manager, and future calls will not be able to retrieve it.
     *       However, the thread that is still using the thread data will continue
     *       to have access to it.
     *       Any code that asks this manager for the thread data will instead get
     *       a new instance.
     */
    RemoveBoth = 2
};

/**
 * @brief Thread-local storage manager.
 * 
 * @details This class manages thread-local storage (TLS) for any number of threads.
 *          Typically, each thread gets its own instance of TlsThreadData.
 * 
 * @details A special feature of this class is that, if necessary, the 
 *          thread-specific instances managed by this class can be "weakened",
 *          which allows disused instances to be cleaned up, while still-in-use
 *          instances remain accessible.
 */
class TlsThreadManager
{
private:
    using MutexType = std::mutex;
    using LockType = std::unique_lock<MutexType>;
    using StrongType = std::shared_ptr<TlsThreadData>;
    using WeakType = std::weak_ptr<TlsThreadData>;
    using KeyType = std::thread::id;
    using StrongWeakPair = std::pair<StrongType, WeakType>;

private:
    mutable MutexType _mutex;
    std::unordered_map<KeyType, StrongWeakPair> _thread_data;

public:
    TlsThreadManager();
    ~TlsThreadManager();

    std::shared_ptr<TlsThreadData> get_thread_data(const std::thread::id& thread_id);
    std::shared_ptr<TlsThreadData> get_current_thread_data();

    void weaken_thread_data(const std::thread::id& thread_id, TlsWeakenType wt);
    void weaken_all_thread_data(TlsWeakenType wt);

private:
    static void _detail_weaken(StrongWeakPair& swp, TlsWeakenType wt);
};


#endif // TLS_THREAD_MANAGER_H

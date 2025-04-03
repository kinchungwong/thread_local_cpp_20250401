#ifndef V0_THREADING_BALK_MUTEX_H
#define V0_THREADING_BALK_MUTEX_H

#include <cstdint>
#include <atomic>
#include <stdexcept>

/**
 * @brief A Balk Pattern mutex. Locking does not block: it either succeeds
 *        immediately, or fails immediately with an exception.
 *
 * @details This class is used for low-overhead protection of a resource
 *          that is normally only used by the same thread that created it.
 *          In other words, it is used for scenarios where any attempts to
 *          lock twice should be considered an error in code logic.
 *
 * @details This class does not allow recursive locking.
 *
 * @exception std::logic_error
 *            Thrown when a thread attempts to lock the mutex for a
 *            second time.
 */
class BalkMutex
{
private:
    std::atomic<uint32_t> _state;

public:
    BalkMutex();
    ~BalkMutex();
    void lock();
    void unlock();

private:
    BalkMutex(const BalkMutex&) = delete;
    BalkMutex(BalkMutex&&) = delete;
    BalkMutex& operator=(const BalkMutex&) = delete;
    BalkMutex& operator=(BalkMutex&&) = delete;
};

#endif // V0_THREADING_BALK_MUTEX_H

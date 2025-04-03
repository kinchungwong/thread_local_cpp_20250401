// include guard for source file is only needed for single-translation-unit build mode.
#ifndef V0_THREADING_BALK_MUTEX_CPP
#define V0_THREADING_BALK_MUTEX_CPP

#include "v0/threading/balk_mutex.h"

BalkMutex::BalkMutex()
    : _state(0)
{
}

BalkMutex::~BalkMutex()
{
}

void BalkMutex::lock()
{
    uint32_t expect_or_actual = 0u;
    if (!_state.compare_exchange_strong(expect_or_actual, 1u))
    {
        throw std::logic_error("BalkMutex: Attempt to lock a mutex that is already locked.");
    }
}

void BalkMutex::unlock()
{
    uint32_t expect_or_actual = 1u;
    if (!_state.compare_exchange_strong(expect_or_actual, 0u))
    {
        throw std::logic_error("BalkMutex: Attempt to unlock a mutex that is not locked.");
    }
}

#endif // V0_THREADING_BALK_MUTEX_CPP

#include "base.h"

namespace CJKit
{
    static IdleTask *__xdelay_idleTask = nullptr;

    IdleTask *getXdelayIdleTask()
    {
        return __xdelay_idleTask;
    }

    IdleTask *clearXdelayIdleTask()
    {
        return setXdelayIdleTask(nullptr);
    }

    IdleTask *setXdelayIdleTask(IdleTask *task)
    {
        IdleTask *old = __xdelay_idleTask;
        __xdelay_idleTask = task;
        return old;
    }

    void xdelay(unsigned long duration)
    {
        if (__xdelay_idleTask == nullptr || duration < XDELAY_MIN_DELAY_IDLE_TASK_MS)
        {
            return delay(duration);
        }

        unsigned long startTime = millis();
        unsigned long elapsed = 0;
        while (elapsed < duration)
        {
            __xdelay_idleTask();

            elapsed = millis() - startTime;

            if (elapsed >= duration)
                break;

            unsigned long rem = duration - elapsed;
            if (rem < XDELAY_MAX_INTERMEDIATE_DELAY_MS)
            {
                delay(rem);
            }
            else
            {
                delay(XDELAY_MAX_INTERMEDIATE_DELAY_MS);
            }

            elapsed = millis() - startTime;
        }
    }
}

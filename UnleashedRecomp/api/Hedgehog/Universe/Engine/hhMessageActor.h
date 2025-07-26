#pragma once

#include "SWA.inl"
#include "Hedgehog/Universe/Engine/hhMessageProcess.h"
#include "Hedgehog/Universe/Thread/hhParallelJob.h"

namespace Hedgehog::Universe
{
    class CMessageActor : public IMessageProcess, public IParallelJob
    {
    public:
        SWA_INSERT_PADDING(0x88);
        CMessageActor()
        {
            GuestToHostFunction<void*>(sub_82E61F28, this);
        }
    };
}

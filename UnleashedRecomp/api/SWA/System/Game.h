#pragma once

namespace SWA
{
    class CGame : public Hedgehog::Universe::TStateMachine<SWA::CGame>
    {
    public:
        SWA_INSERT_PADDING(0x90);
    };
}

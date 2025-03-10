#pragma once

#include <SWA.inl>
#include <Hedgehog/Base/hhObject.h>
#include <Hedgehog/Universe/Engine/hhMessageProcess.h>
#include <Hedgehog/Universe/Engine/hhStateMachineMessageReceiver.h>

namespace Hedgehog::Universe
{
        
    

    class CStateMachineBase : public IStateMachineMessageReceiver, public Base::CObject
    {
    public:
        class CStateBase : public IMessageProcess
        {
        public:
            SWA_INSERT_PADDING(0x08);
            xpointer<void> m_pContext;
            xpointer<CStateMachineBase> m_pStateMachine;
            be<float> m_Time;
            SWA_INSERT_PADDING(0x4C);

            void* GetContextBase() const;

            template<typename T>
            T* GetContextBase() const;

        };
        boost::shared_ptr<CStateBase> ChangeState(Base::CSharedString in_Name, const int in_Priority = 0, const float in_Time = 0, const bool in_Flag = false);

        SWA_INSERT_PADDING(0x60);
    };


}

#include <Hedgehog/Universe/Engine/hhStateMachineBase.inl>

#include "hhStateMachineBase.h"
namespace Hedgehog::Universe
{
    inline void* CStateMachineBase::CStateBase::GetContextBase() const
    {
        return m_pContext;
    }

    inline boost::shared_ptr<CStateMachineBase::CStateBase>* CStateMachineBase::ChangeState(Base::CSharedString in_Name, const int in_Priority, const float in_Time, const bool in_Flag)
    {
        //NOT TESTED!!
        boost::shared_ptr<CStateMachineBase::CStateBase> spState;

        guest_stack_var<boost::shared_ptr<CStateMachineBase::CStateBase>> arg1;
        guest_stack_var<Hedgehog::Base::CSharedString> arg2(in_Name);
        GuestToHostFunction<void>(sub_82E67958, arg1.get(), this, arg2.get(), in_Priority, in_Time, in_Flag);
        return arg1.get();
    }

    template<typename T>
    inline T* CStateMachineBase::CStateBase::GetContextBase() const
    {
        return (T*)m_pContext.get();
    }
}

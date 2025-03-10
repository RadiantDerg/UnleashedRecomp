#include "hhStateMachineBase.h"
namespace Hedgehog::Universe
{
    inline void* CStateMachineBase::CStateBase::GetContextBase() const
    {
        return m_pContext;
    }

    inline boost::shared_ptr<CStateMachineBase::CStateBase> CStateMachineBase::ChangeState(Base::CSharedString in_Name, const int in_Priority, const float in_Time, const bool in_Flag)
    {
        //NOT TESTED!!
        boost::shared_ptr<CStateMachineBase::CStateBase> spState;

        guest_stack_var<CStateMachineBase*> arg0(this);
        guest_stack_var<boost::shared_ptr<CStateMachineBase::CStateBase>> arg1;
        guest_stack_var<Hedgehog::Base::CSharedString> arg2(std::move(in_Name));
        guest_stack_var<int> arg3(in_Priority);
        guest_stack_var<float> arg4(in_Time);
        guest_stack_var<bool> arg5(in_Flag);
        GuestToHostFunction<void>(0x82E67958, arg0.get(), arg1.get(), arg2.get(), arg3.get(), arg4.get(), arg5.get());
        return spState;
    }

    template<typename T>
    inline T* CStateMachineBase::CStateBase::GetContextBase() const
    {
        return (T*)m_pContext.get();
    }
}

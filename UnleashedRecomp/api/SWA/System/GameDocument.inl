namespace SWA
{
    // TODO: Hedgehog::Base::TSynchronizedPtr<CGameDocument>
    inline CGameDocument* CGameDocument::GetInstance()
    {
        return *(xpointer<CGameDocument>*)MmGetHostAddress(0x83367900);
    }
    inline void CGameDocument::AddGameObject(const guest_stack_var<boost::shared_ptr<CGameObject>>& in_spGameObject, const guest_stack_var<Hedgehog::Base::CSharedString>& in_WorldName)
    {
        //for (auto& world : m_pMember->m_Worlds)
        //{
        //    //ignore this, just doing this since csharedstring has no == operator
        //    if (std::string(world.first.c_str()) == std::string(in_WorldName.get()->c_str()))
        //    {
        //        GuestToHostFunction<void>(sub_82510D20, this, world.second.get(), in_spGameObject.get(), 0);
        //        return;
        //    }
        //}
        guest_stack_var<Hedgehog::Base::CSharedString> worldName("main");
        guest_stack_var<Hedgehog::Base::THolder<SWA::CWorld>, false> worldHolder;
        GuestToHostFunction<void>(sub_8250F238, worldHolder.get(), this, worldName.get()); // get holder
        GuestToHostFunction<void>(sub_82510D20, this, worldHolder.get(), in_spGameObject.get(), 0); // add game object
    }
}

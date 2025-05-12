namespace SWA
{
    class CSetObjectEntry;
    class CSetObjectMakeEntry : public Hedgehog::Base::CObject
    {
    public:
        xpointer<std::pair<Hedgehog::Base::CSharedString, be<uint32_t>>> m_pMapIt;
        be<uint32_t> m_pFunc;
        bool m_Field08;

        void SpawnObject(const guest_stack_var<boost::shared_ptr<CSetObjectEntry>>& in_Var)
        {
            GuestToHostFunction<void>(m_pFunc, in_Var.get());
        }
    };
}

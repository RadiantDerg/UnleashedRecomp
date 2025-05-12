namespace SWA
{
    class CEditParam;
    class CSetObjectListener
    {
    public:
        struct Vftable
        {
            be<uint32_t> m_fpDtor;
            be<uint32_t> m_fpInitializeEditParam;
        };
        xpointer<Vftable> m_pVftable;

        void InitializeEditParam(const guest_stack_var<SWA::CEditParam>& in_rEditParam)
        {
            bool isvtablenull = m_pVftable == nullptr;
            GuestToHostFunction<void>(m_pVftable->m_fpInitializeEditParam.get(), this, in_rEditParam.get());
        }
    };
}

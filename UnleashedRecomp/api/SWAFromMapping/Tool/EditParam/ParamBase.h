namespace SWA
{
    class CParamBase : public Hedgehog::Base::CObject
    {
    public:
        xpointer<void> m_vfTable;
        Hedgehog::Base::CSharedString m_Name;
        xpointer<void> m_pVariable;
    };
    SWA_ASSERT_OFFSETOF(CParamBase, m_Name, 0x4);
}

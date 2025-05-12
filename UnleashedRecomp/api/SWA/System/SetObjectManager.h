namespace SWA
{
    class CSetObjectFactory;
    class CSetObjectManager : public Hedgehog::Base::CSynchronizedObject, public CGameObject
    {
    public:
        struct CMember
        {
            boost::shared_ptr <CSetObjectFactory> m_spFactory;
        };
        SWA_INSERT_PADDING(0x4);
        xpointer<CMember> m_pMember;
    };

    SWA_ASSERT_OFFSETOF(CSetObjectManager, m_pMember, 0xC4);
}

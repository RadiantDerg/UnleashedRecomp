namespace SWA
{
    class CAbstractParameter
    {
    public:
        xpointer<void> m_vfTable;
        boost::shared_ptr<SWA::CEditParam> m_pEditParam;
        Hedgehog::Base::CSharedString m_Name;
        Hedgehog::Base::CSharedString m_DisplayName;
        hh::vector<boost::shared_ptr<CAbstractParameter>> m_Children;
        SWA_INSERT_PADDING(0x14);
    };

    class CAbstractParameterNode : public CAbstractParameter
    {
    public:
        SWA_INSERT_PADDING(0x8);
    };

    class CParameterFile : public CAbstractParameter
    {

    };
    class CParameterCategory2 : public CAbstractParameterNode
    {
    public:
        struct Test
        {
            Hedgehog::Base::CSharedString test;
            bool test2;
        };
    public:
        hh::map <Hedgehog::Base::CSharedString, be<uint32_t >> m_ParamNameMap;
        int dword4c;
        int dword50;
        int dword54;
        int dword58;
        int dword5C;
    };
    SWA_ASSERT_OFFSETOF(CParameterCategory2, m_ParamNameMap, 0x40);
    SWA_ASSERT_OFFSETOF(CParameterCategory2, dword50, 0x50);
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_pEditParam, 0x4);
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_Name, 0xC);
    SWA_ASSERT_OFFSETOF(CAbstractParameter, m_Children, 0x14);
}

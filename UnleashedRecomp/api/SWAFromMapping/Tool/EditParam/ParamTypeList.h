namespace SWA
{
    class CParamBase;
    class CParamTypeList : public CParamBase
    {
    public:
        class CMember
        {
        public:
            hh::map<be<uint32_t>, Hedgehog::Base::CSharedString> m_ValueMap;
            be<int> m_FieldC;
            SWA_INSERT_PADDING(0x20); //boost func
            SWA_INSERT_PADDING(0x20); //boost func2
            xpointer<be<int>> m_Field50;
            be<int> m_Field54;
            be<int> m_Field58;
            Hedgehog::Base::CSharedString m_Field5C;
            be<int> m_Field60;
            be<int> m_IsTextType;
            Hedgehog::Base::CSharedString m_ValueText;
            be<bool> m_Field6C;
        };
        void ApplyValue()
        {
            GuestToHostFunction<void>(sub_825B11A8, this);
        }
        CMember* GetMember()
        {
            return (CMember*)m_pVariable.get();
        }
    };
    SWA_ASSERT_OFFSETOF(CParamTypeList::CMember, m_Field50, 0x50);
    SWA_ASSERT_OFFSETOF(CParamTypeList::CMember, m_Field5C, 0x5c);
    SWA_ASSERT_OFFSETOF(CParamTypeList::CMember, m_ValueText, 0x68);
}

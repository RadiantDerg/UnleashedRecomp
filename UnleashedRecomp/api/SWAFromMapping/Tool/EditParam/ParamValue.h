namespace SWA
{
    class CParamBase;
    template<typename T>
    class CParamValue : public CParamBase
    {
    public:
        class FuncData
        {
        public:
            xpointer<T> ptr;
            T m_Field04;
            T m_Field08;
            T m_Maximum;
            T m_Minimum;
            T m_Float14;
        };
        SWA_INSERT_PADDING(0x4);
        FuncData m_pFuncData;
        SWA_INSERT_PADDING(0x10);
        Hedgehog::Base::CSharedString m_Field30;
    };
    SWA_ASSERT_OFFSETOF(CParamValue<int>, m_pFuncData, 0x10);
    SWA_ASSERT_OFFSETOF(CParamValue<int>, m_Field30, 0x38);
}

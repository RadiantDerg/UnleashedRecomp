namespace SWA
{
    class CParamBase;
    class CParamBool : public CParamBase
    {
    public:
        be<bool> m_FieldIdk;
        be<bool> m_FieldIdk2;
        SWA_INSERT_PADDING(0x20);
        Hedgehog::Base::CSharedString m_Field30;
        bool* GetParamBool()
        {
            return (bool*)m_pVariable.get();
        };
    };
}

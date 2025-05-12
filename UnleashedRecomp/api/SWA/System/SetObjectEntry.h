namespace SWA
{
    class CSetObjectListener;
    struct SSetObjectInfo
    {
        boost::shared_ptr<CGameObject> spGameObject;
        boost::shared_ptr<CSetObjectListener> spSetObjectListener;
        float Field10;
        bool Field14;
        alignas(0x10) Hedgehog::Math::CQuaternion Field20;
        size_t Field30;
        size_t Field34;
        bool Field38;
        float Field3C;
    };
    class CSetObjectEntry
    {
    public:
        boost::shared_ptr<SSetObjectInfo> m_spSetObjectInfo;
        Hedgehog::Base::CSharedString m_Name;
    };
}

#pragma once
namespace SWA::Player
{
    class CPlayerContext
    {
    public:
        xpointer<void> __vftable;
        SWA_INSERT_PADDING(12);
        boost::shared_ptr<CMatrixNodeTransform> m_spMatrixNode;
        
    };

    SWA_ASSERT_OFFSETOF(CPlayerContext, m_spMatrixNode, 0x10);
    class CPlayerSpeedContext : public CPlayerContext
    {
    public:

        static CPlayerSpeedContext* GetInstance()
        {
            return *(xpointer<CPlayerSpeedContext>*)MmGetHostAddress(0x83362F98);
        }

    };
}

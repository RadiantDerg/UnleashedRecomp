#pragma once

#include "SWA.inl"
#include "SWA/Player/Character/EvilSonic/Hud/EvilHudGuide.h"
#include "SWA/Player/Character/PlayerSpeedContext.h"

namespace SWA::Player
{
    class CEvilSonicContext : public CPlayerContext
    {
    public:
        SWA_INSERT_PADDING(0x2c4-0x10-0x8);
        boost::shared_ptr<Hedgehog::Mirage::CMatrixNode> m_spMatrixNode2;
        SWA_INSERT_PADDING(0x3BC);
        be<float> m_DarkGaiaEnergy;
        SWA_INSERT_PADDING(0xC0-8);
        boost::shared_ptr<SWA::CMatrixNodeTransform> m_spMatrixNodeTransform2;
        SWA_INSERT_PADDING(0x78);
        be<uint32_t> m_AnimationID;
        SWA_INSERT_PADDING(0x38);
        be<float> m_UnkHudGuideF32;
        be<uint32_t> m_UnkHudGuideU32;
        SWA_INSERT_PADDING(0x18);
        be<EGuideType> m_GuideType;
        SWA_INSERT_PADDING(0xA8);
        be<uint32_t> m_OutOfControlCount;
    };
    SWA_ASSERT_OFFSETOF(CEvilSonicContext, m_spMatrixNode2, 0x2C4);
    SWA_ASSERT_OFFSETOF(CEvilSonicContext, m_spMatrixNodeTransform2, 0x744);
    SWA_ASSERT_OFFSETOF(CEvilSonicContext, m_DarkGaiaEnergy, 0x688);
    SWA_ASSERT_OFFSETOF(CEvilSonicContext, m_AnimationID, 0x7C4);
    SWA_ASSERT_OFFSETOF(CEvilSonicContext, m_DarkGaiaEnergy, 0x688);
    SWA_ASSERT_OFFSETOF(CEvilSonicContext, m_OutOfControlCount, 0x8CC);
}

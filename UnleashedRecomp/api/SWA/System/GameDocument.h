#pragma once

namespace Hedgehog::Database
{
    class CDatabase;
}

namespace SWA
{
    class CWorld2 : public Hedgehog::Base::CSynchronizedObject, public Hedgehog::Universe::CMessageActor
    {
    public:
        SWA_INSERT_PADDING(16-8);
        class CMember
        {
        public:
            int m_pRenderScene;
            void* m_spRenderScene;
            Hedgehog::Base::CSharedString m_Name;
            int dwordC;
            int dword10;
            int dword14;
            int dword18;
            SWA_INSERT_PADDING(80);
            int dword6C;
            int dword70;
        };
        xpointer<CMember> m_pMember;
    };
    SWA_ASSERT_OFFSETOF(CWorld2, m_pMember, 0x98);
    class CSetObjectManager;
    class CGameDocument : public Hedgehog::Base::CSynchronizedObject
    {
    public:
        class CMember
        {
        public:
            struct SScoreInfo
            {
                be<uint32_t> SRank;
                be<uint32_t> ARank;
                be<uint32_t> BRank;
                be<uint32_t> CRank;
                be<uint32_t> DRank;
                SWA_INSERT_PADDING(0x0C);
                be<float> PointMarkerRecordSpeed;
                SWA_INSERT_PADDING(0x0C);
                be<uint32_t> PointMarkerCount;
                be<uint32_t> EnemyScore;
                be<uint32_t> TrickScore;
                SWA_INSERT_PADDING(0x10);
            };

            SWA_INSERT_PADDING(0xC);
            hh::map<Hedgehog::Base::CSharedString, boost::shared_ptr<CWorld2>> m_Worlds;
            SWA_INSERT_PADDING(0x1C-0xC-0xC);
            boost::shared_ptr<Hedgehog::Database::CDatabase> m_spDatabase;
            SWA_INSERT_PADDING(0x60);
            boost::shared_ptr<CSetObjectManager> m_pSetObjectManager;
            SWA_INSERT_PADDING(0x20);
            Hedgehog::Base::CSharedString m_StageName;
            xpointer<CSoundAdministrator> m_pSoundAdministrator;
            SWA_INSERT_PADDING(0x48);
            xpointer<CGeneralWindow> m_pGeneralWindow;
            SWA_INSERT_PADDING(0xD8);
            SScoreInfo m_ScoreInfo;
            SWA_INSERT_PADDING(0x0C);
        };

        // TODO: Hedgehog::Base::TSynchronizedPtr<CGameDocument>
        static CGameDocument* GetInstance();
        void AddGameObject(const guest_stack_var<boost::shared_ptr<CGameObject>>& in_spGameObject, const guest_stack_var<Hedgehog::Base::CSharedString>& in_WorldName);

        xpointer<void> m_pVftable;
        xpointer<CMember> m_pMember;
    };

    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, SRank, 0x00);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, ARank, 0x04);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, BRank, 0x08);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, CRank, 0x0C);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, DRank, 0x10);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, PointMarkerRecordSpeed, 0x20);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, PointMarkerCount, 0x30);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, EnemyScore, 0x34);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember::SScoreInfo, TrickScore, 0x38);
    SWA_ASSERT_SIZEOF(CGameDocument::CMember::SScoreInfo, 0x4C);

    SWA_ASSERT_OFFSETOF(CGameDocument::CMember, m_spDatabase, 0x1C);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember, m_pSetObjectManager, 0x84);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember, m_StageName, 0xAC);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember, m_pSoundAdministrator, 0xB0);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember, m_pGeneralWindow, 0xFC);
    SWA_ASSERT_OFFSETOF(CGameDocument::CMember, m_ScoreInfo, 0x1D8);
    SWA_ASSERT_SIZEOF(CGameDocument::CMember, 0x230);

    SWA_ASSERT_OFFSETOF(CGameDocument, m_pMember, 0x08);
    SWA_ASSERT_SIZEOF(CGameDocument, 0x0C);
}

#include "GameDocument.inl"

#pragma once

#include "SWA.inl"
#include "Hedgehog/Base/Thread/hhSynchronizedPtr.h"
#include "Hedgehog/Universe/Engine/hhMessageActor.h"
#include "Hedgehog/Universe/Engine/hhUpdateUnit.h"
namespace SWA
{
    class CGameDocument;
    class CWorld;

    class CGameObject : public Hedgehog::Universe::CUpdateUnit, public Hedgehog::Universe::CMessageActor
    {
    public:
        struct Vftable
        {
            be<uint32_t> Initialize;
            be<uint32_t> AddCallback;
            be<uint32_t> RemoveCallback;
            be<uint32_t> fn10;
            be<uint32_t> KillCallback;
            be<uint32_t> fn14;
            be<uint32_t> fn18;
        };
        class CMember
        {
        public:
            Hedgehog::Base::TSynchronizedPtr<CGameDocument> m_pGameDocument;
            Hedgehog::Base::TSynchronizedPtr<CWorld> m_pWorld;
            SWA_INSERT_PADDING(0x70);
        };
        CGameObject()
        {
            GuestToHostFunction<void*>(sub_82514A50, this);
        };
        xpointer<Vftable> m_pVftable2;
        xpointer<CMember> m_pMember;
    };
}

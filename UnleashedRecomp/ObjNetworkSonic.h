#pragma once

#include <api/SWA.h>
#include <api/Hedgehog/Base/System/hhAllocator.h>
#include <ui/reddog/debug_draw.h>
#include "NetworkMan.h"
#include <stddef.h>

struct SMotionInfo
{
    xpointer<const char> pName;
    xpointer<const char> pFileName;
    be<float> Speed = 1.0f;
    be < int32_t> RepeatType = eMotionRepeatType_Loop;
    be<float> StartFrame = 0.0f;
    be<float> EndFrame = -1.0f;
    be<float> Field18 = -1.0f;
    be < int32_t> Field1C = 0;

    // These fields are set by the game. Don't touch.
    be<int32_t> Field20 = -1;
    be<int32_t> Field24 = -1;
    be<int32_t> Field28 = -1;
    be<int32_t> Field2C = -1;
};
namespace SWA
{
    class CGameObject3D : public SWA::CGameObject
    {
    public:
        be<uint32_t> m_FieldBC;
        be<uint32_t> m_FieldC0;
        boost::shared_ptr<CMatrixNodeTransform> m_spMatrixNodeTransform;
        SWA_INSERT_PADDING(0xE0 - 0xC8);
        CGameObject3D()
        {
            GuestToHostFunction<void>(sub_82515C40, this);
        }
        static void AddCallback(CGameObject3D* This, Hedgehog::Base::THolder<SWA::CWorld>* worldHolder, SWA::CGameDocument* pGameDocument, boost::shared_ptr<Hedgehog::Database::CDatabase>* spDatabase)
        {
            GuestToHostFunction<void>(sub_82515338, This, worldHolder, pGameDocument, spDatabase);
        }
    };
    SWA_ASSERT_OFFSETOF(CGameObject3D, m_FieldC0, 0xC0);
    SWA_ASSERT_OFFSETOF(CGameObject3D, m_spMatrixNodeTransform, 0xC4);
}

namespace Hedgehog::Animation
{
    class CPose : public Base::CObject
    {
    public:
        struct VfTable
        {
            be<uint32_t> fn00;
            be<uint32_t> fn04;
            be<uint32_t> fn08;
            be<uint32_t> fn0C;
            be<uint32_t> fn10;
            be<uint32_t> fn14;
            be<uint32_t> fn18;
            be<uint32_t> fn1c;
        };
        xpointer<void> m_pVftable;
        //CPose()
        //{
        //    GuestToHostFunction<void>(sub_82E31C00, this);
        //}
    };
    class CAnimationPose : public CPose
    {
    public:
        SWA_INSERT_PADDING(0x154u);
        //Object needed for weird function call
        CAnimationPose(SWA::CGameObject* in_Object, boost::shared_ptr<Database::CDatabase>* in_spDatabase, const Base::CSharedString& in_rName)
        {
            guest_stack_var<Base::CSharedString> name(in_rName);
            guest_stack_var<uint32_t> dataThing;
            GuestToHostFunction<void*>(sub_82512CE8, dataThing.get(), in_Object);
            GuestToHostFunction<void*>(sub_82BBA760, this, dataThing.get(), name.get());
            GuestToHostFunction<void*>(sub_82DF9958, dataThing.get());
        }

        void AddMotionInfo(uint32_t in_GuestAnimationListAddress, uint32_t in_AnimationListCount)
        {
            GuestToHostFunction<void>(sub_82BBBAC0, this, in_GuestAnimationListAddress, in_AnimationListCount);
        }
        void Update(Hedgehog::Universe::SUpdateInfo* upd)
        {
            GuestToHostFunction<void>(sub_82BBC050, this, upd);
        }
        void CreateAnimationCache()
        {
            guest_stack_var<boost::anonymous_shared_ptr> animCache;
            GuestToHostFunction<void>(sub_82BB70E0, animCache.get());
            GuestToHostFunction<void>(sub_82BB6428, this, animCache.get());

        }
    };
}
class IAnimationContext : public Hedgehog::Base::CObject
{
public:
    struct Vftable : public Hedgehog::Base::CObject
    {
        be<uint32_t> GetAnimationPose;
        be<uint32_t> GetVelocityForAnimationSpeed;
        be<uint32_t> GetVelocityForAnimationChange;
    };
    xpointer<Vftable> m_pVftable3;
   Hedgehog::Animation::CAnimationPose* poseTEMPRONG;
   Hedgehog::Math::CVector vec;
    IAnimationContext()
    {
        m_pVftable3 = new Vftable();
    }
};
class CAnimationStateSingle : public Hedgehog::Universe::CStateMachineBase::CStateBase
{
public:
    SWA_INSERT_PADDING(388 - 96);
    CAnimationStateSingle()
    {

    }
};
class CAnimationStateMachine : public Hedgehog::Universe::CStateMachineBase
{
public:
    SWA_INSERT_PADDING(0x2C);

    void ChangeStateAlt(Hedgehog::Base::CSharedString const& in_rName)
    {
        guest_stack_var<boost::anonymous_shared_ptr> out_spAnimState;
        guest_stack_var<Hedgehog::Base::CSharedString > name(in_rName);
        GuestToHostFunction<void>(sub_82300540, out_spAnimState.get(), this, name.get());
    }
    CAnimationStateMachine() : Hedgehog::Universe::CStateMachineBase()
    {
        GuestToHostFunction<void*>(sub_82301060, this);
    }
    void Update(Hedgehog::Universe::SUpdateInfo* in_rUpdateInfo)
    {
        GuestToHostFunction<void>(sub_82300398, this, in_rUpdateInfo);
    }
    boost::anonymous_shared_ptr* AddAnimationState(const Hedgehog::Base::CSharedString& in_rName)
    {
        guest_stack_var<boost::anonymous_shared_ptr> spAnimationState;
        guest_stack_var<Hedgehog::Base::CSharedString> name(in_rName);
        GuestToHostFunction<void>(sub_82300F40, spAnimationState.get(), this, name.get(), name.get());
        return spAnimationState.get();
    }
    boost::anonymous_shared_ptr* AddAnimationStateBlend(const Hedgehog::Base::CSharedString& in_rName)
    {
        guest_stack_var<boost::anonymous_shared_ptr> spAnimationState;
        guest_stack_var<Hedgehog::Base::CSharedString> name(in_rName);
        GuestToHostFunction<void>(sub_82300F40, spAnimationState.get(), this, name.get(), name.get());
        return spAnimationState.get();
    }
};
class CObjNetworkSonic : public SWA::CGameObject3D, public IAnimationContext
{
public:
    uint32_t clientID = -1;
    Hedgehog::Math::CVector m_Pos;
    Hedgehog::Math::CQuaternion m_Rot;
    CAnimationStateMachine* m_AnimationStateMachine;
    boost::shared_ptr < Hedgehog::Animation::CAnimationPose> m_AnimatorPose;
    Hedgehog::Mirage::CSingleElement* singleElement;
    Hedgehog::Math::CVector vec;
    void RegisterAnimations(Hedgehog::Animation::CAnimationPose* animPose, Hedgehog::Mirage::CSingleElement* model, CAnimationStateMachine* stateMachine)
    {
        //std::vector<hh::anim::SMotionInfo> pEntries = std::vector<hh::anim::SMotionInfo>(0, { "","" });
        //for (size_t i = 0; i < anims.size(); i++)
        //{
        //	pEntries.push_back(Hedgehog::Animation::SMotionInfo(anims[i].m_stateName.c_str(), anims[i].m_fileName.c_str(), 1, !anims[i].m_isLoop));
        //}
        auto motionInfo = ((SMotionInfo*)g_memory.Translate(0x8326BB88));
        animPose->AddMotionInfo(0x8326BB88, 10);
        //animationStateMachine = boost::make_shared< Sonic::CAnimationStateMachine>();
        //auto test = animationStateMachine->GetContext();

        //guest_stack_var<boost::shared_ptr<Hedgehog::Animation::CPose>> test(animPose);
        for (size_t i = 0; i < 10; i++)
        {
            stateMachine->AddAnimationState(Hedgehog::Base::CSharedString(motionInfo[i].pName.get()));
        }
    }
    static void AddCallback(CObjNetworkSonic* This, Hedgehog::Base::THolder<SWA::CWorld>* a2, SWA::CGameDocument* pGameDocument, boost::shared_ptr<Hedgehog::Database::CDatabase>* spDatabase)
    {
        Reddog::DebugDraw::DrawTextLog("AddCallback", 0);
       //auto test44 =  This->m_pVftable3;
        SWA::CGameObject3D::AddCallback(This, a2, pGameDocument, spDatabase);
        int playerType = net::NetManager::GetClientFromId(This->clientID)->GetPlayerType();

        Hedgehog::Mirage::CMirageDatabaseWrapper* test2 = new Hedgehog::Mirage::CMirageDatabaseWrapper(spDatabase->get());
        //Single Element creation
        This->singleElement = new Hedgehog::Mirage::CSingleElement(test2->GetModelData(playerType == 0 ? "SonicRoot" : "EvilRoot"));

        This->singleElement->BindMatrixNode(This->m_spMatrixNodeTransform);
        //guest_stack_var<Hedgehog::Base::CSharedString> name("SonicRoot");
        //auto test3 = (Hedgehog::Animation::CAnimationPose*)__HH_ALLOC(0x154);
        This->m_AnimationStateMachine = new CAnimationStateMachine();
        
        //This->context->vec = Hedgehog::Math::CVector(1, 1, 1);
        This->m_AnimationStateMachine->m_pContext = static_cast<IAnimationContext*>(This);

        This->m_AnimatorPose = boost::make_shared<Hedgehog::Animation::CAnimationPose>(This, spDatabase, playerType == 0 ? "SonicRoot" : "EvilRoot");
        //This->singleElement->BindPose(&This->m_AnimatorPose);
        //This->RegisterAnimations(This->m_AnimatorPose.get(), nullptr, This->m_AnimationStateMachine);
        auto motionInfo = ((SMotionInfo*)g_memory.Translate(0x8326BB88));
        This->m_AnimatorPose->AddMotionInfo(0x8326BB88, 10);
        //animationStateMachine = boost::make_shared< Sonic::CAnimationStateMachine>();
        //auto test = animationStateMachine->GetContext();

        guest_stack_var<boost::anonymous_shared_ptr> testatatata(This->m_AnimatorPose.get());
        //This is bindpose
        GuestToHostFunction<void*>(sub_82E1DF80, This->singleElement, testatatata.get());
        //guest_stack_var<boost::shared_ptr<Hedgehog::Animation::CPose>> test(animPose);
        for (size_t i = 0; i < 10; i++)
        {
            This->m_AnimationStateMachine->AddAnimationState(Hedgehog::Base::CSharedString(motionInfo[i].pName.get()));
        }
        This->m_AnimatorPose->CreateAnimationCache();
        guest_stack_var<boost::shared_ptr<Hedgehog::Mirage::CSingleElement>> singleElementPtr(This->singleElement);

        //This->context->poseTEMPRONG = This->m_AnimatorPose.get();
        This->m_AnimationStateMachine->ChangeStateAlt("Stand");
        SWA::CGameDocument::GetInstance()->AddUpdateUnit("0", This);
        SWA::CGameObject::AddRenderable(This, 0x83367940, singleElementPtr.get(), true);
    };
    static void UpdateParallel(CObjNetworkSonic* This, Hedgehog::Universe::SUpdateInfo* in_rUpdateInfo)
    {
        //go update
        GuestToHostFunction<void>(sub_82514010, This);
        //auto e = net::NetManager::GetClientFromId(This->clientID);
        guest_stack_var<Hedgehog::Math::CVector> vec(This->m_Pos);
        guest_stack_var<Hedgehog::Math::CQuaternion> vec2(This->m_Rot);
        GuestToHostFunction<void>(sub_825153E0, This, vec.get());
        GuestToHostFunction<void>(sub_82515450, This, vec2.get());

        //This->context->poseTEMPRONG = This->m_AnimatorPose.get();
        //This->m_AnimationStateMachine->ChangeStateAlt("Stand");
        This->m_AnimationStateMachine->Update(in_rUpdateInfo);
        This->m_AnimatorPose->Update(in_rUpdateInfo);
        Reddog::DebugDraw::DrawTextLog("Update", 0);
    };
    static Hedgehog::Animation::CAnimationPose* GetAnimationPose(IAnimationContext* This)
    {
        return static_cast<CObjNetworkSonic*>(This)->m_AnimatorPose.get();
    };
    static Hedgehog::Math::CVector* Test55(IAnimationContext* This)
    {
        return &static_cast<CObjNetworkSonic*>(This)->vec;
    }; 
    static Hedgehog::Math::CVector* Test56(IAnimationContext* This)
    {
        return &static_cast<CObjNetworkSonic*>(This)->vec;
    };
    void Kill()
    {
        GuestToHostFunction<void>(sub_82513ED8, this);
    }
    void SetPosition(Hedgehog::Math::CVector in_Pos)
    {
        m_Pos = Hedgehog::Math::CVector(in_Pos.X, in_Pos.Y, in_Pos.Z);
    }
    void SetRotation(Hedgehog::Math::CVector4 in_Rot)
    {
        m_Rot.X = in_Rot.X;
        m_Rot.Y = in_Rot.Y;
        m_Rot.Z = in_Rot.Z;
        m_Rot.W = in_Rot.W;
    }
    void SetID(int uid)
    {
        clientID = uid;
    }
   
    CObjNetworkSonic()
    {
        uint32_t functionOffset = PPC_CODE_BASE + PPC_CODE_SIZE + (20 * 4);
        g_memory.InsertFunction(functionOffset, HostToGuestFunction<AddCallback>);
        g_memory.InsertFunction(functionOffset + 4, HostToGuestFunction<UpdateParallel>);
        g_memory.InsertFunction(functionOffset + 8, HostToGuestFunction<GetAnimationPose>);
        g_memory.InsertFunction(functionOffset + 12, HostToGuestFunction<Test55>);
        g_memory.InsertFunction(functionOffset + 16, HostToGuestFunction<Test56>);

        //uintptr_t base = reinterpret_cast<uintptr_t>(this);
        //uintptr_t iac = reinterpret_cast<uintptr_t>(static_cast<IAnimationContext*>(this));

       // size_t offset = iac - base;
        //auto test2 = getIAnimationContextVtable(this);
        //int offset = offsetof(CObjNetworkSonic, m_pVftable3);
        //auto vft = ((xpointer<be<uint32_t>>)((xpointer<be<uint32_t>>*)this)[offset]);
        //((xpointer<be<uint32_t>>)((xpointer<be<uint32_t>>*)this)[offset])[0] = be<uint32_t>(functionOffset + 8);
        //((xpointer<be<uint32_t>>)((xpointer<be<uint32_t>>*)this)[offset])[1] = be<uint32_t>(functionOffset + 12);
        //((xpointer<be<uint32_t>>)((xpointer<be<uint32_t>>*)this)[offset])[2] = be<uint32_t>(functionOffset + 16);
        SWA_OVERRIDE_VIRTUAL(this, 5, functionOffset);
        SWA_OVERRIDE_VIRTUAL(this, 3, functionOffset + 4);

        SWA_OVERRIDE_VIRTUAL(static_cast<IAnimationContext*>(this), 0, functionOffset + 8);
        SWA_OVERRIDE_VIRTUAL(static_cast<IAnimationContext*>(this), 1, functionOffset + 12);
        SWA_OVERRIDE_VIRTUAL(static_cast<IAnimationContext*>(this), 2, functionOffset + 16);
    };
};

#pragma once

#include <api/SWA.h>
#include <api/Hedgehog/Base/System/hhAllocator.h>
#include <ui/reddog/debug_draw.h>
#include "NetworkMan.h"
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

class CMirageDatabaseWrapper : public Hedgehog::Base::CObject
{
public:
    SWA_INSERT_PADDING(0x8);
    CMirageDatabaseWrapper(Hedgehog::Database::CDatabase* in_pDatabase)
    {
        GuestToHostFunction<void*>(sub_82E5E6C8, this, in_pDatabase);
        //GuestToHostFunction<void*>(sub_82E5E6C8, this, in_pDatabase->get());
    }
    boost::shared_ptr<Hedgehog::Mirage::CModelData>* GetModelData(const Hedgehog::Base::CSharedString& in_rName)
    {
        guest_stack_var<boost::shared_ptr< Hedgehog::Mirage::CModelData>> model;
        guest_stack_var<Hedgehog::Base::CSharedString> modelName(in_rName);
        ////MirageDtabaseWrapper
        //GuestToHostFunction<void*>(sub_82E5E6C8, test2, spDatabase->get());

        //GetModelData
        GuestToHostFunction<void*>(sub_82E37318, model.get(), this, modelName.get(), 0);

        return model.get();
    }
};
class CSingleElement : public Hedgehog::Mirage::CRenderable
{
public:
    //wrong
    SWA_INSERT_PADDING(0x90);


    CSingleElement(boost::shared_ptr<Hedgehog::Mirage::CModelData>* spModelData)
    {
        GuestToHostFunction<void*>(sub_82E1F820, this, spModelData);
    }
    void BindMatrixNode(const boost::shared_ptr < SWA::CMatrixNodeTransform>& in_spMatrixNode)
    {
        GuestToHostFunction<void*>(sub_82E1E100, this, &in_spMatrixNode);
    }
};
SWA_ASSERT_SIZEOF(CSingleElement, 0x98u);
class CObjNetworkSonic : public SWA::CGameObject3D
{
public:
    uint32_t clientID = -1;
    Hedgehog::Math::CVector m_Pos;
    Hedgehog::Math::CQuaternion m_Rot;
    static void AddCallback(CObjNetworkSonic* This, Hedgehog::Base::THolder<SWA::CWorld>* a2, SWA::CGameDocument* pGameDocument, boost::shared_ptr<Hedgehog::Database::CDatabase>* spDatabase)
    {
        Reddog::DebugDraw::DrawTextLog("AddCallback", 0);

        guest_stack_var<boost::shared_ptr< Hedgehog::Mirage::CModelData>> model;
        int playerType = net::NetManager::GetClientFromId(This->clientID)->GetPlayerType();
        //guest_stack_var<Hedgehog::Base::CSharedString> modelName(playerType == 0 ? "SonicRoot" : "EvilRoot");

        CMirageDatabaseWrapper* test2 = new CMirageDatabaseWrapper(spDatabase->get());
        ////MirageDtabaseWrapper
        //GuestToHostFunction<void*>(sub_82E5E6C8, test2, spDatabase->get());

        ;
        //GetModelData
        //GuestToHostFunction<void*>(sub_82E37318, model.get(), test2, modelName.get(), 0);

        //SingleElement ctor
        auto singleElement = new CSingleElement(test2->GetModelData(playerType == 0 ? "SonicRoot" : "EvilRoot"));
        //GuestToHostFunction<void*>(sub_82E1F820, singleElement, model.get());
        guest_stack_var<boost::shared_ptr<CSingleElement>> singleElementPtr(singleElement);

        singleElement->BindMatrixNode(This->m_spMatrixNodeTransform);
        //Bind
        //GuestToHostFunction<void*>(sub_82E1E100, singleElement, &This->m_spMatrixNodeTransform);

        SWA::CGameDocument::GetInstance()->AddUpdateUnit("0", This);

        //AddRenderable
        GuestToHostFunction<void*>(sub_82514360, This, ((be<uint32_t>*)g_memory.Translate(0x8336795C))->get(), singleElementPtr.get(), 1);

        SWA::CGameObject3D::AddCallback(This, a2, pGameDocument, spDatabase);
        //GuestToHostFunction<void>(sub_82515338, This, a2, pGameDocument, spDatabase);
    };
    static void UpdateParallel(CObjNetworkSonic* This)
    {
        //auto e = net::NetManager::GetClientFromId(This->clientID);
        guest_stack_var<Hedgehog::Math::CVector> vec(This->m_Pos);
        guest_stack_var<Hedgehog::Math::CQuaternion> vec2(This->m_Rot);
        GuestToHostFunction<void>(sub_825153E0, This, vec.get());
        GuestToHostFunction<void>(sub_82515450, This, vec2.get());
        Reddog::DebugDraw::DrawTextLog("Update", 0);
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
        SWA_OVERRIDE_VIRTUAL(this, 5, functionOffset);
        SWA_OVERRIDE_VIRTUAL(this, 2, functionOffset + 4);
    };
};

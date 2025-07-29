#include "app.h"
#include <api/SWA.h>
#include <gpu/video.h>
#include <install/installer.h>
#include <kernel/function.h>
#include <os/process.h>
#include <patches/audio_patches.h>
#include <patches/inspire_patches.h>
#include <ui/reddog/debug_draw.h>
#include <ui/game_window.h>
#include <user/config.h>
#include <user/paths.h>
#include <user/registry.h>
#include <api/Hedgehog/Base/System/hhAllocator.h>
#include "NetworkMan.h"
#include "ObjNetworkSonic.h"
void App::Restart(std::vector<std::string> restartArgs)
{
    os::process::StartProcess(os::process::GetExecutablePath(), restartArgs, os::process::GetWorkingDirectory());
    Exit();
}

void App::Exit()
{
    Config::Save();

#ifdef _WIN32
    timeEndPeriod(1);
#endif

    std::_Exit(0);
}

// SWA::CApplication::CApplication
PPC_FUNC_IMPL(__imp__sub_824EB490);
PPC_FUNC(sub_824EB490)
{
    App::s_isInit = true;
    App::s_isMissingDLC = !Installer::checkAllDLC(GetGamePath());
    App::s_language = Config::Language;

    SWA::SGlobals::Init();
    Registry::Save();

    __imp__sub_824EB490(ctx, base);
}

static std::thread::id g_mainThreadId = std::this_thread::get_id();


//class VTableTest
//{
//public:
//    xpointer<be<uint32_t>> vftable;
//};
//namespace SWA
//{
//    class CGameObject3D : public SWA::CGameObject
//    {
//    public:
//        be<uint32_t> m_FieldBC;
//        be<uint32_t> m_FieldC0;
//        boost::shared_ptr<CMatrixNodeTransform> m_spMatrixNodeTransform;
//        SWA_INSERT_PADDING(0xE0 - 0xC8);
//        CGameObject3D()
//        {
//            GuestToHostFunction<void>(sub_82515C40, this);
//        }
//    };
//    SWA_ASSERT_OFFSETOF(CGameObject3D, m_FieldC0, 0xC0);
//    SWA_ASSERT_OFFSETOF(CGameObject3D, m_spMatrixNodeTransform, 0xC4);
//}
//
//class CMirageDatabaseWrapper : public Hedgehog::Base::CObject
//{
//public:
//    SWA_INSERT_PADDING(0x8);
//    CMirageDatabaseWrapper()
//    {
//        //GuestToHostFunction<void*>(sub_82E5E6C8, this, in_pDatabase->get());
//    }
//    boost::shared_ptr<Hedgehog::Mirage::CModelData>* GetModel(const Hedgehog::Base::CSharedString& in_rName)
//    {
//        guest_stack_var<boost::shared_ptr<Hedgehog::Mirage::CModelData>> spCsdProject;
//        guest_stack_var<Hedgehog::Base::CSharedString> name(in_rName);
//        guest_stack_var<be<uint32_t>> flag(0);
//        GuestToHostFunction<void>(sub_82E37318, spCsdProject.get(), this, name.get(), flag.get());
//        return spCsdProject.get();
//    }
//};
//class CSingleElement : public Hedgehog::Mirage::CRenderable
//{
//public:
//    //wrong
//    SWA_INSERT_PADDING(0xA0);
//
//
//    //CSingleElement(boost::shared_ptr<Hedgehog::Mirage::CModelData>* spModelData)
//    //{
//    //    GuestToHostFunction<void*>(sub_82E1F820, this, spModelData);
//    //}
//};
//class TestClass : public SWA::CGameObject3D
//{
//public:
//    const char* name = "Testing";
//    static void AddCallback(TestClass* This, Hedgehog::Base::THolder<SWA::CWorld>* a2, SWA::CGameDocument* pGameDocument, boost::shared_ptr<Hedgehog::Database::CDatabase>* spDatabase)
//    {
//        Reddog::DebugDraw::DrawTextLog("AddCallback", 10);
//
//        CMirageDatabaseWrapper* test2 = new CMirageDatabaseWrapper();
//        guest_stack_var<boost::shared_ptr< Hedgehog::Mirage::CModelData>> model;
//        guest_stack_var<Hedgehog::Base::CSharedString> modelName("SonicRoot");
//
//        //MirageDtabaseWrapper
//        GuestToHostFunction<void*>(sub_82E5E6C8, test2, spDatabase->get());
//
//        //GetModelData
//        GuestToHostFunction<void*>(sub_82E37318, model.get(), test2, modelName.get(), 0);
//
//        //SingleElement ctor
//        auto singleElement = (CSingleElement*)__HH_ALLOC(0x98u);
//        GuestToHostFunction<void*>(sub_82E1F820, singleElement, model.get());
//        guest_stack_var<boost::shared_ptr<CSingleElement>> singleElementPtr(singleElement);
//
//
//        //Bind
//        GuestToHostFunction<void*>(sub_82E1E100, singleElement, &This->m_spMatrixNodeTransform);
//
//        SWA::CGameDocument::GetInstance()->AddUpdateUnit("0", This);
//
//        //AddRenderable
//        GuestToHostFunction<void*>(sub_82514360, This, ((be<uint32_t>*)g_memory.Translate(0x8336795C))->get(), singleElementPtr.get(), 0);
//        
//        GuestToHostFunction<void>(sub_82515338, This, a2, pGameDocument, spDatabase);
//    };
//    static void UpdateParallel(TestClass* This)
//    {
//        auto player = SWA::Player::CPlayerSpeedContext::GetInstance();
//        auto pos = player->m_spMatrixNode->m_Transform.m_Position;
//        auto rot = player->m_spMatrixNode->m_Transform.m_Rotation;
//        guest_stack_var<Hedgehog::Math::CVector> vec(pos);
//        guest_stack_var<Hedgehog::Math::CQuaternion> vec2(rot);
//        GuestToHostFunction<void>(sub_825153E0, This, vec.get());
//        GuestToHostFunction<void>(sub_82515450, This, vec2.get());
//        Reddog::DebugDraw::DrawTextLog("Update", 1);
//    };
//    TestClass()
//    {
//        uint32_t functionOffset = PPC_CODE_BASE + PPC_CODE_SIZE + (20 * 4);
//        g_memory.InsertFunction(functionOffset, HostToGuestFunction<AddCallback>);
//        g_memory.InsertFunction(functionOffset + 4, HostToGuestFunction<UpdateParallel>);
//        SWA_OVERRIDE_VIRTUAL(this, 5,functionOffset);
//        SWA_OVERRIDE_VIRTUAL(this, 2,functionOffset + 4);
//        auto v = (be<uint32_t>*)(this) + (4 * 5);
//        *v = functionOffset;
//        auto reallyBad = (size_t*)this;
//        //(xpointer<SWA::CGameObject::Vftable>)reallyBad[0]
//        //m_pVftable2->UpdateParallel = functionOffset;
//    };
//};

// Rotate a point around Y-axis
Reddog::Vector3 RotateY(const Reddog::Vector3& p, float angleRadians) {
    float cosA = std::cos(angleRadians);
    float sinA = std::sin(angleRadians);
    return {
        p.x * cosA - p.z *sinA,
        p.y,
        p.x * sinA + p.z * cosA
    };
}

std::vector<Reddog::Vector3> GenerateRotatedBox3D(float cx, float cy, float cz,
    float width, float height, float depth,
    float angleRadians) {
    float hw = width / 2.0f;
    float hh = height / 2.0f;
    float hd = depth / 2.0f;

    // Local box corners (centered at origin)
    std::vector<Reddog::Vector3> localCorners = {
        {-hw, -hh, -hd}, {+hw, -hh, -hd}, {+hw, +hh, -hd}, {-hw, +hh, -hd},
        {-hw, -hh, +hd}, {+hw, -hh, +hd}, {+hw, +hh, +hd}, {-hw, +hh, +hd}
    };

    // Apply rotation and translate to center
    std::vector<Reddog::Vector3> worldCorners;
    for (const auto& corner : localCorners)
    {
        Reddog::Vector3 rotated = RotateY(corner, angleRadians);
        worldCorners.push_back({ rotated.x + cx, rotated.y + cy, rotated.z + cz });
    }

    return worldCorners;
}
PPC_FUNC_IMPL(__imp__sub_82365EC8);
PPC_FUNC(sub_82365EC8)
{
    for (const auto& test : net::NetManager::m_ClientMap)
    {
        test->ResetPlayer();
        test->SetPlayerType(0);
    }
    __imp__sub_82365EC8(ctx, base);
}
PPC_FUNC_IMPL(__imp__sub_823C46E0);
PPC_FUNC(sub_823C46E0)
{
    for (const auto& test : net::NetManager::m_ClientMap)
    {
        test->ResetPlayer();
        test->SetPlayerType(1);
    }
    __imp__sub_823C46E0(ctx, base);
}
static std::string g_MultiPreviousStage = "";
void UpdateNetwork(PPCContext& ctx, uint8_t* base, bool isWerehog)
{
    Hedgehog::Math::CVector pos;
    Hedgehog::Math::CQuaternion rot;
    if (!isWerehog)
    {
        auto player = SWA::Player::CPlayerSpeedContext::GetInstance();
        pos = player->m_spMatrixNode->m_Transform.m_Position;
        rot = player->m_spMatrixNode->m_Transform.m_Rotation;
    }
    else
    {
        auto player = (SWA::Player::CEvilSonicContext*)App::g_MultiWerehogPtr;
        pos = player->m_spMatrixNode->m_Transform.m_Position;
        rot = player->m_spMatrixNode->m_Transform.m_Rotation;
    }

    if (net::NetManager::isConnected)
    {
        for (const auto& word : net::NetManager::m_ClientMap)
        {
            word->Update();
        }
        auto e = SimpleVec4(pos.X.get(), pos.Y.get(), pos.Z.get());
        auto f = SimpleVec4(rot.X.get(), rot.Y.get(), rot.Z.get(), rot.W.get());
        Reddog::DebugDraw::DrawTextLog(std::format("X: {0} Y: {1} Z: {2}", e.x, e.y, e.z).c_str(), 0);
        net::NetManager::SendPacketVec4(e, PacketType::MOVE);
        net::NetManager::SendPacketVec4(f, PacketType::ROTATE);
        if (g_MultiPreviousStage != SWA::CGameDocument::GetInstance()->m_pMember->m_StageName.c_str())
        {
            net::NetManager::SendPacketStage(SWA::CGameDocument::GetInstance()->m_pMember->m_StageName.c_str(), 0);
            g_MultiPreviousStage = SWA::CGameDocument::GetInstance()->m_pMember->m_StageName.c_str();
        }
        for (size_t i = 0; i < net::NetManager::m_ClientMap.size(); i++)
        {
            auto playerPos = net::NetManager::m_ClientMap[i]->GetPosition();


            Reddog::SDrawText text;
            text.Text = net::NetManager::m_ClientMap[i]->GetUsername();
            Reddog::DebugDraw::DrawText2D(text, Reddog::Vector3(playerPos.X, playerPos.Y + 2, playerPos.Z));
        }
    }
}



//CEvilPlayer update
PPC_FUNC_IMPL(__imp__sub_823C2988);
PPC_FUNC(sub_823C2988)
{
    UpdateNetwork(ctx, base, 1);
    __imp__sub_823C2988(ctx, base);
}
//CPlayerSpeed update
PPC_FUNC_IMPL(__imp__sub_82302D10);
PPC_FUNC(sub_82302D10)
{
    UpdateNetwork(ctx, base, 0);
    __imp__sub_82302D10(ctx, base);
}
//class CGoalHud
//{
//public:
//    struct CContext
//    {
//        struct HudField
//        {
//            SWA_INSERT_PADDING(52);
//            be<float> Time;
//        };
//        SWA_INSERT_PADDING(300);
//        xpointer<HudField> HudField;
//    };
//    SWA_INSERT_PADDING(8);
//    xpointer< CContext> Context;
//};
//SWA_ASSERT_OFFSETOF(CGoalHud, Context, 8);
//SWA_ASSERT_OFFSETOF(CGoalHud::CContext, HudField, 300);
////CPlayerSpeed update
//PPC_FUNC_IMPL(__imp__sub_824A1170);
//PPC_FUNC(sub_824A1170)
//{
//    __imp__sub_824A1170(ctx, base);
//    auto ptr = (CGoalHud*)g_memory.Translate(ctx.r3.u32);
//    auto time = ptr->Context->HudField->Time;
//    float float2 = *reinterpret_cast<float*>(PPC_LOAD_U32(PPC_LOAD_U32(ctx.r3.u32 + 300) + 52));
//    //net::NetManager::SendPacketStageEnd(SWA::CGameDocument::GetInstance()->m_pMember->m_StageName.c_str(), );
//}


// SWA::CApplication::Update
PPC_FUNC_IMPL(__imp__sub_822C1130);
PPC_FUNC(sub_822C1130)
{
    if (GetAsyncKeyState(VK_F6))
    {
        net::NetManager::Initialize();
        if (net::NetManager::Connect("HKRY (in Unleashed)", "127.0.0.1", 7777) == 0)
        {
            Reddog::DebugDraw::DrawTextLog("Connection success", 10);
        }
        else
        {

            Reddog::DebugDraw::DrawTextLog("Connection fail", 10);
        }
    }
    if (net::NetManager::isConnected)
    {
        net::NetManager::ProcessPackets();
    }
    Video::WaitOnSwapChain();

    // Correct small delta time errors.
    if (Config::FPS >= FPS_MIN && Config::FPS < FPS_MAX)
    {
        double targetDeltaTime = 1.0 / Config::FPS;

        if (abs(ctx.f1.f64 - targetDeltaTime) < 0.00001)
            ctx.f1.f64 = targetDeltaTime;
    }

    App::s_deltaTime = ctx.f1.f64;
    App::s_time += App::s_deltaTime;

    // This function can also be called by the loading thread,
    // which SDL does not like. To prevent the OS from thinking
    // the process is unresponsive, we will flush while waiting
    // for the pipelines to finish compiling in video.cpp.
    if (std::this_thread::get_id() == g_mainThreadId)
    {
        SDL_PumpEvents();
        SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
        GameWindow::Update();
    }

    AudioPatches::Update(App::s_deltaTime);
    InspirePatches::Update();

    // Apply subtitles option.
    if (auto pApplicationDocument = SWA::CApplicationDocument::GetInstance())
        pApplicationDocument->m_InspireSubtitles = Config::Subtitles;

    if (Config::EnableEventCollisionDebugView)
        *SWA::SGlobals::ms_IsTriggerRender = true;

    if (Config::EnableGIMipLevelDebugView)
        *SWA::SGlobals::ms_VisualizeLoadedLevel = true;

    if (Config::EnableObjectCollisionDebugView)
        *SWA::SGlobals::ms_IsObjectCollisionRender = true;

    if (Config::EnableStageCollisionDebugView)
        *SWA::SGlobals::ms_IsCollisionRender = true;

    __imp__sub_822C1130(ctx, base);
}


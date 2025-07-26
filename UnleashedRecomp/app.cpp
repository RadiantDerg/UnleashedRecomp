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
#define SWA_OVERRIDE_VIRTUAL(obj, index, value) \
    ((xpointer<be<uint32_t>>)((xpointer<be<uint32_t>>*)obj)[0])[index] = be<uint32_t>(value)


class VTableTest
{
public:
    xpointer<be<uint32_t>> vftable;
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
    };
    SWA_ASSERT_OFFSETOF(CGameObject3D, m_FieldC0, 0xC0);
    SWA_ASSERT_OFFSETOF(CGameObject3D, m_spMatrixNodeTransform, 0xC4);
}

class CMirageDatabaseWrapper : public Hedgehog::Base::CObject
{
public:
    SWA_INSERT_PADDING(0x8);
    CMirageDatabaseWrapper()
    {
        //GuestToHostFunction<void*>(sub_82E5E6C8, this, in_pDatabase->get());
    }
    boost::shared_ptr<Hedgehog::Mirage::CModelData>* GetModel(const Hedgehog::Base::CSharedString& in_rName)
    {
        guest_stack_var<boost::shared_ptr<Hedgehog::Mirage::CModelData>> spCsdProject;
        guest_stack_var<Hedgehog::Base::CSharedString> name(in_rName);
        guest_stack_var<be<uint32_t>> flag(0);
        GuestToHostFunction<void>(sub_82E37318, spCsdProject.get(), this, name.get(), flag.get());
        return spCsdProject.get();
    }
};
class CSingleElement : public Hedgehog::Mirage::CRenderable
{
public:
    //wrong
    SWA_INSERT_PADDING(0xA0);


    //CSingleElement(boost::shared_ptr<Hedgehog::Mirage::CModelData>* spModelData)
    //{
    //    GuestToHostFunction<void*>(sub_82E1F820, this, spModelData);
    //}
};
class TestClass : public SWA::CGameObject3D
{
public:
    const char* name = "Testing";
    static void AddCallback(TestClass* This, Hedgehog::Base::THolder<SWA::CWorld>* a2, SWA::CGameDocument* pGameDocument, boost::shared_ptr<Hedgehog::Database::CDatabase>* spDatabase)
    {
        Reddog::DebugDraw::DrawTextLog("AddCallback", 10);

        CMirageDatabaseWrapper* test2 = new CMirageDatabaseWrapper();
        guest_stack_var<boost::shared_ptr< Hedgehog::Mirage::CModelData>> model;
        guest_stack_var<Hedgehog::Base::CSharedString> modelName("SonicRoot");

        //MirageDtabaseWrapper
        GuestToHostFunction<void*>(sub_82E5E6C8, test2, spDatabase->get());

        //GetModelData
        GuestToHostFunction<void*>(sub_82E37318, model.get(), test2, modelName.get(), 0);

        //SingleElement ctor
        auto singleElement = (CSingleElement*)__HH_ALLOC(0x98u);
        GuestToHostFunction<void*>(sub_82E1F820, singleElement, model.get());
        guest_stack_var<boost::shared_ptr<CSingleElement>> singleElementPtr(singleElement);

        SWA::CGameDocument::GetInstance()->AddUpdateUnit("0", This);

        //AddRenderable
        GuestToHostFunction<void*>(sub_82514360, This, ((be<uint32_t>*)g_memory.Translate(0x8336795C))->get(), singleElementPtr.get(), 0);

    };
    static void UpdateParallel(PPCContext* ctx)
    {
        Reddog::DebugDraw::DrawTextLog("Update", 1);
    };
    TestClass()
    {
        uint32_t functionOffset = PPC_CODE_BASE + PPC_CODE_SIZE + (20 * 4);
        g_memory.InsertFunction(functionOffset, HostToGuestFunction<AddCallback>);
        g_memory.InsertFunction(functionOffset + 4, HostToGuestFunction<UpdateParallel>);
        SWA_OVERRIDE_VIRTUAL(this, 5,functionOffset);
        SWA_OVERRIDE_VIRTUAL(this, 2,functionOffset + 4);
        auto v = (be<uint32_t>*)(this) + (4 * 5);
        *v = functionOffset;
        auto reallyBad = (size_t*)this;
        //(xpointer<SWA::CGameObject::Vftable>)reallyBad[0]
        //m_pVftable2->UpdateParallel = functionOffset;
    };
};

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

// SWA::CApplication::Update
PPC_FUNC_IMPL(__imp__sub_822C1130);
PPC_FUNC(sub_822C1130)
{
    if (GetAsyncKeyState(VK_F6))
    {
        net::NetManager::Initialize();
        net::NetManager::Connect("HKRY (in Unleashed)", "thiskwasior.ct8.pl", 6969);
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
    auto player = SWA::Player::CPlayerSpeedContext::GetInstance();
    if (player)
    {
        if (net::NetManager::isConnected)
        {
            auto pos = player->m_spMatrixNode->m_Transform.m_Position;
            //auto rot = player->m_spMatrixNode->m_Transform.m_Rotation;
            float x = pos.X.get();
            float y = pos.Y.get();
            float z = pos.Z.get();
            auto e = SimpleVec4(x, y, z);
            //auto f = SimpleVec4(rot.X, rot.Y, rot.Z);
            net::NetManager::SendPacketVec4(e, PacketType::MOVE);
            //net::NetManager::SendPacketVec4(f, PacketType::ROTATE);
            net::NetManager::SendPacketStage("DChina_3", 0);
            printf("");
            printf("");
            for (size_t i = 0; i < net::NetManager::m_ClientMap.size(); i++)
            {
                auto playerPos = net::NetManager::m_ClientMap[i]->GetPosition();
                auto linePoints = GenerateRotatedBox3D(playerPos.X, playerPos.Y, playerPos.Z, 1, 2, 1, net::NetManager::m_ClientMap[i]->GetRotation().Y);
                const int edges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
                };

                for (size_t i = 0; i < 12; i++)
                {
                    auto line = Reddog::SDrawLine();
                    line.Start = linePoints[edges[i][0]];
                    line.End = linePoints[edges[i][1]];
                    line.Colour = IM_COL32(255, 255, 255, 255);
                    Reddog::DebugDraw::DrawLine(line);

                }
                Reddog::SDrawText text;
                text.Text = net::NetManager::m_ClientMap[i]->GetUsername();
                Reddog::DebugDraw::DrawText2D(text, Reddog::Vector3(playerPos.X, playerPos.Y + 2, playerPos.Z));
            }
        }
        
    }
    if (GetAsyncKeyState(VK_F7))
    {
        auto obj = boost::make_shared<TestClass>();

        boost::shared_ptr<SWA::CGameObject> barPtr(reinterpret_cast<SWA::CGameObject*>(obj.get()));

        guest_stack_var<Hedgehog::Base::CSharedString> worldName("main");
        guest_stack_var<Hedgehog::Base::THolder<SWA::CWorld>, false> worldHolder;
        guest_stack_var<boost::shared_ptr<TestClass>> boost(obj);
        GuestToHostFunction<void>(sub_8250F238, worldHolder.get(), SWA::CGameDocument::GetInstance(), worldName.get()); // get holder
        GuestToHostFunction<void>(sub_82510D20, SWA::CGameDocument::GetInstance(), worldHolder.get(), boost.get(), 0); // add game object
        //GuestToHostFunction<int>(sub_82510D20, SWA::CGameDocument2::GetInstance(), in_pText);
    }
    __imp__sub_822C1130(ctx, base);
}


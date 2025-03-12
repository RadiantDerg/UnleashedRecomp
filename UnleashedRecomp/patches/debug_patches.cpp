#include <api/SWA.h>
#include <ui/game_window.h>
#include <user/config.h>
#include <ui/reddog/debug_draw.h>
#include <ui/imgui_utils.h>
#include <patches/aspect_ratio_patches.h>
#include <patches/debug_patches.h>
#include "ui/reddog/windows/TestWindow.h"


// TODO: Move elsewhere
inline char* UTF16BE_to_Cstr(const wchar_t* text)
{
    std::vector<uint16_t> charBuffer;
    uint16_t len = 0;
    auto ptr = (uint16_t*)text;

    while (ptr[len] != 0)
    {
        charBuffer.push_back(ByteSwap<uint16_t>(text[len]));
        ++len;
    }
    charBuffer.push_back(0);

    thread_local char fmtMultiByte[1024];
    WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(charBuffer.data()), -1, fmtMultiByte, sizeof(fmtMultiByte), 0, 0);
    return fmtMultiByte;
}

struct ARGB8_COLOR
{
    uint8_t a, r, g, b;

    /// Pack color to ImU32 Color
    [[nodiscard]] ImU32 ToImU32() const {
        return a << 24 | b << 16 | g << 8 | r;
    }
};

//// DEBUG_DrawTextAtPosition (HE_DebugDrawTextDraw_1)
//PPC_FUNC_IMPL(__imp__sub_822C7130);
//PPC_FUNC(sub_822C7130)
//{
//    //auto test = UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r8.u32));
//    //Reddog::DebugDraw::DrawTextLog(test, 0);
//    __imp__sub_822C7130(ctx, base);
//}
//
// DEBUG_DrawTextAtPosition2 (HE_DebugDrawTextDraw_2)
//PPC_FUNC_IMPL(__imp__sub_822C6EA8);
//PPC_FUNC(sub_822C6EA8)
//{
//    /*Reddog::SDrawText drawText{
//        {Scale(g_aspectRatioOffsetX + 720), Scale(g_aspectRatioOffsetY + 36)},
//        UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r8.u32)),
//        0.0f,
//        1.0f,
//        (*(const ARGB8_COLOR*)g_memory.Translate(ctx.r7.u32)).ToImU32(),
//        Reddog::eDrawTextFlags_NoShadow
//    };
//    Reddog::DebugDraw::DrawTextLog(drawText);*/
//
//    Reddog::DebugDraw::DrawTextLog(UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r8.u32)), 0);
//    //auto pos = (const Hedgehog::Math::CVector*)g_memory.Translate(ctx.r5.u32);
//    //Reddog::DebugDraw::DrawText2D(drawText, { pos->X, pos->Y, pos->Z });
//
//    __imp__sub_822C6EA8(ctx, base);
//}

// SWA::CDebugDrawText::DrawDebugText1
PPC_FUNC_IMPL(__imp__sub_822CC300);
PPC_FUNC(sub_822CC300)
{
    be f1 = ctx.f1.f64; // Text X position 
    be f2 = ctx.f2.f64; // Text Y position 

    Reddog::SDrawText drawText{
        {
            Scale(g_aspectRatioOffsetX + static_cast<float>(f1)),
            Scale(g_aspectRatioOffsetY + static_cast<float>(f2))
        },
        UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r8.u32)),
        0.0f,
        0.85f,
        (*(const ARGB8_COLOR*)g_memory.Translate(ctx.r7.u32)).ToImU32(),
        Reddog::eDrawTextFlags_NoShadow
    };

    Reddog::DebugDraw::DrawText2D(drawText);

    __imp__sub_822CC300(ctx, base);
}

// SWA::CDebugDrawText::DrawDebugText2 @ 0x822CC0D8
PPC_FUNC_IMPL(__imp__sub_822CC0D8);
PPC_FUNC(sub_822CC0D8)
{
    Reddog::DebugDraw::DrawTextLog(UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r8.u32)), 6);

    __imp__sub_822CC0D8(ctx, base);
}

// ::GetIsDebugRenderForGameObject()
PPC_FUNC_IMPL(__imp__sub_82512BF8);
PPC_FUNC(sub_82512BF8)
{
    if (DebugPatches::ms_IsForceGameObjectDebugRender)
        ctx.r3.u8 = 1; // Always return true
    else
        __imp__sub_82512BF8(ctx, base);
}

// boost::~::SWA::CDebugDraw::CMember::SDrawLine
PPC_FUNC_IMPL(__imp__sub_822C9398);
PPC_FUNC(sub_822C9398)
{
    auto a2 = (Hedgehog::Math::CVector*)g_memory.Translate(ctx.r4.u32);
    auto a3 = (Hedgehog::Math::CVector*)g_memory.Translate(ctx.r5.u32);
    //auto a4 = (be<unsigned int>*)g_memory.Translate(ctx.r6.u32);
    auto a4 = (ARGB8_COLOR*)g_memory.Translate(ctx.r6.u32);

    Reddog::Vector3 start(a2->X, a2->Y, a2->Z);
    Reddog::Vector3 end(a3->X, a3->Y, a3->Z);

    const Reddog::SDrawLine line{
        start, end, a4->ToImU32()
    };

    Reddog::DebugDraw::DrawLine(line);

    __imp__sub_822C9398(ctx, base);
}



// SWA::CStageManager::UpdateParallel
//PPC_FUNC_IMPL(__imp__sub_82521C68);
//PPC_FUNC(sub_82521C68)
//{
//    __imp__sub_82521C68(ctx, base);
//}

// SWA::CStageManager::UpdateSerial
PPC_FUNC_IMPL(__imp__sub_82522040);
PPC_FUNC(sub_82522040)
{
    const auto& This = static_cast<SWA::CStageManager*>(g_memory.Translate(ctx.r3.u32));

    __imp__sub_82522040(ctx, base);

    // Draw player position
    if (Reddog::DebugDraw::GetIsDrawPosition())
    {
        // TODO (RadiantDerg): Reimplement SWA::CStageManager ability to draw progress ratio
        // NOTE: Currently does not work, is the API mapping horribly misaligned/misunderstood?
        if (This->m_spStageGuidePathController)
        {
            if (This->m_spStageGuidePathController->m_spPathAnimationController)
            {
                auto PAC = This->m_spStageGuidePathController->m_spPathAnimationController;
                //Reddog::DebugDraw::DrawTextLog(fmt::format("m_DistanceAlongPath = {:.2f}", PAC->m_DistanceAlongPath.get()).c_str());
            }

            // Ratio
            if (This->m_StageGuidePathLength.get() > 0.0f)
            {
                float distance = 0.0f; // GetDistanceAlongPath(a1->m_StageGuidePathController)
                //a1->m_StageGuidePathRatio = distance / a1->m_StageGuidePathLength;

                const Reddog::SDrawText ratioText{
                {Scale(g_aspectRatioOffsetX + 720), Scale(g_aspectRatioOffsetY + 36)},
                    fmt::format("{:.1f}m [{:.1f}/100.0]", This->m_StageGuidePathLength.get(), This->m_StageGuidePathRatio.get()),
                    0,
                    3.25f,
                    0xFFFFFFFF,
                    Reddog::eDrawTextFlags_NoShadow
                };

                Reddog::DebugDraw::DrawText2D(ratioText);
            }
        }

        // Position
        const Reddog::SDrawText positionText{
            {Scale(g_aspectRatioOffsetX + 750), Scale(g_aspectRatioOffsetY + 120)},
            fmt::format("( {:.2f}, {:.2f}, {:.2f} )", This->m_PlayerPosition.X.get(), This->m_PlayerPosition.Y.get(), This->m_PlayerPosition.Z.get()),
            0,
            2.0f,
            0xFFFFFFFF,
            Reddog::eDrawTextFlags_NoShadow
        };

        Reddog::DebugDraw::DrawText2D(positionText);
    }

}



/// <summary>
/// ALL OF THIS IS VERY BAD!!! Please replace once better options exist!!!
/// </summary>
/// <param name=""></param>
/// 

PPC_FUNC_IMPL(__imp__sub_827B69A0);
PPC_FUNC(sub_827B69A0)
{
    TestWindow::SetObjectManagerActor = (CSetObjectManager*)(base + PPC_LOAD_U32(ctx.r3.u32));
    __imp__sub_827B69A0(ctx, base);
}

PPC_FUNC_IMPL(__imp__sub_82540248);
PPC_FUNC(sub_82540248)
{
    TestWindow::GamemodeActor = (Hedgehog::Universe::CMessageActor*)(base + ctx.r3.u32);
    __imp__sub_82540248(ctx, base);
}

///
/// CObjSpring::OnSetEditorEnter
///
PPC_FUNC_IMPL(__imp__sub_825E8778);
PPC_FUNC(sub_825E8778)
{
    __imp__sub_825E8778(ctx, base);
}

/// 
/// SWA::CSetEditorCamera
/// (Replaces SetEditor1st' camera with 2nd's)
/// 
PPC_FUNC_IMPL(__imp__sub_825A5038);
PPC_FUNC(sub_825A5038)
{
    sub_825B7F30(ctx, base);
}
/// 
/// boost::shared_ptr<SWA::CSetEditorCamera>
/// (Replaces SetEditor1st' camera with 2nd's)
/// 
PPC_FUNC_IMPL(__imp__sub_825A0FB0);
PPC_FUNC(sub_825A0FB0)
{
    sub_825B7578(ctx, base);
}

// SetEditor Text thing #1
PPC_FUNC_IMPL(__imp__sub_822CAC88);
PPC_FUNC(sub_822CAC88)
{
    Reddog::DebugDraw::DrawTextLog(UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r4.u32)), 3);
    __imp__sub_822CAC88(ctx, base);
}

// SetEditor Text thing #2
PPC_FUNC_IMPL(__imp__sub_822CB5F8);
PPC_FUNC(sub_822CB5F8)
{
    Reddog::DebugDraw::DrawTextLog(UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r4.u32)), 3);
    __imp__sub_822CB5F8(ctx, base);
}

// SetEditor Text thing #3
PPC_FUNC_IMPL(__imp__sub_822CAAC8);
PPC_FUNC(sub_822CAAC8)
{
    Reddog::DebugDraw::DrawTextLog(UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r4.u32)), 3);
    __imp__sub_822CAAC8(ctx, base);
}

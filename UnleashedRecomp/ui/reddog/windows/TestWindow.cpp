#include "TestWindow.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
#include <ui/reddog/debug_draw.h>
static TestWindow g_window;


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
/// Move elsewhere
///
inline char* UTF16BE_to_Cstr(const wchar_t* text)
{
    std::vector<uint16_t> testVec;
    uint16_t len = 0;
    auto ptr = (uint16_t*)text;
    while (ptr[len] != 0)
    {
        testVec.push_back(ByteSwap<uint16_t>(text[len]));
        ++len;
    }
    testVec.push_back(0);
    thread_local char fmtMultiByte[1024];
    WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(testVec.data()), -1, fmtMultiByte, sizeof(fmtMultiByte), 0, 0);
    return fmtMultiByte;
}
///
/// Move elsewhere
///
//HE_DebugDrawTextDraw_1
PPC_FUNC_IMPL(__imp__sub_822C6EA8);
PPC_FUNC(sub_822C6EA8)
{    
    auto test = UTF16BE_to_Cstr((const wchar_t*)g_memory.Translate(ctx.r8.u32));
    Reddog::DebugDraw::DrawTextLog(test, 0);
    __imp__sub_822C6EA8(ctx, base);
}
///
/// CObjSpring::OnSetEditorEnter
///
PPC_FUNC_IMPL(__imp__sub_825E8778);
PPC_FUNC(sub_825E8778)
{    
    __imp__sub_825E8778(ctx, base);
}
void TestWindow::Draw()
{
    if (Begin())
    {

        if (Reddog::Button("Start SetEditor1st"))
        {
            GuestToHostFunction<int>(sub_8253A2E0, TestWindow::GamemodeActor, 0);
        }
        if (Reddog::Button("Stop SetEditor1st"))
        {
            GuestToHostFunction<int>(sub_8253A488, TestWindow::GamemodeActor, 0);
        }
        if (Reddog::Button("Start Instancebrush"))
        {
            GuestToHostFunction<int>(sub_8253A638, TestWindow::GamemodeActor, 0);
        }
        if (Reddog::Button("Stop Instancebrush"))
        {
            GuestToHostFunction<int>(sub_8253A7D0, TestWindow::GamemodeActor, 0);
        }
        if (Reddog::Button("Start LoginController"))
        {
            GuestToHostFunction<int>(sub_8253A978, TestWindow::GamemodeActor, 0);
        }
        if (Reddog::Button("Stop LoginController"))
        {
            GuestToHostFunction<int>(sub_8253AB10, TestWindow::GamemodeActor, 0);
        }
        if (Reddog::Button("Start Freecam"))
        {
            GuestToHostFunction<int>(sub_8253ACB8, TestWindow::GamemodeActor, 0);
        }
        if (Reddog::Button("Stop Freecam"))
        {
            GuestToHostFunction<int>(sub_8253ADB8, TestWindow::GamemodeActor, 0);
        }
    }
    End();
}

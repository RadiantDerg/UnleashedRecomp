#include "TestWindow.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
#include <ui/reddog/debug_draw.h>

static TestWindow g_window;


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

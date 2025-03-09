#include "ParamEditorWindowTest.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>

static ParamEditorWindowTest g_window;

void ParamEditorWindowTest::Draw()
{
    if (Begin())
    {
        ImGui::TextColored({ 1, 0, 0, 1 }, "For testing purposes only, use Hedge Mod Manager to toggle these.");
    }
    End();
}

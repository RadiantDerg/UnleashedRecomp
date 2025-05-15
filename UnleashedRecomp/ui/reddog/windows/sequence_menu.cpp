#include "sequence_menu.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
#include <ui/reddog/debug_draw.h>
#include <kernel/heap.h>
#include <gpu/imgui/imgui_snapshot.h>

static SequenceMenu g_window;

void SequenceMenu::Draw()
{
    if (Begin())
    {
        if (ImGui::BeginListBox("##sequences", {-1, -1}))
        {
            for (int i = sequenceLogs.size() - 1; i >= 0; i--)
            {
                ImGui::Text((std::to_string(i + 1) + " " + sequenceLogs[i]).c_str());
            }
            ImGui::EndListBox();
        }
    }
    End();
}

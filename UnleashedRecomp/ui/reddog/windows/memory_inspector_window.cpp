#include "memory_inspector_window.h"
#include <ui/reddog/reddog_controls.h>
#include <imgui_internal.h>
#include "ui/imgui_utils.h"
#include <user/config.h>
#include <SWA.h>

static MemoryInspector g_window;

void MemoryInspector::Draw()
{
    if (Begin())
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Grey Out Zeroes", NULL, m_IsGreyOutZeroes))
                    m_IsGreyOutZeroes = !m_IsGreyOutZeroes;

                ImGui::Separator();

                ImGui::TextColored(GUI_TEXTCOLOR_WARN, "-- Danger Zone --");
                if (ImGui::MenuItem("Allow Editing", NULL, !m_IsMemoryReadOnly))
                    m_IsMemoryReadOnly = !m_IsMemoryReadOnly;

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
    End();
}

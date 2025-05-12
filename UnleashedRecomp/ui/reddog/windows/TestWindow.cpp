#include "TestWindow.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
#include <ui/reddog/debug_draw.h>
#include <kernel/heap.h>
#include <gpu/imgui/imgui_snapshot.h>

static TestWindow g_window;

bool t;
uint32_t GetVtable(void* in_Ptr)
{
    return *reinterpret_cast<be<uint32_t>*>(in_Ptr);
}
void TestWindow::Draw()
{
    ImFont* font = ImFontAtlasSnapshot::GetFont("FOT-SeuratPro-M.otf");
    float defaultScale = font->Scale;
    font->Scale = ImGui::GetDefaultFont()->FontSize / font->FontSize;
    ImGui::PushFont(font);
    if (ImGui::Begin("Nextino"))
    {
        if (Reddog::Button("Fetch Object List"))
        {
            t = true;
        }
        if (t)
        {
            auto m_pSetObjectManager = (SWA::CSetObjectManager*)SWA::CGameDocument::GetInstance()->m_pMember->m_pSetObjectManager.get();
            auto whatever = m_pSetObjectManager->m_pMember->m_spFactory->m_MakeList;
            if (ImGui::BeginListBox("Objects"))
            {
                for (auto& m_ObjName : m_pSetObjectManager->m_pMember->m_spFactory->m_NameToMakeListIndexMap)
                {
                    ImGui::Text(m_ObjName.first.c_str());
                }
                ImGui::EndListBox();
            }

            guest_stack_var<boost::shared_ptr<SWA::CSetObjectEntry>> spEntry;
            m_pSetObjectManager->m_pMember->m_spFactory->m_MakeList[2].SpawnObject(spEntry);

            guest_stack_var<SWA::CEditParam> pEditParam;
            spEntry->get()->m_spSetObjectInfo->spSetObjectListener->InitializeEditParam(pEditParam);
            for (auto& pParam : pEditParam->m_ParamList)
            {
                auto in = GetVtable(pParam.get());
                auto namee = pParam->m_Name.c_str();
                printf("");
            }
        }
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
    ImGui::PopFont();
    ImGui::End();
}

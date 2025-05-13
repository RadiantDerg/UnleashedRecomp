#include "TestWindow.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
#include <ui/reddog/debug_draw.h>
#include <kernel/heap.h>
#include <gpu/imgui/imgui_snapshot.h>

static TestWindow g_window;

bool t;
int testingIdx;
uint32_t GetVtable(void* in_Ptr)
{
    return *reinterpret_cast<be<uint32_t>*>(in_Ptr);
}
std::string GetParamType(uint32_t in_Vtable)
{
    switch (in_Vtable)
    {
    case 0x8203C31C:
        return "[TYPELIST]";
    case 0x8203C374:
        return "[FLOAT]";
    case 0x8203C234:
        return "[TARGET_LIST]";
    case 0x8203C184:
        return "[TARGET]";
    case 0x8203C05C:
        return "[LIST_SETID]";
    case 0x8203C0E4:
        return "[POSITION]";
    case 0x8203BE54:
        return "[DUMMY]";
    case 0x8203BDAC:
        return "[BOOL]";
    case 0x8203C40C:
        return "[ULONG]";
    case 0x8203C4BC:
        return "[INT]";
    case 0x8203C464:
        return "[LONG]";
    }
    return std::format("[{:x}]", in_Vtable);
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
            if (ImGui::BeginListBox("Objects"))
            {
                for (auto& m_ObjName : m_pSetObjectManager->m_pMember->m_spFactory->m_NameToMakeListIndexMap)
                {
                    ImGui::Text(m_ObjName.first.c_str());
                }
                ImGui::EndListBox();
            }
            ImGui::InputInt("ObjIdx", &testingIdx);

            guest_stack_var<boost::shared_ptr<SWA::CSetObjectEntry>> spEntry;
            m_pSetObjectManager->m_pMember->m_spFactory->m_MakeList[testingIdx].SpawnObject(spEntry);

            guest_stack_var<SWA::CEditParam> pEditParam;
            spEntry->get()->m_spSetObjectInfo->spSetObjectListener->InitializeEditParam(pEditParam);
            if (ImGui::Button("Try Spawn"))
                SWA::CGameDocument::GetInstance()->AddGameObject(spEntry->get()->m_spSetObjectInfo->spGameObject, guest_stack_var < Hedgehog::Base::CSharedString>("main"));

            if (ImGui::BeginListBox("tEST"))
            {
                for (auto& m_ObjName : m_pSetObjectManager->m_pMember->m_spFactory->m_NameToMakeListIndexMap)
                {
                    if (m_ObjName.second == testingIdx)
                    {
                        ImGui::Text(m_ObjName.first.c_str());
                        break;
                    }
                }
                for (auto& pParam : pEditParam->m_ParamList)
                {
                    auto in = GetVtable(pParam.get());
                    auto namee = pParam->m_Name.c_str();
                    ImGui::Text((std::string(namee) + GetParamType(in)).c_str());
                }
                ImGui::EndListBox();
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

#include "ParamEditorWindowTest.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
static ParamEditorWindowTest g_window;
static std::string GetString(const char* value)
{
    if (!value || value == (const char*)0x13E0DC0)
        return std::string();

    WCHAR wideChar[1024];
    CHAR multiByte[1024];

    MultiByteToWideChar(932, 0, value, -1, wideChar, 1024);
    WideCharToMultiByte(CP_UTF8, 0, wideChar, -1, multiByte, 1024, 0, 0);

    return std::string(multiByte);
}
const char* __fmt__ = "[%s]    %s    \"%s\"";
void ParamEditorWindowTest::Draw()
{
    if (Begin())
    {
        auto paramEditor = SWA::CGameDocument2::GetInstance()->m_pMember->m_spParameterEditor2nd;
        auto listTest = paramEditor.get()->m_spGlobalParameterManager;
        auto list = listTest->m_GlobalParameterFileList;
        auto t = list.size();
        
        for (auto e : list)
        {
            if (ImGui::CollapsingHeader(GetString(e->m_Name.c_str()).c_str()))
            {
                // Group nodes
                for (const auto& prmGroup : e->m_Children)
                {
                    auto name = GetString(prmGroup.get()->m_Description.c_str());
                    //auto type = GetVTableType(*reinterpret_cast<size_t*>(prmGroup.get()));
                    //ImGui::Text(name.c_str());

                    // Category nodes
                    if (ImGui::TreeNode(name.c_str()))
                    {

                        for (boost::shared_ptr<SWA::CAbstractParameter> prmList : prmGroup->m_Children)
                        {
                            auto name2 = GetString(prmList.get()->m_Description.c_str());
                            //type = GetVTableType(*reinterpret_cast<size_t*>(prmList.get()));

                            //if (ImGui::TreeNode(name2.c_str()))
                            //{
                            //    auto test = prmList.get()->m_Children.size();
                            //    for (boost::shared_ptr<SWA::CAbstractParameter> prmList2 : prmList->m_Children)
                            //    {
                            //        auto name2 = GetString(prmList2.get()->m_Description.c_str());
                            //        //type = GetVTableType(*reinterpret_cast<size_t*>(prmList.get()));
                            //        auto test = prmList2->m_pEditParam;
                            //        // Parameter nodes
                            //        for (const auto& parameter : prmList2->m_pEditParam->m_ParamList)
                            //        {
                            //            ImGui::TextColored(ImVec4(1, 0, 0, 1), parameter->m_Name.c_str());
                            //        }
                            //    }
                            //    ImGui::TreePop();
                            //}
                        }
                        ImGui::TreePop();
                    }
                }
            }
        }
    }
    End();
}

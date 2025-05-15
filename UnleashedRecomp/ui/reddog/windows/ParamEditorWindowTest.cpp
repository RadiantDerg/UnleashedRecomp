#include "ParamEditorWindowTest.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
#include <ui/reddog/editparam_draw.h>
#include <gpu/imgui/imgui_snapshot.h>


static ParamEditorWindowTest g_window;



const char* __fmt__ = "[%s]    %s    \"%s\"";
bool m_AcUseJapanese = false;
int m_AcFileIndex = -1, m_AcCategoryIndex1 = -1, m_AcCategoryIndex2 = -1;
const char* GetTextAccurate(const SWA::CAbstractParameter* param)
{
    return m_AcUseJapanese ? param->m_DisplayName.c_str() : param->m_Name.c_str();
}
void ParamEditorWindowTest::Draw()
{
    if (SWA::CGameDocument2::GetInstance() != nullptr)
    {
        auto paramEditor = SWA::CGameDocument2::GetInstance()->m_pMember->m_spParameterEditor2nd;
        auto list = paramEditor.get()->m_spGlobalParameterManager->m_GlobalParameterFileList;
        if (Config::PreviewBuildControls)
        {
            //?
            Flags = (Reddog::EWindowFlags)(Reddog::eWindowFlags_NoTitleBar | Reddog::eWindowFlags_NoMove | Reddog::eWindowFlags_NoResize);
            ImGui::SetNextWindowPos({ 50,50 });
            // Coords from ParameterEditorWindow.prm.xml
            ImGui::SetNextWindowSize({ 503,630 });
            if (Begin())
            {
                ImGui::Text("【Parameter Editor】");
                ImGui::Text("--------------");
                ImGui::Selectable(" < Save all files >");
                ImGui::Selectable(" < Change settings for all files >");
                if (ImGui::Selectable(" < Temp Go back >"))
                {
                    if (m_AcCategoryIndex2 != -1)
                    {
                        m_AcCategoryIndex2 = -1;
                    }
                    else 
                    {
                        if (m_AcCategoryIndex1 != -1)
                        {
                            m_AcCategoryIndex1 = -1;
                        }
                        else 
                        {
                            m_AcFileIndex = -1;
                        }
                    }
                }
                Reddog::Checkbox("Use Japanese", &m_AcUseJapanese);
                ImGui::Text("--------------");
                if (m_AcFileIndex == -1)
                {
                    for (size_t i = 0; i < list.size(); i++)
                    {
                        Reddog::CEditParamDraw::DrawEditParam(list[i]->m_pEditParam);
                        if (ImGui::Selectable(Reddog::CEditParamDraw::GetString(GetTextAccurate(list[i].get())).c_str()))
                        {
                            m_AcFileIndex = i;
                        }
                    }
                }
                else
                {
                    if (m_AcCategoryIndex1 == -1)
                    {
                        // Group nodes
                        for (size_t i = 0; i < list[m_AcFileIndex]->m_Children.size(); i++)
                        {
                            auto name = Reddog::CEditParamDraw::GetString(GetTextAccurate(list[m_AcFileIndex]->m_Children[i].get()));
                            if (ImGui::Selectable((name + " []").c_str()))
                            {
                                m_AcCategoryIndex1 = i;
                            }   
                        }
                    }
                    else 
                    {
                        if (m_AcCategoryIndex2 == -1)
                        {
                            for (size_t i = 0; i < list[m_AcFileIndex]->m_Children[m_AcCategoryIndex1]->m_Children.size(); i++)
                            {
                                const boost::shared_ptr<SWA::CAbstractParameter>& prmList = list[m_AcFileIndex]->m_Children[m_AcCategoryIndex1]->m_Children[i];
                                SWA::CParameterCategory2* categ = (SWA::CParameterCategory2*)prmList.get();
                                auto name2 = Reddog::CEditParamDraw::GetString(GetTextAccurate(prmList.get()));
                                //type = GetVTableType(*reinterpret_cast<size_t*>(prmList.get()));

                                uint32_t vtable = *reinterpret_cast<be<uint32_t>*>(prmList.get());
                                if (ImGui::Selectable((name2 + " []").c_str()))
                                {
                                    m_AcCategoryIndex2 = i;
                                }
                            }
                        }
                        else 
                        {
                            auto& editParam = list[m_AcFileIndex]->m_Children[m_AcCategoryIndex1]->m_Children[m_AcCategoryIndex2]->m_pEditParam;
                            Reddog::CEditParamDraw::DrawEditParam(editParam);
                        }
                    }
                }
            }
            End();
        }
        else
        {
            Flags = Reddog::eWindowFlags_None;
            if (Begin())
            {
                if (ImGui::BeginListBox("##parameditor", ImVec2(-1, -1)))
                {
                    for (auto e : list)
                    {
                        Reddog::CEditParamDraw::DrawEditParam(e->m_pEditParam);
                        if (ImGui::CollapsingHeader(Reddog::CEditParamDraw::GetString(e->m_Name.c_str()).c_str()))
                        {
                            // Group nodes
                            for (const auto& prmGroup : e->m_Children)
                            {
                                auto name = Reddog::CEditParamDraw::GetString(prmGroup.get()->m_Name.c_str());
                                //auto type = GetVTableType(*reinterpret_cast<size_t*>(prmGroup.get()));
                                //ImGui::Text(name.c_str());

                                Reddog::CEditParamDraw::DrawEditParam(prmGroup->m_pEditParam);
                                // Category nodes
                                if (ImGui::TreeNode(name.c_str()))
                                {
                                    //ParamCategory
                                    for (const boost::shared_ptr<SWA::CAbstractParameter>& prmList : prmGroup->m_Children)
                                    {
                                        SWA::CParameterCategory2* categ = (SWA::CParameterCategory2*)prmList.get();
                                        auto name2 = Reddog::CEditParamDraw::GetString(prmList.get()->m_Name.c_str());
                                        //type = GetVTableType(*reinterpret_cast<size_t*>(prmList.get()));

                                        uint32_t vtable = *reinterpret_cast<be<uint32_t>*>(prmList.get());
                                        if (ImGui::TreeNode(name2.c_str()))
                                        {
                                            // ...Parameter nodes?
                                            Reddog::CEditParamDraw::DrawEditParam(prmList->m_pEditParam);
                                            ImGui::TreePop();
                                        }
                                    }
                                    ImGui::TreePop();
                                }
                            }
                        }
                    }
                    ImGui::EndListBox();
                }

            }
            End();
        }
    }
}

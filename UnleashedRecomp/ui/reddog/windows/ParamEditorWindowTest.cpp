#include "ParamEditorWindowTest.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>
#include <SWA.h>
#include <ui/reddog/editparam_draw.h>


static ParamEditorWindowTest g_window;



const char* __fmt__ = "[%s]    %s    \"%s\"";


void ParamEditorWindowTest::Draw()
{
    if (Begin())
    {
        if (SWA::CGameDocument2::GetInstance() == nullptr)
            return End();

        auto paramEditor = SWA::CGameDocument2::GetInstance()->m_pMember->m_spParameterEditor2nd;
        auto listTest = paramEditor.get()->m_spGlobalParameterManager;
        auto list = listTest->m_GlobalParameterFileList;
        auto t = list.size();
        //paramEditor->m_Field12C = true;
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

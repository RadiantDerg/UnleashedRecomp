#include <api/SWA.h>
#include "editparam_draw.h"
#include "reddog_controls.h"

void Reddog::CEditParamDraw::DrawEditParam(const boost::shared_ptr<SWA::CEditParam>& in_EditParam)
{
    if (in_EditParam.get() == nullptr)
        return;

    for (const auto& prmList2 : in_EditParam->m_ParamList)
    {
        EParamType type = (EParamType)(*reinterpret_cast<be<uint32_t>*>(prmList2.get())).get();
        switch (type)
        {
        case BOOL:
        {
            SWA::CParamBool* pParam = (SWA::CParamBool*)prmList2.get();
            Reddog::Checkbox(pParam->m_Name.c_str(), pParam->GetParamBool());
            //ImGui::SameLine();
            //ImGui::Checkbox(param->m_Name.c_str(), &param->m_FieldIdk2);
            ImGui::SetItemTooltip(Reddog::CEditParamDraw::GetString(pParam->m_Field30.c_str()).c_str());
            break;
        }
        case FLOAT:
        {
            SWA::CParamValue<be<float>>* pParam = (SWA::CParamValue<be<float>>*)prmList2.get();
            float value = pParam->m_pFuncData.ptr->get();
            char buf[128];
            sprintf(buf, "%s [%.2f - %.2f]", pParam->m_Name.c_str(), pParam->m_pFuncData.m_Minimum.get(), pParam->m_pFuncData.m_Maximum.get());
            Reddog::InputFloat(buf, &value);
            *pParam->m_pFuncData.ptr = value;
            if(pParam->m_Field30.get() != nullptr && (int)pParam->m_Field30.get() > 300000000) // Incredibly hacky crash fix.
                ImGui::SetItemTooltip(Reddog::CEditParamDraw::GetString(pParam->m_Field30.c_str()).c_str());
            break;
        }
        case LONG:
        {
            SWA::CParamValue<be<long>>* pParam = (SWA::CParamValue<be<long>>*)prmList2.get();
            //This might not be a good idea, but using inputscalar gave huge numbers
            int value = pParam->m_pFuncData.ptr->get();
            Reddog::InputInt(pParam->m_Name.c_str(), &value);
            *pParam->m_pFuncData.ptr = value;
            break;
        }
        case ULONG:
        {
            SWA::CParamValue<be<unsigned long>>* pParam = (SWA::CParamValue<be<unsigned long>>*)prmList2.get();
            //This might not be a good idea, but using inputscalar gave huge numbers
            uint32_t value = pParam->m_pFuncData.ptr->get();
            Reddog::InputScalar(pParam->m_Name.c_str(), ImGuiDataType_U32, &value);
            *pParam->m_pFuncData.ptr = value;
            break;
        }
        case INT:
        {
            SWA::CParamValue<be<int>>* pParam = (SWA::CParamValue<be<int>>*)prmList2.get();
            int value = pParam->m_pFuncData.ptr->get();
            Reddog::InputInt(pParam->m_Name.c_str(), &value);
            *pParam->m_pFuncData.ptr = value;
            break;
        }
        case TYPE_LIST:
        {
            SWA::CParamTypeList* pParam = (SWA::CParamTypeList*)prmList2.get();
            SWA::CParamTypeList::CMember* member = pParam->GetMember();
            auto values = pParam->GetMember()->m_ValueMap;
            auto ptr = member->m_Field50.get();
            if (ImGui::BeginCombo(pParam->m_Name.c_str(), GetString(member->m_ValueMap.at((int)*member->m_Field50).c_str()).c_str()))
            {
                for (auto& a : member->m_ValueMap)
                {
                    if (!ImGui::Selectable(GetString(a.second.c_str()).c_str()))
                        continue;

                    *member->m_Field50 = a.first;
                    member->m_Field54 = a.first;
                    member->m_Field58 = a.first;
                    //pParam->ApplyValue();
                }

                ImGui::EndCombo();
            };
            for (auto& val : values)
            {
                auto valname = val.second.c_str();
                printf("");
            }
            break;
        }
        default:
        {
            ImGui::Text(prmList2->m_Name.c_str());
            break;
        }
        }
    }
}

inline std::string Reddog::CEditParamDraw::GetString(const char* value)
{
    if (!value || value == (const char*)0x13E0DC0)
        return std::string();

    WCHAR wideChar[1024];
    CHAR multiByte[1024];

    MultiByteToWideChar(932, 0, value, -1, wideChar, 1024);
    WideCharToMultiByte(CP_UTF8, 0, wideChar, -1, multiByte, 1024, 0, 0);

    return std::string(multiByte);
}

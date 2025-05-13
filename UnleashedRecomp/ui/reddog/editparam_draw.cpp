#include <api/SWA.h>
#include "editparam_draw.h"
class CParamBool : public SWA::CParamBase
{
public:
    be<bool> m_FieldIdk;
    be<bool> m_FieldIdk2;
    SWA_INSERT_PADDING(0x20);
    Hedgehog::Base::CSharedString m_Field30;
    bool* GetParamBool()
    {
        return (bool*)m_pVariable.get();
    };
};
template<typename T>
class CParamValue : public SWA::CParamBase
{
public:
    class FuncData
    {
    public:
        xpointer<T> ptr;
        T float4;
        T current;
        T max;
        T defaultval;
        T float14;
    };
    SWA_INSERT_PADDING(0x4);
    FuncData m_pFuncData;
    Hedgehog::Base::CSharedString m_Field30;
};
SWA_ASSERT_OFFSETOF(CParamValue<int>, m_pFuncData, 0x10);
//SWA_ASSERT_OFFSETOF(CParamValue<int>, m_Field30, 0x38);
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
            CParamBool* param = (CParamBool*)prmList2.get();
            auto test = param->m_FieldIdk;
            ImGui::Checkbox(param->m_Name.c_str(), param->GetParamBool());
            //ImGui::SameLine();
            //ImGui::Checkbox(param->m_Name.c_str(), &param->m_FieldIdk2);
            ImGui::SetItemTooltip(Reddog::CEditParamDraw::GetString(param->m_Field30.c_str()).c_str());
            break;
        }
        case FLOAT:
        {
            CParamValue<be<float>>* param = (CParamValue<be<float>>*)prmList2.get();
            float value = param->m_pFuncData.ptr->get();
            ImGui::DragFloat(param->m_Name.c_str(), &value);
            *param->m_pFuncData.ptr = value;
            break;
        }
        case LONG:
        {
            CParamValue<be<long>>* param = (CParamValue<be<long>>*)prmList2.get();
            float value = param->m_pFuncData.ptr->get();
            ImGui::InputScalar(param->m_Name.c_str(), ImGuiDataType_S64, &value);
            *param->m_pFuncData.ptr = value;
            break;
        }
        case ULONG:
        {
            CParamValue<be<unsigned long>>* param = (CParamValue<be<unsigned long>>*)prmList2.get();
            float value = param->m_pFuncData.ptr->get();
            ImGui::InputScalar(param->m_Name.c_str(), ImGuiDataType_U64, &value);
            *param->m_pFuncData.ptr = value;
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

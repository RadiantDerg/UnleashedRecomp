#pragma once

namespace Reddog
{
    enum class EButtonTextAlignment
    {
        Left,
        Center,
        Right
    };

    void InitControlsResources();
    bool Button(const char* label);
    bool InputInt(const char* label, int* v);
    bool InputFloat(const char* label, float* v);
    bool InputScalar(const char* label, ImGuiDataType type, void* pData);
    bool Checkbox(const char* label, bool* v);
    bool ExplicitButton(const char* label, EButtonTextAlignment textAlignment = EButtonTextAlignment::Center, const ImVec2& size = { 0, 0 }, float fontScale = 1);
    void Separator(float upperPadding = 0, float lowerPadding = 0);
}

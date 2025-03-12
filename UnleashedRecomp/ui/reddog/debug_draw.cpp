#include <api/SWA.h>
#include <ui/reddog/debug_draw.h>
#include "ui/imgui_utils.h"
#include <user/config.h>


namespace Reddog
{
    bool operator<= (const ImVec2& a, const ImVec2& b) { return a.x <= b.x && a.y <= b.y; }
    bool operator>= (const ImVec2& a, const ImVec2& b) { return a.x >= b.x && a.y >= b.y; }
    Hedgehog::Math::CVector4 operator /= (const Hedgehog::Math::CVector4& vec4, float divisor)
    {
        auto result = vec4;

        if (result.X.value != 0.0f)
            result.X.value /= divisor;
        if (result.Y.value != 0.0f)
            result.Y.value /= divisor;
        if (result.Z.value != 0.0f)
            result.Z.value /= divisor;
        if (result.W.value != 0.0f)
            result.W.value /= divisor;

        return result;
    }

    Hedgehog::Math::CVector4 operator* (const Hedgehog::Math::CMatrix& m33, const Hedgehog::Math::CVector4& v4)
    {
        return {
            m33.m_M00 * v4.X + m33.m_M01 * v4.Y + m33.m_M02 * v4.Z, // Row 1
            m33.m_M10 * v4.X + m33.m_M11 * v4.Y + m33.m_M12 * v4.Z, // Row 2
            m33.m_M20 * v4.X + m33.m_M21 * v4.Y + m33.m_M22 * v4.Z, // Row 3
            v4.W 
            };
    }

    Hedgehog::Math::CVector4 operator* (const Hedgehog::Math::CMatrix44& m44, const Hedgehog::Math::CVector4& v4)
    {
        return {
            m44.data[0] * v4.X + m44.data[1] * v4.Y + m44.data[2] * v4.Z + m44.data[3] * v4.W,      // Row 1
            m44.data[4] * v4.X + m44.data[5] * v4.Y + m44.data[6] * v4.Z + m44.data[7] * v4.W,      // Row 2
            m44.data[8] * v4.X + m44.data[9] * v4.Y + m44.data[10] * v4.Z + m44.data[11] * v4.W,    // Row 3
            m44.data[12] * v4.X + m44.data[13] * v4.Y + m44.data[14] * v4.Z + m44.data[15] * v4.W   // Row 4
        };
    }

    ImVec2 GetNDCCoordinate(const Vector3& in_rPosition)
    {
        auto& res = ImGui::GetIO().DisplaySize;

        if (Reddog::DebugDraw::ms_pCamera != 0)
        {
            // Reference
            //const auto camera = SWA::CGameDocument::GetInstance()->GetWorld()->GetCamera();
            //Hedgehog::Math::CVector4 ndc = camera->m_MyCamera.m_View * Hedgehog::Math::CVector4(in_rVec.x, in_rVec.y, in_rVec.z, 1.0f);
            //ndc = camera->m_MyCamera.m_Projection * ndc;

            const auto camera = Reddog::DebugDraw::ms_pCamera;

            // Expand matrix to 4x4
            Hedgehog::Math::CMatrix44 test {
                camera->m_MyCamera.m_View.m_M00, camera->m_MyCamera.m_View.m_M01, camera->m_MyCamera.m_View.m_M02, 0.0f,
                camera->m_MyCamera.m_View.m_M10, camera->m_MyCamera.m_View.m_M11, camera->m_MyCamera.m_View.m_M12, 0.0f,
                camera->m_MyCamera.m_View.m_M20, camera->m_MyCamera.m_View.m_M21, camera->m_MyCamera.m_View.m_M22, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            // Multiply
            Hedgehog::Math::CVector4 bruh(test * Hedgehog::Math::CVector4(in_rPosition.x, in_rPosition.y, in_rPosition.z, 1.0f));

            // I think it all goes downhill from here...
            Hedgehog::Math::CVector4 ndc = camera->m_MyCamera.m_Projection * bruh;


            if (ndc.W > 0.0f) // Check if given position is in front of the camera
            {
                // normalize
                if (ndc.W != 0.0f)
                    ndc /= ndc.W;

                const ImVec2 screen_pos = {
                    res.x / 2 * (1 - -ndc.X), // x
                    res.y / 2 * (1 - ndc.Y)  // y
                };

                return screen_pos; // Return screen coordinates
            }
        }

        return { -1, -1 }; // Return invalid
    }


    void Exec_DrawLines(ImDrawList* drawList, ImVec2 canvasSize, float delta)
    {
        if (DebugDraw::ms_LineList.empty())
            return;

        for (auto line : DebugDraw::ms_LineList)
        {
            const auto start = GetNDCCoordinate(line.Start);
            const auto end = GetNDCCoordinate(line.End);

            // Prevent wrap around
            if (start >= ImVec2(0, 0) && end >= ImVec2(0, 0)
                && start <= canvasSize && end <= canvasSize)
            {
                drawList->AddLine(start, end, line.Colour, 2.5f);
            }
        }

        DebugDraw::ms_LineList.clear();
    }


    void Exec_DrawScreenText(ImDrawList* drawList, ImVec2 canvasSize, float delta, ImFont* font)
    {
        if (DebugDraw::ms_FreeTextList.empty())
            return;

        auto fontSize = Scale(12.0f);

        for (auto freeText = DebugDraw::ms_FreeTextList.begin(); freeText != DebugDraw::ms_FreeTextList.end();)
        {
            // Draw as overlay
            if ((freeText->Flags & eDrawTextFlags_Overlay) == eDrawTextFlags_Overlay)
                drawList = ImGui::GetForegroundDrawList();
            else
                drawList = ImGui::GetBackgroundDrawList();

            // Draw only on valid screen coordinates
            if (freeText->Position >= ImVec2(0, 0) && freeText->Position <= canvasSize)
            {
                if ((freeText->Flags & eDrawTextFlags_NoShadow) == eDrawTextFlags_NoShadow)
                    drawList->AddText(font, fontSize * freeText->Scale, freeText->Position, freeText->Colour, freeText->Text.c_str());
                else
                    DrawTextWithShadow(drawList, font, fontSize * freeText->Scale, freeText->Position, freeText->Colour, freeText->Text.c_str(), 1.0f, 1.0f, IM_COL32(0, 0, 0, 128));
            }

            // Decrement timer
            freeText->Time -= delta;

            // Remove if expired
            if (freeText->Time < 0.0f)
                freeText = DebugDraw::ms_FreeTextList.erase(freeText);
        }
    }


    void Exec_DrawLogText(ImDrawList* drawList, ImVec2 canvasSize, float delta, ImFont* font)
    {
        if (DebugDraw::ms_LogTextList.empty())
            return;

        constexpr ImGuiWindowFlags flags{ 0
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoFocusOnAppearing
            | ImGuiWindowFlags_AlwaysAutoResize
        };

        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));

        ImGui::Begin("Status", nullptr, flags);
        ImGui::PushFont(font);
        ImGui::SetWindowFontScale(Scale(0.4f));

        for (auto logText = DebugDraw::ms_LogTextList.begin(); logText != DebugDraw::ms_LogTextList.end();)
        {
            bool useColor = logText->Colour != (ImU32)-1;

            if (useColor)
                ImGui::PushStyleColor(ImGuiCol_Text, logText->Colour);

            ImGui::TextUnformatted(logText->Text.c_str());

            if (useColor)
                ImGui::PopStyleColor();

            // Decrement timer
            logText->Time -= delta;

            // Remove if expired
            if (logText->Time < 0.0f)
                logText = DebugDraw::ms_LogTextList.erase(logText);

            if (logText != DebugDraw::ms_LogTextList.end())
                ImGui::Separator();
        }

        ImGui::SetWindowPos({ canvasSize.x - Scale(2) - ImGui::GetWindowSize().x, Scale(2) });
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor();
        ImGui::PopFont();
        ImGui::End();
    }


    void DebugDraw::DrawLine(const SDrawLine& in_rLine)
    {
        if (Config::Debug && !ms_IsRendering && GetIsDrawDebug())
            ms_LineList.push_back(in_rLine);
    }

    void DebugDraw::DrawText2D(const SDrawText& in_rText)
    {
        if (Config::Debug && !ms_IsRendering && GetIsDrawText())
            ms_FreeTextList.push_back(in_rText);
    }

    void DebugDraw::DrawText2D(const SDrawText& in_rText, const Vector3& in_rPosition)
    {
        if (Config::Debug && !ms_IsRendering && GetIsDrawText())
        {
            auto txt = in_rText;
            txt.Position = GetNDCCoordinate(in_rPosition);
            ms_FreeTextList.push_back(txt);
        }
    }

    void DebugDraw::DrawTextLog(const SDrawText& in_rText)
    {
        if (Config::Debug && !ms_IsRendering && GetIsDrawText())
            ms_LogTextList.push_back(in_rText);
    }

    void DebugDraw::DrawTextLog(const char* in_Text, float in_Time, ImU32 in_Colour, ImU16 in_Priority)
    {
        if (Config::Debug && !ms_IsRendering && GetIsDrawText())
            ms_LogTextList.push_back({ ImVec2(0,0), in_Text, in_Time, 0, in_Colour, eDrawTextFlags_None, in_Priority });
    }

    void DebugDraw::Render(ImFont* font)
    {
        auto& io = ImGui::GetIO();
        auto& res = io.DisplaySize;
        float deltaTime = io.DeltaTime;
        auto drawList = ImGui::GetBackgroundDrawList();

        /*DrawText2D({ ImVec2(Scale(50), Scale(300)), "TEST1 NO SHADOW", 0, 2, 0xFF37C800, eDrawTextFlags_NoShadow });
        DrawText2D({ ImVec2(Scale(50), Scale(325)), "TEST2 OVERLAY", 0, 2, 0xFF37C800, eDrawTextFlags_Overlay });
        DrawText2D({ ImVec2(Scale(50), Scale(350)), "TEST3 SCALE", 0, 5, 0xFF37C800 });
        DrawTextLog("TEST1 NORMAL");
        DrawTextLog("TEST2 COLORED", 0, 0xFF37C800);*/

        auto stats = fmt::format("== Stats ==\nLines: {}\nTexts: {}\nLogs: {}", ms_LineList.size(), ms_FreeTextList.size(), ms_LogTextList.size());
        SDrawText text = { ImVec2(Scale(40), Scale(75)), stats, 0, 0.75f };
        DrawText2D(text);

        ms_IsRendering = true;

        Exec_DrawLines(drawList, res, deltaTime);
        Exec_DrawScreenText(nullptr, res, deltaTime, font);
        Exec_DrawLogText(drawList, res, deltaTime, font);

        ms_IsRendering = false;
    }

    bool DebugDraw::GetIsDrawDebug()
    {
        return *SWA::SGlobals::ms_IsRenderDebugDraw;
    }

    bool DebugDraw::GetIsDrawText()
    {
        return *SWA::SGlobals::ms_IsRenderDebugDrawText;
    }

    bool DebugDraw::GetIsDrawPosition()
    {
        return *SWA::SGlobals::ms_IsRenderDebugPositionDraw;
    }
}

#include "reddog_window.h"
#include <gpu/video.h>
#include <ui/reddog/reddog_manager.h>
#include <ui/game_window.h>
#include <ui/imgui_utils.h>

#include <res/images/reddog/button_close_1.dds.h>
#include <res/images/reddog/button_close_2.dds.h>
#include <res/images/reddog/button_minimum_1.dds.h>
#include <res/images/reddog/button_minimum_2.dds.h>
#include <res/images/reddog/button_pin_1.dds.h>
#include <res/images/reddog/button_pin_2.dds.h>
#include <res/images/reddog/common_icon.dds.h>
#include <res/images/reddog/title_bar.dds.h>

constexpr float MIN_WINDOW_SIZE = 21.0f;

static std::unique_ptr<GuestTexture> g_upButtonClose1;
static std::unique_ptr<GuestTexture> g_upButtonClose2;
static std::unique_ptr<GuestTexture> g_upButtonMinimum1;
static std::unique_ptr<GuestTexture> g_upButtonMinimum2;
static std::unique_ptr<GuestTexture> g_upButtonPin1;
static std::unique_ptr<GuestTexture> g_upButtonPin2;
static std::unique_ptr<GuestTexture> g_upCommonIcon;
static std::unique_ptr<GuestTexture> g_upTitleBar;
static std::unique_ptr<GuestTexture> g_upWindowFrame;

float g_defaultFontScale;
ImGuiStyle g_defaultStyle;

static std::vector<Reddog::IWindow*>& Reddog::GetWindows()
{
    static std::vector<Reddog::IWindow*> g_windows;
    return g_windows;
}

void Reddog::InitWindowResources()
{
    g_defaultFontScale = Reddog::Manager::ms_pFont->Scale;

    auto& style = ImGui::GetStyle();
    g_defaultStyle = style;

    g_upButtonClose1 = LoadTexture(g_button_close_1, sizeof(g_button_close_1));
    g_upButtonClose2 = LoadTexture(g_button_close_2, sizeof(g_button_close_2));
    g_upButtonMinimum1 = LoadTexture(g_button_minimum_1, sizeof(g_button_minimum_1));
    g_upButtonMinimum2 = LoadTexture(g_button_minimum_2, sizeof(g_button_minimum_2));
    g_upButtonPin1 = LoadTexture(g_button_pin_1, sizeof(g_button_pin_1));
    g_upButtonPin2 = LoadTexture(g_button_pin_2, sizeof(g_button_pin_2));
    g_upCommonIcon = LoadTexture(g_common_icon, sizeof(g_common_icon));
    g_upTitleBar = LoadTexture(g_title_bar, sizeof(g_title_bar));
}

void Reddog::Window::BeginStyle()
{
    Reddog::Manager::ms_pFont->Scale = ImGui::GetDefaultFont()->FontSize / Reddog::Manager::ms_pFont->FontSize;

    ImGui::PushFont(Reddog::Manager::ms_pFont);

    UpdateStyle();
}

void Reddog::Window::UpdateStyle()
{
    auto& style = ImGui::GetStyle();

    style.WindowMinSize = { 0, MIN_WINDOW_SIZE };

    auto colBg = IsFocused
        ? ImGui::ColorConvertU32ToFloat4(ActiveColour)
        : ImGui::ColorConvertU32ToFloat4(InactiveColour);

    auto colBlack = ImVec4(0, 0, 0, 1);
    auto colTrans = ImVec4(0, 0, 0, 0);
    auto colWhite = ImVec4(1, 1, 1, 1);

    style.Colors[ImGuiCol_Border] = colTrans;
    style.Colors[ImGuiCol_Button] = colTrans;
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 1);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.675f, 0.675f, 0.675f, 1);
    style.Colors[ImGuiCol_CheckMark] = colBlack;
    style.Colors[ImGuiCol_ChildBg] = colBg;
    style.Colors[ImGuiCol_FrameBg] = colBg;
    style.Colors[ImGuiCol_PopupBg] = colBg;
    style.Colors[ImGuiCol_ResizeGrip] = colTrans;
    style.Colors[ImGuiCol_ResizeGripActive] = colTrans;
    style.Colors[ImGuiCol_ResizeGripHovered] = colTrans;
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.6f, 0.6f, 0.6f, 1);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1, 0.6f, 0, 1);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1, 0.6f, 0, 1);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1, 0.6f, 0, 1);
    style.Colors[ImGuiCol_SeparatorActive] = colTrans;
    style.Colors[ImGuiCol_SeparatorHovered] = colTrans;
    style.Colors[ImGuiCol_SliderGrab] = colWhite;
    style.Colors[ImGuiCol_Tab] = ImVec4(0.467f, 0.467f, 0.467f, 1);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.667f, 0.667f, 0.667f, 1);
    style.Colors[ImGuiCol_Text] = colBlack;
    style.Colors[ImGuiCol_WindowBg] = colBg;
}

void Reddog::Window::EndStyle()
{
    ImGui::PopFont();

    Reddog::Manager::ms_pFont->Scale = g_defaultFontScale;

    ImGui::GetStyle() = g_defaultStyle;
}

void Reddog::Window::CreateWindowGrips()
{
    if (!ImWindow || !IsFocused)
        return;

    auto& io = ImGui::GetIO();

    auto windowPos = ImWindow->Pos;
    auto windowSize = ImWindow->Size;

    constexpr float resizeMargin = 5.0f;

    ImRect grips[6] =
    {
        // Corners
        ImRect(windowPos.x, windowPos.y + windowSize.y - resizeMargin, windowPos.x + resizeMargin, windowPos.y + windowSize.y), // Bottom left
        ImRect(windowPos.x + windowSize.x - resizeMargin, windowPos.y + windowSize.y - resizeMargin, windowPos.x + windowSize.x, windowPos.y + windowSize.y), // Bottom right

        // Edges
        ImRect(windowPos.x + resizeMargin, windowPos.y, windowPos.x + windowSize.x - resizeMargin, windowPos.y + resizeMargin), // Top
        ImRect(windowPos.x + resizeMargin, windowPos.y + windowSize.y - resizeMargin, windowPos.x + windowSize.x - resizeMargin, windowPos.y + windowSize.y), // Bottom
        ImRect(windowPos.x, windowPos.y + resizeMargin, windowPos.x + resizeMargin, windowPos.y + windowSize.y - resizeMargin), // Left
        ImRect(windowPos.x + windowSize.x - resizeMargin, windowPos.y + resizeMargin, windowPos.x + windowSize.x, windowPos.y + windowSize.y - resizeMargin)  // Right
    };

    auto hoveredGrip = -1;

    for (int i = 0; i < IM_ARRAYSIZE(grips); i++)
    {
        auto& grip = grips[i];

        if (ImGui::IsMouseHoveringRect(grip.Min, grip.Max))
            hoveredGrip = i;
    }

    if (hoveredGrip < 0)
    {
        GameWindow::SetCursor(ECursorType::DebugDefault);
    }
    else
    {
        if (hoveredGrip == 0)
        {
            GameWindow::SetCursor(ECursorType::DebugRightDiagonal);
        }
        else if (hoveredGrip == 1)
        {
            GameWindow::SetCursor(ECursorType::DebugLeftDiagonal);
        }
        else if (hoveredGrip == 2 || hoveredGrip == 3)
        {
            GameWindow::SetCursor(ECursorType::DebugVertical);
        }
        else
        {
            GameWindow::SetCursor(ECursorType::DebugHorizontal);
        }
    }
}

void Reddog::Window::DrawFrame()
{
    if (!ImWindow)
        return;

    if ((Flags & eWindowFlags_NoResize) == 0)
        CreateWindowGrips();

    UpdateStyle();

    auto drawList = ImGui::GetWindowDrawList();
    auto windowPos = ImWindow->Pos;
    auto windowSize = ImWindow->Size;
    auto isFocused = ImGui::IsWindowFocused();
    auto hasTitleBar = (Flags & eWindowFlags_NoTitleBar) == 0;

    float titleBarHeight = hasTitleBar ? 20.0f : 0.0f;

    ImVec2 windowMin = { windowPos.x, windowPos.y + titleBarHeight };
    ImVec2 windowMax = { windowPos.x + windowSize.x, windowPos.y + windowSize.y };

    drawList->AddRectFilled(windowMin, windowMax, isFocused ? ActiveColour : InactiveColour);
    drawList->AddLine(windowMin, { windowMax.x, windowMin.y }, IM_COL32(170, 170, 170, 255)); // Outer top line.
    drawList->AddLine({ windowMin.x + 1, windowMin.y + 1 }, { windowMin.x + 1, windowMax.y }, IM_COL32(153, 153, 153, 255)); // Outer left line.
    drawList->AddLine({ windowMin.x, windowMax.y - 2 }, { windowMax.x, windowMax.y - 2 }, IM_COL32(51, 51, 51, 255)); // Outer bottom line.
    drawList->AddLine({ windowMax.x - 2, windowMin.y + 1 }, { windowMax.x - 2, windowMax.y }, IM_COL32(85, 85, 85, 255)); // Outer right line.
    drawList->AddLine({ windowMin.x + 2, windowMin.y + 1 }, { windowMax.x - 2, windowMin.y + 1 }, IM_COL32(51, 51, 51, 255)); // Inner top line.
    drawList->AddLine({ windowMin.x + 2, windowMin.y + 2 }, { windowMin.x + 2, windowMax.y - 2 }, IM_COL32(85, 85, 85, 255)); // Inner left line.
    drawList->AddLine({ windowMin.x + 3, windowMax.y - 3 }, { windowMax.x - 2, windowMax.y - 3 }, IM_COL32(170, 170, 170, 255)); // Inner bottom line.
    drawList->AddLine({ windowMax.x - 3, windowMin.y + 2 }, { windowMax.x - 3, windowMax.y - 3 }, IM_COL32(153, 153, 153, 255)); // Inner right line.

    // Close window if unfocused whilst unpinned (yes it works like this in SU preview).
    if (!IsPinned && !isFocused && m_pIsVisible)
        *m_pIsVisible = !*m_pIsVisible;

    if (hasTitleBar)
    {
        auto height = titleBarHeight;
        auto left = PIXELS_TO_UV_COORDS(32, 32, 0, 0, 3, 32);
        auto centre = PIXELS_TO_UV_COORDS(32, 32, 3, 0, 26, 32);
        auto right = PIXELS_TO_UV_COORDS(32, 32, 29, 0, 3, 32);

        auto colTitleBar = isFocused
            ? IM_COL32(220, 220, 247, 255)
            : IM_COL32(176, 176, 176, 255);

        drawList->AddImage(g_upTitleBar.get(), { windowPos.x, windowPos.y }, { windowPos.x + 3, windowPos.y + height }, GET_UV_COORDS(left), colTitleBar);
        drawList->AddImage(g_upTitleBar.get(), { windowPos.x + 3, windowPos.y }, { windowPos.x + windowSize.x - 3, windowPos.y + height }, GET_UV_COORDS(centre), colTitleBar);
        drawList->AddImage(g_upTitleBar.get(), { windowPos.x + windowSize.x - 3, windowPos.y }, { windowPos.x + windowSize.x, windowPos.y + height }, GET_UV_COORDS(right), colTitleBar);

        auto cmnSize = 16.0f;

        ImVec2 cmnIconMin = { windowPos.x + 9, windowPos.y + 2 };
        ImVec2 cmnIconMax = { cmnIconMin.x + cmnSize, cmnIconMin.y + cmnSize };

        drawList->AddImage(g_upCommonIcon.get(), cmnIconMin, cmnIconMax, { 0, 0 }, { 1, 1 }, IM_COL32_WHITE);

        auto colTitleText = isFocused
            ? IM_COL32_WHITE
            : IM_COL32(190, 190, 190, 255);

        drawList->AddText({ cmnIconMax.x + 5, windowPos.y + 4 }, colTitleText, Name);

        float controlsWidth = 0;
        float controlsMargin = 4;

        ImVec2 closeButtonMin = { windowPos.x + windowSize.x - cmnSize - controlsMargin, windowPos.y + 3 };
        ImVec2 closeButtonMax = { closeButtonMin.x + cmnSize, closeButtonMin.y + cmnSize };

        if (IsCloseVisible)
        {
            controlsWidth += closeButtonMax.x - closeButtonMin.x + controlsMargin;

            ImGui::SetCursorScreenPos(closeButtonMin);
            ImGui::InvisibleButton("##CloseButton", { cmnSize, cmnSize });
            bool isCloseButtonActive = ImGui::IsItemActive();

            if (isCloseButtonActive)
            {
                drawList->AddImage(g_upButtonClose2.get(), closeButtonMin, closeButtonMax);
                m_isCloseButtonPressed = true;
            }
            else
            {
                drawList->AddImage(g_upButtonClose1.get(), closeButtonMin, closeButtonMax);

                if (m_isCloseButtonPressed && m_pIsVisible)
                {
                    *m_pIsVisible = !*m_pIsVisible;
                    m_isCloseButtonPressed = false;
                }
            }
        }

        ImVec2 minimumButtonMin = { windowPos.x + windowSize.x - cmnSize - controlsWidth - controlsMargin, windowPos.y + 3 };
        ImVec2 minimumButtonMax = { minimumButtonMin.x + cmnSize, minimumButtonMin.y + cmnSize };

        if (IsMinimumVisible)
        {
            controlsWidth += minimumButtonMax.x - minimumButtonMin.x + controlsMargin;

            ImGui::SetCursorScreenPos(minimumButtonMin);
            ImGui::InvisibleButton("##MinimumButton", { cmnSize, cmnSize });
            bool isMinimumButtonActive = ImGui::IsItemActive();

            if (isMinimumButtonActive)
            {
                drawList->AddImage(g_upButtonMinimum2.get(), minimumButtonMin, minimumButtonMax);
                m_isMinimumButtonPressed = true;
            }
            else
            {
                drawList->AddImage(g_upButtonMinimum1.get(), minimumButtonMin, minimumButtonMax);

                if (m_isMinimumButtonPressed)
                {
                    SetMinimum(!IsMinimum);
                    m_isMinimumButtonPressed = false;
                }
            }
        }

        ImVec2 pinButtonMin = { windowPos.x + windowSize.x - cmnSize - controlsWidth - controlsMargin, windowPos.y + 3 };
        ImVec2 pinButtonMax = { pinButtonMin.x + cmnSize, pinButtonMin.y + cmnSize };

        if (IsPinVisible)
        {
            controlsWidth += pinButtonMax.x - pinButtonMin.x + controlsMargin;

            ImGui::SetCursorScreenPos(pinButtonMin);
            ImGui::InvisibleButton("##PinButton", { cmnSize, cmnSize });
            bool isPinButtonActive = ImGui::IsItemActive();

            if (isPinButtonActive)
            {
                drawList->AddImage(g_upButtonPin2.get(), pinButtonMin, pinButtonMax);
                m_isPinButtonPressed = true;
            }
            else
            {
                drawList->AddImage(g_upButtonPin1.get(), pinButtonMin, pinButtonMax);

                if (m_isPinButtonPressed)
                {
                    IsPinned = !IsPinned;
                    m_isPinButtonPressed = false;
                }
            }
        }

        ImVec2 titleBarMin = { windowPos.x, windowPos.y };
        ImVec2 titleBarMax = { windowPos.x + windowSize.x - controlsWidth, windowPos.y + titleBarHeight };

        if (ImGui::IsMouseHoveringRect(titleBarMin, titleBarMax, false))
        {
            if (IsMinimumVisible && ImGui::IsMouseDoubleClicked(0))
                SetMinimum(!IsMinimum);
        }
    }
}

void Reddog::Window::SetMinimum(bool isMinimum)
{
    IsMinimum = isMinimum;

    if (!ImWindow)
        return;

    auto windowSize = ImWindow->Size;

    if (IsMinimum)
    {
        m_preMinimumWindowSize = windowSize;
        ImGui::SetWindowSize(ImWindow, { windowSize.x, MIN_WINDOW_SIZE });
    }
    else
    {
        ImGui::SetWindowSize(ImWindow, { windowSize.x, m_preMinimumWindowSize.y });
    }
}

void Reddog::Window::SetVisible(bool isVisible)
{
    IsVisible = m_isSetVisible = isVisible;
}

bool Reddog::Window::Begin(bool* pIsVisible)
{
    if (pIsVisible)
    {
        m_pIsVisible = pIsVisible;
    }
    else
    {
        m_pIsVisible = &IsVisible;
    }

    BeginStyle();

    ImGui::SetNextWindowSizeConstraints({ m_minWidth, m_minHeight }, { m_maxWidth, m_maxHeight });

    auto flags = REDDOG_IMGUI_FLAGS;

    if ((Flags & eWindowFlags_NoResize) != 0)
        flags |= ImGuiWindowFlags_NoResize;

    if (m_preBegin)
        m_preBegin();

    bool result = ImGui::Begin(Name, m_pIsVisible, flags);

    if (m_postBegin)
        m_postBegin();

    if (result && *m_pIsVisible)
    {
        ImWindow = ImGui::GetCurrentWindow();
        IsFocused = ImGui::IsWindowFocused();

        if ((Flags & eWindowFlags_Center) != 0 && !m_isSetCentre)
        {
            auto viewport = ImGui::GetMainViewport();
            auto windowPos = ImGui::GetWindowPos();
            auto pos = ImVec2((viewport->Size.x / 2) - (ImWindow->Size.x / 2), (viewport->Size.y / 2) - (ImWindow->Size.y / 2));

            ImGui::SetWindowPos(pos);
        }

        // Keep centring window until it has been moved.
        if (ImGui::IsMouseDragging(0) && ImGui::IsWindowHovered())
            m_isSetCentre = true;

        if (m_isSetVisible)
        {
            ImGui::SetWindowFocus();
            m_isSetVisible = false;
        }

        DrawFrame();

        return true;
    }

    return false;
}

void Reddog::Window::End()
{
    if (m_preEnd)
        m_preEnd();

    ImGui::End();

    if (m_postEnd)
        m_postEnd();

    EndStyle();
}

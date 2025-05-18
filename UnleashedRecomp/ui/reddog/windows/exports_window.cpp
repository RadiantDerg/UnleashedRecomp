#include "exports_window.h"
#include <ui/reddog/reddog_controls.h>
#include <user/config.h>

static ExportsWindow g_window;

void ExportsWindow::Draw()
{
    if (Begin())
    {
        ImGui::TextColored({ 1, 0, 0, 1 }, "For testing purposes only, use Hedge Mod Manager to toggle these.");

        ImGui::SeparatorText("Audio");
        Reddog::Checkbox("Disable Boost Filter", &Config::DisableBoostFilter.Value);

        ImGui::SeparatorText("Debug");
        Reddog::Checkbox("Enable Event Collision Debug View", &Config::EnableEventCollisionDebugView.Value);
        Reddog::Checkbox("Enable GI Mip Level Debug View", &Config::EnableGIMipLevelDebugView.Value);
        Reddog::Checkbox("Enable Object Collision Debug View", &Config::EnableObjectCollisionDebugView.Value);
        Reddog::Checkbox("Enable Stage Collision Debug View", &Config::EnableStageCollisionDebugView.Value);

        ImGui::SeparatorText("Fixes");
        Reddog::Checkbox("Fix Eggmanland using Event Gallery Transition", &Config::FixEggmanlandUsingEventGalleryTransition.Value);
        Reddog::Checkbox("Fix Unleash Out of Control Drain", &Config::FixUnleashOutOfControlDrain.Value);

        ImGui::SeparatorText("Gameplay");
        Reddog::Checkbox("Allow Cancelling Unleash", &Config::AllowCancellingUnleash.Value);
        Reddog::Checkbox("Disable D-Pad Movement", &Config::DisableDPadMovement.Value);
        Reddog::Checkbox("Homing Attack on Jump", &Config::HomingAttackOnJump.Value);
        Reddog::Checkbox("Save Score at Checkpoints", &Config::SaveScoreAtCheckpoints.Value);

        ImGui::SeparatorText("System");
        Reddog::Checkbox("Boot Directly to Title", &Config::SkipIntroLogos.Value);
        Reddog::Checkbox("Disable Windows 11 Rounded Corners", &Config::DisableDWMRoundedCorners.Value);
        Reddog::Checkbox("Use Official Title on Title Bar", &Config::UseOfficialTitleOnTitleBar.Value);

        ImGui::SeparatorText("UI");
        Reddog::Checkbox("Disable Auto Save Warning", &Config::DisableAutoSaveWarning.Value);
        Reddog::Checkbox("Disable DLC Icon", &Config::DisableDLCIcon.Value);
        Reddog::Checkbox("Toggle UI/HUD on F8", &Config::HUDToggleKey.Value);
        Reddog::Checkbox("Use Arrows for Time of Day Transition", &Config::UseArrowsForTimeOfDayTransition.Value);
        
        ImGui::SeparatorText("Uncategorized");
        Reddog::Checkbox("Show FPS", &Config::ShowFPS.Value);
        Reddog::Checkbox("Show Console", &Config::ShowConsole.Value);
        Reddog::Checkbox("Disable Low Resolution Font On Custom UI", &Config::DisableLowResolutionFontOnCustomUI.Value);

        ImGui::SeparatorText("DebugMenu");
        Reddog::Checkbox("Preview Build style menus", &Config::PreviewBuildControls.Value);
        Reddog::Checkbox("Suppress Criware Warnings", &Config::SilenceCriWarnings.Value);
    }
    End();
}

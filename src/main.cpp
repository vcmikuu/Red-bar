#include "main.hpp"
#include "ModConfig.hpp"

#include "GlobalNamespace/GameEnergyUIPanel.hpp"
#include "GlobalNamespace/GameEnergyCounter.hpp"
#include "GlobalNamespace/IGameEnergyCounter.hpp"
#include "GlobalNamespace/FileDifficultyBeatmap.hpp"
#include "GlobalNamespace/FileDifficultyBeatmap.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/MultiplayerLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/BeatmapKey.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"

#include "RedBarViewController.hpp"

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"

#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"

#include "custom-types/shared/register.hpp"

#include "bsml/shared/BSML.hpp"

#include "scotland2/shared/modloader.h"

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace BSML;

UnityEngine::Material* energyBarMaterialStore = nullptr;
UnityEngine::UI::Image* energyBarStore = nullptr;
float energyy = 0.0;
int pos = 0;

void SetColor(GameEnergyUIPanel* self, float energy) {
    UnityEngine::Color color;

    color = getModConfig().Defhp.GetValue();
    color.a = getModConfig().Alpha.GetValue();
    

    
    UnityEngine::UI::Image* energyBar = self->_energyBar;
    UnityEngine::Material* energyBarMaterial = energyBar->get_material();
    if (energyBarMaterialStore != energyBarMaterial || energyBarStore != energyBar) {
        energyBarMaterialStore = energyBarMaterial;
        energyBarStore = energyBar;
    }
    if (energy != energyy) {
        energyy = energy;
    }


    if(getModConfig().FullFade.GetValue()) {
        double fraction = energy;
        color.r = getModConfig().Diehp.GetValue().r + (getModConfig().Highhp.GetValue().r - getModConfig().Diehp.GetValue().r) * fraction;
        color.g = getModConfig().Diehp.GetValue().g + (getModConfig().Highhp.GetValue().g - getModConfig().Diehp.GetValue().g) * fraction;
        color.b = getModConfig().Diehp.GetValue().b + (getModConfig().Highhp.GetValue().b - getModConfig().Diehp.GetValue().b) * fraction;
    } else {
        if (energy < 0.15) {
            if (!getModConfig().AlwaysRainbow.GetValue()) {
                color.r = getModConfig().Diehp.GetValue().r;
                color.g = getModConfig().Diehp.GetValue().g;
                color.b = getModConfig().Diehp.GetValue().b;
            }
        } else if (energy < 0.5) {
            if (!getModConfig().AlwaysRainbow.GetValue()) {
                double fraction = (energy - 0.15f)*2.8571428f;
                color.r = getModConfig().Diehp.GetValue().r + (getModConfig().Lowhp.GetValue().r - getModConfig().Diehp.GetValue().r) * fraction;
                color.g = getModConfig().Diehp.GetValue().g + (getModConfig().Lowhp.GetValue().g - getModConfig().Diehp.GetValue().g) * fraction;
                color.b = getModConfig().Diehp.GetValue().b + (getModConfig().Lowhp.GetValue().b - getModConfig().Diehp.GetValue().b) * fraction;
            }
        } else if (energy > 0.5 && energy < 0.7) {
            if (!getModConfig().AlwaysRainbow.GetValue()) {
                double fraction = (energy - 0.5f)*5.0f;
                color.r = getModConfig().Lowhp.GetValue().r + (getModConfig().Defhp.GetValue().r - getModConfig().Lowhp.GetValue().r) * fraction;
                color.g = getModConfig().Lowhp.GetValue().g + (getModConfig().Defhp.GetValue().g - getModConfig().Lowhp.GetValue().g) * fraction;
                color.b = getModConfig().Lowhp.GetValue().b + (getModConfig().Defhp.GetValue().b - getModConfig().Lowhp.GetValue().b) * fraction;
            }
        } else if (energy > 0.95) {
            if (!getModConfig().Rainbow.GetValue()) {
                double fraction = (energy - 0.95f)*20.0f;
                color.r = getModConfig().Midhp.GetValue().r + (getModConfig().Highhp.GetValue().r - getModConfig().Midhp.GetValue().r) * fraction;
                color.g = getModConfig().Midhp.GetValue().g + (getModConfig().Highhp.GetValue().g - getModConfig().Midhp.GetValue().g) * fraction;
                color.b = getModConfig().Midhp.GetValue().b + (getModConfig().Highhp.GetValue().b - getModConfig().Midhp.GetValue().b) * fraction;
            }
        } else if (energy > 0.7) {
            if (!getModConfig().AlwaysRainbow.GetValue()) {
                double fraction = (energy - 0.7f)*3.3333333f;
                color.r = getModConfig().Defhp.GetValue().r + (getModConfig().Midhp.GetValue().r - getModConfig().Defhp.GetValue().r) * fraction;
                color.g = getModConfig().Defhp.GetValue().g + (getModConfig().Midhp.GetValue().g - getModConfig().Defhp.GetValue().g) * fraction;
                color.b = getModConfig().Defhp.GetValue().b + (getModConfig().Midhp.GetValue().b - getModConfig().Defhp.GetValue().b) * fraction;
            }
        }
    }
    
    if (energy > 0.85 && getModConfig().Fadeout.GetValue()) {
        color.a = (6.667+(-6.667*energy))*getModConfig().Alpha.GetValue();
    } else {
        color.a = getModConfig().Alpha.GetValue();
    }
    energyBar->set_color(color);
}

MAKE_HOOK_MATCH(GameEnergyUIPanel_Start, &GameEnergyUIPanel::Start, void, GameEnergyUIPanel* self) {
    Logger.info("LateUpdate RedBar");
    GameEnergyUIPanel_Start(self);
    SetColor(self, 0.5f);
}

MAKE_HOOK_MATCH(GameEnergyUIPanel_HandleGameEnergyDidChange, &GameEnergyUIPanel::RefreshEnergyUI, void, GameEnergyUIPanel* self, float energy) {
    GameEnergyUIPanel_HandleGameEnergyDidChange(self, energy);
    Logger.info("Energy changed to {}", energy);
    SetColor(self, energy);
}

float * Wheel(int WheelPos) {
  static int c[3];
  static float epic[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  epic[0] = (float) c[0] / 255.0;
  epic[1] = (float) c[1] / 255.0;
  epic[2] = (float) c[2] / 255.0;
 
  return epic;
}

MAKE_HOOK_MATCH(GameEnergyCounter_LateUpdate, &GameEnergyCounter::LateUpdate, void, GameEnergyCounter* self) {
    Logger.info("LateUpdate RedBar");
    GameEnergyCounter_LateUpdate(self);
    if ((energyBarMaterialStore != nullptr && energyBarStore != nullptr && energyy == 1.0 && getModConfig().Rainbow.GetValue()) || (energyBarMaterialStore != nullptr && energyBarStore != nullptr && getModConfig().AlwaysRainbow.GetValue())) {
        float* Heck = Wheel(pos);
        UnityEngine::Color color;
        color.a = 1.0;
        color.r = Heck[0];
        color.g = Heck[1];
        color.b = Heck[2];
        energyBarStore->set_color(color);
        pos++;
        if (pos > 255) {
            pos = 0;
        }
    }
}

MAKE_HOOK_MATCH(SceneManager_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene previousActiveScene, UnityEngine::SceneManagement::Scene nextActiveScene) {
    SceneManager_ActiveSceneChanged(previousActiveScene, nextActiveScene);
    energyBarStore = nullptr;
    energyBarMaterialStore = nullptr;
}

#pragma region Mod setup
/// @brief Called at the early stages of game loading
/// @param info
/// @return
MOD_EXPORT_FUNC void setup(CModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;

    Logger.info("Completed setup!");
}

/// @brief Called later on in the game loading - a good time to install function hooks
/// @return
MOD_EXPORT_FUNC void late_load() {
    il2cpp_functions::Init();

    getModConfig().Init(modInfo);
    BSML::Init();

    BSML::Register::RegisterMainMenu("RedBar", "<color=#ff0000>RedBar", "Manage settings", DidActivate);

    Logger.info("Installing hooks...");
    INSTALL_HOOK(Logger, GameEnergyUIPanel_HandleGameEnergyDidChange);
    INSTALL_HOOK(Logger, GameEnergyCounter_LateUpdate);
    INSTALL_HOOK(Logger, GameEnergyUIPanel_Start);
    INSTALL_HOOK(Logger, SceneManager_ActiveSceneChanged);
    Logger.info("Installed all hooks!");
}
#pragma endregion
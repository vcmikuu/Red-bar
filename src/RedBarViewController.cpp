#include "RedBarViewController.hpp"
#include "ModConfig.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
//#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/ColorPickerButtonController.hpp"
#include "GlobalNamespace/HSVPanelController.hpp"
#include "GlobalNamespace/ColorChangeUIEventType.hpp"
#include "System/Action_2.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Settings/SliderSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/DropdownListSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/ListSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/StringSetting.hpp"
#include "bsml/shared/BSML/Components/ClickableText.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"
#include "UnityEngine/Color.hpp"
#include "config-utils/shared/config-utils.hpp"
#include <stdlib.h>

using namespace BSML;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;
//using namespace RedBar;

//DEFINE_CLASS(RedBarViewController);

void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation) {
        self->get_gameObject()->AddComponent<Touchable*>();
       GameObject* container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());

        BSML::Lite::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().Rainbow)->get_gameObject(), "overrides Fadeout");

        // AlwaysRainbow
        BSML::Lite::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().AlwaysRainbow)->get_gameObject(), "overrides Fadeout, Fade red to green and Rainbow");

        // Fadeout
        AddConfigValueToggle(container->get_transform(), getModConfig().Fadeout);

        // Alpha
        AddConfigValueIncrementFloat(container->get_transform(), getModConfig().Alpha, 1, 0.1f, 0.0f, 1.0f);

        //QuestUI::BeatSaberUI::CreateText(container->get_transform(), "\n\n                                          Values under 0.0 will be 0.0");
        //QuestUI::BeatSaberUI::CreateText(container->get_transform(), "\n                                          Values over 1.0 will be 1.0");
        
        // FullFade
        BSML::Lite::AddHoverHint(AddConfigValueToggle(container->get_transform(), getModConfig().FullFade)->get_gameObject(), "Fades between the 15% and 95% Color");


        AddConfigValueColorPicker(container->get_transform(), getModConfig().Diehp);

        AddConfigValueColorPicker(container->get_transform(), getModConfig().Lowhp);

        AddConfigValueColorPicker(container->get_transform(), getModConfig().Defhp);

        AddConfigValueColorPicker(container->get_transform(), getModConfig().Midhp);

        AddConfigValueColorPicker(container->get_transform(), getModConfig().Highhp);
    }
}
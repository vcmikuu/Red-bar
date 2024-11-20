// Most likely I will not bring this UI for a time yet, I only want to test some things out.

#include "UI/SettingsView.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Color.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "ModConfig.hpp"
//#include "assets.hpp"

DEFINE_TYPE(RedBar::UI, SettingsView);

using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace RedBar;

void RedBar::UI::SettingsView::DidActivate(bool firstActivation, bool addedToHeirarchy, bool screenSystemDisabling)
{
    if (!firstActivation)
        return;

    //BSML::parse_and_construct(IncludedAssets::SettingsView_bsml, this->get_transform(), this);
};

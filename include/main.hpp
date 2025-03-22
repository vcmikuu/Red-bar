#pragma once

#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"

#include "scotland2/shared/loader.hpp"

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

#define MOD_EXPORT __attribute__((visibility("default")))
#ifdef __cplusplus
#define MOD_EXPORT_FUNC extern "C" MOD_EXPORT
#else
#define MOD_EXPORT_FUNC MOD_EXPORT
#endif
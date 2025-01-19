#include "utils/logger.h"
#include <coreinit/filesystem.h>
#include <malloc.h>
#include <wups.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <wups/config/WUPSConfigItemStub.h>
#include <wups/config_api.h>
#include <notifications/notifications.h>

#define PLUGIN_VER "2d"
#define ENABLE_CFG_ID "drcRcEbd"
#define SOUND_CFG_DEFAULT "sndRcEbd"
#define ENABLE_CFG_DEFAULT false
#define SOUND_CFG_DEFAULT false

bool drcRecEnabled = ENABLE_CFG_DEFAULT;
bool sndRecEnabled = SOUND_CFG_DEFAULT

WUPS_PLUGIN_NAME("DRC Recording");
WUPS_PLUGIN_DESCRIPTION("UNDER DEVELOPMENT! Records the GamePad video to a USB output to be used with OBS.");
WUPS_PLUGIN_VERSION(PLUGIN_VER);
WUPS_PLUGIN_AUTHOR("GloriousGlider8");
WUPS_PLUGIN_LICENSE("CC-BY-NC-SA 4.0");

WUPS_USE_WUT_DEVOPTAB(); // i dont even know what this shit does but it was in the example
WUPS_USE_STORAGE("drcRecPlg");

void boolItemChanged(ConfigItemBoolean *item, bool newValue) {
    if (std::string_view(ENABLE_CFG_ID) == item->identifier) {
        drcRecEnabled = newValue;
    } else if (std::string_view(SOUND_CFG_ID) == item->identifier)
	{
		sndRecEnabled = newValue;
	}
	
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle) {
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);

    try {
        root.add(WUPSConfigItemStub::Create("Any features with [BETA] are experimental and may not work."));

        root.add(WUPSConfigItemBoolean::Create(ENABLE_CFG_ID, "Enable Plugin", ENABLE_CFG_DEFAULT, drcRecEnabled, boolItemChanged));
        root.add(WUPSConfigItemBoolean::Create(ENABLE_CFG_ID, "Enable Sound Recording [BETA]", SOUND_CFG_DEFAULT, sndRecEnabled, boolItemChanged));

		auto ytInfCtg = WUPSConfigCategory::Create("IMPORTANT INFORMATION");

			ytInfCtg.add(WUPSConfigItemStub::Create("Usage of this Plugin on any video/streaming platform requires credit."));
			ytInfCtg.add(WUPSConfigItemStub::Create("A link is required in the description to @gloriousglider8-yt's YouTube channel."));
			ytInfCtg.add(WUPSConfigItemStub::Create("E.g. \"@gloriousglider8-yt - GamePad Recorder\" in the Description."));
			ytInfCtg.add(WUPSConfigItemStub::Create("See the GitHub repo for more info."));

        root.add(WUPSConfigItemStub::Create("Plugin by GloriousGlider8 & Contributors (ver. " + PLUGIN_VER + ")."));
		root.add(WUPSConfigItemStub::Create("Special Thanks to NintendoMovies for giving me the motivation to do this!"));
    } catch (std::exception &e) {
		NotificationModule_AddErrorNotification("Failed to add Configuration Options for DRC_REC!");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }
}

void ConfigMenuClosedCallback() {
    WUPSStorageAPI::SaveStorage();
}

INITIALIZE_PLUGIN() {
    initLogging();
    NotificationModule_InitLibrary();
    NotificationModule_AddInfoNotification("DRC Recording Plugin Initalised.");
    DEBUG_FUNCTION_LINE("Initialised DRC Recording Plugin");

    WUPSConfigAPIOptionsV1 configOptions = {.name = "DRC Recording"};
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("Failed to init config api");
    }

    WUPSStorageError storageRes;
    if ((storageRes = WUPSStorageAPI::GetOrStoreDefault(LOG_FS_OPEN_CONFIG_ID, drcRecEnabled, LOF_FS_OPEN_DEFAULT_VALUE)) != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("GetOrStoreDefault failed: %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }
    if ((storageRes = WUPSStorageAPI::SaveStorage()) != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("GetOrStoreDefault failed: %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    deinitLogging();
}

DEINITIALIZE_PLUGIN() {
    NotificationModule_DeInitLibrary();
    DEBUG_FUNCTION_LINE("Deinitialised DRC Recording Plugin");
}
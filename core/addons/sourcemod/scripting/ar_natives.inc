#if defined _ARENAROULETTE_NATIVES_INC
 #endinput
#endif

#define _ARENAROULETTE_NATIVES_INC

#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include "ar_defs.inc"
#include "arena_roulette.inc"
#include "mode_table.inc"

stock void CreateNatives()
{
	CreateNative("ArenaRoulette_RegisterMode", Native_RegisterMode);
	CreateNative("ArenaRoulette_SetForward_CanSelectMode", Native_SetForward_CanSelectMode);
}

stock any Native_RegisterMode(Handle plugin, int numParams)
{
	char modeName[MAX_MODE_NAME_LENGTH] = { '\0' };
	GetPluginInfo(plugin, PlInfo_Name, modeName, sizeof(modeName));

	int apiVersion = GetNativeCell(1);

	if ( apiVersion != ARENAROULETTE_TARGET_API_VERSION )
	{
		LogError(
			"Not registering mode \"%s\": mode specified API version %d when the actual API version is %d",
			modeName,
			apiVersion,
			ARENAROULETTE_TARGET_API_VERSION
		);

		return false;
	}

	int entryIndex = ARMode_AcquireNextFreeModeTableEntry();

	if ( entryIndex < 0 )
	{
		LogError("Could not find free entry in mode table to register mode for plugin \"%s\"", modeName);
		return false;
	}

	ARMode_Table[entryIndex].implementerPlugin = plugin;

	LogMessage("Registered Arena Roulette mode \"%s\" to index %d", modeName, entryIndex);
	return true;
}

stock any Native_SetForward_CanSelectMode(Handle plugin, int numParams)
{
	int index = ARMode_FindEntryByPluginHandle(plugin);

	if ( index < 0 )
	{
		char modeName[MAX_MODE_NAME_LENGTH] = { '\0' };
		GetPluginInfo(plugin, PlInfo_Name, modeName, sizeof(modeName));

		LogError("Could not set CanSelectMode forward for mode \"%s\", as the mode was not registered.", modeName);
		return false;
	}

	ARMode_Table[index].canSelectMode = view_as<ArenaRouletteFwd_CanSelectMode>(GetNativeFunction(1));

	return true;
}

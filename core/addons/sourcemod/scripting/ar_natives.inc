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
}

stock any Native_RegisterMode(Handle plugin, int numParams)
{
	char modeName[MAX_MODE_NAME_LENGTH] = { '\0' };

	GetPluginInfo(plugin, PlInfo_Name, modeName, sizeof(modeName));

	int entryIndex = ARMode_FindNextFreeModeTableEntry();

	if ( entryIndex < 0 )
	{
		LogError("Could not find free entry in mode table to register mode for plugin \"%s\"", modeName);
		return false;
	}

	ARMode_Table[entryIndex].implementerPlugin = plugin;

	if ( numParams % 2 != 0 )
	{
		LogError(
			"ArenaRoulette_RegisterMode called by plugin \"%s\" with incorrect number of arguments," ...
			"truncating last argument.",
			modeName
		);

		numParams = (numParams / 2) * 2;
	}

	for ( int index = 0; index < numParams; index += 2 )
	{
		int propertyType = GetNativeCell(index);

		if ( view_as<int>(propertyType) >= view_as<int>(_ArenaRouletteProperty__FwdBegin) &&
		     view_as<int>(propertyType) < view_as<int>(_ArenaRouletteProperty__FwdEnd) )
		{
			ARMode_Table[entryIndex].SetForward(view_as<ArenaRoulette_Property>(propertyType), GetNativeFunction(index + 1));
		}
		else
		{
			LogError("Plugin \"%s\" provided unrecognised property %d", modeName, view_as<int>(propertyType));
		}
	}

	LogMessage("Registered Arena Roulette mode \"%s\"", modeName);

	return true;
}

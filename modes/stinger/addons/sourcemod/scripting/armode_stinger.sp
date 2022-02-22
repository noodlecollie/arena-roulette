#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include "arena_roulette.inc"

public Plugin myinfo =
{
	name = "AR Mode: Stinger",
	author = "NoodleCollie",
	description = "Pyros must gang up on Heavies to win",
	version = "1.0.0.0",
	url = "https://github.com/noodlecollie/arena_roulette"
}

public void OnAllPluginsLoaded()
{
	if ( !ArenaRoulette_RegisterMode(ArenaRoulette_APIV1) )
	{
		LogError("Could not register mode.");
		return;
	}

	ArenaRoulette_SetForward_CanSelectMode(CanSelectMode);
}

stock bool CanSelectMode()
{
	LogMessage("CanSelectMode() called for Stinger mode");
	return true;
}

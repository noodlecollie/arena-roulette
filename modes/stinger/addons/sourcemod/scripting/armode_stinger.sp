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
	ArenaRoulette_RegisterMode(ArenaRouletteProperty_FwdCanSelectMode, CanSelectMode);
}

stock bool CanSelectMode()
{
	LogMessage("CanSelectMode() called for Stinger mode");
	return true;
}

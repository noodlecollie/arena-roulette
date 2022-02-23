#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include "arena_roulette.inc"
#include "pluginctl/pluginctl.inc"

#define PLUGIN_IDENT "armode_stringer"
#define PLUGIN_VERSION "1.0.0.0"

bool PluginEnabled = false;
bool ModeActive = false;

public Plugin myinfo =
{
	name = "AR Mode: Stinger",
	author = "NoodleCollie",
	description = "Pyros must gang up on Heavies to win",
	version = PLUGIN_VERSION,
	url = "https://github.com/noodlecollie/arena_roulette"
}

public void OnPluginStart()
{
	PCtl_Initialise(PLUGIN_IDENT, PLUGIN_VERSION, OnPluginEnabledStateChanged);

	HookEventEx("arena_round_start", Event_RoundStart, EventHookMode_Post);
	HookEventEx("teamplay_round_win", Event_RoundWin, EventHookMode_Post);
}

public void OnPluginEnd()
{
	PluginEnabled = false;
	ModeActive = false;

	PCtl_Shutdown();
}

public void OnAllPluginsLoaded()
{
	if ( !ArenaRoulette_RegisterMode(ArenaRoulette_APIV1) )
	{
		LogError("Could not register Stinger mode.");
		return;
	}

	LogMessage("Stinger mode successfully registered");

	ArenaRoulette_SetForward_CanSelectMode(CanSelectMode);
}

stock void OnPluginEnabledStateChanged(ConVar convar, const char[] oldValue, const char[] newValue)
{
	PluginEnabled = GetConVarBool(convar);
	LogMessage("Stinger mode %s", PluginEnabled ? "enabled" : "disabled");
}

stock bool CanSelectMode()
{
	LogMessage("CanSelectMode() called for Stinger mode");
	ModeActive = true;
	return true;
}

public void Event_RoundStart(Event event, const char[] name, bool dontBroadcast)
{
	LogMessage("arena_round_start");

	if ( !ModeActive )
	{
		return;
	}

	LogMessage("Event_RoundStart() called for Stinger mode");
}

public void Event_RoundWin(Event event, const char[] name, bool dontBroadcast)
{
	LogMessage("teamplay_round_win");

	if ( !ModeActive )
	{
		return;
	}

	LogMessage("Event_RoundWin() called for Stinger mode");
	ModeActive = false;
}

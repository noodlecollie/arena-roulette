#if defined _ARENAROULETTE_EVENTS_INC
 #endinput
#endif

#define _ARENAROULETTE_EVENTS_INC

#pragma semicolon 1
#pragma newdecls required

#include "mode_table.inc"

stock void HookAllEvents()
{
	HookEventEx("teamplay_round_start", Event_RoundStart, EventHookMode_Post);
}

public void Event_RoundStart(Event event, const char[] name, bool dontBroadcast)
{
	int modeIndex = ARMode_ChooseLeastPlayedMode();

	LogMessage("Arena Roulette chose mode %d to play for this round.", modeIndex);

	if ( modeIndex < 0 )
	{
		return;
	}

	// TODO: Continue
}

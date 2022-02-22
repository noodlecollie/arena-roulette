#if defined _ARENAROULETTE_INC
 #endinput
#endif

#define _ARENAROULETTE_INC

#pragma semicolon 1
#pragma newdecls required

enum ArenaRoulette_APIVersion
{
	ArenaRoulette_APIV1 = 1
};

native bool ArenaRoulette_RegisterMode(ArenaRoulette_APIVersion version);

typedef ArenaRouletteFwd_CanSelectMode = function bool ();
native bool ArenaRoulette_SetForward_CanSelectMode(ArenaRouletteFwd_CanSelectMode fwd);

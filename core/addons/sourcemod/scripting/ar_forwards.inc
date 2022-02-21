#if defined _ARENAROULETTE_FORWARDS_INC
 #endinput
#endif

#define _ARENAROULETTE_FORWARDS_INC

#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include "arena_roulette.inc"

PrivateForward fwCanSelectMode;

stock void CreateForwards()
{
	fwCanSelectMode = CreateForward(ET_Single);
}

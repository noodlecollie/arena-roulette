#if defined _ARENAROULETTE_INC
 #endinput
#endif

#define _ARENAROULETTE_INC

#pragma semicolon 1
#pragma newdecls required

enum ArenaRoulette_Property
{
	ArenaRouletteProperty_Invalid = 0,

	ArenaRouletteProperty_Version = 1,

	ArenaRouletteProperty_FwdCanSelectMode = 2,

	// For internal use only
	_ArenaRouletteProperty__Max,
	_ArenaRouletteProperty__FwdBegin = ArenaRouletteProperty_FwdCanSelectMode,
	_ArenaRouletteProperty__FwdEnd = _ArenaRouletteProperty__Max
};

typedef ArenaRouletteFwd_CanSelectMode = function bool ();

// Expects pairs of arguments. The first of each pair should be
// a ArenaRoulette_ForwardType value, and the second should be
// the function corresponding to this forward.
native bool ArenaRoulette_RegisterMode(any ...);

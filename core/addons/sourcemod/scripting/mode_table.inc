#if defined _ARENAROULETTE_MODE_TABLE_INC
 #endinput
#endif

#define _ARENAROULETTE_MODE_TABLE_INC

#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include "arena_roulette.inc"
#include "ar_defs.inc"
#include "ar_forwards.inc"
#include "mode_stubs.inc"

enum struct ARMode
{
	Handle implementerPlugin;
	int numTimesPlayed;

	ArenaRouletteFwd_CanSelectMode canSelectMode;

	void Reset()
	{
		this.implementerPlugin = null;
		this.numTimesPlayed = 0;
		this.canSelectMode = ARModeStub_CanSelectMode;
	}
}

stock ARMode ARMode_Table[MAX_MODES];
stock int ARMode_TableCount = 0;

stock int ARMode_IndexByLeastPlayed[MAX_MODES] = { -1, ... };

stock int ARMode_FindEntryByPluginHandle(Handle pluginHandle)
{
	if ( pluginHandle == null )
	{
		return -1;
	}

	for ( int index = 0; index < MAX_MODES; ++index )
	{
		if ( ARMode_Table[index].implementerPlugin == pluginHandle )
		{
			return index;
		}
	}

	return -1;
}

stock int ARMode_AcquireNextFreeModeTableEntry()
{
	for ( int index = 0; index < MAX_MODES; ++index )
	{
		if ( ARMode_Table[index].implementerPlugin == null )
		{
			ARMode_AcquireTableEntry(index);
			return index;
		}
	}

	return -1;
}

stock void ARMode_AcquireTableEntry(int index)
{
	if ( index < 0 || index >= MAX_MODES)
	{
		return;
	}

	ARMode_Table[index].Reset();
	++ARMode_TableCount;
}

stock void ARMode_ClearAllPlayCounters()
{
	for ( int index = 0; index < MAX_MODES; ++index )
	{
		ARMode_Table[index].numTimesPlayed = 0;
	}
}

// Assumes that all play counts are zeroed, otherwise
// the array will end up not being ordering by play count.
stock void ARMode_PopulateAndRandomiseLeastPlayedArray()
{
	for ( int index = 0; index < MAX_MODES; ++index )
	{
		ARMode_IndexByLeastPlayed[index] = -1;
	}

	int outIndex = 0;

	for ( int index = 0; index < MAX_MODES; ++index )
	{
		if ( ARMode_Table[index].implementerPlugin == null )
		{
			continue;
		}

		ARMode_IndexByLeastPlayed[outIndex++] = index;
	}

	if ( outIndex < 1 )
	{
		return;
	}

	SortIntegers(ARMode_IndexByLeastPlayed, outIndex, Sort_Random);
}

stock int ARMode_ChooseLeastPlayedMode(int base = 0)
{
	if ( ARMode_TableCount < 1 )
	{
		return -1;
	}

	// Go through the sorted list, checking for runs of modes which have all
	// been played the same number of times. For each run, randomise the
	// order and then ask each mode in turn whether it can currently be
	// played. Pick the first mode that can be played.

	int runBase = 0;
	int runLength = ARMode_RandomiseRangeOfEquallyPlayedModes(runBase);

	while ( runLength > 0 )
	{
		for ( int arrayIndex = runBase; arrayIndex < runBase + runLength; ++arrayIndex )
		{
			int modeIndex = ARMode_IndexByLeastPlayed[arrayIndex];

			if ( ARMode_CanSelectMode(modeIndex) )
			{
				return modeIndex;
			}
		}

		runBase = runBase + runLength;
		runLength = ARMode_RandomiseRangeOfEquallyPlayedModes(runBase);
	}

	return -1;
}

// This function assumes the modes in the IndexByLeastPlayed array are ordered correctly.
stock void ARMode_IncrementTimesPlayed(int index)
{
	if ( index < 0 || index >= MAX_MODES || ARMode_Table[index].implementerPlugin == null )
	{
		LogError("Mode index %d was not valid, not incrementing times played.", index);
		return;
	}

	int indexInLeastPlayedArray = -1;

	for ( indexInLeastPlayedArray = 0; indexInLeastPlayedArray < ARMode_TableCount; ++indexInLeastPlayedArray )
	{
		if ( ARMode_IndexByLeastPlayed[indexInLeastPlayedArray] == index )
		{
			break;
		}
	}

	// Should never happen:
	if ( indexInLeastPlayedArray >= ARMode_TableCount )
	{
		LogError("Could not find mode index %d in least-played utility array! Not incrementing times played.", index);
		return;
	}

	int newIndexInLeastPlayedArray = -1;

	for ( newIndexInLeastPlayedArray = indexInLeastPlayedArray + 1;
	      newIndexInLeastPlayedArray < ARMode_TableCount;
		  ++newIndexInLeastPlayedArray )
	{
		int targetNumTimesPlayed = ARMode_Table[ARMode_IndexByLeastPlayed[newIndexInLeastPlayedArray]].numTimesPlayed;

		if ( targetNumTimesPlayed > ARMode_Table[index].numTimesPlayed + 1 )
		{
			break;
		}
	}

	// We will have found the next entry with a higher play count,
	// so our target slot is one less than this.
	--newIndexInLeastPlayedArray;

	// Should never happen:
	if ( newIndexInLeastPlayedArray < 0 || newIndexInLeastPlayedArray >= ARMode_TableCount )
	{
		LogError("Computed invalid destination index %d! Not incrementing times played.", newIndexInLeastPlayedArray);
		return;
	}

	++ARMode_Table[index].numTimesPlayed;

	// Shuffle all indices down.
	for ( int cursor = indexInLeastPlayedArray; cursor < newIndexInLeastPlayedArray; ++cursor )
	{
		ARMode_IndexByLeastPlayed[cursor] = ARMode_IndexByLeastPlayed[cursor + 1];
	}

	ARMode_IndexByLeastPlayed[newIndexInLeastPlayedArray] = index;
}

stock int ARMode_RandomiseRangeOfEquallyPlayedModes(int base)
{
	if ( base < 0 || base >= ARMode_TableCount )
	{
		return 0;
	}

	int numTimesPlayed = ARMode_Table[ARMode_IndexByLeastPlayed[base]].numTimesPlayed;
	int end = base + 1;

	for ( ; end < ARMode_TableCount; ++end )
	{
		int modeIndex = ARMode_IndexByLeastPlayed[end];

		if ( ARMode_Table[modeIndex].numTimesPlayed != numTimesPlayed )
		{
			break;
		}
	}

	int numInRange = end - base;

	// Should never happen:
	if ( numInRange < 1 )
	{
		return 0;
	}

	if ( numInRange > 1 )
	{
		int[] indices = new int[numInRange];

		for ( int index = 0; index < numInRange; ++index )
		{
			indices[index] = ARMode_IndexByLeastPlayed[base + index];
		}

		SortIntegers(indices, numInRange, Sort_Random);

		for ( int index = 0; index < numInRange; ++index )
		{
			ARMode_IndexByLeastPlayed[base + index] = indices[index];
		}
	}

	return numInRange;
}

stock bool ARMode_CanSelectMode(int modeIndex)
{
	// TODO: Uncommenting the canSelectMode check causes a compiler error:
	// "cannot assign null to a non-nullable type"
	// What is a function pointer default-initialised to if it can't be null?
	// Can we find out?
	if ( modeIndex < 0 ||
	     modeIndex >= MAX_MODES ||
	     /*ARMode_Table[modeIndex].canSelectMode == null ||*/
	     ARMode_Table[modeIndex].implementerPlugin == null )
	{
		return false;
	}

	fwCanSelectMode.AddFunction(ARMode_Table[modeIndex].implementerPlugin, ARMode_Table[modeIndex].canSelectMode);

	bool result = false;
	Call_StartForward(fwCanSelectMode);
	Call_Finish(result);

	fwCanSelectMode.RemoveAllFunctions(ARMode_Table[modeIndex].implementerPlugin);
	return result;
}

#ifndef __MatchState_H__
#define __MatchState_H__

#include <BWAPI.h>

using namespace std;

struct MatchState {
	string mapHash;
	string mapName;
	string strategyId;
	string ownRace;
	//string opponentRace;

	double qValue;

	MatchState()
	{
		qValue = 1;
	}
};

#endif
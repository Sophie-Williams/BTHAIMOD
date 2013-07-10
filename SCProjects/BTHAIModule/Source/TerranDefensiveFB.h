#ifndef __TERRANDEFENSIVEFB_H__
#define __TERRANDEFENSIVEFB_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for a defensive Marine/Firebat/Siege Tank/Goliath
 * based strategy.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TerranDefensiveFB : public Commander {

private:
	int stage;
	
	Squad* mainSquad;
	Squad* backupSquad;
	Squad* sc1;
	Squad* sc2;
	
public:
	TerranDefensiveFB();

	/** Destructor. */
	~TerranDefensiveFB();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "TerranDefensiveFB";
	}
};

#endif

#ifndef __TERRANDEFENSIVE_H__
#define __TERRANDEFENSIVE_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for a defensive Marine/Siege Tank/Goliath
 * based strategy.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TerranDefensive : public Commander {

private:
	int stage;
	
	Squad* mainSquad;
	Squad* backupSquad;
	Squad* sc1;
	Squad* sc2;
	
public:
	TerranDefensive();

	/** Destructor. */
	~TerranDefensive();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "TerranDefensive";
	}
};

#endif

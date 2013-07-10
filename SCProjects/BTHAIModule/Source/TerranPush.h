#ifndef __TERRANPUSH_H__
#define __TERRANPUSH_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for the Terran Siege tank push strategy.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TerranPush : public Commander {

private:
	int stage;
	
	Squad* mainSquad;
	Squad* backupSquad;
	Squad* sc1;
	Squad* sc2;
	
public:
	TerranPush();

	/** Destructor. */
	~TerranPush();

	/** Called each update to issue orders. */
	virtual void computeActions();

	/** Returns the unique id for this strategy. */
	static string getStrategyId()
	{
		return "TerranPush";
	}
};

#endif

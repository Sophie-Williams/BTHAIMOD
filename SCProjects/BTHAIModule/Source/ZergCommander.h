#ifndef __ZERGCOMMANDER_H__
#define __ZERGCOMMANDER_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/** This is the Commander class for playing Zerg.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ZergCommander : public Commander {

private:
	int stage;
	
	Squad* mainGroundSquad;
	Squad* mainAirSquad;
	Squad* backupSquad;
	Squad* scoutSquad;
	
public:
	ZergCommander();

	/** Destructor. */
	~ZergCommander();

	/** Called each update to issue orders. */
	virtual void computeActions();
};

#endif

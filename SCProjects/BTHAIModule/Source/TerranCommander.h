#ifndef __TERRANCOMMANDER_H__
#define __TERRANCOMMANDER_H__

#include "Commander.h"
#include "Squad.h"
#include "BaseAgent.h"
#include "Constructor.h"

using namespace BWAPI;
using namespace BWTA;
using namespace std;

/**  This is the Commander class for playing Terran.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class TerranCommander : public Commander {

private:
	int stage;
	
	Squad* mainSquad;
	Squad* backupSquad;
	Squad* scoutSquad;
	Squad* airSquad1;
	Squad* airSquad2;
	
public:
	TerranCommander();

	/** Destructor. */
	~TerranCommander();

	/** Called each update to issue orders. */
	virtual void computeActions();
};

#endif

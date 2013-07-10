#include "LurkerAgent.h"
#include "NavigationAgent.h"
#include "Commander.h"
#include "ExplorationManager.h"
#include "TargetingAgent.h"

LurkerAgent::LurkerAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "LurkerAgent";
	//Broodwar->printf("LurkerAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void LurkerAgent::computeActions()
{
	/*if (unit->isDetected())
	{
		if (unit->isBurrowed())
		{
			unit->unburrow();
			return;
		}
		defensive = true;
		NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	}*/

	int eCnt = enemyGroundUnitsWithinRange(unit->getType().groundWeapon().maxRange());
	eCnt += enemyAirToGroundUnitsWithinRange(unit->getType().groundWeapon().maxRange());
	
	if (eCnt > 0 && !unit->isBurrowed())
	{
		if (unit->burrow()) return;
	}
	if (eCnt == 0 && unit->isBurrowed())
	{
		unit->unburrow();
		return;
	}

	defensive = false;
	if (!unit->isBurrowed()) defensive = true;

	if (!unit->isBurrowed()) NavigationAgent::getInstance()->computeMove(this, goal, defensive);
}

#include "GoliathAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "TargetingAgent.h"

GoliathAgent::GoliathAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "GoliathAgent";
	//Broodwar->printf("GoliathAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void GoliathAgent::computeActions()
{
	defensive = false;
	if (unit->getAirWeaponCooldown() > 2 || unit->getGroundWeaponCooldown() > 2) defensive = true;
	
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	if (!defensive) TargetingAgent::checkTarget(this);
}

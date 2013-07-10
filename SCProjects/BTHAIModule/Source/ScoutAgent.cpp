#include "ScoutAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "TargetingAgent.h"

ScoutAgent::ScoutAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "ScoutAgent";
	//Broodwar->printf("ScoutAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void ScoutAgent::computeActions()
{
	if (chargeShields())
	{
		return;
	}

	defensive = false;
	if (unit->getAirWeaponCooldown() > 2 || unit->getGroundWeaponCooldown() > 2) defensive = true;
	
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	if (!defensive) TargetingAgent::checkTarget(this);
}

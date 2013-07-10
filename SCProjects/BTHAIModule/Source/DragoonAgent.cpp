#include "DragoonAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "Commander.h"
#include "TargetingAgent.h"
#include "Profiler.h"

DragoonAgent::DragoonAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "DragoonAgent";
	//Broodwar->printf("DragoonAgent created (%s)", unit->getType().getName().c_str());
	goal = TilePosition(-1, -1);

	defensive = false;
}

void DragoonAgent::computeActions()
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

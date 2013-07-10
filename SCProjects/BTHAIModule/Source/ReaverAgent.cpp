#include "ReaverAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "TargetingAgent.h"

ReaverAgent::ReaverAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "ReaverAgent";
	//Broodwar->printf("ReaverAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void ReaverAgent::computeActions()
{
	int maxLoad = 5;
	if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0)
	{
		maxLoad = 10;
	}

	if(unit->getScarabCount() < maxLoad)
	{
		unit->train(UnitTypes::Protoss_Scarab);
		return;
	}

	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
}

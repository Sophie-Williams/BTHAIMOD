#include "ArbiterAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"

ArbiterAgent::ArbiterAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "ArbiterAgent";
	//Broodwar->printf("ArbiterAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void ArbiterAgent::computeActions()
{
	if (chargeShields())
	{
		return;
	}

	NavigationAgent::getInstance()->computeMove(this, goal, true);
}

#include "CarrierAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"

CarrierAgent::CarrierAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "CarrierAgent";
	//Broodwar->printf("CarrierAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void CarrierAgent::computeActions()
{
	if (chargeShields())
	{
		return;
	}

	if (Broodwar->canMake(unit, UnitTypes::Protoss_Interceptor))
	{
		unit->train(UnitTypes::Protoss_Interceptor);
		return;
	}

	NavigationAgent::getInstance()->computeMove(this, goal, true);
}

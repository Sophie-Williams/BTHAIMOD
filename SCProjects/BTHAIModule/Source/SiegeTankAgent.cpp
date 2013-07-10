#include "SiegeTankAgent.h"
#include "AgentManager.h"
#include "NavigationAgent.h"
#include "TargetingAgent.h"
#include "Commander.h"

SiegeTankAgent::SiegeTankAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "SiegeTankAgent";
	//Broodwar->printf("SiegeTankAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void SiegeTankAgent::computeActions()
{
	if (Broodwar->self()->hasResearched(TechTypes::Tank_Siege_Mode))
	{
		bool goSiege = false;

		//If we have enemy targets nearby, go into siege mode
		int eCnt = enemyGroundUnitsWithinRange(getGroundRange(UnitTypes::Terran_Siege_Tank_Siege_Mode));
		if (eCnt > 0)
		{
			goSiege = true;
		}
		if (eCnt == 0)
		{
			goSiege = false;
		}

		//If we are defending and are at the defense position, go
		//in siege mode
		Squad* sq = Commander::getInstance()->getSquad(squadID);
		if (sq != NULL)
		{
			if (!sq->isActive())
			{
				int range = UnitTypes::Terran_Siege_Tank_Siege_Mode.groundWeapon().maxRange() * 0.5;
				double d = unit->getDistance(Position(goal));
				if (d <= range) goSiege = true;
			}
		}

		if (goSiege && !unit->isSieged()) unit->siege();
		if (!goSiege && unit->isSieged()) unit->unsiege();
	}

	defensive = false;
	TargetingAgent::checkTarget(this);

	//The tank cant move if sieged
	if (!unit->isSieged())
	{
		NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	}
}

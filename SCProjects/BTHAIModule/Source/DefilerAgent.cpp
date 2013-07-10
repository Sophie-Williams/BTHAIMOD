#include "DefilerAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "TargetingAgent.h"

DefilerAgent::DefilerAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "DefilerAgent";
	//Broodwar->printf("DefilerAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void DefilerAgent::computeActions()
{
	if (checkDarkSwarm())
	{
		return;
	}
	if (Broodwar->self()->hasResearched(TechTypes::Consume))
	{
		if (checkConsume())
		{
			return;
		}
	}

	defensive = false;
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	TargetingAgent::checkTarget(this);
}

bool DefilerAgent::checkConsume()
{
	if (unit->getEnergy() > 150)
	{
		//Already enough energy. Dont do anything.
		return false;
	}

	vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	for (int i = 0; i < (int)agents.size(); i++)
	{
		BaseAgent* agent = agents.at(i);
		if (agent->isAlive() && agent->isOfType(UnitTypes::Zerg_Zergling))
		{
			double dist = agent->getUnit()->getTilePosition().getDistance(unit->getTilePosition());
			if (dist <= 2)
			{
				unit->useTech(TechTypes::Consume, agent->getUnit());
				//Broodwar->printf("Used Consume on %s", agent->getUnitType().getName().c_str());
				return true;
			}
		}
	}

	return false;
}

bool DefilerAgent::checkDarkSwarm()
{
	if (unit->getEnergy() < 100)
	{
		return false;
	}

	int maxRange = 8 * 32;

	vector<BaseAgent*> agents = AgentManager::getInstance()->getAgents();
	for (int i = 0; i < (int)agents.size(); i++)
	{
		if (agents.at(i)->isOfType(UnitTypes::Zerg_Mutalisk) && agents.at(i)->isAlive())
		{
			if (agents.at(i)->getUnit()->isAttacking())
			{
				//A Mutalisk is in combat. Cover it in Dark Swarm.
				unit->useTech(TechTypes::Dark_Swarm, agents.at(i)->getUnit()->getPosition());
				return true;
			}
		}
	}
	
	return false;
}

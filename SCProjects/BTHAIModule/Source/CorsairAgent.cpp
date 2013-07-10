#include "CorsairAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "TargetingAgent.h"

CorsairAgent::CorsairAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "CorsairAgent";
	//Broodwar->printf("CorsairAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
	lastUseFrame = Broodwar->getFrameCount();
}

void CorsairAgent::computeActions()
{
	if (chargeShields())
	{
		return;
	}

	if (Broodwar->getFrameCount() - lastUseFrame >= 80)
	{
		TechType web = TechTypes::Disruption_Web;
		if (Broodwar->self()->hasResearched(web))
		{
			if (unit->getEnergy() >= 125)
			{
				Unit* target = getClosestEnemyAirDefense(320);
				if (target != NULL)
				{
					if (target->getEnsnareTimer() == 0)
				{
						unit->useTech(web, target);
						lastUseFrame = Broodwar->getFrameCount();
						//Broodwar->printf("[%d] Using Disruption Web on %s", unitID, target->getType().getName().c_str());
						return;
					}
				}
			}
		}
	}

	defensive = false;
	if (unit->getAirWeaponCooldown() > 2 || unit->getGroundWeaponCooldown() > 2) defensive = true;
	
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	if (!defensive) TargetingAgent::checkTarget(this);
}

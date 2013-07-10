#include "WraithAgent.h"
#include "NavigationAgent.h"
#include "AgentManager.h"
#include "Commander.h"
#include "TargetingAgent.h"

WraithAgent::WraithAgent(Unit* mUnit)
{
	unit = mUnit;
	type = unit->getType();
	unitID = unit->getID();
	agentType = "WraithAgent";
	//Broodwar->printf("WraithAgent created (%s)", unit->getType().getName().c_str());
	
	goal = TilePosition(-1, -1);
}

void WraithAgent::computeActions()
{
	if (checkUseCloak())
	{
		return;
	}

	defensive = false;
	if (!unit->isCloaked()) defensive = true;
	if (unit->getAirWeaponCooldown() > 2 || unit->getGroundWeaponCooldown() > 2) defensive = true;
	
	NavigationAgent::getInstance()->computeMove(this, goal, defensive);
	if (!defensive) TargetingAgent::checkTarget(this);
}

bool WraithAgent::checkUseCloak()
{
	TechType cloak = TechTypes::Cloaking_Field;
	if (Broodwar->self()->hasResearched(cloak))
	{
		if (!unit->isCloaked())
		{
			if (unit->getEnergy() >= 25 && !isDetectorWithinRange(unit->getTilePosition(), 192))
			{
				int range = 10 * 32;
				int eCnt = enemyUnitsWithinRange(range);
				if (eCnt > 0)
				{
					unit->useTech(cloak);
					return true;
				}
			}
		}
		//Dont decloak since it is costly to first use cloak and
		//keeping it up is cheap.
		/*if (unit->isCloaked())
		{
			int range = 10 * 32;
			int eCnt = enemyUnitsWithinRange(range);
			if (eCnt == 0)
			{
				unit->decloak();
				return true;
			}
		}*/
	}
	return false;
}

bool WraithAgent::canBeAttacked()
{
	for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++)
	{
		UnitType et = (*i)->getType();
		if (et.groundWeapon().targetsAir() || et.airWeapon().targetsAir())
		{
			int range = et.groundWeapon().maxRange();
			if (et.airWeapon().maxRange() > range)
			{
				range = et.airWeapon().maxRange();
			}

			double dist = (*i)->getPosition().getDistance(unit->getPosition());
		
			if (dist <= range)
			{
				if (!unit->isCloaked())
				{
					return true;
				}
				if (unit->isCloaked() && (*i)->getType().isDetector())
				{
					return true;
				}
			}
		}
	}

	return false;
}

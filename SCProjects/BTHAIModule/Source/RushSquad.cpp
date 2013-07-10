#include "RushSquad.h"
#include "UnitAgent.h"
#include "AgentManager.h"
#include "ExplorationManager.h"
#include "Commander.h"

RushSquad::RushSquad(int mId, string mName, int mPriority)
{
	this->id = mId;
	this->type = RUSH;
	this->moveType = AIR;
	this->name = mName;
	this->priority = mPriority;
	activePriority = priority;
	active = false;
	required = false;
	goal = Broodwar->self()->getStartLocation();
	goalSetFrame = 0;
	currentState = STATE_NOT_SET;
}

bool RushSquad::isActive()
{
	return active;
}

void RushSquad::defend(TilePosition mGoal)
{
	if (!active)
	{
		setGoal(mGoal);
	}
}

void RushSquad::attack(TilePosition mGoal)
{

}

void RushSquad::assist(TilePosition mGoal)
{
	if (!isUnderAttack())
	{
		currentState = STATE_ASSIST;
		setGoal(mGoal);
	}
}

void RushSquad::computeActions()
{
	if (!active)
	{
		//Check if we need workers in the squad
		for (int i = 0; i < (int)setup.size(); i++)
		{
			if (setup.at(i).current < setup.at(i).no && setup.at(i).type.isWorker())
			{
				int no =  setup.at(i).no - setup.at(i).current;
				for (int j = 0; j < no; j++)
				{
					BaseAgent* w = AgentManager::getInstance()->findClosestFreeWorker(Broodwar->self()->getStartLocation());
					if (w != NULL) addMember(w);
				}
			}
		}

		if (isFull())
		{
			//Broodwar->printf("Ready at frame %d", Broodwar->getFrameCount());
			active = true;
		}

		if (analyzed)
		{
			TilePosition defSpot = Commander::getInstance()->findChokePoint();
			if (defSpot.x() != -1)
			{
				goal = defSpot;
			}
		}
		return;
	}

	//First, remove dead agents
	for(int i = 0; i < (int)agents.size(); i++)
	{
		if(!agents.at(i)->isAlive())
		{
			agents.erase(agents.begin() + i);
			i--;
		}
	}

	if (active)
	{
		if (activePriority != priority)
		{
			priority = activePriority;
		}

		Unit* target = findWorkerTarget();
		if (target != NULL)
		{
			for (int i = 0; i < (int)agents.size(); i++)
			{
				BaseAgent* agent = agents.at(i);
				if (agent->isAlive())
				{
					agent->getUnit()->attack(target);
				}
			}
		}

		TilePosition ePos = ExplorationManager::getInstance()->getClosestSpottedBuilding(Broodwar->self()->getStartLocation());
		if (ePos.x() == -1)
		{
			TilePosition nGoal = getNextStartLocation();
			if (nGoal.x() >= 0)
			{
				this->goal = nGoal;
				setMemberGoals(goal);
			}
		}
		else
		{
			this->goal = ePos;//getClosestStartLocation(ePos);
			setMemberGoals(goal);
		}

		checkAttack();
	}
}

Unit* RushSquad::findWorkerTarget()
{
	try {
		double maxRange = 12 * 32;

		for (int i = 0; i < (int)agents.size(); i++)
		{
			BaseAgent* agent = agents.at(i);

			for(set<Unit*>::const_iterator i=Broodwar->enemy()->getUnits().begin();i!=Broodwar->enemy()->getUnits().end();i++)
			{
				if ((*i)->exists())
				{
					if ((*i)->getType().isWorker())
				{
						double dist = agent->getUnit()->getDistance((*i));
						if (dist <= maxRange)
				{
							return (*i);
						}
					}	
				}
			}
		}
	}
	catch (exception)
	{

	}

	return NULL;
}

void RushSquad::clearGoal()
{
	goal = TilePosition(-1, -1);
}

TilePosition RushSquad::getGoal()
{
	return goal;
}

bool RushSquad::hasGoal()
{
	if (goal.x() < 0 || goal.y() < 0)
	{
		return false;
	}
	return true;
}

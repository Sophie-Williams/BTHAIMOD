#include "ProtossCommander.h"
#include "BuildplanEntry.h"
#include "ExplorationSquad.h"
#include "AgentManager.h"

ProtossCommander::ProtossCommander()
{
	ownDeadScore = 0;
	enemyDeadScore = 0;
	debug_bp = false;
	debug_sq = false;

	stage = 0;
	currentState = DEFEND;

	builder = Constructor::getInstance();

	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 8));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, 10));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Assimilator, 12));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Cybernetics_Core, 14));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 15));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Forge, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, 18));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::Singularity_Charge, 20));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Photon_Cannon, 23));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 23));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, 25));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Citadel_of_Adun, 29));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Pylon, 30));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Robotics_Facility, 33));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Gateway, 33));
	buildplan.push_back(BuildplanEntry(UnitTypes::Protoss_Templar_Archives, 34));

	mainSquad = new Squad(1, Squad::OFFENSIVE, "MainSquad", 10);
	mainSquad->setBuildup(true);
	mainSquad->setRequired(true);
	squads.push_back(mainSquad);

	backupSquad = new Squad(2, Squad::OFFENSIVE, "BackupSquad", 11);
	backupSquad->setRequired(false);
	backupSquad->setBuildup(true);
	squads.push_back(backupSquad);

	scoutSquad = new ExplorationSquad(3, "ScoutingSquad", 11);
	scoutSquad->setRequired(false);
	scoutSquad->setBuildup(true);
	squads.push_back(scoutSquad);
	
	noWorkers = 17;
	noWorkersPerRefinery = 2;
}

ProtossCommander::~ProtossCommander()
{
	for (int i = 0; i < (int)squads.size(); i++)
	{
		delete squads.at(i);
	}
	instance = NULL;
}

void ProtossCommander::computeActions()
{
	computeActionsBase();

	int cSupply = Broodwar->self()->supplyUsed() / 2;

	if (cSupply >= 17 && stage == 0)
	{
		mainSquad->addSetup(UnitTypes::Protoss_Zealot, 8);
		mainSquad->addSetup(UnitTypes::Protoss_Dragoon, 7);
		stage++;
		noWorkers = 20;
	}
	if (cSupply >= 34 && stage == 1)
	{
		mainSquad->addSetup(UnitTypes::Protoss_Dragoon, 7);
		//mainSquad->addSetup(UnitTypes::Protoss_Dark_Templar, 6);
		//mainSquad->addSetup(UnitTypes::Protoss_High_Templar, 4);
		mainSquad->setBuildup(false);	

		stage++;
	}
}

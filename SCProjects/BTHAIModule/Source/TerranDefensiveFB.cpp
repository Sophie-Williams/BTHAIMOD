#include "TerranDefensiveFB.h"
#include "BuildplanEntry.h"
#include "AgentManager.h"
#include "ExplorationSquad.h"
#include "RushSquad.h"
#include "BuildingPlacer.h"
#include "ExplorationManager.h"

TerranDefensiveFB::TerranDefensiveFB()
{
	ownDeadScore = 0;
	enemyDeadScore = 0;
	debug_bp = false;
	debug_sq = false;

	stage = 0;
	currentState = DEFEND;
	
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 12));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 14));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 18));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, 20));
	buildplan.push_back(BuildplanEntry(TechTypes::Tank_Siege_Mode, 22));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 23));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::U_238_Shells, 30));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 31));

	mainSquad = new Squad(1, Squad::OFFENSIVE, "MainSquad", 10);
	mainSquad->addSetup(UnitTypes::Terran_Marine, 10);
	mainSquad->setRequired(true);
	mainSquad->setBuildup(true);
	squads.push_back(mainSquad);

	sc1 = new RushSquad(4, "ScoutingSquad1", 11);
	sc1->setRequired(false);
	
	sc2 = new ExplorationSquad(5, "ScoutingSquad2", 11);
	sc2->setRequired(false);
	squads.push_back(sc2);

	backupSquad = new Squad(2, Squad::OFFENSIVE, "BackupSquad", 10);
	backupSquad->setRequired(false);
	backupSquad->setBuildup(true);
	squads.push_back(backupSquad);

	noWorkers = 16;
	noWorkersPerRefinery = 2;
}

TerranDefensiveFB::~TerranDefensiveFB()
{
	for (int i = 0; i < (int)squads.size(); i++)
	{
		delete squads.at(i);
	}
	instance = NULL;
}

void TerranDefensiveFB::computeActions()
{
	computeActionsBase();

	noWorkers = 12 * AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Command_Center) + 2 * AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Refinery);
	if (noWorkers > 30) noWorkers = 30;

	int cSupply = Broodwar->self()->supplyUsed() / 2;
	int min = Broodwar->self()->minerals();
	int gas = Broodwar->self()->gas();

	if (cSupply >= 20 && stage == 0)
	{
		mainSquad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 6);
		mainSquad->addSetup(UnitTypes::Terran_SCV, 1);
		mainSquad->addSetup(UnitTypes::Terran_Marine, 5);
		mainSquad->addSetup(UnitTypes::Terran_Firebat, 10);
		mainSquad->addSetup(UnitTypes::Terran_Medic, 6);

		stage++;
	}
	if (cSupply >= 30 && min > 200 && stage == 1)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Armory, cSupply));

		mainSquad->addSetup(UnitTypes::Terran_Goliath, 6);
		mainSquad->setBuildup(false);

		stage++;
	}
	if (cSupply >= 45 && min > 200 && stage == 2)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, cSupply));
		
		sc1->addSetup(UnitTypes::Terran_SCV, 1);
		sc1->setPriority(1);
		sc1->setActivePriority(1000);
		squads.push_back(sc1);

		stage++;
	}
	if (min > 500 && stage == 3)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Command_Center, cSupply));

		stage++;
	}
	
	if (stage == 4 && AgentManager::getInstance()->countNoBases() >= 2)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, cSupply));

		stage++;
	}
	if (stage == 5 && min > 500 && gas > 500)
	{
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Vehicle_Weapons, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, cSupply));
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Science_Facility, cSupply));
		
		stage++;
	}
	if (stage == 6 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Science_Facility) > 0)
	{
		mainSquad->addSetup(UnitTypes::Terran_Science_Vessel, 1);

		buildplan.push_back(BuildplanEntry(TechTypes::Cloaking_Field, cSupply));

		backupSquad->addSetup(UnitTypes::Terran_Wraith, 5);
		backupSquad->setBuildup(false);

		sc2->addSetup(UnitTypes::Terran_Wraith, 1);
		sc2->setBuildup(false);

		stage++;
	}
	if (stage == 7 && AgentManager::getInstance()->countNoFinishedUnits(UnitTypes::Terran_Science_Vessel) > 0)
	{
		if (Broodwar->enemy()->getRace().getID() == Races::Protoss.getID())
		{
			buildplan.push_back(BuildplanEntry(TechTypes::EMP_Shockwave, cSupply));
		}
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Vehicle_Weapons, cSupply));
		
		stage++;
	}
}

#include "TerranCommander.h"
#include "BuildplanEntry.h"
#include "AgentManager.h"
#include "ExplorationSquad.h"
#include "BuildingPlacer.h"
#include "ExplorationManager.h"

TerranCommander::TerranCommander()
{
	ownDeadScore = 0;
	enemyDeadScore = 0;
	debug_bp = false;
	debug_sq = false;

	stage = 0;
	currentState = DEFEND;
	
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 9));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 12));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Refinery, 12));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 15));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 16));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Bunker, 18));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Supply_Depot, 22));
	buildplan.push_back(BuildplanEntry(TechTypes::Tank_Siege_Mode, 24));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Academy, 26));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Barracks, 28));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::U_238_Shells, 26)); //29
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Factory, 30));
	buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Armory, 50));

	buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Vehicle_Weapons, 60));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Vehicle_Weapons, 63));
	buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Vehicle_Weapons, 66));
	
	mainSquad = new Squad(1, Squad::OFFENSIVE, "MainSquad", 10);
	mainSquad->setRequired(true);
	mainSquad->setBuildup(true);
	squads.push_back(mainSquad);
	
	backupSquad = new Squad(2, Squad::OFFENSIVE, "BackupSquad", 11);
	backupSquad->setRequired(false);
	backupSquad->setBuildup(true);
	squads.push_back(backupSquad);

	scoutSquad = new ExplorationSquad(3, "ScoutingSquad", 11);
	scoutSquad->setRequired(false);
	scoutSquad->setBuildup(true);
	squads.push_back(scoutSquad);

	airSquad1 = new Squad(4, Squad::OFFENSIVE, "AirSquad1", 11);
	airSquad1->setRequired(false);
	airSquad1->setBuildup(true);
	squads.push_back(airSquad1);

	airSquad2 = new Squad(5, Squad::OFFENSIVE, "AirSquad2", 12);
	airSquad2->setRequired(false);
	airSquad2->setBuildup(true);
	squads.push_back(airSquad2);

	noWorkers = 16;
	noWorkersPerRefinery = 2;
}

TerranCommander::~TerranCommander()
{
	for (int i = 0; i < (int)squads.size(); i++)
	{
		delete squads.at(i);
	}
	instance = NULL;
}

void TerranCommander::computeActions()
{
	computeActionsBase();

	int cSupply = Broodwar->self()->supplyUsed() / 2;

	if (cSupply >= 16 && stage == 0)
	{
		mainSquad->addSetup(UnitTypes::Terran_Marine, 6);
		stage++;
		noWorkers = 18;
	}
	if (cSupply >= 24 && stage == 1)
	{
		mainSquad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 6);
		mainSquad->addSetup(UnitTypes::Terran_SCV, 1);
		mainSquad->addSetup(UnitTypes::Terran_Goliath, 4);
		mainSquad->addSetup(UnitTypes::Terran_Marine, 12);
		mainSquad->addSetup(UnitTypes::Terran_Medic, 4);
		mainSquad->setBuildup(false);

		scoutSquad->addSetup(UnitTypes::Terran_Vulture, 2);
		scoutSquad->setBuildup(false);

		stage++;
	}
	if (cSupply >= 29 && stage == 2)
	{
		noWorkers = 24;
		stage++;
	}
	if (cSupply >= 35 && stage == 3)
	{
		TilePosition expSite = BuildingPlacer::getInstance()->findExpansionSite();
		TilePosition startPoint = Broodwar->self()->getStartLocation();
		double dist = expSite.getDistance(startPoint);
		if (dist <= 45)
		{
			//Broodwar->printf("Expansion site close. Doing early expansion. (%d tiles)", (int)dist);
			Constructor::getInstance()->addBuildingFirst(UnitTypes::Terran_Command_Center);
		}
		stage++;
	}
	if (cSupply >= 60 && stage == 4)
	{
		backupSquad->addSetup(UnitTypes::Terran_Siege_Tank_Tank_Mode, 4);
		backupSquad->addSetup(UnitTypes::Terran_Goliath, 6);
		backupSquad->addSetup(UnitTypes::Terran_Marine, 12);
		backupSquad->addSetup(UnitTypes::Terran_Medic, 4);
		mainSquad->setBuildup(false);
		stage++;
	}

	if (Broodwar->self()->minerals() >= 1200 && stage == 5)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, cSupply));
		stage++;
	}
	if (Broodwar->self()->minerals() >= 1200 && stage == 6)
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Science_Facility, cSupply));
		
		mainSquad->addSetup(UnitTypes::Terran_Science_Vessel, 1);
		if (ExplorationManager::enemyIsProtoss())
		{
			//If we're facing Protoss, research EMP shockwave to deal
			//with shields.
			buildplan.push_back(BuildplanEntry(TechTypes::EMP_Shockwave, cSupply));
		}
		buildplan.push_back(BuildplanEntry(TechTypes::Defensive_Matrix, cSupply));

		airSquad1->addSetup(UnitTypes::Terran_Battlecruiser, 2);
		airSquad1->setBuildup(false);

		buildplan.push_back(BuildplanEntry(TechTypes::Yamato_Gun, cSupply));

		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Ship_Weapons, cSupply+1));
		buildplan.push_back(BuildplanEntry(UpgradeTypes::Terran_Ship_Weapons, cSupply+4));

		stage++;
	}
	if (Broodwar->self()->minerals() >= 1200 && stage == 7 && airSquad1->isActive())
	{
		buildplan.push_back(BuildplanEntry(UnitTypes::Terran_Starport, cSupply));

		airSquad2->addSetup(UnitTypes::Terran_Battlecruiser, 2);
		airSquad2->setBuildup(false);

		stage++;
	}
}

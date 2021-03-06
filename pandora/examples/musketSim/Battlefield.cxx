
#include <Battlefield.hxx>

#include <Soldier.hxx>
#include <Agent.hxx>
#include <Bullet.hxx>
#include <BattlefieldConfig.hxx>

#include <Raster.hxx>
#include <Point2D.hxx>
#include <Exceptions.hxx>
#include <Statistics.hxx>
#include <Rectangle.hxx>
#include <GeneralState.hxx>
#include <Logger.hxx>

#include <cmath>
#include <limits>

namespace MusketSim 
{

Battlefield::Battlefield( Engine::Simulation & simulation, const BattlefieldConfig & config ) : World(simulation, 1, true, config._resultsFile), _config(config)
{
}

Battlefield::~Battlefield()
{
}

void Battlefield::createRasters()
{
	registerDynamicRaster("virtualRecord", true);
	registerDynamicRaster("impacts", true);

	getDynamicRaster("virtualRecord").setInitValues(0, std::numeric_limits<int>::max(), 0);
	getDynamicRaster("impacts").setInitValues(0, std::numeric_limits<int>::max(), 0);
}

void Battlefield::createAgents()
{
	// 10 meters before starting combat
	// TODO to config!
	int distanceBetweenSides = 320;

	// blue side, top zone
	int numBlueSoldiers = _config._numBlueSoldiers;
	int numBlueColumns = numBlueSoldiers/_config._blueRanks;

	int numRedSoldiers = _config._numRedSoldiers;
	int numRedColumns = numRedSoldiers/_config._redRanks;


	// 50 meters for each side
	// 12 is 2 * 3 ranks * 2(0.5m)
	int heightBoundary = _simulation.getSize() - distanceBetweenSides - 12;
	heightBoundary /= 2;
	int widthBoundary = (_simulation.getSize() - std::max(numBlueColumns, numRedColumns)*2)/2;

	for(int i=0; i<numBlueSoldiers; i++)
	{
		int column = numBlueColumns-1-i%numBlueColumns;
		int rank = i/numBlueColumns;
		// each 1m.
		Engine::Point2D<int> posInRegiment((column*2)+widthBoundary, heightBoundary - rank*2);
	
		if(_boundaries.isInside(posInRegiment))
		{
			std::ostringstream oss;
			oss << "Soldier_blue" << i;
			Soldier * newSoldier = new Soldier(oss.str(), true, rank+1, _config._blueCohesionRating, _config._blueCohesionDistance, _config._blueAccuracy );
			newSoldier->setPosition(posInRegiment);

			int reloadingTime = 48;
			int interval = 0;
			int order = 0;	
			int platoon = (column*16/numBlueColumns)+1;
			switch((int)_config._blueTactics)
			{
				case eVolley:
					newSoldier->setDelayBeforeFirstFire(0);
					break;
				case eRanks:
					interval = reloadingTime/_config._blueRanks;
					order =  _config._blueRanks - rank - 1;
					newSoldier->setDelayBeforeFirstFire(order * interval); 
					break;
				case ePlatoon:
					interval = reloadingTime/4;
					if(rank!=0)
					{
						switch(platoon)
						{	
							case 1:
							case 4:
							case 7:
							case 10:
							case 13:
							case 16:
								order = 0;
								break;
							case 3:
							case 6:
							case 11:
							case 14:
								order = 1;
								break;
							default:
								order = 2;
						}
					}
					else
					{
						if(platoon==8 || platoon==9)
						{
							order = 2;
						}
						else
						{
							order = 3;
						}
					}
					newSoldier->setDelayBeforeFirstFire(order * interval); 
					break;
				case eCatalan:		
					interval = reloadingTime/8;
					switch(platoon)
					{	
						case 1:
						case 16:
							order = 0;
							break;
						case 2:
						case 15:
							order = 1;
							break;
						case 3:
						case 14:
							order = 2;
							break;
						case 4:
						case 13:
							order = 3;
							break;
						case 5:
						case 12:
							order = 4;
							break;
						case 6:
						case 11:
							order = 5;
							break;
						case 7:
						case 10:
							order = 6;
							break;
						case 8:
						case 9:
							order = 7;
							break;
						default:
							order = -1;
					}
					newSoldier->setDelayBeforeFirstFire(order * interval); 
					break;
				default:
					break;
			}	
			addAgent(newSoldier);
		}
	}


	for(int i=0; i<numRedSoldiers; i++)
	{
		int column = i%numRedColumns;
		int rank = i/numRedColumns;
		//std::cout << _id << " red soldier id: " << i << " of: "<< numRedSoldiers << " in rank: " << rank+1 << "/" << numRedLines << " and column: " << column << "/" << numRedColumns << std::endl;
		// each 1m.
		Engine::Point2D<int> posInRegiment((column*2)+widthBoundary, heightBoundary +distanceBetweenSides+(rank*2));
		if(_boundaries.isInside(posInRegiment))
		{
			std::ostringstream oss;
			oss << "Soldier_red" << i;
			Soldier * newSoldier = new Soldier(oss.str(), false, rank+1, _config._redCohesionRating, _config._redCohesionDistance, _config._redAccuracy );
			newSoldier->setPosition(posInRegiment);	

			int reloadingTime = 48;
			int interval = 0;
			int order = 0;	
			int platoon = (column*16/numRedColumns)+1;
			switch((int)_config._redTactics)
			{
				case eVolley:
					newSoldier->setDelayBeforeFirstFire(0);
					break;
				case eRanks:
					interval = reloadingTime/_config._redRanks;
					order =  _config._redRanks - rank - 1;
					newSoldier->setDelayBeforeFirstFire(order * interval); 
					break;
				case ePlatoon:
					interval = reloadingTime/4;
					if(rank!=0)
					{
						switch(platoon)
						{	
							case 1:
							case 4:
							case 7:
							case 10:
							case 13:
							case 16:
								order = 0;
								break;
							case 3:
							case 6:
							case 11:
							case 14:
								order = 1;
								break;
							default:
								order = 2;
						}
					}
					else
					{
						if(platoon==8 || platoon==9)
						{
							order = 2;
						}
						else
						{
							order = 3;
						}
					}
					newSoldier->setDelayBeforeFirstFire(order * interval); 
					break;
				case eCatalan:		
					interval = reloadingTime/8;
					switch(platoon)
					{	
						case 1:
						case 16:
							order = 0;
							break;
						case 2:
						case 15:
							order = 1;
							break;
						case 3:
						case 14:
							order = 2;
							break;
						case 4:
						case 13:
							order = 3;
							break;
						case 5:
						case 12:
							order = 4;
							break;
						case 6:
						case 11:
							order = 5;
							break;
						case 7:
						case 10:
							order = 6;
							break;
						case 8:
						case 9:
							order = 7;
							break;
						default:
							order = -1;
					}
					newSoldier->setDelayBeforeFirstFire(order * interval); 
					break;
				default:
					break;
			}	
			addAgent(newSoldier);
		}
	}
}
	
} // namespace MusketSim


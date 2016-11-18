#include <BWAPI.h>
#include <BWTA.h>
#include <MATH.h>
#include "PotentialFields.h"
using namespace BWAPI;
Position ForceMul(Position force,double a){
	Position ret = Position(force.x()*a,force.y()*a);
	return ret;
}

Position PotentialFields::GetForce(Unit* item,Position targetPos){
	
	double AttractorForceScale = attractorCoefficient*pow(targetPos.getDistance(item->getPosition()),attractorExponential);
	Position ret =ForceMul(targetPos-item->getPosition(),AttractorForceScale);
	if(Broodwar->enemy()->getUnits().size()>0){ 
		for(std::set<Unit*>::const_iterator j=Broodwar->enemy()->getUnits().begin();j!=Broodwar->enemy()->getUnits().end();j++)
		{	
			double enemyRepulserForceScale = enemyRepulserCoefficient*pow((*j)->getPosition().getDistance(item->getPosition()),enemyRepulserExponential);
			Position enemyRepulserForce =ForceMul(item->getPosition()-(*j)->getPosition(),enemyRepulserForceScale);
			ret +=enemyRepulserForce;
		}
	}
	if(Broodwar->self()->getUnits().size()>0){ 
		for(std::set<Unit*>::const_iterator j=Broodwar->self()->getUnits().begin();j!=Broodwar->self()->getUnits().end();j++)
		{	
			double friendlyRepulserForceScale = friendlyRepulserCoefficient*pow((*j)->getPosition().getDistance(item->getPosition()),friendlyRepulserExponential);
			Position friendlyRepulserForce =ForceMul(item->getPosition()-(*j)->getPosition(),friendlyRepulserForceScale);
			ret +=friendlyRepulserForce;
		}
	}
	return ret;
}


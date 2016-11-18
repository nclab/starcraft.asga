#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <string>

class PotentialFields{
private:	
	double attractorCoefficient;
	double attractorExponential;
	double friendlyRepulserCoefficient;
	double friendlyRepulserExponential;
	double enemyRepulserCoefficient;
	double enemyRepulserExponential;
public:
	PotentialFields(double ac,double ae,double fc,double fe,double ec,double ee){
		attractorCoefficient = ac;
		friendlyRepulserCoefficient = fc;
		enemyRepulserCoefficient = ec;
		attractorExponential = ae;
		friendlyRepulserExponential =fe;
		enemyRepulserExponential = ee;
	};
	BWAPI::Position GetForce(BWAPI::Unit* item,BWAPI::Position targetPos);
};


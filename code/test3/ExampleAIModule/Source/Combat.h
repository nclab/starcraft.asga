#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <string>
enum MyStrategy{
	KITING,
	ATKCLOSEST,
	ATKLOWHP,
	ATKFLY,
	ATKAIRWPEAON,
	NONE
};
class Combat{
private:	
	//BWAPI::Game* _Broodwar;
	static const int SOL_LENTH = 15;//STRATEGY_NUM * 5
	static const int STRATEGY_NUM = 3;
	int sol[SOL_LENTH];
	static const int diff = 1;
	std::map<int,int> damgeTable;
	int GetSol(MyStrategy,int);
    bool mycheck(BWAPI::Unit* i,BWAPI::Unit* closestTarget,BWAPI::Unit* lestHPinRangeTarget,int enemyInRange,int alleyInRange,int alleyCanAtkFlyDamage,int enemyCanFlyHP,MyStrategy myStrategy);
public:
	Combat(int temp){};
	void AttackLestestHpEnemy(BWAPI::Unit* i);
	void AttackLestestHpFlyEnemy(BWAPI::Unit* i);
	void AttackLestestHpAirWeaponEnemy(BWAPI::Unit* i);
	void AttackClosestEnemy(BWAPI::Unit* i);
	void KiteAttack(BWAPI::Unit* i);
	void RandMove(BWAPI::Unit* i);
	void Idle(BWAPI::Unit* i);
	void UnitsRun();
	MyStrategy GetStrategy(BWAPI::Unit* i);
	//int getSol(){return sol;}
	void SetFileName(std::string filename);
};

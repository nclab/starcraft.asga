#include "Combat.h"
#include <algorithm>
#include <map>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace BWAPI;
bool myfunction (Unit* x,Unit* y) { return (x->getHitPoints()<y->getHitPoints()); }

void DrawAttackLine(Unit* x,Unit* y){
	Broodwar->drawLineMap(x->getPosition().x(),x->getPosition().y(),
					                 y->getPosition().x(),y->getPosition().y(),
									 Colors::White );
}
void DrawAttackLineB(Unit* x,Unit* y){
	Broodwar->drawLineMap(x->getPosition().x(),x->getPosition().y(),
		y->getPosition().x(),y->getPosition().y(),Colors::Red);
}

int inline GetGroundDamagePoint(Unit* me){
	return me->getType().groundWeapon().damageAmount();
};
int inline GetAirDamagePoint(Unit* me){
	return me->getType().airWeapon().damageAmount();
};
bool Combat::mycheck(Unit* i,Unit* closestTarget,Unit* lestHPinRangeTarget,int enemyInRange,int alleyInRange,int alleyCanAtkFlyDamage,int enemyCanFlyHP,MyStrategy myStrategy){
    bool ret = false;
	int a,b,c,d;
	a = GetSol(myStrategy,1);
	b = GetSol(myStrategy,2);
	c = GetSol(myStrategy,3);
	d = GetSol(myStrategy,4);
	if (a==0 && b==0 && c==0 && d==0) {
		return false;
	}
	if ((i->getType().groundWeapon().maxRange()-closestTarget->getType().groundWeapon().maxRange()>a|| a ==0 )&&
		(enemyInRange < b || b ==0)  &&
		(lestHPinRangeTarget->getHitPoints() < c*GetGroundDamagePoint(i)|| c==0 || lestHPinRangeTarget == i) &&
		(enemyInRange - alleyInRange + 5 <= d || d==0)
		){
		ret = true;
	}
	 
	return ret;
}


int Combat::GetSol(MyStrategy myStrategy,int index){
	return sol[myStrategy*4+index-1];
}
void Combat::SetFileName(std::string filename){
	std::string tempName("GA/sol/");
	tempName.append(filename);
	std::fstream fp;
	fp.open(tempName.c_str(), std::ios::in);//開啟檔案
    if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
		Broodwar->sendText("filename: %s",filename.c_str());
        Broodwar->sendText("Fail to open file: %s",tempName.c_str());
		printf("filename: %s",filename.c_str());
        printf("Fail to open file: %s",tempName.c_str());
    }
	for (int i = 0;i < SOL_LENTH;++i){
		fp>>sol[i];
	}
	fp.close();//關閉檔案
}


void Combat::UnitsRun(){
	damgeTable.clear();
	if(Broodwar->self()->getUnits().size()>0){
		for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
		{
			MyStrategy mStrategy = GetStrategy(*i);
			//Broodwar->sendText("%d",mStrategy);
			char* retStr = "NONE";
			switch (mStrategy){				
			case KITING: 
				KiteAttack(*i);
				retStr = "KITING";
				break;
			case ATKCLOSEST: 
				AttackClosestEnemy(*i);
				retStr = "CLOSE";
				break;
			case ATKLOWHP: 
				AttackLestestHpEnemy(*i);//InRange
				retStr = "HP";
				break;
			case ATKFLY: 
				AttackLestestHpFlyEnemy(*i);
				retStr = "FLY";
				break;
			case ATKAIRWPEAON: 
				AttackLestestHpAirWeaponEnemy(*i);
				retStr = "AIRWEAPON";
				break;
				
			default:
				//do nothing
				//Idle(*i);
				break;
			}
			//show strategy on unit
			//Broodwar->drawTextMap((*i)->getPosition().x(),(*i)->getPosition().y(),retStr,Colors::White);
		}	
		for(std::set<Unit*>::const_iterator j=Broodwar->enemy()->getUnits().begin();j!=Broodwar->enemy()->getUnits().end();j++)
		{	
			char tempstr[5] = "0";
			/*if((*j)->getHitPoints()>0){
				sprintf(tempstr ,"%d",damgeTable[(*j)->getID()]);
				Broodwar->drawTextMap((*j)->getPosition().x(),(*j)->getPosition().y(),tempstr,Colors::White);
				sprintf(tempstr ,"%d",(*j)->getHitPoints());
				Broodwar->drawTextMap((*j)->getPosition().x(),(*j)->getPosition().y()-8,tempstr,Colors::White);
			}*/
			/*if((*j)->getType().isFlyer()){
				sprintf(tempstr ,"%s","flyer");
				Broodwar->drawTextMap((*j)->getPosition().x(),(*j)->getPosition().y()-8,tempstr,Colors::White);
			}*/
		}
	}
					
}


void Combat::AttackLestestHpEnemy(Unit* i){	
	if (Broodwar->enemy()->getUnits().size()>0 && i->getGroundWeaponCooldown() ==0 ){
		std::vector<BWAPI::Unit*> UnitsVector(Broodwar->enemy()->getUnits().begin(),Broodwar->enemy()->getUnits().end()); 
		if(UnitsVector.size()>1){
			std::sort(UnitsVector.begin(),UnitsVector.end(),myfunction);
		}// sort enemy by hp
		Unit* target = UnitsVector.front();
		for(std::vector<Unit*>::const_iterator j=UnitsVector.begin();j!=UnitsVector.end();j++){
			if( damgeTable[(*j)->getID()] < (*j)->getHitPoints() &&
				(i->getDistance(*j) <= i->getType().groundWeapon().maxRange())){
				target = (*j);
				break;
			}
		}
		if(!i->isAttacking()){	
			damgeTable[target->getID()]+=GetGroundDamagePoint(i);
			i->attack(target);
			DrawAttackLine(i,target);
		}
	}
	char tempstr[5] = "0";
	sprintf(tempstr ,"%d",i->getGroundWeaponCooldown());
	Broodwar->drawTextMap(i->getPosition().x(),i->getPosition().y()-6,tempstr,Colors::White);
}


void Combat::AttackLestestHpFlyEnemy(Unit* i){	
	if (Broodwar->enemy()->getUnits().size()>0 && i->getGroundWeaponCooldown() ==0 ){
		std::vector<BWAPI::Unit*> UnitsVector(Broodwar->enemy()->getUnits().begin(),Broodwar->enemy()->getUnits().end()); 
		if(UnitsVector.size()>1){
			std::sort(UnitsVector.begin(),UnitsVector.end(),myfunction);
		}// sort enemy by hp
		Unit* target = UnitsVector.front();
		for(std::vector<Unit*>::const_iterator j=UnitsVector.begin();j!=UnitsVector.end();j++){
			
			if( (*j)->getType().isFlyer() &&
				damgeTable[(*j)->getID()] < (*j)->getHitPoints() &&
				(i->getDistance(*j) <= i->getType().airWeapon().maxRange())){
				target = (*j);
				break;
			}
		}
		if(!i->isAttacking()){	
			damgeTable[target->getID()]+=GetGroundDamagePoint(i);
			i->attack(target);
			DrawAttackLine(i,target);
		}
	}
	char tempstr[5] = "0";
	sprintf(tempstr ,"%d",i->getGroundWeaponCooldown());
	Broodwar->drawTextMap(i->getPosition().x(),i->getPosition().y()-6,tempstr,Colors::White);
}


void Combat::AttackLestestHpAirWeaponEnemy(Unit* i){	
	if (Broodwar->enemy()->getUnits().size()>0 && i->getGroundWeaponCooldown() ==0 ){
		std::vector<BWAPI::Unit*> UnitsVector(Broodwar->enemy()->getUnits().begin(),Broodwar->enemy()->getUnits().end()); 
		if(UnitsVector.size()>1){
			std::sort(UnitsVector.begin(),UnitsVector.end(),myfunction);
		}// sort enemy by hp
		Unit* target = UnitsVector.front();
		for(std::vector<Unit*>::const_iterator j=UnitsVector.begin();j!=UnitsVector.end();j++){
			
			if( GetAirDamagePoint(*j) >  0 &&
				damgeTable[(*j)->getID()] < (*j)->getHitPoints() &&
				(i->getDistance(*j) <= i->getType().groundWeapon().maxRange())){
				target = (*j);
				break;
			}
		}
		if(!i->isAttacking()){	
			damgeTable[target->getID()]+=GetGroundDamagePoint(i);
			i->attack(target);
			DrawAttackLine(i,target);
		}
	}
	char tempstr[5] = "0";
	sprintf(tempstr ,"%d",i->getGroundWeaponCooldown());
	Broodwar->drawTextMap(i->getPosition().x(),i->getPosition().y()-6,tempstr,Colors::White);
}


void Combat::KiteAttack(Unit* i){
	//find closest and move
	if(Broodwar->enemy()->getUnits().size()>0){ 
		Unit* target= *Broodwar->enemy()->getUnits().begin();
		for(std::set<Unit*>::const_iterator j=Broodwar->enemy()->getUnits().begin();j!=Broodwar->enemy()->getUnits().end();j++)
		{	
			if((*j)->getHitPoints()>0){
				if(i->getDistance(*j) < i->getDistance(target) || target->getHitPoints() <=0 ){
					target = (*j);
				}
			}
		}
		
		if (i->getGroundWeaponCooldown()==0 && i->getDistance(target) <= i->getType().groundWeapon().maxRange() + 1)
		{
			i->attack(target);	
			DrawAttackLine(i,target);		
		}else{
			DrawAttackLineB(i,target);
			Position pos = i->getPosition() - target->getPosition();
			int tempMaxRange = (target->getType().groundWeapon().maxRange()+diff)>i->getType().groundWeapon().maxRange()?(target->getType().groundWeapon().maxRange()+diff): i->getType().groundWeapon().maxRange();
			double posX = target->getPosition().x() + pos.x() /pos.getLength() * tempMaxRange;
			double posY = target->getPosition().y() +pos.y() /pos.getLength() * tempMaxRange;
			Position posFinal(posX,posY);
			i->move(posFinal);				
			
			/*Broodwar->drawTextMap(i->getPosition().x(),i->getPosition().y(),
				target->getType().c_str(),Colors::White);*/
		}
	}

}

void Combat::RandMove(Unit* i){
	double mydis = 5.0;
	Unit* target;
	double AvgPosX = 0.0;
	double AvgPosY = 0.0;
	int unitCount = 0;
	for(std::set<Unit*>::const_iterator j=Broodwar->enemy()->getUnits().begin();j!=Broodwar->enemy()->getUnits().end();j++)
	{	
		if((*j)->getHitPoints()>0){
			AvgPosX += (*j)->getPosition().x();
			AvgPosY += (*j)->getPosition().y();
			++unitCount;

			target = *Broodwar->enemy()->getUnits().begin();
			if(i->getDistance(*j) < i->getDistance(target) || target->getHitPoints() <=0 ){
				target = (*j);
			}
		}
	}
	Position avgPos(AvgPosX/unitCount,AvgPosY/unitCount);

	if (true)
	{
		Position pos = i->getPosition() - avgPos;
		double posX = i->getPosition().x() + pos.x() /pos.getLength() * mydis;
		double posY = i->getPosition().y() + pos.y() /pos.getLength() * mydis;
		//double posY = pos.y:
		Position posMid(posX,posY);

		pos =  target->getPosition() - posMid;
		posX = posMid.x() + pos.x() /pos.getLength() *(pos.getLength()- i->getType().groundWeapon().maxRange());
		posY = posMid.y() + pos.y() /pos.getLength() *(pos.getLength()- i->getType().groundWeapon().maxRange());
		//double posY = pos.y:
		Position posFinal(posX,posY);
		//Position posFinal(-100,-100);
		i->move(posFinal);
		/*Broodwar->sendText("my pos: %d %d",(*i)->getPosition().x(),(*i)->getPosition().y());
		Broodwar->sendText("target pos: %d %d",target->getPosition().x(),target->getPosition().y());
		Broodwar->sendText("distance %d %d",pos.x(),pos.y());
		Broodwar->sendText("range %d",(*i)->getType().groundWeapon().maxRange());
		Broodwar->sendText("move to %d %d",posFinal.x(),posFinal.y());*/
		//attack
		Broodwar->sendText("range %d",i->getType().groundWeapon().maxRange());
		Broodwar->sendText("move to %d %d",posFinal.x(),posFinal.y());
	}  


}


void Combat::Idle(Unit* i){
	i->stop();
}
void Combat::AttackClosestEnemy(Unit* i){
	Unit* target;//find closest
	if(Broodwar->enemy()->getUnits().size()>0 && i->getGroundWeaponCooldown() == 0 ){
		target = *Broodwar->enemy()->getUnits().begin();
		for(std::set<Unit*>::const_iterator j=Broodwar->enemy()->getUnits().begin();j!=Broodwar->enemy()->getUnits().end();j++)
		{	
			if((*j)->getHitPoints()>0){
				if( (target->getHitPoints() <= 0)||
					(i->getDistance(*j) < i->getDistance(target)) &&
					(damgeTable[(*j)->getID()]<= (*j)->getHitPoints())){
					target = (*j);
				}
			}
		}
		if(target && target->getHitPoints()>=0){
			if(!i->isAttacking() && i->getGroundWeaponCooldown()==0){	
				damgeTable[target->getID()]+=GetGroundDamagePoint(i);
				i->attack(target);
				DrawAttackLine(i,target);
			}
		}
	}
}


MyStrategy Combat::GetStrategy(BWAPI::Unit* i){
	Unit* closestTarget;
    Unit* lestHPinRangeTarget;
	int enemyInRange = 0;
	int alleyInRange = 0;
    int alleyCanAtkFlyDamage = 0;
	int enemyCanFlyHP = 0;
	int hp = 999;
	if (Broodwar->enemy()->getUnits().size()>0){
		closestTarget = *Broodwar->enemy()->getUnits().begin();
		for(std::set<Unit*>::const_iterator j=Broodwar->enemy()->getUnits().begin();j!=Broodwar->enemy()->getUnits().end();j++)
		{	
			if((*j)->getHitPoints()>0){
				if( (closestTarget->getHitPoints() <= 0)||
					(i->getDistance(*j) < i->getDistance(closestTarget))){
					closestTarget = (*j);
				}
				if((*j)->getType().isFlyer()){
					enemyCanFlyHP += (*j)->getHitPoints();
				}
                if( i->getDistance(*j) <= i->getType().groundWeapon().maxRange()){
					++enemyInRange;
					if((*j)->getHitPoints() < hp){
						lestHPinRangeTarget = (*j);
						hp = (*j)->getHitPoints();
					}
				}
			}
		}
	}else{
		closestTarget = i;
		lestHPinRangeTarget = i;
	}
	if(hp == 999){
		lestHPinRangeTarget = i;
	}

	if (Broodwar->self()->getUnits().size()>0){
		for(std::set<Unit*>::const_iterator j=Broodwar->self()->getUnits().begin();j!=Broodwar->self()->getUnits().end();j++)
		{	
			if(GetAirDamagePoint(*j) > 0){
					alleyCanAtkFlyDamage+= GetAirDamagePoint(*j);
				}
			if((*j)->getHitPoints()>0){
                if( i->getDistance(*j) <= i->getType().groundWeapon().maxRange()){
					++alleyInRange;
				}
			}
		}
	}
	
	MyStrategy ret = NONE;
	bool priority[STRATEGY_NUM];
	priority[KITING] = mycheck(i,closestTarget,lestHPinRangeTarget,enemyInRange,alleyInRange,alleyCanAtkFlyDamage,enemyCanFlyHP,KITING);
	priority[ATKCLOSEST] = mycheck(i,closestTarget,lestHPinRangeTarget,enemyInRange,alleyInRange,alleyCanAtkFlyDamage,enemyCanFlyHP,ATKCLOSEST);
	priority[ATKLOWHP] = mycheck(i,closestTarget,lestHPinRangeTarget,enemyInRange,alleyInRange,alleyCanAtkFlyDamage,enemyCanFlyHP,ATKLOWHP);
	priority[ATKFLY] = mycheck(i,closestTarget,lestHPinRangeTarget,enemyInRange,alleyInRange,alleyCanAtkFlyDamage,enemyCanFlyHP,ATKFLY);
	//priority[ATKAIRWPEAON] = mycheck(i,closestTarget,lestHPinRangeTarget,enemyInRange,alleyInRange,alleyCanFly,enemyCanFly,ATKAIRWPEAON);

	for(int j =0;j < STRATEGY_NUM;++j ){
		int priorityIndex =  sol[SOL_LENTH - STRATEGY_NUM + j];
		if (priority[priorityIndex]){
			ret = (MyStrategy)priorityIndex;
			break;
		}
	}
	char mystr[STRATEGY_NUM+3]= {0}; 
	for(int j =0;j < STRATEGY_NUM;++j ){
		if (priority[j]){
			mystr[j] = '1';
		}else{
			mystr[j] = '0';
		}
	}

	mystr[STRATEGY_NUM+1] = '0'+ret;
	Broodwar->drawTextMap(i->getPosition().x(),i->getPosition().y(),mystr,Colors::White);
	
    return ret;
}
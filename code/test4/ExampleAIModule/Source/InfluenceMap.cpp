#include <BWAPI.h>
#include <BWTA.h>
#include "InfluenceMap.h"
using namespace BWAPI;

void InfluenceMap:: DrawIM(){
	for (int i = 0; i < tableHeight; ++i)
	{
		for (int j = 0; j < tableWidth; ++j)
		{
			//Broodwar->drawText(CoordinateType::Map,(i+0.5)*boxSize,(j+0.5)*boxSize,"%d",IMtable[i][j]);
			Broodwar->drawTextMap((i+0.5)*boxSize,(j+0.5)*boxSize,"%.2f",IMtable[i][j]);
		}
	}
}
void InfluenceMap:: Analyze(){
	ClearMap();
	if(Broodwar->enemy()->getUnits().size()>0){ 
		for(std::set<Unit*>::const_iterator j=Broodwar->enemy()->getUnits().begin();j!=Broodwar->enemy()->getUnits().end();j++)
		{	
			AnalyzeUnit(*j);
		}
	}
}
void InfluenceMap:: AnalyzeUnit(Unit* item){
	int damage = item->getType().groundWeapon().damageAmount();
	int Range = item->getType().groundWeapon().maxRange();
    Range = Range/32+8;
	
	for (int i = item->getTilePosition().x()- Range;i<item->getTilePosition().x()+Range;++i){
		for (int j = item->getTilePosition().y()- Range;j<item->getTilePosition().y()+ Range;++j){
			TilePosition tp(i,j);
			if(i==item->getTilePosition().x() && j ==item->getTilePosition().y()){
				IMtable[i][j] = damage;
			}else{
				IMtable[i][j] = damage / item->getTilePosition().getDistance(tp);
			}
			
		}
	}
}

void InfluenceMap:: Init(){
	tableWidth =Broodwar->mapWidth()*32/boxSize;
	tableHeight =Broodwar->mapHeight()*32/boxSize;
}
void InfluenceMap:: ClearMap(){
	for (int i = 0;i<=tableWidth;++i){
		for (int j = 0;j<=tableHeight;++j){
			IMtable[i][j] = 0;
		}
	}
}

Position InfluenceMap::FindBestPos(BWAPI::Unit* item,int Range){
	float min = IMtable[item->getTilePosition().x()][item->getTilePosition().y()];
	int rX=item->getTilePosition().x(),rY=item->getTilePosition().y();
	for (int i = item->getTilePosition().x()- Range;i<=item->getTilePosition().x()+Range;++i){
		for (int j = item->getTilePosition().y()- Range;j<=item->getTilePosition().y()+ Range;++j){
			Broodwar->drawTextMap((i)*boxSize,(j)*boxSize,"%.2f",IMtable[i][j]);
			TilePosition TP(i,j);
			if(IMtable[i][j]<min && item->getTilePosition().getDistance(TP)< Range){
				rX = i;
				rY = j;
			}
		}
	}
	Position ret(rX*32,rY*32);
	return ret;
}
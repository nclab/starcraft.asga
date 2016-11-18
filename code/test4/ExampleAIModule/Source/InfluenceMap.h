#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <string>

class InfluenceMap{
private:	
	int boxSize;
	int tableWidth;
	int tableHeight;
	float IMtable[1000][1000];
public:
	InfluenceMap(int p){
		boxSize= p;
	};
	void Init();
	void DrawIM();
	void Analyze();
	void AnalyzeUnit(BWAPI::Unit* item);
	void ClearMap();
	BWAPI::Position FindBestPos(BWAPI::Unit* item,int Range = 1);
};


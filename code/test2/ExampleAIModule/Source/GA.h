#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <string>
#include <vector>

struct GAsol{
		int solnum;
		int fitness;
		GAsol(){
			solnum = -1;
			fitness = -1;
		};
		GAsol(const GAsol& other){
			solnum = other.solnum;
			fitness = other.fitness;
		};
		bool operator < (const GAsol& y)
		{
			return fitness<y.fitness;
		};
};

static class GA{
private:	
	
	static int Generation;
	static int SolutionNo;
	static int MapCount;
	static const int MAXGENERATION = 90;
	static const int MAPMAXCOUNT = 9;
	static const int SOL_LENTH = 15;
	static const int STRATEGY_NUM = 3;
	static const int POPULATION = 40;
	static const int LEVELSIZE = 10;
	static const int MUTATION = 1; 
	static std::vector<GAsol> solVector;
public:
	GA();
	std::string SolutionName;
	std::string GetFileName();
	void SetScore(int score);
	static void ReadFitness();
	static void CreateNextGeneration();
	static void generateSolution(int index,int solA,int solB);
	static void generateRandSolution(int index);
	static int GetSolLenth(){return SOL_LENTH;}
	static int GetStrategyNum(){return STRATEGY_NUM;}
}MyGA;


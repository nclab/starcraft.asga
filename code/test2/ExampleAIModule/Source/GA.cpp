#include "GA.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

bool mySortOp(GAsol lhs,GAsol rhs)
{
	return lhs.fitness<rhs.fitness;
} 
int GA::Generation = 0;
int GA::SolutionNo = 0;
int GA::MapCount = 1;
std::vector<GAsol> GA::solVector;

string GA:: GetFileName(){
	return SolutionName;
}
GA::GA(){
	solVector = vector<GAsol>(POPULATION);
	solVector.resize(POPULATION);
	SolutionName = "XD";
	SolutionNo =0;
	char filename[]="GA/Progress.txt";
	std::fstream fp;
	fp.open(filename, std::ios::in);//開啟檔案
	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
		std::cout<<"Fail to open file: "<<filename<<std::endl;
	}
	fp>>Generation>>SolutionNo>>MapCount;
	fp.close();//關閉檔案
	if(SolutionNo == 0){
		CreateNextGeneration();
		SolutionNo = 1;
	}
	string temp;
	std::stringstream ss(temp);
	ss<<"G"<<Generation<<"S"<<SolutionNo<<".txt";
	SolutionName = ss.str();

}


void GA::SetScore(int score){
	if(SolutionNo == 0){

	}else{
		{
			char filename[]="GA/log.txt";
			fstream fp;
			fp.open(filename, ios::out|ios::app);//開啟檔案
			if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
				std::cout<<"Fail to open file: "<<filename<<std::endl;
			}
			fp<<"Name:"<<SolutionName<<" No: "<<MapCount<<" Score: "<<score<<std::endl;
			fp.close();//關閉檔案

			char filename2[] = "GA/FitnessLog.txt";
			if(SolutionNo == 1 && MapCount==1){
				fp.open(filename2, ios::out|ios::trunc);
			}else{
				fp.open(filename2, ios::out|ios::app);//開啟檔案 
			}

			if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
				std::cout<<"Fail to open file: "<<filename2<<std::endl;
			}
			fp<<SolutionNo<<" "<<MapCount<<" "<<score<<std::endl;
			fp.close();//關閉檔案
		}
		char filename[] ="GA/Progress.txt";
		fstream fp;
		fp.open(filename, std::ios::out);//開啟檔案
		if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
			std::cout<<"Fail to open file: "<<filename<<std::endl;
		}
		if(MapCount< MAPMAXCOUNT){
			fp<<Generation<<" "<<SolutionNo<<" "<<MapCount+1<<std::endl;
		}else if(SolutionNo<POPULATION && MapCount == MAPMAXCOUNT){
			fp<<Generation<<" "<<SolutionNo+1<<" "<<1<<std::endl;
		}else{
			fp<<Generation<<" 0 1"<<std::endl;
		}
		fp.close();//關閉檔案
	}
}

void GA::CreateNextGeneration(){
	if(Generation==0){
		++Generation;
		for(int i =1;i <=POPULATION;i++){
			generateRandSolution(i);		
		}
	}else if(Generation>MAXGENERATION){
		exit(0);
	}else{
		ReadFitness();
		++Generation;
		for(int i =1; i<=POPULATION;i++){
			int rankLevel = 1+ i/LEVELSIZE;			
			int solA = rand()%(rankLevel*LEVELSIZE);
			int solB = solA;
			while (solA ==solB){
				solB= rand()%(rankLevel*LEVELSIZE);
			}
			generateSolution(i,solVector[solA].solnum,solVector[solB].solnum);	
		}
	}
}

void GA::generateRandSolution(int index){
	string filename;
	std::stringstream ss(filename);
	ss<<"GA/sol/G"<<Generation<<"S"<<index<<".txt";
	fstream fp;
	fp.open(ss.str().c_str(), std::ios::out);//開啟檔案
    if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
        std::cout<<"Fail to open file: "<<filename<<std::endl;
    }
	for(int i =0;i< SOL_LENTH - STRATEGY_NUM ;i++){
		fp<<rand()%10<<" ";
	}
	vector<int> tempVec = vector<int>(STRATEGY_NUM);
	for(int i =0;i< STRATEGY_NUM ;i++){
		tempVec[i]=i;
	}
	for(int i =0;i< STRATEGY_NUM ;i++){
		int tempindex = rand()%tempVec.size();
		int tempret = tempVec[tempindex];
		tempVec.erase(tempVec.begin()+tempindex);
		fp<<tempret<<" ";
	}
	fp<<std::endl;
	fp.close();//關閉檔案
}

void GA::generateSolution(int index,int solA,int solB){
	string filename;
	std::stringstream ss(filename);
	fstream fp,fsA,fsB;
	int tempsolA[SOL_LENTH],tempsolB[SOL_LENTH];
	ss<<"GA/sol/G"<<Generation-1<<"S"<<solA<<".txt";
	fsA.open(ss.str().c_str(), std::ios::in);
	for(int i =0;i< SOL_LENTH ;i++){
		fsA>>tempsolA[i];
	}
	fsA.close();
	ss.str("");
	ss.clear();
	ss<<"GA/sol/G"<<Generation-1<<"S"<<solB<<".txt";
	fsB.open(ss.str().c_str(), std::ios::in);
	for(int i =0;i< SOL_LENTH ;i++){
		fsB>>tempsolB[i];
	}
	fsB.close();
	ss.str("");
	ss.clear();
	ss<<"GA/sol/G"<<Generation<<"S"<<index<<".txt";
	fp.open(ss.str().c_str(), std::ios::out);//開啟檔案
	for(int i =0;i< SOL_LENTH - STRATEGY_NUM ;i++){
		if(rand()%100 < MUTATION){
			fp<<rand()%10<<" ";
		}else{
			if(rand()%2){
				fp<<tempsolA[i]<<" ";
			}else{
				fp<<tempsolB[i]<<" ";
			}
		}
	}
	
	if(rand()%100 < MUTATION){
		vector<int> tempVec = vector<int>(STRATEGY_NUM);
		for(int i =0;i< STRATEGY_NUM ;i++){
			tempVec[i]=i;
		}
		for(int i =0;i< STRATEGY_NUM ;i++){
			int tempindex = rand()%tempVec.size();
			int tempret = tempVec[tempindex];
			tempVec.erase(tempVec.begin()+tempindex);
			fp<<tempret<<" ";
		}
	}else{	
		vector<int> tempVecA = vector<int>(STRATEGY_NUM);
		vector<int> tempVecB = vector<int>(STRATEGY_NUM);
		for(int i =0;i< STRATEGY_NUM ;i++){
			tempVecA[i]= tempsolA[i + SOL_LENTH - STRATEGY_NUM];
			tempVecB[i]= tempsolA[i + SOL_LENTH - STRATEGY_NUM];
		}
		if(rand()%2){
            int tempret =  tempVecA[0];
			fp<<tempret<<" ";
			vector<int>::iterator it = std::find(tempVecB.begin(),tempVecB.end(),tempret);
			tempVecB.erase(it);
			for(it = tempVecB.begin();it != tempVecB.end();it++ ){
				fp<<*it<<" ";
			}
		}else{
			int tempret =  tempVecB[0];
			fp<<tempret<<" ";
			vector<int>::iterator it = std::find(tempVecA.begin(),tempVecA.end(),tempret);
			tempVecA.erase(it);
			for(it = tempVecA.begin();it != tempVecA.end();it++ ){
				fp<<*it<<" ";
			}
		}
	}
	fp<<std::endl;
	fp.close();
}
void GA::ReadFitness(){
	char filename[]="GA/FitnessLog.txt";
	fstream fp,fdb;
	fdb.open("debug.txt", std::ios::out);//開啟檔案
	fdb<<"debug start"<<endl;
	fdb.flush();
	fp.open(filename, ios::in);//開啟檔案
	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
		std::cout<<"Fail to open file: "<<filename<<std::endl;
	}
	GAsol gaSol = GAsol();
	while(fp>>gaSol.solnum)  // EOF is false here
	{	
		int mapcount;
		fp>>mapcount>>gaSol.fitness;
		if(mapcount ==1){
			solVector[gaSol.solnum-1].solnum=gaSol.solnum;
			solVector[gaSol.solnum-1].fitness=gaSol.fitness;
		}else{
			solVector[gaSol.solnum-1].fitness+=gaSol.fitness;
		}
		//solVector.push_back(gaSol);
	}
	fp.close();//關閉檔案
	for(int i =0;i<solVector.size();i++){
		fdb<<solVector[i].fitness/MAPMAXCOUNT<<" ";fdb.flush();
	}
	fdb<<endl;fdb.flush();
	
	//sort(solVector.begin(),solVector.end());
	for(int i =0;i<solVector.size();++i){
		int maxindex = i;
		for(int j =i;j<solVector.size();++j){
			if(solVector[j].fitness>solVector[maxindex].fitness){
				maxindex = j;
			}
		}
		if(maxindex != i){
			//fdb<<solVector[i].fitness<<" "<<solVector[maxindex].fitness;fdb.flush();
			gaSol = solVector[maxindex];
			solVector[maxindex] = solVector[i];
			solVector[i] = gaSol;
			//fdb<<solVector[i].fitness<<" "<<solVector[maxindex].fitness;fdb.flush();
		}
	}
	for(int i =0;i<solVector.size();i++){
		fdb<<solVector[i].fitness/MAPMAXCOUNT<<" ";fdb.flush();
	}
	fdb<<endl;	fdb.flush();
	fdb.close();//關閉檔案
	//system("pause");
}

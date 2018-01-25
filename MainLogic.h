#ifndef __MainLogic_H__
#define __MainLogic_H__
#include "Game.h"
#include <string>
#include "plotData.h"

inline void promptGame(int * N, int * M)
{
	string temp;
	cout << "# of Players? (N)" << endl;
	getline(cin, temp);
	*N = stoi(temp);
	temp = "";

	cout << "# of Potato Passes? (M)" << endl;
	getline(cin, temp);
	*M = stoi(temp);
}

inline void mainLogic(AbstractJosephus * gInterface)
{

	cout << endl << "!!! BEGAN TESTING " << " !!!" << endl;
	cout << endl << "!!! TESTING PLAY !!!" << endl;
	int N;

	auto g = Game(gInterface);
	g.play();


	//Time Code, M Constant
	cout << endl << "!!! TESTING TIME DEPENDENCE ON PLAYERS!!!" << endl;
	auto mData = queue<plotData>();
	double totalTime = 0;
	double testTime;
	auto trials = 0;
	auto eliminations = 0;
	auto M = 3;
	for (N = 4; N <= 1024; N *= 2)
	{
		trials++;
		g.reset(N, M);
		testTime = g.timePlay(&eliminations);		
		totalTime += testTime;
		mData.push(plotData(N, M, testTime, eliminations));
	}
	auto averageTime = totalTime / trials;
	cout << "CPU time in seconds for M:" << M << " N:[4,8,16,32...1024]" << endl;
	cout << "Total Test Time : " << totalTime << endl;
	cout << "Average Test Time : " << averageTime << endl;
	cout << "Elim Times printed to file due to #" << endl;



	//Time Code, N constant
	cout << endl << "!!! TESTING TIME DEPENDENCE ON PLAYERS!!!" << endl;
	auto nData = queue<plotData>();
	totalTime = 0;
	trials = 0;
	eliminations = 0;
	N = 1024;
	for (M = 2; M < N; M *= 2)
	{
		trials++;
		g.reset(N, M);
		testTime = g.timePlay(&eliminations);
		totalTime += testTime;
		nData.push(plotData(N, M, testTime, eliminations));
	}
	averageTime = totalTime / trials;
	cout << "CPU time in seconds for M:" << "[2,4,8,16,32...N)" << " N:" << N << endl;
	cout << "Total Test Time : " << totalTime << endl;
	cout << "Average Test Time : " << averageTime << endl;
	cout << "Elim Times printed to file due to #" << endl;


	//Done, Yay!
	cout << endl << "!!! FINISHED TESTING !!!" << endl;
	cout << endl << "!!! WRITING ELIM TIME RESULTS !!!" << endl;

	auto mFile = ofstream("mData.csv");
	while (mData.size() > 0)
	{
		auto data = mData.front();
		mData.pop();
		data.writeToFile(mFile);
	}
	mFile.close();
	auto nFile = ofstream("nData.csv");
	while (nData.size() > 0)
	{
		auto data = nData.front();
		nData.pop();
		data.writeToFile(nFile);
	}
	nFile.close();


	//Prompt to avoid closing the terminal!
	cout << endl << "!!! PRESS ANY KEY TO EXIT !!!" << endl;
	string temp;
	getline(cin, temp);
	
}
#endif
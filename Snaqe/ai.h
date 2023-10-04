#pragma once
#include <QPoint>
#include <vector>
#include <list>
#include <set>
#include "astar.h"
#include "hamilton.h"

using namespace std;

typedef unsigned char uint8;

class AI
{
public:
	AI(int);
	~AI();

	QPoint MakeDecision(vector<QPoint>, QPoint);
	void ResetAI(void);

private:
	// vector<vector<int>> CreateMaze(vector<QPoint>);
	vector<QPoint> VirtualSnake(vector<QPoint>, vector<QPoint>);
	QPoint GetNextStep(vector<QPoint>, vector<QPoint>, int);
	QPoint NextStep(QPoint);
	int LoadSteps(vector<QPoint>);
	bool CanEat(vector<QPoint>, vector<QPoint>);
	bool FindFood(vector<QPoint>, QPoint);
	bool FindFoodByHamilton(vector<QPoint>, QPoint);
	bool FindTail(vector<QPoint>);
	bool FindTailByHamilton(vector<QPoint>);
	bool PathNotUsedUp(uint8);
	QPoint CanGo(vector<QPoint>, QPoint);
	bool DeadStep(QPoint, vector<QPoint>);

	// for debug
	bool ValidPath(vector<QPoint>);
	bool ValidStep(QPoint);
	bool IsNeighbor(QPoint, QPoint);
	bool HasRepeated(vector<QPoint>);

	vector<QPoint> predict;
	Hamilton *hamilton;
	Astar *astar;
	int N, step;
	uint8 typath;

	enum enumType
	{
		HAMILTON_FOOD = 0,
		ASTAR_FOOD,
		HAMILTON_TAIL,
		ASTAR_TAIL,
		EMPTY_PATH
	};
};
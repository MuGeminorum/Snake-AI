#pragma once
#include <QPoint>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Block
{
	bool available, visited;
};

class Hamilton
{
public:
	Hamilton(int);
	~Hamilton();
	void InitHamilton(vector<QPoint>);
	bool GetPath(QPoint, vector<QPoint> &);

private:
	vector<vector<Block>> CreateMask(vector<QPoint>);
	bool IsNeigbor(QPoint, QPoint);
	void DFS(int, int, int, vector<QPoint> &);
	void LoadPath(int, vector<QPoint> &);
	void RegionGrow(QPoint);

	// For debug
	void PrintMat(vector<vector<int>>);
	void PrintArr(vector<QPoint>);

	vector<vector<Block>> mask;
	vector<vector<int>> G;
	vector<QPoint> vertex;
	vector<int> visited, prior;
	QPoint head;
	int N;
	bool found;
};
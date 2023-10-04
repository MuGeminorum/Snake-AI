#pragma once
#include <QPoint>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

struct Point
{
	int x, y;
	int F, G, H; // F=G+H
	Point *parent;
	Point(int _x, int _y) : x(_x), y(_y), F(0), G(0), H(0), parent(NULL) {}
};

class Astar
{
public:
	Astar(int);
	~Astar();
	bool GetPath(QPoint, vector<QPoint> &);
	bool HasPath(QPoint);
	void InitAstar(vector<QPoint>);

private:
	vector<vector<int>> CreateMaze(vector<QPoint>);
	vector<Point *> GetSurroundPoints(const Point *, bool) const;
	list<Point *> GetPath(Point &, Point &, bool);
	Point *FindPath(Point &, Point &, bool);
	Point *InList(const list<Point *> &, const Point *) const;
	Point *GetLeastFpoint(void);
	QPoint FillDiag(QPoint, QPoint, QPoint);
	int CalcG(Point *, Point *);
	int CalcH(Point *, Point *);
	int CalcF(Point *);
	bool CanReach(const Point *, const Point *, bool) const;
	bool SmoothPath(vector<QPoint> &);
	bool FindDiag(QPoint, QPoint);

	// For debug
	bool HasDiag(vector<QPoint> QPath);
	void PrintPath(vector<QPoint> QPath);

	vector<vector<int>> maze;
	list<Point *> openList, closeList;
	QPoint head, neck;
	int N;
};
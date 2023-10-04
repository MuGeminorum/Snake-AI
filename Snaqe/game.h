#pragma once
#include <vector>

using namespace std;

class Game
{
public:
	Game(int);
	~Game();

	QPoint GetSnakeHead(void);
	QPoint GetSnakeNeck(void);
	QPoint GetSnakeTail(void);
	QPoint GetFood(void);
	int GetSnake(vector<QPoint> &);
	int SnakeMove(void);
	int GetSnakeLength(void);
	void SetSnakeDirection(QPoint);
	void Reset(void);

private:
	int RandNum(int, int);
	bool OnSnake(QPoint);
	bool EatFood(void);
	bool SnakeDead(void);
	void SnakeGrow(void);
	void CreateFood(void);
	void CreateSnake(void);

	struct SnakeBody
	{
		QPoint NOW;
		QPoint OLD;

		SnakeBody()
		{
			NOW = QPoint(0, 0);
			OLD = QPoint(0, 0);
		}
	};

	vector<SnakeBody> snake;
	vector<QPoint> emptyBlocks;
	QPoint headDirection, food;
	int N;
};

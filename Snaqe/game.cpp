#include <QtWidgets>
#include <stdlib.h>
#include <time.h>
#include "game.h"

Game::Game(int screen_size)
{
	N = screen_size;
	CreateSnake();
	CreateFood();
}

Game::~Game()
{
}

int Game::GetSnake(vector<QPoint> &snakeBody)
{
	for (auto i : snake)
		snakeBody.push_back(i.NOW);

	return snake.size();
}

QPoint Game::GetFood()
{
	return food;
}

void Game::CreateSnake()
{
	int a = 2;
	int b = N - 2;
	int x = RandNum(a, b);
	int y = RandNum(a, b);

	SnakeBody head;
	head.NOW = QPoint(x, y);
	head.OLD = head.NOW;
	snake.push_back(head);

	switch (RandNum(0, 4)) // QPoint(cos(k*pi/2), sin(k*pi/2))
	{
	case 0:
		headDirection = QPoint(1, 0);
		break;

	case 1:
		headDirection = QPoint(0, 1);
		break;

	case 2:
		headDirection = QPoint(-1, 0);
		break;

	default:
		headDirection = QPoint(0, -1);
		break;
	}

	SnakeBody tail;
	tail.NOW = snake.front().NOW - headDirection;
	snake.push_back(tail);
}

int Game::RandNum(int a, int b)
{
	srand((unsigned)time(NULL));
	return (rand() % (b - a)) + a;
}

QPoint Game::GetSnakeHead()
{
	return snake.front().NOW;
}

QPoint Game::GetSnakeNeck()
{
	return snake.at(1).NOW;
}

QPoint Game::GetSnakeTail()
{
	return snake.back().NOW;
}

void Game::SetSnakeDirection(QPoint towards)
{
	if (headDirection != -towards &&
		snake.front().NOW + towards != snake.at(1).NOW)
		headDirection = towards;
}

bool Game::SnakeDead()
{
	int L = snake.size();
	QPoint H = snake.front().NOW;

	if (L > N * N || H.x() >= N || H.y() >= N || H.x() < 0 || H.y() < 0)
		return true; // check overbound

	if (L > 4) // check bit self
	{
		for (int i = 4; i <= L - 1; i++)
		{
			if (snake.front().NOW == snake.at(i).NOW)
				return true;
		}
	}

	return false;
}

int Game::SnakeMove() // upd snake position from tail to head
{
	int res = 1;
	int L = snake.size();

	snake[L - 1].OLD = snake[L - 1].NOW;

	for (int i = L - 1; i >= 1; i--)
		snake[i].NOW = snake[i - 1].NOW;

	snake[0].NOW += headDirection;

	if (EatFood())
	{
		SnakeGrow();

		if (GetSnakeLength() == N * N)
			return -3; // Win

		CreateFood();
		res = 2; // Eat
	}

	if (SnakeDead())
		return -2; // Lose

	return res; // 1 = Playing
}

void Game::CreateFood(void)
{
	emptyBlocks.clear();
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			QPoint tmp = QPoint(i, j);
			if (!OnSnake(tmp))
				emptyBlocks.push_back(tmp);
		}
	}

	food = emptyBlocks.at(RandNum(0, emptyBlocks.size()));
}

bool Game::OnSnake(QPoint p)
{
	for (auto i : snake)
		if (i.NOW == p)
			return true;

	return false;
}

void Game::SnakeGrow(void)
{
	SnakeBody tail;
	tail.NOW = snake.back().OLD;
	snake.push_back(tail);
}

int Game::GetSnakeLength()
{
	return snake.size();
}

bool Game::EatFood()
{
	return food == snake.front().NOW;
}

void Game::Reset()
{
	snake.clear();
	CreateSnake();
	CreateFood();
}
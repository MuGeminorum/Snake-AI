#include <QVector3D>
#include <QDebug>
#include <QPoint>
#include <QKeyEvent>
#include "screen.h"

#define N 8
#define h 1.0f / N
#define d 0.125f * h

Screen::Screen(QWidget *parent) : QOpenGLWidget(parent)
{
	autoMode = false;
	gameStatus = RESET;
	game = new Game(N);
	ai = new AI(N);
}

void Screen::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background
	setFocusPolicy(Qt::StrongFocus);
}

void Screen::resizeGL(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
}

void Screen::paintGL()
{
	switch (gameStatus)
	{
	case EAT: // 2
		DrawGame();
		emit update_length_sender(game->GetSnakeLength());
		gameStatus = PLAYING;
		break;

	case PLAYING: // 1
		DrawGame();
		break;

	case RESET: // 0
		DrawGame();
		gameStatus = PAUSE;
		break;

	case PAUSE: // -1
		DrawGame();
		break;

	case GAMEOVER: // -2
		DrawLose();
		break;

	case WIN: // -3
		DrawCup();
		break;

	default:
		break;
	}
}

void Screen::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space)
		PauseResume();

	if (event->key() == Qt::Key_F1)
		ChangeMode();

	if (event->key() == Qt::Key_F2)
		ResetGame();

	UpDownLeftRight(event->key());
}

void Screen::update_screen_receiver(void)
{
	if (gameStatus > 0)
	{
		if (autoMode)
		{
			vector<QPoint> snake;
			game->GetSnake(snake);
			QPoint food = game->GetFood();
			QPoint aiDecision = ai->MakeDecision(snake, food);
			game->SetSnakeDirection(aiDecision);
		}

		gameStatus = (status)game->SnakeMove();
		update();
	}
}

void Screen::UpDownLeftRight(int key)
{
	if (gameStatus == PLAYING && !autoMode)
	{
		switch (key)
		{
		case Qt::Key_Left:
			game->SetSnakeDirection(QPoint(-1, 0));
			break;
		case Qt::Key_Right:
			game->SetSnakeDirection(QPoint(1, 0));
			break;
		case Qt::Key_Up:
			game->SetSnakeDirection(QPoint(0, 1));
			break;
		case Qt::Key_Down:
			game->SetSnakeDirection(QPoint(0, -1));
			break;
		default:
			break;
		}
	}
}

void Screen::PauseResume()
{
	if (gameStatus == PLAYING)
	{
		gameStatus = PAUSE;
		emit pause_game_sender();
	}
	else if (gameStatus == PAUSE)
	{
		gameStatus = PLAYING;
		emit resume_game_sender();
	}
}

void Screen::ChangeMode()
{
	autoMode = !autoMode;
	ai->ResetAI();
	emit mode_change_sender(autoMode);
}

void Screen::ResetGame()
{
	gameStatus = RESET;
	game->Reset();
	ai->ResetAI();
	update();
	emit reset_game_sender();
}

void Screen::DrawBlock(QPoint p, int R, int G, int B)
{
	glBegin(GL_QUADS);
	glColor3f(R, G, B);
	glVertex2f(p.x() * h + d, p.y() * h + d); // left bottom
	glColor3f(R, G, B);
	glVertex2f(p.x() * h + d, (p.y() + 1) * h - d); // left top
	glColor3f(R, G, B);
	glVertex2f((p.x() + 1) * h - d, (p.y() + 1) * h - d); // right top
	glColor3f(R, G, B);
	glVertex2f((p.x() + 1) * h - d, p.y() * h + d); // right bottom
	glEnd();
}

void Screen::LinkBlocks(QPoint p0, QPoint p1, int R, int G, int B)
{
	float midX = 0.5f * (p0.x() + p1.x() + 1); // get x of midpoint of p0 and p1 block
	float midY = 0.5f * (p0.y() + p1.y() + 1); // get y of midpoint of p0 and p1 block

	float dx = (p0.x() == p1.x()) ? (0.5f * h - d) : d;
	float dy = (p0.y() == p1.y()) ? (0.5f * h - d) : d;

	float left = midX * h - dx;
	float right = midX * h + dx;
	float bottom = midY * h - dy;
	float top = midY * h + dy;

	glBegin(GL_QUADS);
	glColor3f(R, G, B);
	glVertex2f(left, bottom); // left bottom
	glColor3f(R, G, B);
	glVertex2f(left, top); // left top
	glColor3f(R, G, B);
	glVertex2f(right, top); // right top
	glColor3f(R, G, B);
	glVertex2f(right, bottom); // right bottom
	glEnd();
}

void Screen::DrawSnake(void)
{
	int i;
	vector<QPoint> Sn;
	int L = game->GetSnake(Sn);

	DrawBlock(Sn.front(), 1, 1, 0); // Yellow head
	for (i = 1; i < L - 1; i++)
	{
		DrawBlock(Sn.at(i), 0, 1, 0); // Green body
	}
	DrawBlock(Sn.back(), 0, 1, 1); // Cyan tail

	for (i = 0; i <= L - 2; i++)
	{
		LinkBlocks(Sn.at(i), Sn.at(i + 1), 0, 1, 0); // Green linkers
	}
}

void Screen::DrawFood(void)
{
	DrawBlock(game->GetFood(), 1, 0, 0); // Draw red food.
}

void Screen::DrawCup(void)
{
	int i, j;

	int cup_8[N][N] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 1, 1, 1, 0, 1},
		{1, 0, 1, 1, 1, 1, 0, 1},
		{0, 1, 1, 1, 1, 1, 1, 0},
		{0, 0, 1, 1, 1, 1, 0, 0},
		{0, 0, 0, 1, 1, 0, 0, 0},
		{0, 0, 0, 1, 1, 0, 0, 0},
		{0, 0, 1, 1, 1, 1, 0, 0} // 0
	};

	glClear(GL_COLOR_BUFFER_BIT);

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (cup_8[i][j])
			{
				DrawBlock(QPoint(j, N - i - 1), 1, 1, 0);
			}
		}
	}

	emit update_length_sender(game->GetSnakeLength());
	emit win_game_sender();
}

void Screen::DrawLose(void)
{
	int i, j;

	int Lose_8[N][N] = {
		{1, 0, 0, 0, 0, 0, 0, 1},
		{0, 1, 0, 0, 0, 0, 1, 0},
		{0, 0, 1, 0, 0, 1, 0, 0},
		{0, 0, 0, 1, 1, 0, 0, 0},
		{0, 0, 0, 1, 1, 0, 0, 0},
		{0, 0, 1, 0, 0, 1, 0, 0},
		{0, 1, 0, 0, 0, 0, 1, 0},
		{1, 0, 0, 0, 0, 0, 0, 1} // 0
	};

	glClear(GL_COLOR_BUFFER_BIT);

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (Lose_8[i][j])
			{
				DrawBlock(QPoint(j, N - i - 1), 1, 0, 1);
			}
		}
	}

	emit lose_game_sender();
}

void Screen::DrawGame()
{
	glClear(GL_COLOR_BUFFER_BIT);
	DrawFood();
	DrawSnake();
}
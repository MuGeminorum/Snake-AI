#ifndef GLWIDGET_H
#define GLWIDGET_H

#ifdef QT_DEBUG
#pragma comment(lib, "qtmaind.lib")
#pragma comment(lib, "Qt5Cored.lib")
#pragma comment(lib, "Qt5Guid.lib")
#pragma comment(lib, "Qt5OpenGLd.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "Qt5Widgetsd.lib")
#endif

#include <QtWidgets/QOpenGLWidget>
#include "game.h"
#include "ai.h"

class QVector3D;
class Screen : public QOpenGLWidget
{
	Q_OBJECT

public:
	Screen(QWidget * = 0);

private:
	enum status
	{
		EAT = 2,
		PLAYING = 1,
		RESET = 0,
		PAUSE = -1,
		GAMEOVER = -2,
		WIN = -3,
	};

	AI *ai;
	Game *game;
	bool autoMode;
	status gameStatus;

	void DrawCup(void);
	void DrawLose(void);
	void DrawFood(void);
	void DrawSnake(void);
	void DrawGame(void);
	void ResetGame(void);
	void ChangeMode(void);
	void PauseResume(void);
	void UpDownLeftRight(int);
	void DrawBlock(QPoint, int, int, int);
	void LinkBlocks(QPoint, QPoint, int, int, int);

protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int, int);
	void keyPressEvent(QKeyEvent *);

signals:
	void win_game_sender(void);
	void lose_game_sender(void);
	void pause_game_sender(void);
	void resume_game_sender(void);
	void reset_game_sender(void);
	void mode_change_sender(bool);
	void update_length_sender(int);

public slots:
	void update_screen_receiver(void);
};

#endif // GLWIDGET_H

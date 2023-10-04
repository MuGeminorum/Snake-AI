#ifndef SNAQE_H
#define SNAQE_H

#include <QtWidgets>
#include <QTimer>

class Snaqe : public QMainWindow
{
	Q_OBJECT

public:
	Snaqe(QWidget * = 0);
	~Snaqe();

private:
	long lastTime;
	int timerInterval;
	QLabel *autoLabel;
	QLabel *timeLabel;
	QLabel *timeValue;
	QLabel *lengthValue;
	QLabel *lengthLabel;
	QLabel *messageLabel;
	QTimer *refreshTimer;
	QWidget *centralWidget;
	QOpenGLWidget *openGLWidget;
	void setupUi(QMainWindow *);
	void retranslateUi(QMainWindow *);

public slots:
	void update_time_receiver(void);
	void update_length_receiver(int);
	void mode_change_receiver(bool);
	void win_game_receiver(void);
	void lose_game_receiver(void);
	void pause_game_receiver(void);
	void resume_game_receiver(void);
	void reset_game_receiver(void);
};

#endif // SNAQE_H

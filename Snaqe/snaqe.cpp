#include "snaqe.h"
#include "screen.h"

#define INTERVAL_AUTO 60
#define INTERVAL_MANUAL 180

Snaqe::Snaqe(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
}

Snaqe::~Snaqe()
{
}

void Snaqe::setupUi(QMainWindow *greedyClass)
{
	if (greedyClass->objectName().isEmpty())
		greedyClass->setObjectName(QStringLiteral("greedyClass"));

	greedyClass->setFixedSize(512, 615);
	greedyClass->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

	centralWidget = new QWidget(greedyClass);
	centralWidget->setObjectName(QStringLiteral("centralWidget"));
	timeValue = new QLabel(centralWidget);
	timeValue->setObjectName(QStringLiteral("Score"));
	timeValue->setGeometry(QRect(120, 20, 80, 20));
	QFont font;
	font.setFamily(QStringLiteral("Courier New"));
	font.setPointSize(10);
	timeValue->setFont(font);
	timeValue->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
	timeValue->setTextFormat(Qt::AutoText);

	openGLWidget = new Screen(centralWidget);
	openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
	openGLWidget->setGeometry(QRect(30, 60, 450, 450));
	openGLWidget->setFont(font);
	openGLWidget->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
	openGLWidget->setSizePolicy(sizePolicy);

	lengthValue = new QLabel(centralWidget);
	lengthValue->setObjectName(QStringLiteral("Length"));
	lengthValue->setGeometry(QRect(400, 20, 72, 21));
	lengthValue->setFont(font);
	lengthValue->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
	lengthValue->setTextFormat(Qt::AutoText);

	lengthLabel = new QLabel(centralWidget);
	lengthLabel->setObjectName(QStringLiteral("LengthLabel"));
	lengthLabel->setGeometry(QRect(320, 20, 65, 21));
	lengthLabel->setFont(font);
	lengthLabel->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
	lengthLabel->setTextFormat(Qt::AutoText);
	timeLabel = new QLabel(centralWidget);
	timeLabel->setObjectName(QStringLiteral("ScoreLabel"));
	timeLabel->setGeometry(QRect(40, 20, 51, 20));
	timeLabel->setFont(font);
	timeLabel->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
	timeLabel->setTextFormat(Qt::AutoText);
	messageLabel = new QLabel(centralWidget);
	messageLabel->setObjectName(QStringLiteral("MessageLabel"));
	messageLabel->setGeometry(QRect(40, 530, 431, 31));
	messageLabel->setFont(font);
	messageLabel->setTextFormat(Qt::AutoText);

	autoLabel = new QLabel(centralWidget);
	autoLabel->setObjectName(QStringLiteral("AutoLabel"));
	autoLabel->setGeometry(QRect(40, 565, 431, 31));
	autoLabel->setFont(font);
	autoLabel->setTextFormat(Qt::AutoText);

	greedyClass->setCentralWidget(centralWidget);

	timerInterval = INTERVAL_MANUAL;
	lastTime = 0;
	refreshTimer = new QTimer;

	retranslateUi(greedyClass);

	QObject::connect(refreshTimer, SIGNAL(timeout()), openGLWidget, SLOT(update_screen_receiver()));
	QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(update_time_receiver()));

	QObject::connect(openGLWidget, SIGNAL(pause_game_sender()), this, SLOT(pause_game_receiver()));
	QObject::connect(openGLWidget, SIGNAL(resume_game_sender()), this, SLOT(resume_game_receiver()));
	QObject::connect(openGLWidget, SIGNAL(reset_game_sender()), this, SLOT(reset_game_receiver()));
	QObject::connect(openGLWidget, SIGNAL(lose_game_sender()), this, SLOT(lose_game_receiver()));
	QObject::connect(openGLWidget, SIGNAL(win_game_sender()), this, SLOT(win_game_receiver()));
	QObject::connect(openGLWidget, SIGNAL(mode_change_sender(bool)), this, SLOT(mode_change_receiver(bool)));
	QObject::connect(openGLWidget, SIGNAL(update_length_sender(int)), this, SLOT(update_length_receiver(int)));

	QObject::connect(openGLWidget, SIGNAL(win_game_sender()), refreshTimer, SLOT(stop()));
	QObject::connect(openGLWidget, SIGNAL(lose_game_sender()), refreshTimer, SLOT(stop()));
	QObject::connect(openGLWidget, SIGNAL(pause_game_sender()), refreshTimer, SLOT(stop()));
	QObject::connect(openGLWidget, SIGNAL(reset_game_sender()), refreshTimer, SLOT(stop()));
	QObject::connect(openGLWidget, SIGNAL(resume_game_sender()), refreshTimer, SLOT(start()));

	QMetaObject::connectSlotsByName(greedyClass);
} // setupUi

void Snaqe::retranslateUi(QMainWindow *greedyClass)
{
	greedyClass->setWindowFlags(this->windowFlags() & ~Qt::WindowMaximizeButtonHint);
	greedyClass->setWindowTitle(QApplication::translate("greedyClass", "Snaqe", 0));
	timeValue->setText(QApplication::translate("greedyClass", "00:00:00", 0));
	lengthValue->setText(QApplication::translate("greedyClass", "2", 0));
	lengthLabel->setText(QApplication::translate("greedyClass", "Length:", 0));
	timeLabel->setText(QApplication::translate("greedyClass", "Time:", 0));
	messageLabel->setText(QApplication::translate("greedyClass", "Press [Space] to start.", 0));
	autoLabel->setText(QApplication::translate("greedyClass", "Press [F1] to enter AI mode.", 0));
	refreshTimer->setInterval(timerInterval);
}

void Snaqe::update_time_receiver()
{
	lastTime += timerInterval;

	int ms = lastTime;
	int ss = 1000;
	int mi = ss * 60;
	int hh = mi * 60;

	long hour = ms / hh;
	long minute = (ms - hour * hh) / mi;
	long second = (ms - hour * hh - minute * mi) / ss;

	QString strBuffer = QString("%1:%2:%3")
							.arg(hour, 2, 10, QLatin1Char('0'))
							.arg(minute, 2, 10, QLatin1Char('0'))
							.arg(second, 2, 10, QLatin1Char('0'));

	timeValue->setText(strBuffer);
}

void Snaqe::update_length_receiver(int l)
{
	lengthValue->setText(QString::number(l));
}

void Snaqe::reset_game_receiver()
{
	lastTime = 0;
	lengthValue->setNum(2);
	timeValue->setText(QApplication::translate("greedyClass", "00:00:00", 0));
	messageLabel->setText(QApplication::translate(
		"greedyClass",
		"Press [Space] to start.",
		0));
}

void Snaqe::lose_game_receiver()
{
	messageLabel->setText(QApplication::translate(
		"greedyClass",
		"Press [F2] to try again.",
		0)); // Game over!
}

void Snaqe::pause_game_receiver()
{
	messageLabel->setText(QApplication::translate(
		"greedyClass",
		"Press [Space] to continue.",
		0));
}

void Snaqe::resume_game_receiver()
{
	messageLabel->setText(QApplication::translate(
		"greedyClass",
		"Press [Space] to pause.",
		0));
}

void Snaqe::win_game_receiver()
{
	messageLabel->setText(QApplication::translate(
		"greedyClass",
		"Press [F2] to play again.",
		0)); // Congratulations!
}

void Snaqe::mode_change_receiver(bool autoMode)
{
	if (autoMode)
	{
		timerInterval = INTERVAL_AUTO;
		autoLabel->setText(QApplication::translate(
			"greedyClass",
			"Press [F1] to enter manual mode.",
			0));
	}
	else
	{
		timerInterval = INTERVAL_MANUAL;
		autoLabel->setText(QApplication::translate(
			"greedyClass",
			"Press [F1] to enter auto mode.",
			0));
	}

	refreshTimer->setInterval(timerInterval);
}
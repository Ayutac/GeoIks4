#include "precompiledHeaders.h"
#include "TimerMethodWidget.h"
#include "ui_TimerMethodWidget.h"
#include <QTimer>

TimerMethodWidget::TimerMethodWidget(QWidget *parent, int index, QString methodName, int defaultInterval) :
	QWidget(parent),
	ui(new Ui::TimerMethodWidget)
{
	this->index = index;
	ui->setupUi(this);
	timer = new QTimer(this);
	timer->setInterval(defaultInterval);
	timer->setSingleShot(false);
	timer->stop();
	ui->pbMethodName->setText(methodName);
	ui->sbDelay->setValue(defaultInterval);
	QObject::connect(ui->pbPlay, SIGNAL(pressed()), timer, SLOT(start()));
	QObject::connect(ui->pbStop, SIGNAL(pressed()), timer, SLOT(stop()));
	QObject::connect(ui->sbDelay, SIGNAL(valueChanged(int)), this, SLOT(timerValueChanged(int)));
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timout()));
}

TimerMethodWidget::~TimerMethodWidget()
{
	delete ui;
}

void TimerMethodWidget::timout()
{
	emit methodClicked(index);
}

void TimerMethodWidget::on_pbMethodName_clicked()
{
	emit methodClicked(index);
}

void TimerMethodWidget::timerValueChanged(int value)
{
	timer->setInterval(value);
}


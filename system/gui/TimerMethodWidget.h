#ifndef TIMERMETHODWIDGET_H
#define TIMERMETHODWIDGET_H

#include <QWidget>

namespace Ui { class TimerMethodWidget; }

/// helper class for the timer method user interface
class TimerMethodWidget : public QWidget {
	Q_OBJECT
 private:
	Ui::TimerMethodWidget *ui;
	QTimer *timer;
	/// for communication with method browser - index of the method to be called
	int index;

 private slots:
	void timout();
	void on_pbMethodName_clicked();

 public slots:
	void timerValueChanged(int value);

 public:
	explicit TimerMethodWidget(QWidget *parent, int index, QString methodName, int defaultInterval = 50);
	~TimerMethodWidget();
 signals:
  void methodClicked(int index);

};

#endif // TIMERMETHODWIDGET_H

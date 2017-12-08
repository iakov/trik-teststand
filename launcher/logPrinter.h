#pragma once

#include <QtWidgets>
#include <QtCore/QStringList>
#include <QtCore/QEventLoop>

class LogPrinter : public QWidget
{
	Q_OBJECT

public:
	explicit LogPrinter(QStringList const &log = QStringList());
	void setLog(QStringList const &log);
	void exec();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QEventLoop mEventLoop;

	QVBoxLayout mLayout;
	QListWidget mLogArea;
	QLabel okLabel;
};

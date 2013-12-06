#pragma once

#include <QtGui/QWidget>
#include <QtCore/QStringList>
#include <QtCore/QEventLoop>
#include <QtGui/QListWidget>
#include <QtGui/QHBoxLayout>

class LogPrinter : public QWidget
{
	Q_OBJECT

public:
	explicit LogPrinter(QStringList const &log = QStringList());
	void setLog(QStringList const &log);
	void exec();

protected:
	void keyPressEvent(QKeyEvent *);

private:
	QEventLoop mEventLoop;

	QHBoxLayout mLayout;
	QListWidget mLogArea;
};

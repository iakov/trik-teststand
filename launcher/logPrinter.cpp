#include "logPrinter.h"

LogPrinter::LogPrinter(QStringList const &log)
{
	setWindowState(Qt::WindowFullScreen);

	mLayout.addWidget(&mLogArea);
	setLayout(&mLayout);

	mLogArea.addItems(log);
}

void LogPrinter::setLog(const QStringList &log)
{
	mLogArea.addItems(log);
}

void LogPrinter::exec()
{
	mEventLoop.exec();
}

void LogPrinter::keyPressEvent(QKeyEvent *)
{
	mEventLoop.quit();
}

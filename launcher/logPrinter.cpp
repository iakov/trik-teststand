#include "logPrinter.h"

LogPrinter::LogPrinter(QStringList const &log)
	: okLabel("OK")
{
	setWindowState(Qt::WindowFullScreen);

	okLabel.setAlignment(Qt::AlignRight | Qt::AlignBottom);

	mLayout.addWidget(&mLogArea);
	mLayout.addWidget(&okLabel);
	setLayout(&mLayout);

	mLogArea.addItems(log);
}

void LogPrinter::setLog(const QStringList &log)
{
	mLogArea.addItems(log);
}

void LogPrinter::exec()
{
	show();
	mEventLoop.exec();
	hide();
}

void LogPrinter::keyPressEvent(QKeyEvent *)
{
	mEventLoop.quit();
}

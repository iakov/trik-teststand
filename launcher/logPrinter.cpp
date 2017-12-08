#include "logPrinter.h"

#include <QtCore/QtDebug>

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

void LogPrinter::keyPressEvent(QKeyEvent *event)
{
	qDebug() << Q_FUNC_INFO << event->key();
	mEventLoop.quit();
}

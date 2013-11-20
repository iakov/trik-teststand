#pragma once

#include <QtGui/QApplication>
#include <QtCore/QMap>

class TrikTestApplication : public QApplication
{
	Q_OBJECT

public:
	TrikTestApplication(int argc, char **argv);
	bool notify(QObject *receiver, QEvent *event);

private:
	QMap<int, int> keysConversions;
};

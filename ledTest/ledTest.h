#pragma once

#include "testInterface.h"

#include <QtCore/QObject>
#include <trikControl/brick.h>

class LedTest
		: public QObject
		, public TestInterface
{
	Q_OBJECT
	Q_INTERFACES(TestInterface)

public:
	TestInterface::Result run(trikControl::Brick &brick, QStringList &log);
};

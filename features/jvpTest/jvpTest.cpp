/* Copyright 2014 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "jvpTest.h"
#include <QtCore/QDebug>
#include <QtCore/QThread>

TestInterface::Result jvpTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	Q_UNUSED(brick);
	mLog = &log;
	if (captureVideo(brick, 0) == TestInterface::fail) {
		return TestInterface::fail;
	}

	return TestInterface::success;
}

TestInterface::Result jvpTest::captureVideo(trikControl::BrickInterface &brick, int number)
{
	QString program = "/etc/trik/init-ov7670-320x240.sh";
	QStringList arguments{"0"};
	QProcess *myProcess = new QProcess(this);
	myProcess->start(program, arguments);
	myProcess->waitForFinished(5000);
	QByteArray localReadAllStandardOutput = myProcess->readAll();
	myProcess->close();
	delete myProcess;

	if (localReadAllStandardOutput != "ov7670 successfully initialised\n") {
		mLog->append(tr("Проверка порта JVP%0 завершилась неудачей").arg(number));
		return TestInterface::fail;
	}

	mLog->append(tr("Проверка порта JVP%0 завершилась успешно").arg(number));
	return TestInterface::success;
}

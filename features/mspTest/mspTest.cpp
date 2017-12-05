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

#include "mspTest.h"

#include <QtCore/QtDebug>
#include <QtCore/QThread>

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <trikControl/motorInterface.h>
#include <trikControl/encoderInterface.h>

#include "i2cCommunicator.h"
#include "usbCommunicator.h"

static int const delay = 500;
static int const maxDifference = 100;
static int const power = 100;


TestInterface::Result MspTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	mBrick = &brick;
	mLog = &log;

	log.append(tr("Запуск теста моторов"));

	if (testCase("M1", "E1") == TestInterface::fail) {
		return TestInterface::fail;
	}

	if (testCase("M2", "E2") == TestInterface::fail) {
		return TestInterface::fail;
	}

	if (testCase("M3", "E3") == TestInterface::fail) {
		return TestInterface::fail;
	}

	if (testCase("M4", "E4") == TestInterface::fail) {
		return TestInterface::fail;
	}

	return TestInterface::success;
}

TestInterface::Result MspTest::testCase(QString const &motorPort, QString const &encoderPort)
{
	qDebug() << Q_FUNC_INFO << motorPort << encoderPort;
	auto motor = mBrick->motor(motorPort);
	qDebug() << Q_FUNC_INFO << motor;
	if (!motor) {
		mLog->append(tr("Не удалось получить доступ к мотору на порту ") + motorPort);
		return TestInterface::fail;
	}

	auto encoder = mBrick->encoder(encoderPort);
	qDebug() << Q_FUNC_INFO << encoder;
	if (!encoder) {
		mLog->append(tr("Не удалось получить доступ к датчику угла поворота на порту ") + encoderPort);
		return TestInterface::fail;
	}

	qDebug() << Q_FUNC_INFO << "encoder and motor -exists";

	encoder->reset();
	mLog->append(tr("Подана мощность +%1% на мотор ").arg(power) + motorPort);
	motor->setPower(power);
	QThread::msleep(delay);
	motor->powerOff();
	float const angle1 = encoder->read();
	mLog->append(tr("Считан угол ") + QString::number(angle1) + tr(" рад с ") + encoderPort);

	if (fabs(angle1) > maxDifference) {
		return TestInterface::success;
	}

	return TestInterface::fail;
}


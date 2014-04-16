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

#include <math.h>
#include <stdlib.h>

#include <trikControl/motor.h>
#include <trikControl/encoder.h>

#define FIRMWARE_PATH "/home/root/mspdev.txt"

static int const delay = 500;
static int const maxDifference = 100;

TestInterface::Result MspTest::run(trikControl::Brick &brick, QStringList &log)
{
	mBrick = &brick;
	mLog = &log;

	system("mspflasher -o " FIRMWARE_PATH);
	mResult = TestInterface::success;
	testCase("JB1", "JM1");
	testCase("JB2", "JM2");
	testCase("JB3", "JM3");
	testCase("JB4", "JM4");
	return mResult;
}

void MspTest::testCase(QString const &motorPort, QString const &encoderPort)
{
	trikControl::Motor *motor = mBrick->motor(motorPort);

	if (motor == NULL) {
		mLog->append(tr("Не удалось получить доступ к мотору на порту ") + motorPort);
		return;
	}

	trikControl::Encoder *encoder = mBrick->encoder(encoderPort);

	if (encoder == NULL) {
		mLog->append(tr("Не удалось получить доступ к датчику угла поворота на порту ") + encoderPort);
		return;
	}

	encoder->reset();
	motor->setPower(100);
	mLog->append(tr("Подана мощность +100% на мотор ") + motorPort);
	mBrick->wait(delay);
	motor->powerOff();
	float const angle1 = encoder->read();
	mLog->append(tr("Считан угол ") + QString::number(angle1) + tr(" рад с ") + encoderPort);

	encoder->reset();
	motor->setPower(-100);
	mLog->append(tr("Подана мощность -100% на мотор ") + motorPort);
	mBrick->wait(delay);
	motor->powerOff();
	float const angle2 = encoder->read();
	mLog->append(tr("Считан угол ") + QString::number(angle2) + tr(" рад с ") + encoderPort);

	if (((angle1 < 0 && angle2 > 0) || (angle1 > 0 && angle2 < 0)) && fabs(angle1 + angle2) < maxDifference) {
	} else {
		mResult = TestInterface::fail;
	}
}

Q_EXPORT_PLUGIN2(trikTest, MspTest)

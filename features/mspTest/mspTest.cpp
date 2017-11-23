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

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <trikControl/motorInterface.h>
#include <trikControl/encoderInterface.h>

#include "i2cCommunicator.h"
#include "usbCommunicator.h"

//#define FIRMWARE_PATH " /etc/msp-v6-latest.txt"

static int const delay = 500;
static int const maxDifference = 100;
static int const power = 100;
#define wait msleep


TestInterface::Result MspTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	mBrick = &brick;
	mLog = &log;

	log.append(tr("Запуск теста моторов"));

//	if (loadFirmware() == TestInterface::fail) {
//		return TestInterface::fail;
//	}

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

TestInterface::Result MspTest::loadFirmware()
{
	return TestInterface::success;

//	system("mspflasher -o " FIRMWARE_PATH);
//	mLog->append(tr("Произведена прошивка MSP430"));

//	UsbCommunicator usbCommunicator;
//	usbCommunicator.scan();
//	QList<UsbCommunicator::Device> usbDevices = usbCommunicator.devices();

//	UsbCommunicator::Device mspUsbDevice;
//	mspUsbDevice.vendor = 0x2047;
//	mspUsbDevice.product = 0x0200;

//	if (usbDevices.contains(mspUsbDevice)) {
//		mLog->append(tr("MSP430 определяется как USB-устройство - прошивка прошла неверно"));
//		return TestInterface::fail;
//	}

//	QMap<int, QString> busFiles;
//	busFiles[2] = "/dev/i2c-2";
//	I2cCommunicator i2cCommunicator(busFiles);

//	if (!i2cCommunicator.isOnboard(2, 0x48)) {
//		mLog->append(tr("MSP430 не определяется как I2C-устройство - ошибка"));
//		return TestInterface::fail;
//	}
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
	wait(delay);
	motor->powerOff();
	float const angle1 = encoder->read();
	mLog->append(tr("Считан угол ") + QString::number(angle1) + tr(" рад с ") + encoderPort);

	encoder->reset();
	mLog->append(tr("Подана мощность -%1% на мотор ").arg(power) + motorPort);
	motor->setPower(-power);
	wait(delay);
	motor->powerOff();
	float const angle2 = encoder->read();
	mLog->append(tr("Считан угол ") + QString::number(angle2) + tr(" рад с ") + encoderPort);
	mLog->append(tr("fabs(angle1 + angle2) = ") + QString::number(fabs(angle1 + angle2) ));

	if (((angle1 < 0 && angle2 > 0) || (angle1 > 0 && angle2 < 0))
			&& fabs(fabs(angle1) - fabs(angle2)) < maxDifference)
	{
		return TestInterface::success;
	}

	return TestInterface::fail;
}


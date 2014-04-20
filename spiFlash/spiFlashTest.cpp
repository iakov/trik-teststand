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

#include "spiFlashTest.h"

#include <QtCore/QList>
#include <QtCore/QMap>

#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <trikControl/motor.h>
#include <trikControl/encoder.h>

#include "i2cCommunicator.h"
#include "usbCommunicator.h"

#define FIRMWARE_PATH " /etc/msp-v6-latest.txt"

TestInterface::Result SpiFlashTest::run(trikControl::Brick &brick, QStringList &log)
{
	mLog = &log;
	if (flash() == TestInterface::fail) {
		return TestInterface::fail;
	}

	return TestInterface::success;
}
TestInterface::Result SpiFlashTest::flash()
{
	int error = system("./write_loader.sh");
	if(error){
		mLog->append(tr("Прошивка завершилась ошибкой"));
		return TestInterface::fail;
	}
	mLog->append(tr("Произведена прошивка загрузчика"));

	return TestInterface::success;
}

Q_EXPORT_PLUGIN2(trikTest, SpiFlashTest);


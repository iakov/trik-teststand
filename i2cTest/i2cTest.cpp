/* Copyright 2013 Roman Kurbatov
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

#include "i2cTest.h"

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>

#include "configurer.h"
#include "i2cCommunicator.h"

TestInterface::Result I2cTest::run(trikControl::BrickInterface &, QStringList &log)
{
	Configurer configurer;
	I2cCommunicator i2cCommunicator(configurer.busFiles());

	QList<QString> devices = configurer.devices();
	QMap<QString, Configurer::Location> locations = configurer.locations();

	TestInterface::Result result = TestInterface::success;
	foreach (QString const &device, devices)
	{
		if (i2cCommunicator.isOnboard(locations[device].bus, locations[device].address))
		{
			log.append(tr("Найдено устройство: ") + device);
		}
		else
		{
			result = TestInterface::fail;
			log.append(tr("Не найдено устройство: ") + device);
		}
	}

	return result;
}

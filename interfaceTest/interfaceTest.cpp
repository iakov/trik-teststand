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

#include "interfaceTest.h"

#include <QtNetwork/QNetworkInterface>
#include <QtCore/QList>
#include <QtCore/QRegExp>

TestInterface::Result InterfaceTest::run(trikControl::Brick &, QStringList &log)
{
	QRegExp regExp("wlan[0-9]");

	TestInterface::Result result = TestInterface::fail;

	foreach (QNetworkInterface const &interface, QNetworkInterface::allInterfaces())
	{
		if (regExp.exactMatch(interface.name()))
		{
			result = success;
			log.append(tr("Найден интерфейс ") + interface.name());
		}
	}

	if (result == fail)
	{
		log.append(tr("Не найдено ни одного интерфейса"));
	}

	return result;
}

Q_EXPORT_PLUGIN2(trikTest, InterfaceTest)

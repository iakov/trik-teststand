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

#include "connectTest.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QTimer>

#include "configurer.h"

TestInterface::Result ConnectTest::run(trikControl::Brick &brick, QStringList &log)
{
	mWiFi = new trikWiFi::TrikWiFi("/tmp/trikwifi", "/run/wpa_supplicant/wlan0", this);
	mLog = &log;

	QList<trikWiFi::NetworkConfiguration> networks = mWiFi->listNetworks();

	Configurer configurer;
	QString ssid = configurer.ssid();
	QString key = configurer.key();

	mId = -1;
	foreach (trikWiFi::NetworkConfiguration const &network, networks)
	{
		if (network.ssid == ssid)
		{
			mId = network.id;
			break;
		}
	}

	if (mId == -1)
	{
		mId = mWiFi->addNetwork();
		mWiFi->setSsid(mId, ssid);
	}

	if (key.isEmpty())
	{
		mWiFi->setNoKeyNeeded(mId);
	}
	else
	{
		mWiFi->setKey(mId, key);
	}

	QTimer::singleShot(0, this, SLOT(connect()));
	QObject::connect(mWiFi, SIGNAL(connected()), SLOT(connected()));
	int const timeout = 30;
	QTimer::singleShot(timeout * 1000, this, SLOT(timedOut()));

	if (mEventLoop.exec() == 0)
		return TestInterface::success;
	else
		return TestInterface::fail;
}

void ConnectTest::connect()
{
	mWiFi->connect(mId);
}

void ConnectTest::connected()
{
	mLog->append(tr("Соединение прошло успешно"));
	mEventLoop.exit(0);
}

void ConnectTest::timedOut()
{
	mLog->append(tr("Не удалось установить соединение"));
	mEventLoop.exit(-1);
}

Q_EXPORT_PLUGIN2(trikTest, ConnectTest)

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

#include "usbConnectTest.h"

#include <QtCore/QList>

#include "messageBox.h"
#include "usbCommunicator.h"

TestInterface::Result UsbConnectTest::run(trikControl::BrickInterface &, QStringList &)
{
	UsbCommunicator usbCommunicator;

	usbCommunicator.scan();
	int initialCount = usbCommunicator.devices().count();

	MessageBox messageBox(tr("Подключите любое устройство к порту USB"));
	messageBox.exec();

	usbCommunicator.scan();
	int countAfterConnecting = usbCommunicator.devices().count();

	return (countAfterConnecting > initialCount) ? TestInterface::success : TestInterface::fail;
}

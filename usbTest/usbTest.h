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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <libusb-1.0/libusb.h>

#include "testInterface.h"
#include "usbCommunicator.h"

class UsbTest: public QObject, public TestInterface
{
	Q_OBJECT
	Q_INTERFACES(TestInterface)

public:
	TestInterface::Result run(trikControl::Brick &, QStringList &log);

private:
	QStringList *mLog;
	QMap<uint16_t, QString> mVendorNames;
	QMap<uint16_t, QString> mProductNames;
	QList<UsbCommunicator::Device> mAllDevices;

	void logDevice(UsbCommunicator::Device const &device, bool found);
};

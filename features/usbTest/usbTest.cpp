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

#include "usbTest.h"

#include "usbCommunicator.h"
#include "configurer.h"

TestInterface::Result UsbTest::run(trikControl::BrickInterface &, QStringList &log)
{
	mLog = &log;

	Configurer configurer;
	mVendorNames = configurer.vendorNames();
	mProductNames = configurer.productNames();
	mAllDevices = configurer.devices();

	UsbCommunicator usbCommunicator;
	usbCommunicator.scan();
	QList<UsbCommunicator::Device> foundDevices = usbCommunicator.devices();

	foreach (UsbCommunicator::Device const &device, foundDevices)
	{
		logDevice(device, true);
	}

	TestInterface::Result result = TestInterface::success;

	foreach (UsbCommunicator::Device const &device, mAllDevices)
	{
		if (!foundDevices.contains(device))
		{
			result = TestInterface::fail;
			logDevice(device, false);
		}
	}

	return result;
}

void UsbTest::logDevice(const UsbCommunicator::Device &device, bool found)
{
	if (found)
		mLog->append(tr("Найдено устройство:"));
	else
		mLog->append(tr("Не найдено устройство:"));

	mLog->append("Vendor ID: " + QString::number(device.vendor, 16).rightJustified(4, '0'));
	mLog->append("Product ID: " + QString::number(device.product, 16).rightJustified(4, '0'));

	if (mVendorNames.contains(device.vendor))
		mLog->append(mVendorNames[device.vendor]);

	if (mProductNames.contains(device.product))
		mLog->append(mProductNames[device.product]);

	mLog->append(QString());
}

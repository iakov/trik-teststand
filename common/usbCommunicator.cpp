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

#include "usbCommunicator.h"

UsbCommunicator::UsbCommunicator()
{
}

void UsbCommunicator::scan()
{
	mDevices.clear();
	libusb_context *context;
	libusb_init(&context);
	libusb_device **list;
	ssize_t deviceCount = libusb_get_device_list(context, &list);
	for (ssize_t i = 0; i < deviceCount; ++i)
	{
		libusb_device *device = list[i];
		libusb_device_descriptor descriptor;
		libusb_get_device_descriptor(device, &descriptor);
		Device deviceInfo;
		deviceInfo.vendor = descriptor.idVendor;
		deviceInfo.product = descriptor.idProduct;
		mDevices.append(deviceInfo);
	}
	libusb_exit(context);
}

QList<UsbCommunicator::Device> UsbCommunicator::devices()
{
	return mDevices;
}

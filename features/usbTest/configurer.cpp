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

#include "configurer.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>

Configurer::Configurer()
		: config("usbTest")
{
	QFile file("usbTest.xml");
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Failed to open usbTest.xml for reading";
		throw "Failed to open usbTest.xml for reading";
	}
	if (!config.setContent(&file))
	{
		file.close();
		qDebug() << "usbTest.xml parsing failed";
		throw "usbTest.xml parsing failed";
	}

	file.close();

	initVendorNames();
	initProductNames();
	initDevices();
}

QMap<uint16_t, QString> Configurer::vendorNames()
{
	return mVendorNames;
}

QMap<uint16_t, QString> Configurer::productNames()
{
	return mProductNames;
}

QList<UsbCommunicator::Device> Configurer::devices()
{
	return mDevices;
}

void Configurer::initVendorNames()
{
	QDomElement const root = config.documentElement();

	QDomElement const stages = root.elementsByTagName("vendors").at(0).toElement();
	for (QDomNode child = stages.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "vendor") {
			qDebug() << "Malformed <vendors> tag";
			throw "usbTest.xml parsing failed";
		}

		uint16_t id = childElement.attribute("id").toUShort(NULL, 16);
		QString name = childElement.attribute("name");
		mVendorNames[id] = name;
	}
}

void Configurer::initProductNames()
{
	QDomElement const root = config.documentElement();

	QDomElement const stages = root.elementsByTagName("products").at(0).toElement();
	for (QDomNode child = stages.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "product") {
			qDebug() << "Malformed <products> tag";
			throw "usbTest.xml parsing failed";
		}

		uint16_t id = childElement.attribute("id").toUShort(NULL, 16);
		QString name = childElement.attribute("name");
		mProductNames[id] = name;
	}
}

void Configurer::initDevices()
{
	QDomElement const root = config.documentElement();

	QDomElement const stages = root.elementsByTagName("devices").at(0).toElement();
	for (QDomNode child = stages.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "device") {
			qDebug() << "Malformed <devices> tag";
			throw "usbTest.xml parsing failed";
		}

		UsbCommunicator::Device device;
		device.vendor = childElement.attribute("vendor").toUShort(NULL, 16);
		device.product = childElement.attribute("product").toUShort(NULL, 16);
		mDevices.append(device);
	}
}

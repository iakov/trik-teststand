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
	: config("i2cTest")
{
	QFile file("i2cTest.xml");
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Failed to open i2cTest.xml for reading";
		throw "Failed to open i2cTest.xml for reading";
	}
	if (!config.setContent(&file))
	{
		file.close();
		qDebug() << "i2cTest.xml parsing failed";
		throw "i2cTest.xml parsing failed";
	}

	file.close();

	initBusFiles();
	initDevices();
}

QMap<int, QString> Configurer::busFiles()
{
	return mBusFiles;
}

QList<QString> Configurer::devices()
{
	return mDevices;
}

QMap<QString, Configurer::Location> Configurer::locations()
{
	return mLocations;
}

void Configurer::initBusFiles()
{
	QDomElement const root = config.documentElement();

	QDomElement const busses = root.elementsByTagName("busses").at(0).toElement();

	for (QDomNode child = busses.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "bus") {
			qDebug() << "Malformed <busses> tag";
			throw "i2cTest.xml parsing failed";
		}

		int id = childElement.attribute("id").toInt();
		QString file = childElement.attribute("file");
		mBusFiles[id] = file;
	}
}

void Configurer::initDevices()
{
	QDomElement const root = config.documentElement();

	QDomElement const devices = root.elementsByTagName("devices").at(0).toElement();

	for (QDomNode child = devices.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "device") {
			qDebug() << "Malformed <devices> tag";
			throw "i2cTest.xml parsing failed";
		}

		QString name = childElement.attribute("name");
		Location location;
		location.bus = childElement.attribute("bus").toInt();
		location.address = childElement.attribute("address").toInt(NULL, 16);
		mDevices.append(name);
		mLocations[name] = location;
	}
}

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
#include <QtXml/QDomDocument>
#include <QtCore/QDebug>

Configurer::Configurer()
{
	QDomDocument config("connectTest");

	QFile file("connectTest.xml");
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Failed to open connectTest.xml for reading";
		throw "Failed to open connectTest.xml for reading";
	}
	if (!config.setContent(&file))
	{
		file.close();
		qDebug() << "connectTest.xml parsing failed";
		throw "connectTest.xml parsing failed";
	}

	file.close();

	QDomElement const root = config.documentElement();
	for (QDomNode child = root.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (!child.isElement())
			continue;

		QDomElement const childElement = child.toElement();

		QString tagName = childElement.tagName();
		if (tagName == "ssid")
		{
			mSsid = childElement.text();
		}
		else if (tagName == "key")
		{
			mKey = childElement.text();
		}
		else
		{
			qDebug() << "Unknown element in connectTest.xml";
			throw "connectTest.xml parsing failed";
		}
	}
}

QString Configurer::ssid()
{
	return mSsid;
}

QString Configurer::key()
{
	return mKey;
}

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
	QDomDocument config("trikTest");

	QFile file("trikTest.xml");
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Failed to open trikTest.xml for reading";
		throw "Failed to open trikTest.xml for reading";
	}
	if (!config.setContent(&file))
	{
		file.close();
		qDebug() << "trikTest.xml parsing failed";
		throw "trikTest.xml parsing failed";
	}

	file.close();

	QDomElement const root = config.documentElement();

	if (root.elementsByTagName("tests").isEmpty())
	{
		qDebug() << "trikTest.xml does not have <tests> tag";
		throw "trikTest.xml parsing failed";
	}

	QDomElement const tests = root.elementsByTagName("tests").at(0).toElement();
	for (QDomNode child = tests.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "test") {
			qDebug() << "Malformed <tests> tag";
			throw "trikTest.xml parsing failed";
		}

		QString name = childElement.attribute("name");
		QString file = childElement.attribute("file");
		mFiles[name] = file;
	}
}

QMap<QString, QString> Configurer::files()
{
	return mFiles;
}

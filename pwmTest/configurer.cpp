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

#include <QtCore/QDebug>
#include <QtCore/QFile>

Configurer::Configurer()
		: config("pwmTest")
{
	QFile file("pwmTest.xml");
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Failed to open pwmTest.xml for reading";
		throw "Failed to open pwmTest.xml for reading";
	}
	if (!config.setContent(&file))
	{
		file.close();
		qDebug() << "pwmTest.xml parsing failed";
		throw "pwmTest.xml parsing failed";
	}

	file.close();

	initStages();
	initValues();
}

QList<Configurer::Stage> Configurer::stages()
{
	return mStages;
}

QList<Configurer::Value> Configurer::values()
{
	return mValues;
}

void Configurer::initStages()
{
	QDomElement const root = config.documentElement();

	QDomElement const stages = root.elementsByTagName("stages").at(0).toElement();
	for (QDomNode child = stages.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "stage") {
			qDebug() << "Malformed <stages> tag";
			throw "pwmTest.xml parsing failed";
		}

		Stage stage;
		stage.generatorPort = childElement.attribute("generatorPort");
		stage.capturePort = childElement.attribute("capturePort");
		mStages.append(stage);
	}
}

void Configurer::initValues()
{
	QDomElement const root = config.documentElement();

	QDomElement const stages = root.elementsByTagName("values").at(0).toElement();
	for (QDomNode child = stages.firstChild()
			; !child.isNull()
			; child = child.nextSibling())
	{
		if (!child.isElement()) {
			continue;
		}

		QDomElement const childElement = child.toElement();
		if (childElement.nodeName() != "value") {
			qDebug() << "Malformed <values> tag";
			throw "pwmTest.xml parsing failed";
		}

		Value value;
		value.frequency = childElement.attribute("frequency").toInt();
		value.duty = childElement.attribute("duty").toInt();
		mValues.append(value);
	}
}

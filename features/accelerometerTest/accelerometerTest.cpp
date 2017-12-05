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

#include "accelerometerTest.h"

#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QtDebug>
#include <QtCore/QThread>

#include "messageBox.h"
#include "yesNoBox.h"

const int READINGS = 50;
const int UNIQUE_READINGS = 5;

TestInterface::Result AccelerometerTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	trikControl::VectorSensorInterface *localAccelerometer = brick.accelerometer();

	QSet<int> first;
	QSet<int> second;
	QSet<int> third;

	for (int i = 0; i < READINGS; ++i) {
		QVector<int> localRead = localAccelerometer->read();

		first.insert(localRead[0]);
		second.insert(localRead[1]);
		third.insert(localRead[2]);
		qDebug() << Q_FUNC_INFO << localRead;
		QThread::msleep(100);
	}

	if (first.size() > UNIQUE_READINGS
			&& second.size() > UNIQUE_READINGS
			&& third.size() > UNIQUE_READINGS) {
		return success;
	}

	log.append(tr("После %0 считываний, акселерометр выдал слишком мало уникальных значений: %1, %2, %3")
			.arg(READINGS)
			.arg(first.size())
			.arg(second.size())
			.arg(third.size()));

	return fail;
}

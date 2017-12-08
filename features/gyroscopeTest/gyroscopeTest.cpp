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

#include "gyroscopeTest.h"

#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QtDebug>

const int READINGS = 50;
const int GYROSCOPE_VECTOR_SIZE = 3;
const int UNIQUE_READINGS = 5;


TestInterface::Result GyroscopeTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	mReadNumber = 0;
	mReadings.resize(GYROSCOPE_VECTOR_SIZE);
	mGyroscope = brick.gyroscope();

	mTimer.setSingleShot(true);
	connect(&mTimer, &QTimer::timeout, this, &GyroscopeTest::handleTimeOut);
	mTimer.start(2500);

	connect(mGyroscope, &trikControl::GyroSensorInterface::newData, this, &GyroscopeTest::handleNewData);

	mLoop.exec();

	if (std::all_of(mReadings.begin(), mReadings.end()
			, [](const QSet<int> &set){return set.size() > UNIQUE_READINGS;})) {
		return success;
	}

	log.append(tr("После %0 считываний, гироскоп выдал слишком мало уникальных значений")
			.arg(READINGS));

	return fail;
}

void GyroscopeTest::handleNewData(QVector<int> data)
{
	++mReadNumber;
	if (mReadNumber > READINGS) {
		disconnect(&mTimer, &QTimer::timeout, this, &GyroscopeTest::handleTimeOut);
		disconnect(mGyroscope, &trikControl::GyroSensorInterface::newData, this, &GyroscopeTest::handleNewData);
		mLoop.quit();
	}

	qDebug() << Q_FUNC_INFO << data;

	for (int j = 0; j < GYROSCOPE_VECTOR_SIZE; ++j) {
		mReadings[j].insert(data[j]);
	}
}

void GyroscopeTest::handleTimeOut()
{
	qDebug() << Q_FUNC_INFO << "exit from Timer";
	disconnect(mGyroscope, &trikControl::GyroSensorInterface::newData, this, &GyroscopeTest::handleNewData);
	mLoop.quit();
}

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

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

#include "testInterface.h"
#include "messageBox.h"

class GyroscopeTest : public QWidget, public TestInterface
{
	Q_OBJECT
	Q_INTERFACES(TestInterface)
	Q_PLUGIN_METADATA(IID "com.trikset.teststand.gyroscope")

public:
	TestInterface::Result run(trikControl::BrickInterface &brick, QStringList &log);

private:
	trikControl::GyroSensorInterface *mGyroscope;
	QStringList *mLog;

	QVBoxLayout mLayout;
	QLabel mTitleLabel;
	QListWidget mReadingsList;

	QEventLoop mEventLoop;
	QTimer mDurationTimer;
	QTimer mTickTimer;

private slots:
	void printReading();
};

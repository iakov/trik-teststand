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
#include <QtCore/QVector>

#include "messageBox.h"
#include "yesNoBox.h"

TestInterface::Result GyroscopeTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	mGyroscope = brick.gyroscope();
	mLog = &log;

	setWindowState(Qt::WindowFullScreen);

	mLayout.addWidget(&mTitleLabel);
	mLayout.addWidget(&mReadingsList);
	setLayout(&mLayout);

	mTitleLabel.setText(tr("Показания гироскопа"));
	mTitleLabel.setAlignment(Qt::AlignTop | Qt::AlignLeft);

	int const duration = 5;

	MessageBox messageBox(tr("Буду считываться показания гироскопа\n"
		"в течение ") + QString::number(duration) + tr(" секунд"));
	messageBox.exec();

	mDurationTimer.setInterval(duration * 1000);
	mDurationTimer.setSingleShot(true);
	connect(&mDurationTimer, SIGNAL(timeout()), &mEventLoop, SLOT(quit()));
	mDurationTimer.start();

	mTickTimer.setInterval(100);
	mTickTimer.setSingleShot(false);
	connect(&mTickTimer, SIGNAL(timeout()), SLOT(printReading()));
	mTickTimer.start();

	mEventLoop.exec();

	YesNoBox yesNoBox(tr("Исправен ли гироскоп?"));
	if (yesNoBox.exec() == YesNoBox::yes)
		return success;
	else
		return fail;
}

void GyroscopeTest::printReading()
{
	QVector<int> const reading = mGyroscope->read();
	QString const readingString = "(" + QString::number(reading[0])
			+ ", " + QString::number(reading[1])
			+ ", " + QString::number(reading[2]) + ")";
	mReadingsList.addItem(readingString);
	mReadingsList.setCurrentRow(mReadingsList.count() - 1);
	mLog->append(readingString);
}

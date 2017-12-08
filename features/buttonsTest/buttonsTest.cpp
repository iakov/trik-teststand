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

#include "buttonsTest.h"

#include <QtCore/QtDebug>

//using namespace trikTest;

void ButtonsTest::init()
{
	mDelay = 25;
	mAllButtons << Qt::Key_Return << Qt::Key_Left << Qt::Key_Right << Qt::Key_Up <<
			Qt::Key_Down << Qt::Key_PowerOff << Qt::Key_Escape;

	mButtonsNames[Qt::Key_PowerOff] = "PowerDown";
	mButtonsNames[Qt::Key_Escape] = "Menu";
	mButtonsNames[Qt::Key_Return] = "Enter";
	mButtonsNames[Qt::Key_Left] = "Left";
	mButtonsNames[Qt::Key_Right] = "Right";
	mButtonsNames[Qt::Key_Up] = "Up";
	mButtonsNames[Qt::Key_Down] = "Down";
}

TestInterface::Result ButtonsTest::run(trikControl::BrickInterface &, QStringList &log)
{
	init();
	mLog = &log;
	const auto message = tr("Тестирование кнопок.\n Нажмите по очереди все кнопки\nв течение %0 секунд")
			.arg(mDelay);

	mTopLabel.setText(message);
	mTopLabel.setAlignment(Qt::AlignCenter);
	mBottomLabel.setAlignment(Qt::AlignCenter);
	mLayout.addWidget(&mTopLabel);
	mLayout.addWidget(&mBottomLabel);
	setLayout(&mLayout);

	show();
	mTimer.setInterval(mDelay * 1000);
	mTimer.setSingleShot(true);
	connect(&mTimer, &QTimer::timeout, this, &ButtonsTest::timeout);

	mResult = fail;
	mTimer.start();
	qDebug() << Q_FUNC_INFO << __LINE__;
	mEventLoop.exec();
	deleteLater();
	return mResult;
}

void ButtonsTest::keyPressEvent(QKeyEvent *event)
{
	mPressedButtons.insert(event->key());
	const auto message = tr("Нажата кнопка ") + mButtonsNames.value(event->key());
	qDebug() << Q_FUNC_INFO << QKeySequence(event->key()).toString() << message;
	mBottomLabel.setText(message);

	if (mPressedButtons.contains(mAllButtons)) {
		disconnect(&mTimer, SIGNAL(timeout()));
		mTimer.stop();
		mResult = success;
		mEventLoop.quit();
	}
}

void ButtonsTest::timeout()
{
	const auto message = tr("Не были нажаты кнопки:");
	mLog->append(message);
	qDebug() << Q_FUNC_INFO << message;
	for (int button : mAllButtons)
	{
		if (!mPressedButtons.contains(button)) {
			mLog->append(mButtonsNames[button]);
			qDebug() << mButtonsNames[button];
		}
	}

	mResult = fail;
	mEventLoop.quit();
}

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

//using namespace trikTest;

TestInterface::Result ButtonsTest::run(trikControl::Brick &, QStringList &log)
{
	mLog = &log;
	mDelay = 10;

	setWindowState(Qt::WindowFullScreen);

	mAllButtons << Qt::Key_PowerDown /* << Qt::Key_Meta */ << Qt::Key_Return << Qt::Key_Left <<
			Qt::Key_Right << Qt::Key_Up << Qt::Key_Down;

	mButtonsNames[Qt::Key_PowerDown] = "PowerDown (F1)";
	mButtonsNames[Qt::Key_Meta] = "Menu (F2)";
	mButtonsNames[Qt::Key_Return] = "Enter (F6)";
	mButtonsNames[Qt::Key_Left] = "Left (F3)";
	mButtonsNames[Qt::Key_Right] = "Right (F7)";
	mButtonsNames[Qt::Key_Up] = "Up (F5)";
	mButtonsNames[Qt::Key_Down] = "Down (F4)";

	mTopLabel.setText(tr("Тестирование кнопок\n"
			"Нажмите по очереди все кнопки\n"
			"в течение ") + QString::number(mDelay) + tr(" секунд"));
	mTopLabel.setAlignment(Qt::AlignCenter);
	mBottomLabel.setAlignment(Qt::AlignCenter);
	mLayout.addWidget(&mTopLabel);
	mLayout.addWidget(&mBottomLabel);
	setLayout(&mLayout);

	mTimer.setInterval(mDelay * 1000);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));

	mResult = fail;
	mTimer.start();
	mEventLoop.exec();
	return mResult;
}

void ButtonsTest::keyPressEvent(QKeyEvent *event)
{
	mPressedButtons.insert(event->key());
	mBottomLabel.setText(tr("Нажата кнопка ") + mButtonsNames.value(event->key()));
	if (mPressedButtons.contains(mAllButtons))
	{
		mTimer.stop();
		disconnect(&mTimer, SIGNAL(timeout()));
		mResult = success;
		mEventLoop.quit();
	}
}

void ButtonsTest::timeout()
{
	mLog->append(tr("Не были нажаты кнопки:"));
	foreach (const int button, mAllButtons)
	{
		if (!mPressedButtons.contains(button))
			mLog->append(mButtonsNames[button]);
	}
	mResult = fail;
	mEventLoop.quit();
}

Q_EXPORT_PLUGIN2(trikTest, ButtonsTest)

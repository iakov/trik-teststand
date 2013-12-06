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

#include "pwmTest.h"

#include <trikControl/servoMotor.h>
#include <trikControl/pwmCapture.h>

#include "messageBox.h"

TestInterface::Result PwmTest::run(trikControl::Brick &brick, QStringList &log)
{
	mBrick = &brick;
	mLog = &log;

	Configurer configurer;
	mStages = configurer.stages();
	mPowerValues = configurer.powerValues();

	result = TestInterface::success;

	foreach (Configurer::Stage const &stage, mStages)
	{
		performStage(stage);
	}

	return result;
}

void PwmTest::performStage(Configurer::Stage const &stage)
{
	trikControl::ServoMotor *generator = mBrick->servoMotor(stage.generatorPort);
	if (generator == NULL)
	{
		mLog->append(tr("Невозможно получить доступ к JE") + stage.generatorPort);
		mLog->append(QString());
		qDebug() << "Cannot access to JE" + stage.generatorPort;
		result = fail;
		return;
	}

	trikControl::PwmCapture *capture = mBrick->pwmCapture(stage.capturePort);
	if (capture == NULL)
	{
		mLog->append(tr("Невозможно получить доступ к JC") + stage.capturePort);
		mLog->append(QString());
		qDebug() << "Cannot access to JC" + stage.capturePort;
		result = fail;
		return;
	}

	MessageBox messageBox(tr("Соедините JE") + stage.generatorPort + tr(" и JC") + stage.capturePort);
	messageBox.exec();

	foreach (int const power, mPowerValues)
	{
			generator->setPower(power);
			mLog->append(tr("На выходе:"));
			mLog->append(tr("частота ") + QString::number(generator->frequency()) + tr("Гц"));
			mLog->append(tr("ширина импульса ") + QString::number(generator->duty()) + tr("%"));

			QVector<int> inputFrequency = capture->frequency();
			int inputDuty = capture->duty();
			if (inputFrequency[0] != generator->frequency() || inputFrequency[0] != inputFrequency[1]
							|| inputFrequency[1] != inputFrequency[2] || inputDuty != generator->duty()) {
					result = fail;
			}

			mLog->append(tr("На входе:"));
			mLog->append(tr("частота (") + QString::number(inputFrequency[0]) + "," + QString::number(inputFrequency[1]) + "," +
							QString::number(inputFrequency[2]) + tr(")Гц "));
			mLog->append(tr("ширина импульса ") + QString::number(inputDuty) + "%");
			mLog->append(QString());
	}
}

Q_EXPORT_PLUGIN2(trikTest, PwmTest)

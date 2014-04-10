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

#include <QtCore/QDebug>
#include <trikControl/pwmCapture.h>

#include "pwmGenerator.h"
#include "messageBox.h"

TestInterface::Result PwmTest::run(trikControl::Brick &brick, QStringList &log)
{
	mGeneratorFiles["JE1"].requestFilePath = "/sys/class/pwm/ehrpwm.1:1/request";
	mGeneratorFiles["JE1"].runFilePath = "/sys/class/pwm/ehrpwm.1:1/run";
	mGeneratorFiles["JE1"].frequencyFilePath = "/sys/class/pwm/ehrpwm.1:1/period_freq";
	mGeneratorFiles["JE1"].dutyFilePath = "/sys/class/pwm/ehrpwm.1:1/duty_percent";

	mGeneratorFiles["JE2"].requestFilePath = "/sys/class/pwm/ehrpwm.1:0/request";
	mGeneratorFiles["JE2"].runFilePath = "/sys/class/pwm/ehrpwm.1:0/run";
	mGeneratorFiles["JE2"].frequencyFilePath = "/sys/class/pwm/ehrpwm.1:0/period_freq";
	mGeneratorFiles["JE2"].dutyFilePath = "/sys/class/pwm/ehrpwm.1:0/duty_percent";

	mGeneratorFiles["JE3"].requestFilePath = "/sys/class/pwm/ehrpwm.0:1/request";
	mGeneratorFiles["JE3"].runFilePath = "/sys/class/pwm/ehrpwm.0:1/run";
	mGeneratorFiles["JE3"].frequencyFilePath = "/sys/class/pwm/ehrpwm.0:1/period_freq";
	mGeneratorFiles["JE3"].dutyFilePath = "/sys/class/pwm/ehrpwm.0:1/duty_percent";

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
	GeneratorFiles generatorFiles = mGeneratorFiles[stage.generatorPort];
	if (generatorFiles.requestFilePath.isEmpty())
	{
		mLog->append(tr("Невозможно получить доступ к ") + stage.generatorPort);
		mLog->append(QString());
		qDebug() << "Cannot access to " + stage.generatorPort;
		result = fail;
		return;
	}

	PwmGenerator generator(generatorFiles);

	trikControl::PwmCapture *capture = mBrick->pwmCapture(stage.capturePort);
	if (capture == NULL)
	{
		mLog->append(tr("Невозможно получить доступ к ") + stage.capturePort);
		mLog->append(QString());
		qDebug() << "Cannot access to " + stage.capturePort;
		result = fail;
		return;
	}

	MessageBox messageBox(tr("Соедините ") + stage.generatorPort + tr(" и ") + stage.capturePort);
	messageBox.exec();

	mLog->append(tr("Подаём на ") + stage.generatorPort);
	mLog->append(tr("Считываем с ") + stage.capturePort);
	mLog->append(QString());

	foreach (int const power, mPowerValues)
	{
		int outputFrequency = power;
		int outputDuty = 50;

		generator.setFrequency(power);
		generator.setDuty(outputDuty);

		mLog->append(tr("На выходе:"));
		mLog->append(tr("частота ") + QString::number(outputFrequency) + tr("Гц"));
		mLog->append(tr("ширина импульса ") + QString::number(outputDuty) + tr("%"));

		QVector<int> inputFrequency = capture->frequency();
		int inputDuty = capture->duty();
		mLog->append(tr("На входе:"));
		mLog->append(tr("частота (") + QString::number(inputFrequency[0]) + ","
				+ QString::number(inputFrequency[1]) + "," + QString::number(inputFrequency[2]) + tr(")Гц "));
		mLog->append(tr("ширина импульса ") + QString::number(inputDuty) + "%");

		if (inputFrequency[0] != outputFrequency || inputFrequency[0] != inputFrequency[1]
				|| inputFrequency[1] != inputFrequency[2] /* || inputDuty != outputDuty */ )
		{
			result = fail;
			mLog->append(tr("Ошибка!"));
			mLog->append(QString());
			break;
		}

		mLog->append(QString());
	}
}

Q_EXPORT_PLUGIN2(trikTest, PwmTest)

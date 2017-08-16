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
#include <trikControl/pwmCaptureInterface.h>

#include "messageBox.h"

TestInterface::Result PwmTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	mBrick = &brick;
	mLog = &log;

	Configurer configurer;
	mStages = configurer.stages();
	mValues = configurer.values();

	result = TestInterface::success;

	foreach (Configurer::Stage const &stage, mStages)
	{
		performStage(stage);
	}

	return result;
}

void PwmTest::performStage(Configurer::Stage const &stage)
{
	auto g = mBrick->motor(stage.generatorPort);
	if (!g)
	{
		mLog->append(tr("Невозможно получить доступ к ") + stage.generatorPort);
		mLog->append(QString());
		qDebug() << "Cannot access to " + stage.generatorPort;
		result = fail;
		return;
	}


	auto c = mBrick->pwmCapture(stage.capturePort);
	if (!c)
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

	foreach (Configurer::Value const &value, mValues)
	{
		int outputFrequency = value.frequency;
		int outputDuty = value.duty;

		g->setPeriod(1000000.0 / outputFrequency);
		g->setPower(outputDuty, false);

		mLog->append(tr("На выходе:"));
		mLog->append(tr("частота ") + QString::number(outputFrequency) + tr("Гц"));
		mLog->append(tr("ширина импульса ") + QString::number(outputDuty) + tr("%"));

		QVector<int> inputFrequency = c->frequency();
		int inputDuty = c->duty();
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

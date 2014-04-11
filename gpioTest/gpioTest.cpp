/* Copyright 2014 CyberTech Labs Ltd.
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

#include "gpioTest.h"

TestInterface::Result GpioTest::run(trikControl::Brick &, QStringList &log)
{
	mLog = &log;
	result = TestInterface::fail;

	int const A1_number = 36;
	int const A2_number = 37;

	GpioPort A1(A1_number, "/sys/class/gpio/gpio" + QString::number(A1_number) + "/");
	GpioPort A2(A2_number, "/sys/class/gpio/gpio" + QString::number(A2_number) + "/");

	testCase(A1, A2, 0);
	testCase(A1, A2, 1);
	testCase(A2, A1, 0);
	testCase(A2, A1, 1);

	return result;
}

void GpioTest::testCase(GpioPort &outputPort, GpioPort &inputPort, int value)
{
	mLog->append(tr("Пишем в порт ")
			+ QString::number(outputPort.number())
			+ tr(", читаем из порта ")
			+ QString::number(inputPort.number()));

	outputPort.setValue(value);
	mLog->append(tr("Записали ") + QString::number(value));

	int const readValue = inputPort.getValue();
	mLog->append(tr("Прочитали ") + QString::number(readValue));
	if (readValue != value) {
		result = TestInterface::fail;
	}

	mLog->append(QString());
}

Q_EXPORT_PLUGIN2(trikTest, GpioTest)

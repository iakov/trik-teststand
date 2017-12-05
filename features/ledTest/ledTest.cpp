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

#include "ledTest.h"

#include <trikControl/ledInterface.h>

#include "yesNoBox.h"

TestInterface::Result LedTest::run(trikControl::BrickInterface &brick, QStringList &log)
{
	TestInterface::Result result = success;
	YesNoBox yesNoBox;
	brick.led()->off();

	auto localTimer = new QTimer();
	localTimer->setInterval(300);
	connect(localTimer, &QTimer::timeout, [&brick](){
		brick.led()->green();
		QTimer::singleShot(100, [&brick](){brick.led()->orange();});
		QTimer::singleShot(200, [&brick](){brick.led()->red();});
	});

	localTimer->start();
	yesNoBox.setQuestion(tr("Светодиод моргает?"));
	if (yesNoBox.exec() == YesNoBox::no) {
		result = fail;
		log.append(tr("Не работает светодиод"));
	}

	localTimer->stop();
	brick.led()->off();
	return result;
}

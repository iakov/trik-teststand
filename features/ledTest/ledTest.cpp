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
	brick.led()->green();
	yesNoBox.setQuestion(tr("Светодиод сейчас зелёный?"));
	if (yesNoBox.exec() == YesNoBox::no) {
		result = fail;
		log.append(tr("Не работает зелёный свет"));
	}

	brick.led()->orange();
	yesNoBox.setQuestion(tr("Светодиод сейчас оранжевый?"));
	if (yesNoBox.exec() == YesNoBox::no) {
		result = fail;
		log.append(tr("Не работает оранжевый свет"));
	}

	brick.led()->red();
	yesNoBox.setQuestion(tr("Светодиод сейчас красный?"));
	if (yesNoBox.exec() == YesNoBox::no) {
		result = fail;
		log.append(tr("Не работает красный свет"));
	}

	brick.led()->off();
	return result;
}

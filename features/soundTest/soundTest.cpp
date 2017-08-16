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

#include "soundTest.h"

#include "yesNoBox.h"

TestInterface::Result SoundTest::run(trikControl::BrickInterface &brick, QStringList &)
{
	brick.playTone(500,500);
	YesNoBox yesNoBox(tr("Был слышен звук?"));
	if (yesNoBox.exec() == YesNoBox::yes)
	{
		return TestInterface::success;
	}
	else
	{
		return TestInterface::fail;
	}
}

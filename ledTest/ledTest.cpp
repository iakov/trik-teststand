#include "ledTest.h"

#include <trikControl/led.h>

#include "yesNoBox.h"

TestInterface::Result LedTest::run(trikControl::Brick &brick, QStringList &log)
{
	TestInterface::Result result = success;

	YesNoBox yesNoBox;

	brick.led()->red();
	yesNoBox.setQuestion(tr("Вы видите красный свет?"));
	if (yesNoBox.exec() == YesNoBox::no)
	{
		result = fail;
		log.append(tr("Не работает красный свет"));
	}

	brick.led()->green();
	yesNoBox.setQuestion(tr("Вы видите зелёный свет?"));
	if (yesNoBox.exec() == YesNoBox::no)
	{
		result = fail;
		log.append(tr("Не работает зелёный свет"));
	}

	brick.led()->orange();
	yesNoBox.setQuestion(tr("Вы видите оранжевый свет?"));
	if (yesNoBox.exec() == YesNoBox::no)
	{
		result = fail;
		log.append(tr("Не работает оранжевый свет"));
	}

	return result;
}

Q_EXPORT_PLUGIN2(trikTest, LedTest)

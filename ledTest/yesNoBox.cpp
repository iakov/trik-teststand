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

#include "yesNoBox.h"

#include <QtGui/QKeyEvent>

YesNoBox::YesNoBox(QString const &question)
	: questionLabel(question)
	, yesLabel(tr("Да"))
	, noLabel(tr("Нет"))
{
	setWindowState(Qt::WindowFullScreen);

	questionLabel.setAlignment(Qt::AlignCenter);
	yesLabel.setAlignment(Qt::AlignBottom | Qt::AlignLeft);
	yesLabel.setAlignment(Qt::AlignBottom | Qt::AlignRight);

	answersLayout.addWidget(&noLabel);
	answersLayout.addWidget(&yesLabel);
	mainLayout.addWidget(&questionLabel);
	mainLayout.addLayout(&answersLayout);
	setLayout(&mainLayout);
}

void YesNoBox::setQuestion(QString const &question)
{
	questionLabel.setText(question);
}

YesNoBox::Answer YesNoBox::exec()
{
	if (mEventLoop.exec() == 0)
		return yes;
	else
		return no;
}

void YesNoBox::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_Enter:
		{
			mEventLoop.exit(0);
			break;
		}
		case Qt::Key_Menu:
		{
			mEventLoop.exit(-1);
			break;
		}
		default:
		{
			QWidget::keyPressEvent(event);
			break;
		}
	}
}

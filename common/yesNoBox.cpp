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
#include <QtCore/QtDebug>

YesNoBox::YesNoBox(QString const &question)
	: questionLabel(question)
	, yesLabel(tr("Да"))
	, noLabel(tr("Нет"))
{

	questionLabel.setAlignment(Qt::AlignCenter);
	noLabel.setAlignment(Qt::AlignBottom | Qt::AlignLeft);
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
	setWindowState(Qt::WindowFullScreen);
}

YesNoBox::Answer YesNoBox::exec()
{
	show();
	Answer answer;
	if (mEventLoop.exec() == 0)
		answer = yes;
	else
		answer = no;
	hide();
	return answer;
}

void YesNoBox::keyPressEvent(QKeyEvent *event)
{
	qDebug() << Q_FUNC_INFO << event->key();
	switch (event->key())
	{
		case Qt::Key_Return:
		{
			mEventLoop.exit(0);
			break;
		}
		case Qt::Key_Down:
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

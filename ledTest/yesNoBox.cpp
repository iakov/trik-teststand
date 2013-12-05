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

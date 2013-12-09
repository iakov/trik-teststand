#include <QtGui/QKeyEvent>

#include "messageBox.h"

MessageBox::MessageBox(QString const &message)
	: messageLabel(message)
	, okLabel("OK")
{
	setWindowState(Qt::WindowFullScreen);

	messageLabel.setAlignment(Qt::AlignCenter);
	okLabel.setAlignment(Qt::AlignRight | Qt::AlignBottom);

	layout.addWidget(&messageLabel);
	layout.addWidget(&okLabel);
	setLayout(&layout);
}

void MessageBox::exec()
{
	show();
	mEventLoop.exec();
	hide();
}

void MessageBox::setMessage(const QString &message)
{
	messageLabel.setText(message);
}

void MessageBox::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter)
		mEventLoop.quit();
	else
		QWidget::keyPressEvent(event);
}

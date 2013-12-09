#include "messageBox.h"

MessageBox::MessageBox(QString const &message)
	: messageLabel(message)
{
	setWindowState(Qt::WindowFullScreen);

	messageLabel.setAlignment(Qt::AlignCenter);
	layout.addWidget(&messageLabel);
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

void MessageBox::keyPressEvent(QKeyEvent *)
{
	mEventLoop.quit();
}

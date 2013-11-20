#include "trikTestApplication.h"

#include <QtGui/QKeyEvent>

TrikTestApplication::TrikTestApplication(int argc, char **argv) :
	QApplication(argc, argv)
{
	keysConversions[Qt::Key_F2] = Qt::Key_Meta;
	keysConversions[Qt::Key_F3] = Qt::Key_Left;
	keysConversions[Qt::Key_F4] = Qt::Key_Down;
	keysConversions[Qt::Key_F5] = Qt::Key_Up;
	keysConversions[Qt::Key_F6] = Qt::Key_Enter;
	keysConversions[Qt::Key_F7] = Qt::Key_Right;
}

bool TrikTestApplication::notify(QObject *receiver, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
		if (keyEvent != NULL && keysConversions.contains(keyEvent->key()))
		{
			int newKey = keysConversions[keyEvent->key()];
			*keyEvent = QKeyEvent(QEvent::KeyPress, newKey, Qt::NoModifier);
		}
	}
	return QApplication::notify(receiver, event);
}

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

#include "trikTestApplication.h"

#include <QtGui/QKeyEvent>

using namespace trikTest;

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

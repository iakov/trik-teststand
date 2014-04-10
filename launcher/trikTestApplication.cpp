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

TrikTestApplication::TrikTestApplication(int argc, char **argv)
	: QApplication(argc, argv)
{
	mKeyMappings[Qt::Key_Up] = Qt::Key_Left;
	mKeyMappings[Qt::Key_Right] = Qt::Key_Up;
	mKeyMappings[Qt::Key_Down] = Qt::Key_Right;
	mKeyMappings[Qt::Key_Left] = Qt::Key_Down;
}

bool TrikTestApplication::notify(QObject *receiver, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
		if (keyEvent != NULL && mKeyMappings.contains(keyEvent->key()))
		{
			int newKey = mKeyMappings[keyEvent->key()];
			*keyEvent = QKeyEvent(QEvent::KeyPress, newKey, Qt::NoModifier);
		}
	}
	return QApplication::notify(receiver, event);
}

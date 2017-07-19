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

#include <QtGui/QFont>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>

#include "trikTestApplication.h"
#include "launcher.h"

int main(int argc, char *argv[])
{
	TrikTestApplication a(argc, argv);

	QFont newFont(a.font());
	newFont.setPixelSize(12);
	a.setFont(newFont);

	//QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	Launcher launcher;
	launcher.show();

	QTimer::singleShot(0, &launcher, SLOT(startTesting()));

	return a.exec();
}

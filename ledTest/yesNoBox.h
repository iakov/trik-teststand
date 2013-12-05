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

#pragma once

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QEventLoop>
#include <QtCore/QString>

class YesNoBox : public QWidget
{
	Q_OBJECT

public:
	explicit YesNoBox(QString const &question = QString());
	void setQuestion(QString const &question);

	enum Answer
	{
		yes
		, no
	};

	Answer exec();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QVBoxLayout mainLayout;
	QHBoxLayout answersLayout;
	QLabel questionLabel, yesLabel, noLabel;

	QEventLoop mEventLoop;
};

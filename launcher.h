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
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QStandardItemModel>
#include <QtCore/QModelIndex>
#include <QtGui/QKeyEvent>

#include <trikControl/brick.h>

#include "configurer.h"

namespace trikTest
{

class Launcher : public QWidget
{
	Q_OBJECT

public:
	Launcher();
	void startTesting();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	trikControl::Brick mBrick;
	QMap<QString, QString> mFiles;
	QMap<QString, QStringList> mLogs;
	QMap<QString, int> mRows;

	QVBoxLayout mMainLayout;
	QHBoxLayout mTopLayout, mBottomLayout;
	QLabel mTopButtonsLabels[2], mBottomButtonsLabels[2];
	QStandardItemModel mListModel;
	QListView mListView;

	void performTest(QString const &name);

	enum State
	{
		inProcess
		, finished
	};

	State mState;

	void setState(State state);

	enum TestState
	{
		testInProcess
		, testSuccess
		, testFail
	};

	void setTestState(QString const &name, TestState state);
};

}

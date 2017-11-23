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

#include "launcher.h"

#include <QtCore/QPluginLoader>
#include <QtCore/QList>
#include <QtCore/QDebug>

#include <trikControl/brickFactory.h>

#include "testInterface.h"
#include "trikTestApplication.h"
#include "logPrinter.h"
#include "messageBox.h"

using namespace trikControl;

Launcher::Launcher()
	/* TODO: move TrikControl directory to config file or command line argument */
	: mBrick(BrickFactory::create("./system-config.xml", "./model-config.xml", "/home/root/trik/scripts"))
	, mTable(0, 2)
	, mState(finished)
{
	setWindowState(Qt::WindowFullScreen);

	mTable.setColumnWidth(0, 150);
	mTable.setColumnWidth(1, 100);

	mTopButtonsLabels[0].setAlignment(Qt::AlignTop | Qt::AlignLeft);
	mTopButtonsLabels[1].setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	mTopButtonsLabels[2].setAlignment(Qt::AlignTop | Qt::AlignRight);
	mBottomButtonsLabels[0].setAlignment(Qt::AlignBottom | Qt::AlignLeft);
	mBottomButtonsLabels[1].setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	mBottomButtonsLabels[2].setAlignment(Qt::AlignBottom | Qt::AlignRight);

	mTopButtonsLabels[1].setText(tr("Вверх"));
	mBottomButtonsLabels[1].setText(tr("Вниз"));

	for (int i = 0; i < 3; ++i)
	{
		mTopLayout.addWidget(&mTopButtonsLabels[i]);
		mBottomLayout.addWidget(&mBottomButtonsLabels[i]);
	}

	mTable.setSelectionBehavior(QAbstractItemView::SelectRows);
	mTable.setSelectionMode(QAbstractItemView::SingleSelection);

	mMainLayout.addLayout(&mTopLayout);
	mMainLayout.addWidget(&mTable);
	mMainLayout.addLayout(&mBottomLayout);

	setLayout(&mMainLayout);

	Configurer configurer;
	mTests = configurer.tests();
	mFiles = configurer.files();
}

void Launcher::startTesting()
{
	MessageBox messageBox(tr("Тестирование контроллера ТРИК"));
	messageBox.exec();

	setState(inProcess);
	for (QString const &test : mTests)
	{
		performTest(test);
	}

	setState(finished);
}

void Launcher::keyPressEvent(QKeyEvent *event)
{
	switch (mState)
	{
		case inProcess:
		{
			QWidget::keyPressEvent(event);
			break;
		}
		case finished:
		{
			switch (event->key())
			{
				case Qt::Key_Return:
				{
					LogPrinter logPrinter(mLogs[mTable.item(mTable.currentRow(), 0)->text()]);
					logPrinter.exec();
					break;
				}
				case Qt::Key_Left:
				{
					setState(inProcess);
					performTest(mTable.item(mTable.currentRow(), 0)->text());
					setState(finished);
					break;
				}
				default:
				{
					QWidget::keyPressEvent(event);
					break;
				}
			}
			break;
		}
	}
}

void Launcher::performTest(QString const &name)
{
	setTestState(name, testInProcess);

	mLogs[name].clear();

	QPluginLoader testLoader(mFiles[name]);

	if (testLoader.instance() == NULL)
	{
		setTestState(name, testFail);
		qDebug() << "Cannot load the test: " << testLoader.errorString();
		mLogs[name].append(tr("Не удалось подключить тест"));
		mLogs[name].append(testLoader.errorString());
	}
	else
	{
		TestInterface *test = qobject_cast<TestInterface *>(testLoader.instance());
		if (test == NULL)
		{
			setTestState(name, testFail);
			qDebug() << "Cannot perform type casting";
			mLogs[name].append(tr("Не удалось подключить тест"));
			mLogs[name].append(tr("Ошибка приведения типов"));
		}
		else
		{
			TestInterface::Result result = test->run(*mBrick, mLogs[name]);
			setTestState(name, (result == TestInterface::success) ? testSuccess : testFail);
		}
	}

	testLoader.unload();
}

void Launcher::setState(Launcher::State state)
{
	mState = state;
	switch (state)
	{
		case inProcess:
		{
			mBottomButtonsLabels[0].setText(QString());
			mBottomButtonsLabels[2].setText(QString());
			break;
		}
		case finished:
		{
			mTopButtonsLabels[0].setText(tr("Повторить"));
			mBottomButtonsLabels[2].setText(tr("Журнал"));
			mTable.repaint();
			TrikTestApplication::activeWindow()->repaint();
			break;
		}
	}
}

void Launcher::setTestState(QString const &name, Launcher::TestState state)
{
	if (!mRows.contains(name))
	{
		int row = mTable.rowCount();
		mRows[name] = row;
		mTable.insertRow(row);
		mTable.setItem(row, 0, new QTableWidgetItem(name));
		mTable.setItem(row, 1, new QTableWidgetItem(QString()));
		mTable.item(row, 1)->setTextAlignment(Qt::AlignCenter);
	}

	mTable.selectRow(mRows[name]);

	switch (state)
	{
		case testInProcess:
		{
			mTable.item(mRows[name], 1)->setText("IN PROCESS");
			break;
		}
		case testSuccess:
		{
			mTable.item(mRows[name], 1)->setText("OK");
			break;
		}
		case testFail:
		{
			mTable.item(mRows[name], 1)->setText("FAIL");
			break;
		}
	}

	TrikTestApplication::processEvents();
	mTable.repaint();
	//TrikTestApplication::activeWindow()->repaint();
}

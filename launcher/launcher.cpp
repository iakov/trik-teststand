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
#include <QtCore/QThread>
#include <QtCore/QDateTime>

#include <trikControl/brickFactory.h>

#include "testInterface.h"
#include "trikTestApplication.h"
#include "logPrinter.h"
#include "messageBox.h"

#include <QtCore/QtDebug>

using namespace trikControl;

Launcher::Launcher()
	/* TODO: move TrikControl directory to config file or command line argument */
	: mBrick(BrickFactory::create("./system-config.xml", "./model-config.xml", "/home/root/trik/scripts"))
	, mTable(0, 2)
	, mState(finished)
{
	setWindowState(Qt::WindowFullScreen);
	mTable.setHorizontalHeaderLabels({tr("Тест"), tr("Статус")});

	mTable.setColumnWidth(0, 90);
	mTable.setColumnWidth(1, 90);

	mTopButtonsLabels[0].setAlignment(Qt::AlignTop | Qt::AlignLeft);
	mTopButtonsLabels[1].setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	mTopButtonsLabels[2].setAlignment(Qt::AlignTop | Qt::AlignRight);
	mBottomButtonsLabels[0].setAlignment(Qt::AlignBottom | Qt::AlignLeft);
	mBottomButtonsLabels[1].setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	mBottomButtonsLabels[2].setAlignment(Qt::AlignBottom | Qt::AlignRight);

	mTopButtonsLabels[1].setText(tr("Вверх"));
	mBottomButtonsLabels[1].setText(tr("Вниз"));

	for (int i = 0; i < 3; ++i)	{
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
	MessageBox messageBox(tr("Тестирование контроллера ТРИК:\n")
		+ tr("Нажмите Верх/Вниз, чтобы выбрать тест\n")
		+ tr("Нажмите Влево, чтобы повторить тест\n")
		+ tr("Нажмите Ок, чтобы просмотреть ошибки в тесте\n")
		+ tr("Нажмите Выключение питания, чтобы записать журнал по всем тестам на диск\n"));

	messageBox.exec();

	setState(inProcess);
	for (QString const &test : mTests) {
		performTest(test);
	}

	setState(finished);
}

void Launcher::keyPressEvent(QKeyEvent *event)
{
	qDebug() << Q_FUNC_INFO << event->key();
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
				case Qt::Key_Escape:
				{
					qDebug() << Q_FUNC_INFO << "file writing";
					const auto path = "/home/root/tests.log";
					QFile file(path);
					if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
						return;
					}

					QTextStream out(&file);
					out << tr("\n Журнал тестирования \n") << QDateTime::currentDateTime().toString() << endl;
					for (const QString &key : mLogs.keys()) {
						for (const QString &raw : mLogs[key]) {
							out << raw << "\n";
						}
					}

					system(qPrintable("sync"));
					qDebug() << Q_FUNC_INFO << "sync";
					file.close();

					MessageBox messageBox(tr("Журнал записан на SD карту"));
					messageBox.exec();

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
	QThread::msleep(100);

	mLogs[name].clear();

	QPluginLoader testLoader(mFiles[name]);

	if (!testLoader.instance()) {
		setTestState(name, testFail);
		mLogs[name].append(tr("Не удалось подключить тест"));
		mLogs[name].append(testLoader.errorString());
	} else {
		TestInterface *test = qobject_cast<TestInterface *>(testLoader.instance());
		if (!test) {
			setTestState(name, testFail);
			mLogs[name].append(tr("Не удалось подключить тест"));
			mLogs[name].append(tr("Ошибка приведения типов"));
		} else {
			QThread::msleep(100);
			TestInterface::Result result = test->run(*mBrick, mLogs[name]);
			QThread::msleep(100);
			setTestState(name, (result == TestInterface::success) ? testSuccess : testFail);
			QThread::msleep(100);
		}
	}

	testLoader.unload();
}

void Launcher::setState(Launcher::State state)
{
	mState = state;
	switch (state) {
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
			if (auto activeWindow = TrikTestApplication::activeWindow()) {
				activeWindow->repaint();
			}

			break;
		}
	}
}

void Launcher::setTestState(QString const &name, Launcher::TestState state)
{
	if (!mRows.contains(name)) {
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
			mTable.item(mRows[name], 1)->setText("...");
			break;
		}
		case testSuccess:
		{
			mTable.item(mRows[name], 1)->setText("PASSED");
			break;
		}
		case testFail:
		{
			mTable.item(mRows[name], 1)->setText("FAILED");
			break;
		}
	}

	TrikTestApplication::processEvents();
	mTable.repaint();
	if (auto activeWindow = TrikTestApplication::activeWindow()) {
		activeWindow->repaint();
	}
}

/* Copyright 2014 CyberTech Labs Ltd.
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

#include "pwmGenerator.h"

#include <QtCore/QDebug>

PwmGenerator::PwmGenerator(const PwmTest::GeneratorFiles &files
		, QObject *parent)
	: QObject(parent)
	, mRequestFile(files.requestFilePath)
	, mRunFile(files.runFilePath)
	, mFrequencyFile(files.frequencyFilePath)
	, mDutyFile(files.dutyFilePath)
{
	if (!mRequestFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered | QIODevice::Text)) {
		qDebug() << "Can't open " << mRequestFile.fileName();
	} else {
		mRequestFile.write("1");
		mRequestFile.flush();
	}

	if (!mRunFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered | QIODevice::Text)) {
		qDebug() << "Can't open " << mRunFile.fileName();
	} else {
		mRunFile.write("1");
		mRunFile.flush();
	}

	if (!mFrequencyFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered | QIODevice::Text)) {
		qDebug() << "Can't open " << mFrequencyFile.fileName();
	}

	if (!mDutyFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered | QIODevice::Text)) {
		qDebug() << "Can't open " << mDutyFile.fileName();
	}
}

PwmGenerator::~PwmGenerator()
{
	mDutyFile.close();
	mFrequencyFile.close();

	mRunFile.write("0");
	mRunFile.close();

	mRequestFile.write("0");
	mRequestFile.close();
}

void PwmGenerator::setFrequency(int frequency)
{
	mFrequencyFile.write(QString::number(frequency).toLatin1());
	mFrequencyFile.flush();
}

void PwmGenerator::setDuty(int duty)
{
	mDutyFile.write(QString::number(duty).toLatin1());
	mDutyFile.flush();
}

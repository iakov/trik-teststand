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

#pragma once

#include "testInterface.h"

class jvp1Test
		: public QObject
		, public TestInterface
{
	Q_OBJECT
	Q_INTERFACES(TestInterface)
	Q_PLUGIN_METADATA(IID "com.trikset.teststand.vp1")

public:
	TestInterface::Result run(trikControl::BrickInterface &brick, QStringList &log);

private:
	TestInterface::Result captureVideo();

	QStringList *mLog;
	TestInterface::Result mResult;
};

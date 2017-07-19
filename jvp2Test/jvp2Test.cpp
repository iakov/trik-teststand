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

#include "jvp2Test.h"

TestInterface::Result jvp2Test::run(trikControl::BrickInterface &brick, QStringList &log)
{
	mLog = &log;
	if (captureVideo() == TestInterface::fail) {
		return TestInterface::fail;
	}

	return TestInterface::success;
}
TestInterface::Result jvp2Test::captureVideo()
{
	int error = system("./run_cam_test.sh 1");
        if(error){
                mLog->append(tr("Проверка порта JVP2 завершилась успешно"));
                return TestInterface::fail;
        }
        mLog->append(tr("Проверка порта JVP2 завершилась неудачей"));
	return TestInterface::success;
}

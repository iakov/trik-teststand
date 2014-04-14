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

#include "mspTest.h"

#include <stdio.h>
#include <stdlib.h>

#define FIRMWARE_PATH "/home/root/mspdev.txt"
#define JB1 0x30
#define JB2 0x31
#define JB3 0x32
#define JB4 0x33
#define JM1 0x14
#define JM2 0x15
#define JM3 0x16
#define JM4 0x17

TestInterface::Result MspTest::run(trikControl::Brick &, QStringList &log)
{
	system("mspflasher -o " FIRMWARE_PATH);
	result = TestInterface::success;
	testCase(JB1, JM1);
	testCase(JB2, JM2);
	testCase(JB3, JM3);
	testCase(JB4, JM4);
	return result;
}

void MspTest::testCase(int JBAddress, int JMAddress)
{
	char *cmd;

	system("i2ctest -y 2 0x48 0x10 0x1000 w");
}

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

#include "i2cCommunicator.h"

#include <cerrno>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>

static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
		int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(file,I2C_SMBUS,&args);
}

static inline __s32 i2c_smbus_read_byte(int file)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

I2cCommunicator::I2cCommunicator(QMap<int, QString> const &busFiles)
{
	for (QMap<int, QString>::const_iterator i = busFiles.constBegin(); i != busFiles.constEnd(); ++i)
	{
		mBusFileDescriptors[i.key()] = open(i.value().toLocal8Bit().data(), O_RDWR);
		if (mBusFileDescriptors[i.key()] < 0)
		{
			qDebug() << "Cannot open bus file " << i.value();
		}
	}
}

bool I2cCommunicator::isOnboard(int bus, int address)
{
	if (!mBusFileDescriptors.contains(bus) || mBusFileDescriptors[bus] < 0)
		return false;

	if (ioctl(mBusFileDescriptors[bus], I2C_SLAVE, address) < 0)
	{
		return (errno == EBUSY);
	}

	return (i2c_smbus_read_byte(mBusFileDescriptors[bus]) >= 0);
}

#!/usr/bin/python

"""
Copyright 2015 Ilya Sukhanov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import smbus
import struct

GYRO_ADDRESS = 0x68
PWR_MGMT_1_REG = 0x6B
DATA_REG = 0x3B
DATA_VALUES = ["accelerator_x", "accelerator_y", "accelerator_z", "temperature", "gyro_x", "gyro_y", "gyro_z"]

STRUCT_FORMAT = ">h"  # big endian 16 bit 2's complement
BYTES_PER_VALUE = struct.calcsize(STRUCT_FORMAT)
LEN_VALUES =  BYTES_PER_VALUE * len(DATA_VALUES)


class Gyro(object):
	def __init__(self, bus_address=1):
		self.bus = smbus.SMBus(bus_address)

	def read_data(self):
		data = self.bus.read_i2c_block_data(GYRO_ADDRESS, DATA_REG, LEN_VALUES)	
		print(data)
		data = str(bytearray(data))
		return {
			value_name:Gyro.to_int(data, index*BYTES_PER_VALUE)
			for index, value_name in enumerate(DATA_VALUES)
		}

	@staticmethod
	def to_int(data, index):
		return struct.unpack_from(STRUCT_FORMAT, data, index)[0]
 

	def wake_up(self):
		"""
		Device powers on in sleep mode, flipping bit 2 of power
		management register wakes it up
		"""
		self.bus.write_byte_data(GYRO_ADDRESS, PWR_MGMT_1_REG, 0x00)


if __name__ == "__main__":
    """
    Print accelerator values 'till the "end"
    """
    import time
    gyro = Gyro()
    gyro.wake_up()
    while True:
        data = gyro.read_data()
        for value_name in ["accelerator_x", "accelerator_y", "accelerator_z"]:
            print("{0}:\t{1}".format(value_name, data[value_name]/16384.0))
        time.sleep(1)

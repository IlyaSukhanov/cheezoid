import spidev
import time
import struct

class OutOfBoundsError(RuntimeError):
    pass

SPI_BUS = 0
SPI_DEVICE = 0

class CheezoidDrive():
    def __init__(self):
        self.spi_bus = spidev.SpiDev()
        self.spi_bus.open(SPI_BUS, SPI_DEVICE)

    def move(self, rotation_distance, drive_distance):
        data = bytearray(struct.pack( "<h", rotation_distance))
        data.extend(bytearray(struct.pack( "<h", drive_distance)))
        self.spi_bus.xfer(data)

if __name__ == "__main__":
    drive = CheezoidDrive()
    drive.move(10, 10)
    time.sleep(1)


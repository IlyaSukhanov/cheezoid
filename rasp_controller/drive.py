import time
import struct

try:
    # likely to fail on non-raspberries
    import spidev
except:
    print "WARNING: could not import spidev"
    SpiDev = type("mock_SpiDev", (object,), {"open": lambda self, bus, dev: object(), "xfer": lambda self, bits: None})()
    spidev = type("mock_spidev", (object,), {"SpiDev": lambda self: SpiDev})()

class OutOfBoundsError(RuntimeError):
    pass

SPI_BUS = 0
SPI_DEVICE = 0

class CheezoidDrive():
    def __init__(self):
        self.spi_bus = spidev.SpiDev()
        self.spi_bus.open(SPI_BUS, SPI_DEVICE)
        self.spi_bus.max_speed_hz = 20000
        self.spi_bus.mode = 1

    def move(self, rotation_distance, drive_distance):
        # TODO I suspect this does not work with <0 values.. And xfer doesn't
        #      accept bytearray. Lame.
        self.spi_bus.xfer([rotation_distance>>8 & 0xff, rotation_distance & 0xff])
        time.sleep(.1)
        self.spi_bus.xfer([drive_distance>>8 & 0xff, drive_distance & 0xff])


if __name__ == "__main__":
    drive = CheezoidDrive()
    while True:
        drive.move(10, 48)
        time.sleep(1)

import time
import struct
import logging

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
MAX_ATTEMPTS = 120 
RETRY_INTEVAL = .5  # seconds

class CheezoidDrive():
    def __init__(self):
        self._spi_bus = spidev.SpiDev()
        self._spi_bus.open(SPI_BUS, SPI_DEVICE)
        self._spi_bus.max_speed_hz = 20000
        self._spi_bus.mode = 1
 
    def _send(self, data):
        resp = self._spi_bus.xfer2([data>>8 & 0xff, data & 0xff])
        time.sleep(.1)
        return resp
        
    def move(self, rotation_distance, drive_distance):
        reply_sum = 1
        while reply_sum != 0:
            logging.debug("\tSending: move({0},{1})".format(rotation_distance, drive_distance))
            reply_sum = 0
            reply = [] 
            reply.extend(self._send(rotation_distance))
            reply.extend(self._send(drive_distance))
            logging.debug("\t\treply: {0}".format([hex(byte) for byte in reply]))
            for byte in reply:
                if byte == 0:
                    return
            logging.debug("\t\treply_sum:\t{0}".format(reply_sum))
            time.sleep(.1)


if __name__ == "__main__":
    drive = CheezoidDrive()
    i=0;
    while i<3:
        logging.debug("here1")
        i+=1;
        drive.move(48,96)

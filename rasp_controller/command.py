import spidev
import time

class OutOfBoundsError(RuntimeError):
	pass

SPI_BUS = 0
SPI_DEVICE = 0
FORWARD = 1
BACKWARD = 2

class CheezoidDrive():
	def __init__(self):
		self.spi_bus = spidev.SpiDev()
		self.spi_bus.open(SPI_BUS, SPI_DEVICE)

	def drive(self, distance, direction):
		if distance > 0x7f:
			raise OutOfBoundsError("Exceeded maximum distance")
		if direction == FORWARD:
			self.spi_bus.xfer([distance])
		elif direction == BACKWARD:
			self.spi_bus.xfer([0x80 & distance])

if __name__ == "__main__":
	drive = CheezoidDrive()
	drive.drive(0x7f, FORWARD)
	time.sleep(1)


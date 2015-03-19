Cheezoid
========

Cheezoid is a robot which draws on a whiteboard. Think `turtle graphics`_ in
meatspace. Its an Amplify Maker Days Project.

.. _turtle graphics: http://en.wikipedia.org/wiki/Turtle_graphics

Overview
--------

Cheezoid is made up from:

 - 4 `drive units`_ each made up of

   * motor
   * `dfrobot`_ opto-encoder
   * wheels
   * mounts

 - `Dagu`_ `4 channel motor controller`_ driver

 - `Raspberri Pi B+`_

   * with wifi adapter
   * with camera
   * with `prototype board`_

 - `Microstick`_ development board.

   * `PIC24HJ64GP502`_

 - `MPU-6050`_ 3 axis gyroscope accelerometer

 - 2.1Amp `USB battery`_

The idea is that cheezoid drives (vertically!) on a whiteboard drawing its
trail with a whiteboard marker. There is no permament attachment to the
whiteboard, magnets are used to keep it in place. They are attached to the
bottom of the cheezoid chassis, and thus hover about 1mm from the board
surface. Wheels are made or soft rubber, providing great traction while
also soft enough to avoid damage to the board.

Cheezoid is has two controllers. One for the low level motor and encoder
control, and the other for high level position tracking and drawing
control. Low level controller is a PIC24H on a Microstick development
board. It is hooked up to the 4 channel motor contoller directly, and uses
a mixture of 3v (pwm/direction) and 5v(encoder feedback) signals. The low
level controller recieves commands over `SPI`_ protocol from the High Level
Raspberri Pi B+ controller.

Power is a bit cludgy, USB battery feeds raspberri pi which in turn provies
5V power to the motor controller. During normal operation PIC24H is powered
from Raspberris USB port, but during programmig its connected to laptop.
Programming PIC from raspberri, has not been investigated. Assumption is that
it is not possible as microchip does not provide ARM compatible programming
software.

.. _drive units: http://www.robotshop.com/en/miniq-motor-wheel-set-encoder.html#Specifications
.. _dagu: https://sites.google.com/site/daguproducts/home/instruction-manuals
.. _dfrobot: http://www.dfrobot.com
.. _4 channel motor controller: https://docs.google.com/viewer?a=v&pid=explorer&chrome=true&srcid=0B__O096vyVYqYzBkOTA4ODAtMzdjZC00NThlLWFhMzUtMTFmNWYxN2FkZDli&hl=en_US
.. _microstick: http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=DM330013
.. _PIC24HJ64GP502: http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en534556
.. _Raspberri Pi B+: http://www.raspberrypi.org/products/model-b-plus/
.. _prototype board: http://www.amazon.com/gp/product/B00N1X5CM4/ref=oh_aui_detailpage_o05_s02?ie=UTF8&psc=1
.. _MPU-6050: http://www.i2cdevlib.com/devices/mpu6050#source
.. _USB battery: https://www.amazon.com/gp/product/B00P8ZU782/ref=oh_aui_detailpage_o05_s01?ie=UTF8&psc=1
.. _SPI: http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus

PIC development
---------------
Microchips issued tools are used for pic development. This includes:
 
 * IDE / Programmer: MPLABX (v2.30)
 * Compiler: xc16, aka c30 (v1.24)
 * Part Support: `xc16 part support` (v1.24)

All of these can be downloaded from mplabx_ website.

After installing xc16 check out `peripheral library documentation`_

.. _mplabx: http://www.microchip.com/mplabx-ide-linux-installer
.. _peripheral library documentation: file:///opt/microchip/xc16/v1.24/docs/periph_libs/16-bit%20Peripheral%20Libraries.htm#PIC24H

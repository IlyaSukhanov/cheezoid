Cheezoid
========

Cheezoid is a robot which draws on a whiteboard. Think `turtle graphics`_ in
meatspace. It was originally an Amplify Maker Days project but has since been
open sourced under `apache 2.0 license`_.

.. _turtle graphics: http://en.wikipedia.org/wiki/Turtle_graphics
.. _apache 2.0 license: http://www.apache.org/licenses/LICENSE-2.0

Overview
--------


The idea is that cheezoid drives (vertically!) on a whiteboard drawing as
it goes, with a whiteboard marker. There is no permanent attachment to the
whiteboard, magnets are used to keep it in place. They are attached to the
bottom of the cheezoid chassis, hover about 1mm from the board
surface. Wheels are made or soft rubber, providing great traction while
also soft enough to avoid damage to the board.

Cheezoid is has two controllers. One for the low level motor and encoder
control, and the other for high level position tracking and drawing
control. Low level controller is a PIC24H on a Microstick development
board. It is hooked up to the motor driver directly, and uses
a mixture of 3v (pwm/direction) and 5v(encoder feedback) signals. The low
level controller receives commands over `SPI`_ protocol from the High Level
Raspberry Pi B+ controller.

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

RaspberryPi initialization
--------------------------
On RaspberryPI we're using Raspbian_ distro.

The following additional configuration has been performed through raspi-config:

 * Enable SPI & drivers set to load on boot
 
 * Enable I2C & drivers set to load on boot
 
 * Enable Camera Module

Following packages have been installed:

 * Creature comforts ``apt-get install vim git fish ipython python-setuptools python-dev``

 * SimpleCV & dependencies:

   + ``apt-get install ipython python-opencv python-scipy python-numpy python-setuptools python-pip``

   + ``pip install https://github.com/sightmachine/SimpleCV/zipball/master``

   + ``pip install svgwrite``

 * i2c requirements ``apt-get install python-smbus i2c-tools``
 
 * spi requirements ``pip install spidev``

 * RPIO ``pip install rpio``

Additional OS config has been performed:

  * ``/etc/hostname`` changed to cheezoid
 
  * sshd password login disabled
  
  * `wpa_supplicant configured`_ & wlan0 has been configured as follows::
   
     allow-hotplug wlan0
     iface wlan0 inet manual
     wpa-roam /etc/wpa_supplicant/wpa_supplicant.conf
     iface default inet dhcp

.. _wpa_supplicant configured: http://w1.fi/cgit/hostap/plain/wpa_supplicant/wpa_supplicant.conf
.. _Raspbian: http://www.raspbian.org/

Cheezoid Hardware Platform V1
-----------------------------

The first cheezoid hardware platform is a straight forward 4 wheel drive
platform. Chassis is built from a .236" acrylic sheet cut to 4"x7.5", a hole
is drilled in the middle for the whiteboard marker. And raspberry and motor
driver are screwed down to standoffs made from the same acrylic sheet. All
acrylic to acrylic joints are 'welded'  with Weld-On 4. The rear standoffs
are glued straight to the motor mounts using Devcon Plastic Welder (cream).

The tip of marker is attached to a `sub-micro servo`_ and is guided by a long
acrylic standoff. The other end of the maker is zip tied to a rubber band
which is attached to guide. This spring loads the marker in down position.
Servo is use to lift the marker.

Initially the power was fed straight into raspberry pi, and in turn tapped
out to motor driver over the breakout header. This turned out to be problematic
Undoubtedly the raspberry regulator was not designed for upward of 2amp
current draw. As a precautionary measure the power was then re-wired to
go first into the motor driver, and raspberry tapped from there on.
During normal operation PIC24H is powered from Raspberry's USB port, but during
programming its connected to laptop. Programming PIC from raspberry, has not
been investigated. Assumption is that it is not possible as microchip does not
provide ARM compatible programming software.

Parts list:

 - 4 `drive units`_ each made up of

   * microgear motor 50:1 later swapped for similar `210\:1 microgear motors`_
   * `dfrobot chassis encoder`_
   * wheels
   * mounts

 - `Dagu`_ `4 channel motor controller`_ driver

 - `Raspberry Pi B+`_

   * with wifi adapter
   * with camera (unused)
   * with `prototype board`_

 - `Microstick`_ development board.

   * `PIC24HJ64GP502`_

 - `MPU-6050`_ 3 axis gyroscope accelerometer

 - 2.1Amp `USB battery`_

 - `sub-micro servo`_

.. _drive units: http://www.robotshop.com/en/miniq-motor-wheel-set-encoder.html#Specifications
.. _210\:1 microgear motors: https://www.pololu.com/product/1096
.. _dagu: https://sites.google.com/site/daguproducts/home/instruction-manuals
.. _dfrobot chassis encoder: http://www.dfrobot.com/index.php?route=product/product&product_id=823
.. _4 channel motor controller: https://docs.google.com/viewer?a=v&pid=explorer&chrome=true&srcid=0B__O096vyVYqYzBkOTA4ODAtMzdjZC00NThlLWFhMzUtMTFmNWYxN2FkZDli&hl=en_US
.. _microstick: http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=DM330013
.. _PIC24HJ64GP502: http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en534556
.. _Raspberry Pi B+: http://www.raspberrypi.org/products/model-b-plus/
.. _prototype board: http://www.amazon.com/gp/product/B00N1X5CM4/ref=oh_aui_detailpage_o05_s02?ie=UTF8&psc=1
.. _MPU-6050: http://www.i2cdevlib.com/devices/mpu6050#source
.. _USB battery: https://www.amazon.com/gp/product/B00P8ZU782/ref=oh_aui_detailpage_o05_s01?ie=UTF8&psc=1
.. _sub-micro servo: https://www.pololu.com/product/1053

Lessons learned from platform 1:

 - offloading motor control to pic is pretty good idea
 
   * Interrupt driven encoder feedback with painless time
     information from the input compare module is awesome!

   * straight forward PWM output with Output compare

   * Ran out of PWM output pins on pic, servo had to be
     driven from raspberry.
 
 - Opto encoders
 
   * are rather annoying. Requiring constant adjusting.

   * If there is anything positive about this set up is that
     the motor driver comes with encoder mixing circuitry. Halving
     number on pins needed on pic for encoder feedback (unless you
     care about direction)

   * Since each encoder is tuned separately getting accurate timing
     information is impossible. Averaging could have helped but with
     just 48 events per wheel rotation would have made any corrective
     action too slow.

   * Ended up using only 2 of the 4 encoders. With the assumption
     that the 2 wheels on the same side behaved similarly.

   * Using the phase offset of encoders to determine direction proved
     to be unnecessary

 - Drive system
 
   * 4 wheel drive skid steer is problematic in this application

     + Sideways friction is really high while cheezoid is attached
       to whiteboard easily 2-3x the force of gravity if its on
       horizontal surface

     + steering is not very accurate. We resorted to using the pen as
       a steering assist system. Push it down hard with the pen actuating
       servo to create a pivot point, and reduce wheel friction.

   * Gear reduced motors are quite awesome! Easy to change torque
     just swap the motor.

 - Board attachment

   * Generally the board attachment using magnets works quite well.
     In particular

     + Having many small magnets is much better as it allows to modify
       magnetic force and distribute it differently.

     + gluing metal pads to the chassis to which the magnets were attached
       is much better than gluing magnets to chassis. Made it possible to
       adjust magnets in seconds.


Overall the biggest problem with chassis 1 was the drive train, the lack of
precision in steering while attached to whiteboard made it impractical. All
the other negatives were just small annoyances.

Next platform will be improved by:

 - changing 4 wheel drive to 2 wheel drive with casters. The motors
   will be changed to higher power ones with even lower gear ratio.
   from 27 oz-in (108 oz-in total) at 60 RPM to 70 oz-in (140 oz-in
   total) at 100RPM. Biggest benefit will be to reduce friction
   during skid steer by not dragging the wheels sideways.

 - Slightly smaller, skinnier wheels. Should increase torque and
   reduce smudging.

 - swap opto encoders for hall-effect ones. The hall effect ones
   measure motor position rather than wheel position giving us
   many more events per wheel rotation. 48 events per wheel rotation
   to 7185 (most of these will be discarded by hardware and software
   tricks).

 - replace big 4 channel motor driver board for a smaller package.



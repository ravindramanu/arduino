LIDAR BOT OBJECTIVE:
--------------------

The LIDAR BOT is a simple obstacle avoiding robot that uses a inexpensive
LIDAR (Time-of-flight via LED reflection) sensor that makes its data available
over UART. The LIDAR BOT simply turns right in an attempt to avoid its obstacle
and keeps turning right until it has cleared its object. If no object lies ahead
the bot continues to move forward.

LIDAR BOT CONSTRUCTION:
-----------------------
Chassis       - Slotted Alloy
Wheels        - Semi-soft rubber (x4)
Powertrain    - +12 V, speed-controlled (PWM), Geared DC motors (x4)
DriveType     - Four Wheel Drive (4WD)
Powerplant    - +12 V Battery
Control Units - Arduino UNO REV3 (1), LN298 H-Bridge Controller (1)
Sensors       - TFMini LIDAR Sensor (UART)

PRINCIPLE OF OPERATION:
-----------------------
The LIDAR BOT is controlled by the Arduino UNO which constantly "sees" if there
is anything in front of it by estimating the distance between itself and an
obstacle that might be ahead. When an obstacle gets within 45 cm, the bot will
steer clear of the object by turning right. While the bot is capable of turning
left, this is just a simple lazy bot that does not want to think too much.

To start robot, place on level ground with a charged battery along with desired
obstacles. Flip switch on.

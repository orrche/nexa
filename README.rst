====
Nexa
====

Makefile needs export ARDUINOPATH to be set to your
arduino install i.e.::

  export ARDUINOPATH=~/Downloads/arduino-1.8.5


Build
=====

::

 make


Program
=======

::

 make install


Usage
=====

To use start screen::

 screen /dev/ttyUSB0 9600

Send commands i.e. to turn on A1 switch::

 a0007

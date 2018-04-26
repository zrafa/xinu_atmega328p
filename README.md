# xinu_atmega328p
xinu OS port for avr atmega328p

Original sources:  https://xinu.cs.purdue.edu/files/Xinu-code-MIPS.tar.gz


Como testear
============

```
# git clone https://github.com/zrafa/xinu_atmega328p.git
# cd xinu_atmega328p/apps/platform/atmega328p/step04/
# make clean ; make libclean
# make COMPILER_ROOT=avr-
# ./dude.sh main.hex
# sudo minicom -D /dev/ttyACM0 

```

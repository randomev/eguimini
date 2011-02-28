

avrdude -c stk500v2    -p m1281 -P /dev/tty.usbserial-A7005tux -U lfuse:w:0xfd:m -U hfuse:w:0x84:m -U efuse:w:0xfe:m

avrdude -c stk500v2    -p m1281 -P /dev/tty.usbserial-A7005tux -v


avrdude: Version 5.6, compiled on Apr 15 2009 at 17:54:59
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/

         System wide configuration file is "/usr/local/CrossPack-AVR-20090415/etc/avrdude.conf"
         User configuration file is "/Users/henkka/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : /dev/tty.usbserial-A7005tux
         Using Programmer              : stk500v2
         AVR Part                      : ATMEGA1281
         Chip Erase delay              : 9000 us
         PAGEL                         : PD7
         BS2                           : PA0
         RESET disposition             : dedicated
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :

                                  Block Poll               Page                       Polled
           Memory Type Mode Delay Size  Indx Paged  Size   Size #Pages MinW  MaxW   ReadBack
           ----------- ---- ----- ----- ---- ------ ------ ---- ------ ----- ----- ---------
           eeprom        65    10     8    0 no       4096    8      0  9000  9000 0x00 0x00
           flash         65    10   256    0 yes    131072  256    512  4500  4500 0x00 0x00
           lfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           hfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           efuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           lock           0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           calibration    0     0     0    0 no          1    0      0     0     0 0x00 0x00
           signature      0     0     0    0 no          3    0      0     0     0 0x00 0x00

         Programmer Type : STK500V2
         Description     : Atmel STK500 Version 2.x firmware
         Programmer Model: AVRISP
         Hardware Version: 2
         Firmware Version Master : 2.10
         Vtarget         : 3.2 V
         SCK period      : 0.1 us

avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.10s

avrdude: Device signature = 0x1e9704
avrdude: safemode: lfuse reads as FD
avrdude: safemode: hfuse reads as 9C
avrdude: safemode: efuse reads as F6

avrdude: safemode: lfuse reads as FD
avrdude: safemode: hfuse reads as 9C
avrdude: safemode: efuse reads as F6
avrdude: safemode: Fuses OK

avrdude done.  Thank you.


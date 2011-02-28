avrdude -p atmega1281 -P com1 -c stk500v2 -U flash:r:db101_dump.bin:r
rem pause
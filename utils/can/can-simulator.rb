#!/usr/bin/ruby

#
# Simulates some CAN bus values
#
# Licenced under LGPL
# 
# 2010 (c) henry.palonen@gmail.com
#
# Revision log
#
# 18.7.2010 start
#
# HeP: Henry Palonen, h@yty.net
#
# Normal startup should be something like this;
#
# ./can-simulator.rb /dev/tty.YOUR_OWN_SERIALPORT
#

require "rubygems"
require "serialport"
require "rubygems"
require 'pp'

require 'rubygems'
require 'dm-core'

def send_nowait(sp, msg)
	sp.write(msg + "\r")
end

def send(sp, msg)
	sp.write(msg + "\r")
	#c = sp.gets("\n")
	c = sp.gets("\r")
	puts c
	return c
end

if ARGV.size < 1
  STDERR.print <<EOF
  Usage: ruby #{$0} port_name 
  #bps nbits stopb
EOF
  exit(1)
end

puts "Opening serial port..."
# 115200
#sp = SerialPort.new(ARGV[0].to_s, 115200, 8, 1, SerialPort::NONE)
#sp = SerialPort.new(ARGV[0].to_s, 2400 , 8, 1, SerialPort::NONE)
#sp = SerialPort.new(ARGV[0].to_s, 115200, 8, 1, SerialPort::NONE)
sp = SerialPort.new(ARGV[0].to_s, 57600, 8, 1, SerialPort::NONE)

puts "OK"

#open("/dev/tty", "r+") { |tty|
#open("/dev/ttys000", "r+") { |tty|
#  tty.sync = true

puts "Initializing"
send(sp,"\r")	# flush the FIFO	
send(sp,"\r")	# flush the FIFO	
puts "Querying version information"
send(sp,"V\r")		# version number
puts "Querying errors"
send(sp,"F\r")		# version number
send_nowait(sp,"C")	# closing the CAN-bus if active
puts "Setting speed to 500 Kbit ..."
send(sp,"S6")
puts "Opening CAN bus ..."
send(sp,"O")

#V
#S6
#O
#t10021133
  soc = 0
  puts("Simulating values ...\n")
  volt=1700
  temp=1
  while (1)
      # endless loop
	soc = soc + 1
	if (soc>200)
		soc = 0
	end
	soc_s = "%02x" % soc

        volt = volt + 1
        if (volt>4200)
                volt = 1700
        end
        temp = temp + 1
        temp = temp.to_i
        if (temp>65)
                temp = 1
        end

        soc_s = "%02x" % soc
        temp_s = "%02x" % temp

        volt_s = "%04x" % volt

        # str = "t6301" + soc_s + "64C8130CD60CEE"
        str = "t6308" + soc_s + "00" + "99" + temp_s +  volt_s + "FFFF"
        # "64C8130CD60CEE"

        puts "Sending V: " + volt.to_s + " T: " + temp.to_s + " SOC: " + soc.to_s
        #+ str 
 	puts str
	send(sp, str)
	#send(sp,"t6308" + soc_s + "64C8130CD60CEE")
	#send(sp,"t6408" + soc_s + "64C8130CD60CEE")
	#send(sp,"t6508" + soc_s + "64C8130CD60CEE")
	#send(sp,soc_s + soc_s + "\r\n")
	#puts soc_s
	sleep 0.1;
  end
#}

puts("CLOSING...")
sp.close

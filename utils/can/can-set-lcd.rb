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

if ARGV.size < 6
  STDERR.print <<EOF
  Usage: ruby #{$0} port_name contrast red green blue intensity
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
contrast = ARGV[1].to_s
red = ARGV[2].to_s
green = ARGV[3].to_s
blue = ARGV[4].to_s
intensity = ARGV[5].to_s
str = "t7DD5" + contrast + red + green + blue + intensity
#+ str 
puts str
send(sp, str)

puts("CLOSING...")
sp.close

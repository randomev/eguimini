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
sp = SerialPort.new(ARGV[0].to_s, 57600, 8, 1, SerialPort::NONE)

puts "OK"

#open("/dev/tty", "r+") { |tty|
#open("/dev/ttys000", "r+") { |tty|
#  tty.sync = true

puts "Initializing"
send_nowait(sp,"\r")	# flush the FIFO	
send_nowait(sp,"\r")	# flush the FIFO	
puts "Querying version information"
send(sp,"V\r")		# version number
send_nowait(sp,"C")     # closing the CAN-bus if active

puts "Setting auto transmit (X1)..."
send(sp,"X1")

puts "Setting speed to 500 Kbit ..."
send(sp,"S6")

puts "Setting acceptance mask to all frames..."
send(sp,"M00000000")
send(sp,"mFFFFFFFF")

puts "Opening CAN-channel for configuring..."
send(sp,"O")

puts "Configuring for Auto Startup = ON and Read & Write mode (Q1)..."
send(sp,"Q1")

puts "OK. Power-cycle CAN-adapter and verify visually that yellow led stays on after startup..."

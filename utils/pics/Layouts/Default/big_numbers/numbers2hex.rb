#!/usr/bin/ruby1.8
#
#	eCars-Now! image converter for eGUI mini
#
#	Converter for PNG images
#	It reads a batch of PNG images and converts them to hex-format that
#	is read by Atmel DB101 proto-board
#
#	Licenced under LGPL
#	
#	(c) Henry Palonen <h@yty.net> 2010
#
# Number images should be 32 x 32 px PNG's

require "rubygems"
require "RMagick"
include Magick


def png2hex(filename)

	img = ImageList.new(filename)

	i = 0
	s = Array.new()
	n = Array.new()
	pic = ''
	line = ''
	currentrow = 0
	# r = 0 -> 63
	# c = 0 -> 127

	# 1. scan first column pixels 0-7 from top (row 0) to bottom (row 7), and convert this value to hex
	# 2. scan second column, pixels 0-7 and convert this to hex ...
	# 3. after all columns are scanned, start again from column 0 and from row 8

	puts "/* #{filename} */\n"
	puts "/* Size " << img.rows.to_s << "x" << img.columns.to_s << "px */\n"
	puts ""

	defname = filename.gsub(".png","")
	puts "uint8_t const CAL_PGM_DEF(FLASHPICS_#{defname}[]) = {"

	1.step(img.rows,8).each do |r_plus|
		(1..img.columns).each do |c|
			7.downto(0).each do |r|
				r_pos = r + r_plus
				
				# sanity check
				if r_pos > img.rows
					exit
				end

				raw = img.export_pixels(c,r_pos,1,1,"R")

				if (raw[0] == 65535)
					pic << "0"
				else
					pic << "1"
				end
			end
			# convert binary to hex
			a = pic.to_i(2).to_s(16)
			# leading zero
			if a.length < 2
				a = "0" << a
			end

			line << "0x" << a << ","
			pic = ''
		end
		# output one value
		puts line
		line = ''

	end
	puts "};\n\n"
end

# output definitions for header files
#puts "/* copy this to ../../../production_demo_rev_A/flashpics.h"

(0..9).each do |i|
	filename = i.to_s
	puts "extern uint8_t const CAL_PGM(FLASHPICS_#{filename}[]);"
end

# output actual data
#puts "\n\n/* copy this to ../../../production_demo_rev_A/flashpics.c"

# convert each file one by one
(0..9).each do |i|
	filename = i.to_s + ".png"
	png2hex(filename)
end

png2hex("X.png")
png2hex("minus.png")
png2hex("plus.png")
png2hex("pros.png")
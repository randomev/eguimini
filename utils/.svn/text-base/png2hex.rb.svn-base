#!/usr/bin/ruby1.8

require "rubygems"
require "RMagick"
include Magick

#img = ImageList.new("input.png")
#img = ImageList.new("eCarsLogo128x64px.png")
img = ImageList.new("eGUImini.png")

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

0.step(img.rows,8).each do |r_plus|
	(0..img.columns).each do |c|
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

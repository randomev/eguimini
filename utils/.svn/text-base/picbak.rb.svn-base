#!/usr/bin/ruby1.8

require "rubygems"
require "RMagick"
include Magick

#img = ImageList.new("input.png")
img = ImageList.new("eCarsLogo128x64px.png")

i = 0
s = Array.new()
n = Array.new()
pic = ''

# r = 0 -> 63
# c = 0 -> 127

img.each_pixel { |pixel, c, r|
	#puts "C=" << c.to_s << "R=" << r.to_s
	s[c] = ''
	n[c] = 0
}

img.each_pixel { |pixel, c, r|

	if pixel.red.to_i == 65535
		pic << "0"
	else
		pic << "1"
	end

	if c == 127
		puts pic
		pic = ''
	end
}


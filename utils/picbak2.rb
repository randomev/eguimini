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
ra = ''
# r = 0 -> 63
# c = 0 -> 127

(0..img.rows).each do |r|
	(0..img.columns).each do |c|
		raw = img.export_pixels(c,r,1,1,"R")
		if (raw[0] == 65535)
			pic << "0"
		else
			pic << "1"
		end

		if (c==127)
			puts pic
			pic = ''
		end	
	end
end

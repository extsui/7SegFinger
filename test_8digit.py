# -*- coding: utf-8 -*-

import spidev
import math

def reverse_bit_order(x):
	x_reversed = 0x00
	if (x & 0x80):
		x_reversed |= 0x01
	if (x & 0x40):
		x_reversed |= 0x02
	if (x & 0x20):
		x_reversed |= 0x04
	if (x & 0x10):
		x_reversed |= 0x08
	if (x & 0x08):
		x_reversed |= 0x10
	if (x & 0x04):
		x_reversed |= 0x20
	if (x & 0x02):
		x_reversed |= 0x40
	if (x & 0x01):
		x_reversed |= 0x80
	return x_reversed
	
	
	
	"""
	x = (((x & 0x55) << 1) | ((x & 0xAA) >> 1)) % 0xFF
	x = (((x & 0x33) << 2) | ((x & 0xCC) >> 2)) % 0xFF
	return ((x << 4) | (x >> 4)) % 0xFF
	"""

def calc_checksum(array):
	sum = 0
	for item in array:
		sum += item
	sum %= 256
	return (0xFF - sum) % 256

spi = spidev.SpiDev()
spi.open(0, 0)

#
# CS+ データ送受信タイミング設定 タイプ1
#
# SCK:				￣￣|＿|￣|＿|￣|＿|￣...
# SOp: 末尾ビット→		 <D7>  <D6>	 <D5> ... → 先頭ビット
#
spi.mode = 0

#
# SPIのクロック周波数
# ・500kHz: デフォルト
# ・1MHz:	OK
# ・2MHz:	NG(データ化け発生)
# ⇒1MHzを設定。
#
spi.max_speed_hz = 1000000

#
#
#
data = [ 0x60, 0xDA, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, ]
brightness = [ 100, 100, 100, 100, 100, 100, 100, 100, ]

# 1フレーム作成
xfer_data = [ 0x01 ]	# タイプ=表示データ
xfer_data.extend(brightness)	# 表示データ部
checksum = calc_checksum(xfer_data)
xfer_data.append(checksum)

print xfer_data

#
# RaspberryPiはMSBFirstでしかデータを送信できない。
# (spi.lsbfirstメンバがあるが、Read-Only)
# ⇒送信前にビットを逆転する必要がある。
# [参考URL] http://tightdev.net/SpiDev_Doc.pdf
#
xfer_data = map(reverse_bit_order, xfer_data)

print xfer_data

# フレーム送信
spi.writebytes(xfer_data)

import os
os.system('sleep 1')


num_to_pattern = [
	0xfc, # 0
	0x60, # 1
	0xda, # 2
	0xf2, # 3
	0x66, # 4
	0xb6, # 5
	0xbe, # 6
	0xe4, # 7
	0xfe, # 8
	0xf6, # 9
]



rad = 0.0
while (True):
		xfer_data = [ 0x01 ]
		brightness[0] = int(math.sin(rad + math.pi/8*0) * 50 + 50)
		brightness[1] = int(math.sin(rad + math.pi/8*1) * 50 + 50)
		brightness[2] = int(math.sin(rad + math.pi/8*2) * 50 + 50)
		brightness[3] = int(math.sin(rad + math.pi/8*3) * 50 + 50)
		brightness[4] = int(math.sin(rad + math.pi/8*4) * 50 + 50)
		brightness[5] = int(math.sin(rad + math.pi/8*5) * 50 + 50)
		brightness[6] = int(math.sin(rad + math.pi/8*6) * 50 + 50)
		brightness[7] = int(math.sin(rad + math.pi/8*7) * 50 + 50)
		xfer_data.extend(brightness)
		checksum = calc_checksum(xfer_data)
		xfer_data.append(checksum)
		xfer_data = map(reverse_bit_order, xfer_data)
		spi.writebytes(xfer_data)
		os.system('sleep 0.001')
		rad += 0.05 * 2
		
		import datetime as dt
		now = dt.datetime.now()
		
		xfer_data = [ 0x00 ]
#		data[0] = num_to_pattern[now.year  / 1000 % 10]
#		data[1] = num_to_pattern[now.year  / 100  % 10]
#		data[2] = num_to_pattern[now.year  / 10   % 10]
#		data[3] = num_to_pattern[now.year  / 1    % 10]
#		data[4] = num_to_pattern[now.month / 10   % 10]
#		data[5] = num_to_pattern[now.month / 1    % 10]
#		data[6] = num_to_pattern[now.day   / 10   % 10]
#		data[7] = num_to_pattern[now.day   / 1    % 10]

		
		data[0] = num_to_pattern[now.hour / 10  % 10]
		data[1] = num_to_pattern[now.hour / 1  % 10]
		
		if (now.microsecond < 500*1000):
			data[1] |= 0x01;
		
		data[2] = num_to_pattern[now.minute / 10  % 10]
		data[3] = num_to_pattern[now.minute / 1  % 10]

		if (now.microsecond < 500*1000):
			data[3] |= 0x01;

		data[4] = num_to_pattern[now.second / 10   % 10]
		data[5] = num_to_pattern[now.second / 1    % 10]

		if (now.microsecond < 500*1000):
			data[5] |= 0x01;

		data[6] = num_to_pattern[now.microsecond / 100000  % 10]
		data[7] = num_to_pattern[now.microsecond / 10000  % 10]

#		data[4] = num_to_pattern[now.microsecond / 1000  % 10]
#		data[5] = num_to_pattern[now.microsecond / 100  % 10]
#		data[6] = num_to_pattern[now.microsecond / 10  % 10]
#		data[7] = num_to_pattern[now.microsecond / 1  % 10]

		xfer_data.extend(data)
		checksum = calc_checksum(xfer_data)
		xfer_data.append(checksum)
		xfer_data = map(reverse_bit_order, xfer_data)
		spi.writebytes(xfer_data)

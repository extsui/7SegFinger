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
#spi.max_speed_hz = 500000

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

# LATCHピンの指定
# GPIO21(40番ピン)を使用する。
import RPi.GPIO as GPIO

# ピン番号ではなく、機能名(例:GPIO21)で指定できるようにする。
GPIO.setmode(GPIO.BCM)

# 出力設定
GPIO.setup(21, GPIO.OUT)


xfer_data = [ 0x01 ]
brightness[0] = 100
brightness[1] = 100
brightness[2] = 100
brightness[3] = 100
brightness[4] = 100
brightness[5] = 100
brightness[6] = 100
brightness[7] = 100
xfer_data.extend(brightness)
checksum = calc_checksum(xfer_data)
xfer_data.append(checksum)
xfer_data = map(reverse_bit_order, xfer_data)
spi.writebytes(xfer_data)

xfer_data = [ 0x00 ]
data[0] = 0xFF
data[1] = 0xFF
data[2] = 0xFF
data[3] = 0xFF
data[4] = 0xFF
data[5] = 0xFF
data[6] = 0xFF
data[7] = 0xFF
xfer_data.extend(data)
checksum = calc_checksum(xfer_data)
xfer_data.append(checksum)
xfer_data = map(reverse_bit_order, xfer_data)
spi.writebytes(xfer_data)



# 表示更新(LATCH↑↓)
GPIO.output(21, GPIO.HIGH)
GPIO.output(21, GPIO.LOW)



while (True):
	xfer_data = [ 0x01 ]
	data[0] = 0x00
	data[1] = 0x00
	data[2] = 0x00
	data[3] = 0x00
	data[4] = 0x00
	data[5] = 0x00
	data[6] = 0x00
	data[7] = 0x00
	xfer_data.extend(data)
	checksum = calc_checksum(xfer_data)
	xfer_data.append(checksum)
	xfer_data = map(reverse_bit_order, xfer_data)
	spi.writebytes(xfer_data)
	
	# LATCH更新はしない。
	# データ通信がどのくらい表示に影響するかを調査する。
	
	import time
	# 0.016=60fpsでも、数秒に1回くらいちらつく。糞すぎ。
	# ⇒1990usから1900usにしたらチラつかなくなった。大勝利！
	#time.sleep(0.016)

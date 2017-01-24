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
# SCK:              ￣￣|＿|￣|＿|￣|＿|￣...
# SOp: 末尾ビット→      <D7>  <D6>  <D5> ... → 先頭ビット
#
spi.mode = 0

#
# SPIのクロック周波数
# ・500kHz: デフォルト
# ・1MHz:   OK
# ・2MHz:   NG(データ化け発生)
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

rad = 0.0
while (True):
        xfer_data = [ 0x01 ]
        brightness = int(math.sin(rad) * 50 + 50)
        print brightness
        xfer_data.extend([ brightness ] * 8)
        checksum = calc_checksum(xfer_data)
        xfer_data.append(checksum)
        xfer_data = map(reverse_bit_order, xfer_data)
        spi.writebytes(xfer_data)
        os.system('sleep 0.001')
        rad += 0.05

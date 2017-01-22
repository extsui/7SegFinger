# -*- coding: utf-8 -*-
import spidev

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

def calc_checksum(array):
	sum = 0
	for item in array:
		sum += item
	sum %= 256
	return (0xFF - sum) % 256

spi = spidev.SpiDev()
spi.open(0, 0)

#
# CS+ データ送受信タイミング設定 タイプ4
#
# SCK:              ＿＿|￣|＿|￣|＿|￣|＿...
# SDO: 末尾ビット→    <D7>  <D6>  <D5>   ... → 先頭ビット
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
# テストデータ('1', '2', ... ,'8')
#
data = [ 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, ]
brightness = [ 100, 100, 100, 100, 100, 100, 100, 100, ]

# 1フレーム作成
xfer_data = [ 0x00 ]	# タイプ=表示データ
xfer_data.extend(data)	# 表示データ部
checksum = calc_checksum(xfer_data)
xfer_data.append(checksum)

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

#
# 輝度送信
#
xfer_data = [ 0x01 ]	# タイプ=輝度データ
xfer_data.extend(brightness)
checksum = calc_checksum(xfer_data)
xfer_data.append(checksum)
xfer_data = map(reverse_bit_order, xfer_data)
print xfer_data
spi.writebytes(xfer_data)

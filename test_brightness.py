# -*- coding: utf-8 -*-
import spidev
import argparse

#
# RaspberryPiはMSBFirstでしかデータを送信できない。
# (spi.lsbfirstメンバがあるが、Read-Only)
# ⇒送信前にビットを逆転する必要がある。
# [参考URL] http://tightdev.net/SpiDev_Doc.pdf
#
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

#
# 表示データ設定
#
def set_data(spi, data):
	xfer_data = [ 0x00 ]	# タイプ=表示データ
	xfer_data.extend(data)	# 表示データ部
	checksum = calc_checksum(xfer_data)
	xfer_data.append(checksum)
	xfer_data = map(reverse_bit_order, xfer_data)
	spi.writebytes(xfer_data)

#
# 輝度設定
#
def set_brightness(spi, brightness):
	xfer_data = [ 0x01 ]	# タイプ=輝度データ
	xfer_data.extend(brightness)
	checksum = calc_checksum(xfer_data)
	xfer_data.append(checksum)
	xfer_data = map(reverse_bit_order, xfer_data)
	spi.writebytes(xfer_data)

#
# コマンドラインから輝度情報を取得して表示する。
# $ python test_brightness.py <輝度データ(0-100)>
#
if __name__ == '__main__':
	
	parser = argparse.ArgumentParser()
	parser.add_argument("brightness",
						help="7SegFinger brightness test",
						type=int)
	args = parser.parse_args()
	brightness_value = args.brightness
	print("brightness_value = %d" % brightness_value)
	
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
	# テスト用表示データ(全点灯)
	#
	data = [ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ]
	set_data(spi, data)
	
	#
	# 全部同じ値の輝度データを設定
	#
	
	
	#brightness = [ brightness_value ] * 8
	brightness = [ 0, brightness_value, 0, 0, 0, 0, 0, 0 ]
	
	set_brightness(spi, brightness)
	
	spi.close()

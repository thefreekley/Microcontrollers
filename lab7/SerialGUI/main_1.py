from functools import reduce
import threading
import sys
from PyQt5.QtWidgets import QApplication, QTableWidgetItem
from PyQt5 import uic,QtGui,QtCore
from PyQt5.QtGui import *
from PyQt5.QtGui import QPalette, QColor
from PyQt5.QtCore import QThread,QTimer
from PyQt5 import QtWidgets
import time
import os
import ui_py
import serial
import crc8


ser = ''



CRC_BIT_LENGTH = 8
POLY = 0x7
INIT_CRC = 0x0
XOR_OUT = 0x0
REF_IN = False
REF_OUT = False


roll_text = ''

selected_slave = 0

received = ''

ser = serial.Serial("COM7", 4600, bytesize=8, parity='N', stopbits=1, timeout=0)


if getattr(sys, 'frozen', False):
    os.chdir(sys._MEIPASS)

class Sensors(QThread):

    def __init__(self):
        QThread.__init__(self)




    def run(self):

        while True:
            pass 








class App(QtWidgets.QMainWindow, ui_py.Ui_MainWindow):
    def __init__(self):
        super().__init__()

        self.sensor_detect = Sensors()
        self.sensor_detect.start()

        self.setupUi(self)
        self.start()

        self.timer = QTimer()
        self.timer.timeout.connect(self.set_info)
        self.timer.start(0.1)

    def set_info(self):
        check_serial()


    def start(self):

        self.setWindowTitle("lab6")
        self.show()

        self.pushButton_2.clicked.connect(lambda: slave_1())
        self.pushButton_3.clicked.connect(lambda: slave_2())

def slave_1():
    global selected_slave,roll_text
    roll_text = ''
    ser.write(b'1')
    selected_slave = 1

def slave_2():
    global selected_slave,roll_text
    roll_text = ''
    ser.write(b'2')
    selected_slave = 2



def check_serial():
    global selected_slave, received,roll_text
    if ser:
        data = str(ser.read())[2:-1]
        if data:

            if received != '':
                received += data
            elif data == "#" and received == '':
                received += data
            print(data)
            if received.endswith("*"):
                received = received[1:-1]
                received = received.split(';')

                crc = int("0x" + received[0], 16)

                result = check_crc(received[1], crc, CRC_BIT_LENGTH, POLY, INIT_CRC, REF_IN, REF_OUT, XOR_OUT)

                roll_text = str(roll_text) + received[1]

                if selected_slave==1:
                    ex.label.setText(roll_text + '\n' + str(result))
                elif selected_slave==2:
                    ex.label_2.setText(roll_text + '\n' + str(result))



                received = ''


def check_crc(data, crc, crc_bit_length, poly, init_crc, in_ref, res_ref, final_xor):


    if isinstance(data, str):

        data = convert_ascii_str_to_int_list(data)



    poly_msb = 1 << (crc_bit_length - 1)
    cutter = (1 << crc_bit_length) - 1

    new_crc = init_crc



    for i in range(len(data)):
        temp = data[i]
        if in_ref:
            temp = reverse_bits(temp, 8)
        temp <<= crc_bit_length - 8
        new_crc ^= temp

        for _ in range(8):
            if (new_crc & poly_msb) != 0:
                new_crc = (new_crc << 1) ^ poly
            else:
                new_crc <<= 1
        new_crc &= cutter

    if res_ref:
        new_crc = reverse_bits(new_crc, crc_bit_length)
    new_crc ^= final_xor
    print(new_crc,crc)
    return new_crc == crc


def reverse_bits(number, bits_quantity=None):
    number = bin(number)[2:]

    if bits_quantity is not None:
        return int(''.join(number[::-1] + '0' * (bits_quantity - len(number))), 2)
    return int(''.join(number[::-1]), 2)


def convert_ascii_str_to_int_list(data):
    return [convert_ascii_str_to_int(i) for i in data]


def convert_ascii_str_to_int(data):

    return reduce(lambda x, y: x | y, [ord(data[::-1][i]) * ((2 ** 8) ** i) for i in range(len(data))])




if __name__ == '__main__':



    app = QApplication(sys.argv)

    app.setStyle("Fusion")

    # Now use a palette to switch to dark colors:

    palette = QPalette()
    palette.setColor(QPalette.Window, QColor(53, 53, 53))

    palette.setColor(QPalette.Base, QColor(25, 25, 25))
    palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
    palette.setColor(QPalette.ToolTipBase, QColor(0, 0, 0))
    palette.setColor(QPalette.ToolTipText, QColor(255, 255, 255))
    palette.setColor(QPalette.Text, QColor(255, 255, 255))
    palette.setColor(QPalette.Button, QColor(53, 53, 53))
    palette.setColor(QPalette.ButtonText, QColor(255, 255, 255))
    palette.setColor(QPalette.BrightText, QColor(255, 0, 0))
    palette.setColor(QPalette.Link, QColor(42, 130, 218))
    palette.setColor(QPalette.Highlight, QColor(100, 0, 0))
    palette.setColor(QPalette.HighlightedText, QColor(0, 0, 0))
    app.setPalette(palette)


    ex = App()
    app.exec_()
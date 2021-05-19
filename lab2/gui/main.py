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
import datetime
import sqlite3
import comp


from PyQt5 import QtTest

ser = ''

serial_ports = []
current_serial_port = ""



ser = serial.Serial("COM29", 9600, bytesize=8, parity='N', stopbits=1, timeout=0)


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
        if ser:
            s = str(ser.readline())
            if s.find("1") > 0:
                ex.algorithm_1(0)
            elif s.find("2") > 0:
                ex.algorithm_2(0)

    def start(self):

        self.setWindowTitle("Smart House")
        self.show()
        self.comboBox.currentIndexChanged.connect(lambda: self.set_combobox_ser(self.comboBox.currentText()))

        self.pushButton.clicked.connect(lambda: self.update_serial_ports())
        self.pushButton_2.clicked.connect(lambda: ser.write(str(chr(1)).encode('ascii')))
        self.pushButton_3.clicked.connect(lambda: ser.write(str(chr(2)).encode('ascii')))


    def algorithm_1(self,mode=1):
        if ser:
            for i in range(6):
                arr_led = [0] * 8
                if i != 5:
                    arr_led[i] = 1
                    arr_led[7 - i] = 1
                self.show_led(arr_led)
                QtTest.QTest.qWait(800 + 100*mode)





    def algorithm_2(self,mode=1):
        if ser:
            for i in range(7,0,-2):
                arr_led = [0] * 8
                arr_led[i]=1
                self.show_led(arr_led)
                QtTest.QTest.qWait(800 + 100*mode)

            QtTest.QTest.qWait(800 + 100*mode)

            for i in range(6,-1,-2):
                arr_led = [0] * 8
                arr_led[i]=1
                self.show_led(arr_led)
                QtTest.QTest.qWait(800 + 100*mode)

            self.show_led([0] * 8)



    def show_led(self,arr):
        for i in range(8):
            self.set_led(i,arr[i])


    def set_led(self,number,mode):
        color = "black"
        if mode ==1:
            color = "red"

        if number is 7:
            self.line.setStyleSheet("background-color:"+ color +";")
        elif number is 6:
            self.line_2.setStyleSheet("background-color:"+ color +";")
        elif number is 5:
            self.line_3.setStyleSheet("background-color:" + color + ";")
        elif number is 4:
            self.line_4.setStyleSheet("background-color:" + color + ";")
        elif number is 3:
            self.line_5.setStyleSheet("background-color:" + color + ";")
        elif number is 2:
            self.line_6.setStyleSheet("background-color:" + color + ";")
        elif number is 1:
            self.line_7.setStyleSheet("background-color:" + color + ";")
        elif number is 0:
            self.line_8.setStyleSheet("background-color:" + color + ";")




    def set_combobox_ser(self,newText):

        global current_serial_port
        global ser
        current_serial_port = newText

        try:
            pass
            # ser = serial.Serial(current_serial_port, 9600)
        except serial.SerialException:
            print
            "failed to write to port %s" % current_serial_port
            sys.exit()

    def update_serial_ports(self):
        global serial_ports
        global current_serial_port
        serial_ports = comp.serial_ports()
        self.comboBox.clear()
        for port in serial_ports:
            self.comboBox.addItem(port)



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
    ex.update_serial_ports()
    app.exec_()
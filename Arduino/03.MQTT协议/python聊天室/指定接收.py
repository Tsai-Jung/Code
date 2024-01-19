import sys
import urllib
import urllib.request
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import paho.mqtt.client as mqtt
import time,os,requests,json,threading
import random

broker = 'test.ranye-iot.net'
#broker = 'test.mosquitto.org'
port = 1883
client_id = 'one_dufghkdfiv'+str(random.random())


Onesub_topic="one_dlifgjigr"                            #订阅主题
Twopub_topic="two_dofkosdkf"                            #发布主题
sub_topic=Onesub_topic                            #订阅主题
pub_topic=Twopub_topic                           #发布主题

client = mqtt.Client(client_id)
def on_connect(client, userdata, flags, rc):
    print("Connected with result code: " + str(rc))
    
intext=""
a=""
apro=""
stext=""
rec="off"
switch="收发机一"
def on_message(client, userdata, msg):
    global a,rec,apro
    print(str(msg.payload.decode()))   #msg.topic
    a=msg.payload.decode()
    if(a==apro):
        rec="off"
    else:
        rec="on"
    apro=a

def mqtt_subscribe():
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(broker, port, 60) # 600为keepalive的时间间隔
    client.subscribe(sub_topic, qos=0) #订阅的地方
    client.loop_start()           # 守护连接状态
 
class Windows(QWidget):
    global s
    def __init__(self):
        super().__init__()

        self.initUI()


    def initUI(self):

        self.label= QLabel('通讯密钥:', self)
        self.okButton2 = QPushButton("确定")
        self.okButton2.clicked.connect(self.buttonClicked2)

        self.label2= QLabel('连接主机:', self)

        self.label3= QLabel('发送端:', self)
        self.label4= QLabel('接收端:', self)
        #文字輸入使用
        self.inputText = QLineEdit('',self)
        self.inputText4 = QLineEdit('',self)

        #下拉選項匡
        combo = QComboBox(self)
        combo.addItem("收发机一")
        combo.addItem("收发机二")
        combo.activated[str].connect(self.onActivated)
        
        #時間控鍵使用
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.timecircul)
        self.timer.start(1000) #只能設定一次 3000等於3秒鐘

        #文字標籤使用
        self.inputText2 = QTextEdit('',self)
        self.inputText3 = QTextEdit('',self)
        
        #按鍵使用
        self.okButton = QPushButton("送出")
        self.okButton.clicked.connect(self.buttonClicked)

        
        #建立水平的框架
        hbox0 = QHBoxLayout()
        hbox0.addWidget(self.label)
        hbox0.addWidget(self.inputText4)
        hbox0.addWidget(combo)
        hbox0.addWidget(self.okButton2)

        hbox01 = QHBoxLayout()
        hbox01.addWidget(self.label2)

        hbox02 = QHBoxLayout()
        hbox02.addWidget(self.label3)
        hbox02.addWidget(self.label4)
        
        hbox = QHBoxLayout()
        hbox.addWidget(self.inputText)
        hbox.addWidget(self.okButton)

        #建立水平的框架
        hbox2 = QHBoxLayout()
        hbox2.addWidget(self.inputText2)
        hbox2.addWidget(self.inputText3)
        
        #建立垂直的框架
        vbox = QVBoxLayout()
        vbox.addLayout(hbox0)
        vbox.addLayout(hbox01)
        vbox.addLayout(hbox) #放入框架
        vbox.addLayout(hbox02)
        vbox.addLayout(hbox2) #放入框架
        #vbox.setStretch(1,1)
        
        
        #視窗設定
        self.setLayout(vbox) #顯示框架
        self.setGeometry(100, 100, 700, 500) #起始位置
        self.setWindowTitle('收发机')  #修改標題
        self.setWindowIcon(QIcon('kitty.jpg'))#修改Icon
        self.setWindowFlags(Qt.WindowStaysOnTopHint)#視窗最上層
        
        self.show() #視窗顯示

    def buttonClicked(self):
        global intext
        print('按鍵按下')
        self.inputText2.setPlainText(self.inputText.text()+"\n"+intext)
        intext=self.inputText.text()+"\n"+intext
        client.publish(pub_topic,self.inputText.text(), qos=1, retain=True)
        self.inputText.setText('')

    def buttonClicked2(self):
        global sub_topic,pub_topic,switch
        sub_topic=sub_topic+self.inputText4.text()
        pub_topic=pub_topic+self.inputText4.text()
        client.loop_stop()
        mqtt_subscribe()
        print(switch)
        self.okButton2.setText('已經连接')
        self.label2.setText("连接主机:"+switch+self.inputText4.text())

    def onActivated(self, text):
        global sub_topic,pub_topic,Onesub_topic,Twopub_topic,switch
        if(text=="收发机一"):
            print("收发机一")
            sub_topic=Onesub_topic                         
            pub_topic=Twopub_topic
            switch="收发机一"
            
            
        if(text=="收发机二"):
            print("收发机二")
            sub_topic=Twopub_topic                          
            pub_topic=Onesub_topic
            switch="收发机二"

        print("p="+pub_topic+"  s="+sub_topic)
        
    def keyPressEvent(self, QKeyEvent):
        global intext
        if QKeyEvent.key() == Qt.Key_Return:
            self.inputText2.setText(self.inputText.text()+"\n"+intext)
            intext=self.inputText.text()+"\n"+intext
            client.publish(pub_topic,self.inputText.text(), qos=1, retain=True)
            self.inputText.setText('')            
    def timecircul(self):
        global a,apro,stext,rec
        self.timer.stop()
        #print('時間暫停')
        #print('這裡寫想做的事情')
        if(rec=="on"):
            self.inputText3.setText(a+"\n"+stext)
            stext = a+"\n"+stext
            rec="off"
        self.timer.start()
        #print('開始計時')

    def closeEvent(self, event):
        reply = QtWidgets.QMessageBox.question(self,'提示',"确认退出吗？",QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No,QtWidgets.QMessageBox.No)
        if reply == QtWidgets.QMessageBox.Yes:
            event.accept()
        #用过sys.exit(0)和sys.exit(app.exec_())，但没起效果
            os._exit(0)
        else:
            event.ignore()

def mainprogram():
    app = QApplication(sys.argv)
    ex = Windows()
    sys.exit(app.exec_())

    
if __name__ == '__main__':
    mainprogram()

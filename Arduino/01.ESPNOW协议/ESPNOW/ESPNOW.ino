#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Ticker.h>

//EspNow数据设定
//接收数据保存的结构体和发送方一致
typedef struct struct_message {
  String a;
  int b;
} struct_message;

//创建结构体变量
struct_message myData;
//根据自己的板子修改
uint8_t broadcastAddress[3][6] = {
  { 0xEC, 0xFA, 0xBC, 0x35, 0x19, 0xC7 },  //0号IP
  { 0xF4, 0xCF, 0xA2, 0x75, 0x0A, 0x66 },  //1号IP
  { 0x3C, 0x71, 0xBF, 0x38, 0x24, 0xA2 }   //2号IP

};

//计时
Ticker ticker;
int countT = 0;
void countime() {
  countT = countT + 1;
}

void lastTime(int a) {
  if (countT >= a) {
    digitalWrite(16, LOW);
    //ESP.deepSleep(3e6); //睡3秒
    //ESP.deepSleep(0);  //一直休眠
  }
}

void ResetTime(int a) {
  if (countT >= a) {
    ESP.deepSleep(3e6);  //3秒后重启
    //ESP.deepSleep(0);  //一直休眠
  }
}


//EspNow设定

//接收和发送
//在这种情况下，无论是否成功发送该消息，都会简单地打印出来
String cheakData = "good";
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("发送成功");
    cheakData = "good";
  } else {
    Serial.println("发送不成功");
    cheakData = "Bad";
  }
}

//创建一个回调函数作为接收数据后的串口显示
String worddata;   //临时存放myData.a
int typeword = 0;  //临时开关避免执行时还收到讯息
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));

  worddata = myData.a;
  typeword = 1;
  //简单打印出收到的讯息
  //Serial.print(myData.a);
  //Serial.print(" ");
  //Serial.println(myData.b);
}

//计算板子有几个
int sizelen = 0;  //全部有几块板子
int mymac = 99;   //我的板子是第几块 新的板子是设为99
//espnow初始化
void espnowinit() {
  //设置ESP8266模式
  WiFi.mode(WIFI_STA);
  //初始化 ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //设置ESP8266角色：
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  //打印自己的版子ip
  Serial.print("我的IP地址: ");
  Serial.println(WiFi.macAddress());

  //计算板子有几个
  sizelen = sizeof(broadcastAddress) / sizeof(broadcastAddress[0]);
  Serial.print("板子数量有");
  Serial.println(sizelen);

  //对比自己的板子
  String macnum = "";
  for (int j = 0; j < sizelen; j++) {
    for (int i = 0; i <= 6; i++) {
      String Mac1 = String(broadcastAddress[j][i], HEX);
      Mac1.toUpperCase();
      //Serial.println(Mac1);
      //Serial.println(Mac1.length());
      if (Mac1.length() == 1) {
        Mac1 = "0" + Mac1;
      }
      if (i == 0) {
        macnum = Mac1;
      }
      if (i > 0 and i < 6) {
        macnum = macnum + ":" + Mac1;
      }
    }
    if (macnum == WiFi.macAddress()) {
      mymac = j;
      Serial.print("我的顺序是");
      Serial.println(j);
    }
    if (macnum != WiFi.macAddress()) {
      //与另一个ESP-NOW设备配对以发送数据
      esp_now_add_peer(broadcastAddress[j], ESP_NOW_ROLE_COMBO, 1, NULL, 0);
      //Serial.print("配对的板子");
      //Serial.println(macnum);
    }
  }

  //先前创建的功能。
  esp_now_register_send_cb(OnDataSent);

  //先前创建的功能 测试ESP-NOW通信
  esp_now_register_recv_cb(OnDataRecv);
}
//发送数据
void sendone(int i) {
  esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
  delay(50);
  int a = 0;  //临时计算重新传送次数
  while (cheakData == "Bad" and a < 10) {
    esp_now_send(broadcastAddress[i], (uint8_t *)&myData, sizeof(myData));
    a = a + 1;
    delay(100);
  }
  Serial.print("已经发送给");
  Serial.print(i);
  Serial.println("号");
}

//测电压部份
int analogValue;
void VAo0() {
  analogValue = analogRead(A0);
  Serial.println(analogValue);
  if (analogValue > 1022) {
    analogValue = 1022;
  }
  delay(10);
}

//IO2闪灯一次
void ledshow() {
  digitalWrite(2, LOW);
  delay(100);
  digitalWrite(2, HIGH);
  delay(100);
}

//0号初始化
void mysetup0() {
}


//1号初始化
void mysetup1() {
}

//2号初始化
void mysetup2() {
}



//初始化设定
void setup() {
  Serial.begin(9600);  //串口初始化
  espnowinit();
  if (mymac == 0) mysetup0();
  if (mymac == 1) mysetup1();
  if (mymac == 2) mysetup2();
  ticker.attach(1, countime);
  Serial.println("计时器已启动");
}


//循环里要执行什麽就修改这里44444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444

void myloop0() {  //0号循环
}
void myloop1() {  //1号循环
}
void myloop2() {  //2号循环
}


void loop() {
  if (mymac == 0) myloop0();
  if (mymac == 1) myloop1();
  if (mymac == 2) myloop2();
}

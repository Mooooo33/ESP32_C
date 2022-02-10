#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../lib/ArduinoJson/ArduinoJson.h"


// JOSN是一种轻量级的数据交换格式，遵循一下规则：
// 1.并列的数据之间用逗号(,)分隔
// 2.映射用冒号(:)表示
// 3.并列数据的集合(数组)用方括号([])表示
// 4.映射的集合(对象)用大括号({})表示

//	WiFi的初始化和连接
void WiFi_Connect()
{
	WiFi.begin("FFF", "acb1235678");//SSID,PASSWORD
	while (WiFi.status() != WL_CONNECTED)//查看WIFI是否成功连接
	{ //这里是阻塞程序，直到连接成功
		delay(300);
		Serial.print(".");
	}
}

void setup()
{

	Serial.begin(115200); // open the serial port at 115200 bps;
	delay(100);

	Serial.print("Connecting.. ");

	WiFi_Connect();

	Serial.println("WiFi connected");

	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());//打印IP地址 print IP address
}

// bilibili api: follower
String UID = "442412984";//此处需要修改
String followerUrl = "http://api.bilibili.com/x/relation/stat?vmid=" + UID; // 粉丝数接口

long follower = 0; // 粉丝数

DynamicJsonDocument doc(1024);

//	获取粉丝数
void getBiliBiliFollower()
{
	HTTPClient http;
	http.begin(followerUrl); //HTTP begin
	int httpCode = http.GET();

	if (httpCode > 0)
	{
		// httpCode will be negative on error
		Serial.printf("HTTP Get Code: %d\r\n", httpCode);

		if (httpCode == HTTP_CODE_OK) // 收到正确的内容
		{
			String resBuff = http.getString();

			//	输出示例：{"mid":123456789,"following":226,"whisper":0,"black":0,"follower":867}}
			Serial.println(resBuff);

			//	使用ArduinoJson_6.x版本，具体请移步：https://github.com/bblanchon/ArduinoJson
			deserializeJson(doc, resBuff); //开始使用Json解析
			follower = doc["data"]["follower"];
//			Serial.printf("Follers: %ld \r\n", follower);
			Serial.printf("三箱的b站粉丝: %ld \r\n", follower);

		}
	}
	else
	{
		Serial.printf("HTTP Get Error: %s\n", http.errorToString(httpCode).c_str());
	}

	http.end();
}

void loop()
{

	getBiliBiliFollower();
	delay(5000);
	delay(5000);
}

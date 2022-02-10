#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../lib/ArduinoJson/ArduinoJson.h"


// JOSN��һ�������������ݽ�����ʽ����ѭһ�¹���
// 1.���е�����֮���ö���(,)�ָ�
// 2.ӳ����ð��(:)��ʾ
// 3.�������ݵļ���(����)�÷�����([])��ʾ
// 4.ӳ��ļ���(����)�ô�����({})��ʾ

//	WiFi�ĳ�ʼ��������
void WiFi_Connect()
{
	WiFi.begin("SSID", "password");//SSID,PASSWORD
	while (WiFi.status() != WL_CONNECTED)//�鿴WIFI�Ƿ�ɹ�����
	{ //��������������ֱ�����ӳɹ�
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
	Serial.println(WiFi.localIP());//��ӡIP��ַ print IP address
}

// bilibili api: follower
String UID = "*********";//�˴���Ҫ�޸�
String followerUrl = "http://api.bilibili.com/x/relation/stat?vmid=" + UID; // ��˿���ӿ�

long follower = 0; // ��˿��

DynamicJsonDocument doc(1024);

//	��ȡ��˿��
void getBiliBiliFollower()
{
	HTTPClient http;
	http.begin(followerUrl); //HTTP begin
	int httpCode = http.GET();

	if (httpCode > 0)
	{
		// httpCode will be negative on error
		Serial.printf("HTTP Get Code: %d\r\n", httpCode);

		if (httpCode == HTTP_CODE_OK) // �յ���ȷ������
		{
			String resBuff = http.getString();

			//	���ʾ����{"mid":123456789,"following":226,"whisper":0,"black":0,"follower":867}}
			Serial.println(resBuff);

			//	ʹ��ArduinoJson_6.x�汾���������Ʋ���https://github.com/bblanchon/ArduinoJson
			deserializeJson(doc, resBuff); //��ʼʹ��Json����
			follower = doc["data"]["follower"];
//			Serial.printf("Follers: %ld \r\n", follower);
			Serial.printf("followers num: %ld \r\n", follower);

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

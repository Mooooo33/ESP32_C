/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   ����һ��BLE��������һ�������յ����ӣ����������Է���֪ͨ

   Tʹ�ò��裺
   1. ����һ�� BLE Server
   2. ����һ�� BLE Service
   3. ����һ�� BLE Characteristic
   4. ����һ�� BLE Descriptor
   5. ��ʼ����
   6. ��ʼ�㲥


*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "common.h"

uint8_t txValue = 0;
BLEServer *pServer = NULL;                   //BLEServerָ�� pServer
BLECharacteristic *pTxCharacteristic = NULL; //BLECharacteristicָ�� pTxCharacteristic
bool deviceConnected = false;                //��������״̬
bool oldDeviceConnected = false;             //�ϴ�����״̬

// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define SERVICE_UUID "12a59900-17cc-11ec-9621-0242ac130002" // UART service UUID
#define CHARACTERISTIC_UUID_RX "12a59e0a-17cc-11ec-9621-0242ac130002"
#define CHARACTERISTIC_UUID_TX "12a5a148-17cc-11ec-9621-0242ac130002"

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue(); //������Ϣ

        if (rxValue.length() > 0)
        { //�򴮿�����յ���ֵ
            Serial.print("RX: ");
            for (int i = 0; i < rxValue.length(); i++)
                Serial.print(rxValue[i]);
            Serial.println();
        }
    }
};

void setup()
{
    Serial.begin(115200);

    // ����һ�� BLE �豸
    BLEDevice::init("UART_BLE");

    // ����һ�� BLE ����
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks()); //���ûص�
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // ����һ�� BLE ����
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks()); //���ûص�

    pService->start();                  // ��ʼ����
    pServer->getAdvertising()->start(); // ��ʼ�㲥
    Serial.println(" �ȴ�һ���ͻ������ӣ��ҷ���֪ͨ... ");
}

void loop()
{
    // deviceConnected ������
    if (deviceConnected)
    {
        pTxCharacteristic->setValue(&txValue, 1); // ����Ҫ���͵�ֵΪ1
        pTxCharacteristic->notify();              // �㲥
        txValue++;                                // ָ���ַ�Լ�1
        delay(2000);                              // �����̫���Ҫ���ͣ����������
    }

    // disconnecting  �Ͽ�����
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // ��ʱ�����������
        pServer->startAdvertising(); // ���¹㲥
        Serial.println(" ��ʼ�㲥 ");
        oldDeviceConnected = deviceConnected;
    }

    // connecting  ��������
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
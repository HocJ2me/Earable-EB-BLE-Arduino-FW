#include "BLEConnection.h"


bool deviceConnected = false;


// BLE_CMD_SERVICE_UUID
BLECharacteristic BLEStremService_RX_Characteristic(BLE_CMD_RX_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor BLEStremService_RX_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStremService_RES_Characteristic(BLE_CMD_RES_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStremService_RES_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStremService_HDL_Characteristic(BLE_EER_HDL_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStremService_HDL_Descriptor(BLEUUID((uint16_t)0x2902));

// BLE_STREAM_SERVICE_UUID
BLECharacteristic BLEStream_EEG_Characteristic(BLE_EEG_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_EEG_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_IMU_Characteristic(BLE_IMU_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_IMU_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_PPG_Characteristic(BLE_PPG_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_PPG_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_HR_Characteristic(BLE_HR_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_HR_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_SPO2_Characteristic(BLE_SPO2_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_SPO2_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_BATT_Characteristic(BLE_BATT_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_BATT_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_GYRO_Characteristic(BLE_GYRO_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_GYRO_Descriptor(BLEUUID((uint16_t)0x2902));


//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  };
};

class RXCallBacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
      uint8_t* rxValue = pCharacteristic->getData();
      unsigned int rxLength = pCharacteristic->getLength();
      Serial.print("Received Value: ");
      Serial.print(*rxValue);
      Serial.print("  ");
      Serial.print("Data length: ");
      Serial.print(rxLength);
      Serial.println("  ");
      switch (*rxValue)
      {
        case BLE_CMD_GET_FW_VER:
          Serial.println("BLE_CMD_GET_FW_VER");
          BLEStremService_RES_Characteristic.setValue(DEVICE_HARDWARE_VERSION);
          break;
        case BLE_CMD_GET_BATTERY_LEVEL:
          Serial.println("BLE_CMD_GET_BATTERY_LEVEL");
          BLEStremService_RES_Characteristic.setValue(DEVICE_BATTERY_LEVEL);
          break;
        case BLE_CMD_GET_SERIAL_NUMBER:
          Serial.println("BLE_CMD_GET_SERIAL_NUMBER");
          BLEStremService_RES_Characteristic.setValue(DEVICE_SERIAL_NUMBERR);
          break;
        case BLE_CMD_GET_DEVICE_NAME:
          Serial.println("BLE_CMD_GET_DEVICE_NAME");
          BLEStremService_RES_Characteristic.setValue(DEVICE_NAME);
          break;
        case BLE_CMD_GET_DEVICE_ID:
          Serial.println("BLE_CMD_GET_DEVICE_ID");
          BLEStremService_RES_Characteristic.setValue(DEVICE_ID);
          break;
        case BLE_CMD_GET_UNIX_TIME:
          Serial.println("BLE_CMD_GET_UNIX_TIME");
          BLEStremService_RES_Characteristic.setValue(DEVICE_UNIX_TIME);
          break;
        case BLE_CMD_DATA_STREAM_CTRL:
          bool* setDataChannel;
          String rxString = pCharacteristic->getValue().c_str();
          Serial.println("BLE_CMD_DATA_STREAM_CTRL");
          switch (rxString[1])
          {
          case DATA_CTRL_AFE_ID:
            Serial.println("DATA_CTRL_AFE_ID");
            // setDataChannel = &BLEConnection->_afeStream;
            break;
          case DATA_CTRL_ACCEL_ID:
            Serial.println("DATA_CTRL_ACCEL_ID");
            // setDataChannel = &BLEConnection->_accelStream;
            break;
          case DATA_CTRL_GYRO_ID:
            Serial.println("DATA_CTRL_GYRO_ID");
            // setDataChannel = &BLEConnection._gyroStream;
            break;
          case DATA_CTRL_PPG_LED_ID:
            Serial.println("DATA_CTRL_PPG_LED_ID");
            // setDataChannel = &BLEConnection._ppgLedStream;
            break;
          case DATA_CTRL_HR_SPO2_ID:
            Serial.println("DATA_CTRL_HR_SPO2_ID");
            // setDataChannel = &BLEConnection._hrSpo2Stream;
            break;
          case DATA_CTRL_BATT_ID:
            Serial.println("DATA_CTRL_BATT_ID");
            // setDataChannel = &BLEConnection._battStream;
            break;
          }
          if(rxString[2] == DATA_CTRL_OFF)
          {
              Serial.println("DATA_CTRL_OFF");
              // *setDataChannel = false;
          }
          if(rxString[2] == DATA_CTRL_ON)
          {
              Serial.println("DATA_CTRL_ON");  
              // *setDataChannel = true;     
          }
          break;
      }
      
      BLEStremService_RES_Characteristic.notify();
  }
};

void BLEConnection::loopDataStream()
{
  if(millis() - lastSendData > 5)
  {
      double randomValueInt  = random(100);
      double &randomValue = randomValueInt;

      if(_afeStream)
      {
        BLEStream_EEG_Characteristic.setValue(randomValue);
        BLEStream_EEG_Characteristic.notify();
      }
      if(_accelStream)
      {
        BLEStream_IMU_Characteristic.setValue(randomValue);
        BLEStream_IMU_Characteristic.notify();
      }
      if(_gyroStream)
      {
        BLEStream_GYRO_Characteristic.setValue(randomValue);
        BLEStream_GYRO_Characteristic.notify();
      }
      if(_ppgLedStream)
      {
        BLEStream_PPG_Characteristic.setValue(randomValue);
        BLEStream_PPG_Characteristic.notify();
      }
      if(_hrSpo2Stream)
      {
        BLEStream_HR_Characteristic.setValue(randomValue);
        BLEStream_HR_Characteristic.notify();
        BLEStream_SPO2_Characteristic.setValue(randomValue);
        BLEStream_SPO2_Characteristic.notify();
      }
      if(_battStream)
      {
        BLEStream_BATT_Characteristic.setValue(randomValue);
        BLEStream_BATT_Characteristic.notify();
      }
      lastSendData = millis();
      
  }
}
void BLEConnection::init()
{
    // Create the BLE Device
  BLEDevice::init(DeviceName);

  // Create the BLE Server
  pServerCMD = BLEDevice::createServer();
  pServerCMD->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEServiceCMD = pServerCMD->createService(BLEUUID(BLE_CMD_SERVICE_UUID), 2000, 1);
  // Create BLE Characteristics and Create a BLE Descriptor

  BLEServiceCMD->addCharacteristic(&BLEStremService_RX_Characteristic);
  BLEStremService_RX_Characteristic.setCallbacks(new RXCallBacks());
  BLEStremService_RX_Descriptor.setValue("BLE_CMD_RX_CHAR_UUID");
  BLEStremService_RX_Characteristic.addDescriptor(&BLEStremService_RX_Descriptor);

  BLEServiceCMD->addCharacteristic(&BLEStremService_RES_Characteristic);
  BLEStremService_RES_Descriptor.setValue("BLE_CMD_RES_CHAR_UUID");
  BLEStremService_RES_Characteristic.addDescriptor(&BLEStremService_RES_Descriptor);

  BLEServiceCMD->addCharacteristic(&BLEStremService_HDL_Characteristic);
  BLEStremService_HDL_Descriptor.setValue("BLE_EER_HDL_CHAR_UUID");
  BLEStremService_HDL_Characteristic.addDescriptor(&BLEStremService_HDL_Descriptor);

  // Start the service
  BLEServiceCMD->start();


  pServerSTREAM = BLEDevice::createServer();

  BLEServiceSTREAM = pServerSTREAM->createService(BLEUUID(BLE_STREAM_SERVICE_UUID), 2000, 2);
  // Create BLE Characteristics and Create a BLE Descriptor

  BLEServiceSTREAM->addCharacteristic(&BLEStream_EEG_Characteristic);
  BLEStream_EEG_Descriptor.setValue("BLE_EEG_STREAM_CHAR_UUID");
  BLEStream_EEG_Characteristic.addDescriptor(&BLEStream_EEG_Descriptor);

  BLEServiceSTREAM->addCharacteristic(&BLEStream_IMU_Characteristic);
  BLEStream_IMU_Descriptor.setValue("BLE_IMU_STREAM_CHAR_UUID");
  BLEStream_IMU_Characteristic.addDescriptor(&BLEStream_IMU_Descriptor);

  BLEServiceSTREAM->addCharacteristic(&BLEStream_PPG_Characteristic);
  BLEStream_PPG_Descriptor.setValue("BLE_PPG_STREAM_CHAR_UUID");
  BLEStream_PPG_Characteristic.addDescriptor(&BLEStream_PPG_Descriptor);

  BLEServiceSTREAM->addCharacteristic(&BLEStream_HR_Characteristic);
  BLEStream_HR_Descriptor.setValue("BLE_HR_STREAM_CHAR_UUID");
  BLEStream_HR_Characteristic.addDescriptor(&BLEStream_HR_Descriptor);

  BLEServiceSTREAM->addCharacteristic(&BLEStream_SPO2_Characteristic);
  BLEStream_SPO2_Descriptor.setValue("BLE_SPO2_STREAM_CHAR_UUID");
  BLEStream_SPO2_Characteristic.addDescriptor(&BLEStream_SPO2_Descriptor);

  BLEServiceSTREAM->addCharacteristic(&BLEStream_BATT_Characteristic);
  BLEStream_BATT_Descriptor.setValue("BLE_BATT_STREAM_CHAR_UUID");
  BLEStream_BATT_Characteristic.addDescriptor(&BLEStream_BATT_Descriptor);

  BLEServiceSTREAM->addCharacteristic(&BLEStream_GYRO_Characteristic);
  BLEStream_GYRO_Descriptor.setValue("BLE_GYRO_STREAM_CHAR_UUID");
  BLEStream_GYRO_Characteristic.addDescriptor(&BLEStream_GYRO_Descriptor);

  // Start the service
  BLEServiceSTREAM->start();

  // Start advertising
  BLEAdvertising *pAdvertisingCMD = BLEDevice::getAdvertising();
  pAdvertisingCMD->addServiceUUID(BLE_CMD_SERVICE_UUID);
  // pAdvertising->addServiceUUID(BLE_STREAM_SERVICE_UUID);
  pServerCMD->getAdvertising()->start();

  BLEAdvertising *pAdvertisingStream = BLEDevice::getAdvertising();
  // pAdvertisingCMD->addServiceUUID(BLE_CMD_SERVICE_UUID);
  pAdvertisingStream->addServiceUUID(BLE_STREAM_SERVICE_UUID);
  pServerSTREAM->getAdvertising()->start();



  Serial.println("Waiting a client connection to notify...");
}

BLEConnection::BLEConnection()
{

}

BLEConnection::~BLEConnection()
{

}

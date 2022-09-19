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
BLECharacteristic BLEStream_EEG_Characteristic(BLE_EEG_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor BLEStream_EEG_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_IMU_Characteristic(BLE_IMU_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_IMU_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_PPG_Characteristic(BLE_PPG_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStream_PPG_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStream_HR_Characteristic(BLE_HR_STREAM_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);
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
      Serial.print("Received Value: ");
      Serial.print(*rxValue);
      Serial.println();
      switch (*rxValue)
      {
        case BLE_CMD_GET_FW_VER:
          BLEStremService_RES_Characteristic.setValue(DEVICE_HARDWARE_VERSION);
          break;
        case BLE_CMD_GET_BATTERY_LEVEL:
          BLEStremService_RES_Characteristic.setValue(DEVICE_BATTERY_LEVEL);
          break;
        case BLE_CMD_GET_SERIAL_NUMBER:
          BLEStremService_RES_Characteristic.setValue(DEVICE_SERIAL_NUMBERR);
          break;
        case BLE_CMD_GET_DEVICE_NAME:
          BLEStremService_RES_Characteristic.setValue(DEVICE_NAME);
          break;
        case BLE_CMD_GET_DEVICE_ID:
          BLEStremService_RES_Characteristic.setValue(DEVICE_ID);
          break;
        case BLE_CMD_GET_UNIX_TIME:
          BLEStremService_RES_Characteristic.setValue(DEVICE_UNIX_TIME);
          break;
      }
      
      BLEStremService_RES_Characteristic.notify();
  }
};

void BLEConnection::init()
{
    // Create the BLE Device
  BLEDevice::init(DeviceName);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEServiceCMD = pServer->createService(BLE_CMD_SERVICE_UUID);
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

  // Create the BLE Service
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEServiceSTREAM = pServer->createService(BLE_STREAM_SERVICE_UUID);
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
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_CMD_SERVICE_UUID);
  pAdvertising->addServiceUUID(BLE_STREAM_SERVICE_UUID);
  pServer->getAdvertising()->start();


  Serial.println("Waiting a client connection to notify...");
}

BLEConnection::BLEConnection()
{

}

BLEConnection::~BLEConnection()
{

}

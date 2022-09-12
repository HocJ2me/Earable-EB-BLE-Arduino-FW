#include "BLEConnection.h"


bool deviceConnected = false;

BLECharacteristic BLEStremService_RX_Characteristic(BLE_CMD_RX_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor BLEStremService_RX_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStremService_RES_Characteristic(BLE_CMD_RES_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStremService_RES_Descriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic BLEStremService_HDL_Characteristic(BLE_EER_HDL_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BLEStremService_HDL_Descriptor(BLEUUID((uint16_t)0x2902));


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

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_CMD_SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

BLEConnection::BLEConnection()
{
    init();
}

BLEConnection::~BLEConnection()
{

}

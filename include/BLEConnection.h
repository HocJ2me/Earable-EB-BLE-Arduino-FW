#ifndef BLECONNECTION_H
#define BLECONNECTION_H

#include <iostream>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <configs.h>

#define BLE_CMD_SERVICE_UUID  "45420100-0000-ffff-ff45-415241424c45"
#define BLE_CMD_RX_CHAR_UUID  "45420101-0000-ffff-ff45-415241424c45"
#define BLE_CMD_RES_CHAR_UUID "45420102-0000-ffff-ff45-415241424c45"
#define BLE_EER_HDL_CHAR_UUID "45420103-0000-ffff-ff45-415241424c45"

#define BLE_STREAM_SERVICE_UUID    "45420200-0000-ffff-ff45-415241424c45"
#define BLE_EEG_STREAM_CHAR_UUID   "45420201-0000-ffff-ff45-415241424c45"
#define BLE_IMU_STREAM_CHAR_UUID   "45420202-0000-ffff-ff45-415241424c45"
#define BLE_PPG_STREAM_CHAR_UUID   "45420203-0000-ffff-ff45-415241424c45"
#define BLE_HR_STREAM_CHAR_UUID    "45420204-0000-ffff-ff45-415241424c45"
#define BLE_SPO2_STREAM_CHAR_UUID  "45420205-0000-ffff-ff45-415241424c45"
#define BLE_BATT_STREAM_CHAR_UUID  "45420206-0000-ffff-ff45-415241424c45"
#define BLE_GYRO_STREAM_CHAR_UUID  "45420207-0000-ffff-ff45-415241424c45"


#define BLE_CMD_GET_FW_VER              0x01
#define BLE_CMD_GET_BATTERY_LEVEL       0x02
#define BLE_CMD_DATA_STREAM_CTRL        0x03
#define BLE_CMD_SOFT_RESET              0x05
#define BLE_CMD_GET_SERIAL_NUMBER       0x06
#define BLE_CMD_TIME_SYNC               0x07
#define BLE_CMD_SET_DEVICE_NAME         0x08
#define BLE_CMD_GET_DEVICE_NAME         0x09
#define BLE_CMD_GET_UNIX_TIME           0x0B
#define BLE_CMD_GET_DEVICE_ID           0x0C

#define DATA_CTRL_AFE_ID                0x01
#define DATA_CTRL_ACCEL_ID              0x02
#define DATA_CTRL_GYRO_ID               0x04
#define DATA_CTRL_PPG_LED_ID            0x08
#define DATA_CTRL_HR_SPO2_ID            0x10
#define DATA_CTRL_BATT_ID               0x20

#define DATA_CTRL_OFF                   0x00
#define DATA_CTRL_ON                    0x01    

#define DATA_CTRL_AFE_ID               0x01
#define DATA_CTRL_ACCEL_ID             0x02
#define DATA_CTRL_GYRO_ID              0x04
#define DATA_CTRL_PPG_LED_ID           0x08
#define DATA_CTRL_HR_SPO2_ID           0x10
#define DATA_CTRL_BATT_ID              0x20

#define DATA_CTRL_OFF                  0x00
#define DATA_CTRL_ON                   0x01
class BLEConnection
{
private:
    BLEServer *pServerCMD;
    BLEServer *pServerSTREAM;
	BLEService *BLEServiceCMD;
    BLEService *BLEServiceSTREAM;
    bool _afeStream = false;
    bool _accelStream = false;
    bool _gyroStream = false;
    bool _ppgLedStream = false;
    bool _hrSpo2Stream = false;
    bool _battStream = false;
    unsigned long lastSendData = 0;
public:
    void init();
    void loopDataStream();
    BLEConnection(/* args */);
    ~BLEConnection();
};

#endif
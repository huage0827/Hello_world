#pragma once

#include <iostream>
#include <string.h>

using namespace std;

/*******************************
 * 结构体：SimuIOCardCfg_t
 * 说明： IO板卡配置定义
 *******************************/
typedef struct TagSimuIOCardCfg{
	unsigned short  usVendorID;
	unsigned short  usDeviceID;
	unsigned int  ulClassCode;
	string szVendorID;
	string szDeviceID;
	string szClassCode;
	unsigned int ulCardNum;
	string szVendor;
	string szDevice;
	string szClass;
}SimuIOCardCfg_t;

/********************************
 * IO板卡默认配置文件名
 ********************************/
#define SIMU_IO_CARD_DEF_CFG_FILE "/usr/simulab/bin/SimuIOCardCfg.xml"

/********************************
 * SimuLab支持的板卡
 ********************************/

#pragma once

#include <iostream>
#include <string.h>

using namespace std;

/*******************************
 * �ṹ�壺SimuIOCardCfg_t
 * ˵���� IO�忨���ö���
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
 * IO�忨Ĭ�������ļ���
 ********************************/
#define SIMU_IO_CARD_DEF_CFG_FILE "/usr/simulab/bin/SimuIOCardCfg.xml"

/********************************
 * SimuLab֧�ֵİ忨
 ********************************/

//*********************************************************
//This file defines the basic interface of EventAPP
//Author:WK
//Data:2012.09.22
//**********************************************************

#ifndef __EVENT_APP_H_
#define __EVENT_APP_H_

#include "EventAPPConstant.h"
#include "VSDEvent.h"
#include "EventAPPStructure.h"

#if defined(WIN32)
#ifdef EVENTAPP_EXPORTS
#define EVENTAPP_API __declspec(dllexport)
#else
#define EVENTAPP_API __declspec(dllimport)
#endif
#elif defined(LINUX)
#ifdef EVENTAPP_EXPORTS
#define EVENTAPP_API __attribute__ ((visibility("default")))
#else
#define EVENTAPP_API
#endif
#else
#error "No platform specified!"
#endif

#ifdef __cplusplus
extern "C"{
#endif

// EventAPP ����ͼƬ���в��õ�������Υ����Ϣ�������Υ�µ�ͼƬ����
class EVENTAPP_API EventAPP
{
public:
	EventAPP();
	~EventAPP();

	// ��ʼ��EventAPP���ڵ��������ӿں���֮ǰ���˺���Ӧ���Ѿ����ù�
	// ������� irParam: ��ʾEventAPP��Ҫ�Ĳ������ã���EventAPPParam�ı���
	APPRESULT Init(const EventAPPParam& irParam);

	// ����ͼƬ������Ҫ�ӿڡ�
	// ������� ipImage: Ҫ�����ͼ��ָ��
	// ������� ipObjectMulti: ͨ��VSDEvent::ProcssFram�õ���������
	// ������� isRedLightOn: ��ʾ��ǰ֡����Ƿ�����
	// ������� opResult: �������ǰͼƬΪֹ���ո��뿪��������ĳ�����ͼƬ�����Լ��Ƿ�Υ�µ���Ϣ����EventMultiAPPResult�ı���
	APPRESULT ProcessFram(LPRImage* ipImage, const VSDObjectMulti* ipObjectMulti, int iLightStatus[MAX_VIRTUAL_LOOPS], EventMultiAPPResult* opResult); 

	// 
	APPRESULT AddSubTitle(LPRImage* ipImage, const std::string& irString, LPRImage** opImage);

private:
	// We don't want user to copy this object
	EventAPP(const EventAPP&);
	EventAPP& operator=(const EventAPP&);
	// Hide the implementation
	void* mObject;

};

#ifdef __cplusplus
}
#endif

#endif
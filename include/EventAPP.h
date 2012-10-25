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
	EventAPP(const EventAPPParam& irParam);
	~EventAPP();

	// ��ʼ��EventAPP���ڵ��������ӿں���֮ǰ���˺���Ӧ���Ѿ����ù�
	// ������� irParam: ��ʾEventAPP��Ҫ�Ĳ������ã���EventAPPParam�ı���
	// APPRESULT Init(const EventAPPParam& irParam);

	// ����ͼƬ������Ҫ�ӿڡ�
	// ������� ipImage: Ҫ�����ͼ��ָ��
	// ������� ipObjectMulti: ͨ��VSDEvent::ProcssFrame�õ���������
	// ������� isRedLightOn: ��ʾ��ǰ֡����Ƿ�����
	// ������� opResult: �������ǰͼƬΪֹ���ո��뿪��������ĳ�����ͼƬ�����Լ��Ƿ�Υ�µ���Ϣ����EventMultiAPPResult�ı���
	APPRESULT ProcessFrame(LPRImage* ipImage, const VSDObjectMulti* ipObjectMulti,const VSDObjectTrackMulti* ipObjectTrackMult, int iLightStatus[MAX_VIRTUAL_LOOPS], EventMultiAPPResult* opResult); 

	// ��ͼƬ��ָ������Ļ
	// ������� ipImage: ��Ҫ������Ļ��ԴͼƬ
	// ������� ipString: ��Ҫ���ӵ���Ļ
	// ������� oppImage: ���Ӻ���Ļ��ͼƬ
	APPRESULT AddSubTitle(LPRImage* ipImage, const EventSubtitleOverlay &subtitles, const EventSubtitleImages* ipSubtitleImages ,LPRImage** oppImage);

	// ��ָ����Ŀ��ͼƬ�ϳ�һ��ͼƬ
	// ������� ipImage: ָ������ͼƬ������
	// ������� iNumOfImages: ����ͼƬ�ĸ���
	// ������� irRect: ��Ϊ��Ҫ��ipImage��ָ�ĵ�һ��ͼƬ�ĳ���λ�÷Ŵ�����irRect�������ڵķ�Χ
	// ������� oppImage; ָ��ϳɵ�ͼƬָ���ָ��
	APPRESULT SynthesisImages(LPRImage** ipImage, int iNumOfImages, const VSDRect& irRect, LPRImage** oppImage);

	// ��ָ����Ŀ��ͼƬ�ϳ���Ƶ
	// ������� ipImage: ָ������ͼƬ������
	// ������� iNumOfImage: ����ͼƬ����Ŀ
	// ������� orMeida: �������Ƶ
	APPRESULT Convert2Media(LPRImage** ipImage, int iNumOfImages, EventMedia& orMedia);

private:
	
	//APPRESULT ConstructResult(int iObjectBreakRule, int iRuleType,int uid, int startIndex, EventMultiAPPResult* opResultMulti, int& orResultCount);
	// ��ֹ�����͸���EventAPP
	EventAPP(const EventAPP&);
	EventAPP& operator=(const EventAPP&);
	// ����EventAPP��ʵ��
	void* mObject;

};

#ifdef __cplusplus
}
#endif

#endif

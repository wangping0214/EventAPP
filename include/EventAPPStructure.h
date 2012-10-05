#ifndef __EVENT_APP_STRUCTURE_H_
#define __EVENT_APP_STRUCTURE_H_

#include "VSDEventStructure.h"
#include "LPRStructure.h"
#include "EventAPPConstant.h"
#include <inttypes.h>

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

// ��¼����Υ��¼��Χ
struct EventRecordParam
{
	int mBreakRuleAhead[RULE_TYPES];   // VSD_BR_TURN_LEFT �� VSD_BR_LOW_SPEED 9��Υ�£������е�ÿһ��ֱ��¼����Υ����Ϊ��Ӧ��ǰ����֡����¼��
	int mBreakRuleBehind[RULE_TYPES];  // VSD_BR_TURN_LEFT �� VSD_BR_LOW_SPEED 9��Υ�£������е�ÿһ��ֱ��¼����Υ����Ϊ��Ӧ¼��Υ����Ϊ��Ķ���֡ 
	int mViedoFormat;				   // ��Ƶ��ʽ��ȡֵ��ΧΪΪEventAPPViedoFormat
	int mBitFrequent;				   // ��Ƶ������
};

struct EventMedia
{
	uint8_t *mBufferPtr;
	int mBufferSize;
};

// �ϳ�ͼƬ����Ļ��������Ϣ
struct EventFont 
{
	int mFontSize;					   // �����С
	int mFontFamily;				   // �����ʽ
	COLORREF mFontColor;			   // ������ɫ
	int mFontOrientation;			   // �����ŷų���ȡֵΪ��ΧΪEventAPPFontOrientation
	int mFontX;						   // ������ʼλ�õ�X����
	int mFontY;                        // ������ʼλ�õ�Y����
};

// N��ͼƬ�ϳ�һ��
struct EventImageSynthesis
{
	int mNumberofImage;				   // �ϳ�ͼƬ����Ŀ
	int mPicOrientation;               // ͼƬ�İڷţ�ȡֵ��ΧΪEventAPPPicOrientation
	double mZoonRatio;                 // ԭʼͼƬ�����ű���
};

// EventAPP����
struct EventAPPParam
{
	VSDEventParam mVSDParam;			// VSDEvent�Ĳ���
	VSDRatioLine mleftTurnLine;			// ��ת�߱�־�ߣ���ֹ���ʱ���ΪΥ��
	VSDRatioLine mRightTurnLine;		// ��ת�߱�־�ߣ���ֹ�ҹ�ʱ���ΪΥ��
	VSDRatioLine mStraightLine;			// ֱ�б�־�ߣ�Խ�����ߺ��ֹ����ת
	EventRecordParam mRecordParam;		// ����Υ�������Υ��¼�����
	double mRatioToCrossLine;			// ����ѹ�߱�����ֵ�����������ֵ�ж�ѹ����ʻ
	EventFont mFont;					// �ϳ�ͼƬ����ĸ��Ϣ
	EventImageSynthesis mImageSynthesis;// ��N��ͼƬ�ϳ�һ��ͼƬ�Ĳ���
};


// EventAPP����ͼƬ��������
struct EventAPPResult
{
	int mID;															// �����ID
	int mNumOfImage;													// ͼ��ĸ���
	wchar_t mPlate[LPR_PLATE_STR_LEN];									// ����ĳ��ƺ�
	VSDBreakRule mBreakRule;											// ID����Υ���Ĺ���
	LPRImage* mImage[MAX_FRAM_AHEAD + MAX_FRAM_BHEIND + 3];			    // �����ͼƬ���У����mBreakRule��VSD_BR_NONE����ômNumberOfImage��ֵһ����1��������λ��ͣ���߸�������Ƭ
																		// ���mBreakRule����VSD_BR_NONE����ôǰMAX_FRAM_AHEAD + MAX_FRAM_BEHIND�������ϳ�¼�񣬺������ŷֱ��ǽӴ�ͣ���ߣ��뿪ͣ���ߣ�Υ��ʱ������ͼƬ
};


// ���APPResult
struct EventMultiAPPResult
{
	int mNumOfResult;													// APPResult�ĸ���
	EventAPPResult mppAPPResult[MAX_OBJECTS * RULE_TYPES];				// ָ��APPResultָ�������
};


EVENTAPP_API void __stdcall FreeMultiAPPResult(EventMultiAPPResult* ipMultiAPPResult);
EVENTAPP_API void __stdcall FreeAPPResult(EventAPPResult* ipAPPResult);
EVENTAPP_API APPRESULT __stdcall EventAPP_LoadParam(const char* ipFileName, EventAPPParam* ipEventParam);

#endif

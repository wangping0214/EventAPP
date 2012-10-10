//*********************************************************
//This file defines the basic interface of EventAPP
//Author:WK
//Data:2012.09.22`
//**********************************************************
#define __DEBUG
#include "EventAPP.h"
#include "MediaConverter.h"
#include "SubtitleOverlay.h"
#include <deque>
#include <map>
#include <list>
#include <fstream>
#include <sstream>
#include <LPRVideo.h>
#include <LPR.h>

#include "ImageSynthesis.h"



#if defined(WIN32)
#define Int64 __int64
#elif  defined(LINUX)
#define Int64 long long
#else
#error "No platform specified!"
#endif

#ifdef __DEBUG
using namespace std;
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cstring>
#define TRACE(string) std::cout << string << std::endl
ofstream lNoBreakRuleLog("NoBreakRule.txt");
wofstream lBreakRuleHistoryLog("BreakRuleHistory.txt");
ofstream lBreakOutputLog("BreakRuleOutput.txt");
std::wstring lCurrentPicName;
#endif


// ������������ֵ�е���Сֵ
template<typename T>
static T __stdcall MinT(T a, T b)
{
	return a < b ? a : b; 
}

// ������������ֵ�е����ֵ
template<typename T>
static T __stdcall MaxT(T a, T b)
{
	return a > b ? a : b;
}

// ͼ���ϵľ��Ծ���ת��Ϊ��Ծ���
static inline void __stdcall ToRatioRECT(const VSDRect& irRect, float iXRatio, float iYRatio, VSDRatioRECT& orRECT)
{
	orRECT.left = irRect.x * iXRatio;
	orRECT.top = irRect.y * iYRatio;
	orRECT.right = orRECT.left + irRect.width * iXRatio;
	orRECT.bottom = orRECT.top + irRect.height * iYRatio;
}

// �жϵ��Ƿ��ھ�����
static bool __stdcall IsInRect(const VSDRatioPoint& irPoint,const VSDRatioRECT& irRect)
{
	return (irPoint.x > irRect.left) && (irPoint.x < irRect.right) && (irPoint.y < irRect.bottom) && (irPoint.y > irRect.top);
}

// �жϵ��Ƿ���ֱ���ϣ���ΪirLine�����ھ��εı߿�����irLine����ƽ����x��y������
static bool __stdcall IsOnLine(const VSDRatioPoint& irPoint, const VSDRatioLine& irLine)
{
	if(irLine.pt1.x == irLine.pt2.x)
		return irPoint.x == irLine.pt1.x && ((irPoint.y - irLine.pt1.y) * (irPoint.y - irLine.pt2.y) <= 0);
	else
		return irPoint.y == irLine.pt1.y && ((irPoint.x - irLine.pt1.x) * (irPoint.x - irLine.pt2.x) <= 0);
}

// �ж������߶��Ƿ��ཻ�����ཻ�򷵻�true������orPointC�����򷵻�false
// ����������������������������������������������������������������������������������������������������
// ��������������������ע�⣬irLineB��Դ�ھ��εı߿�����irLineB����ƽ����x��y���꣡������������������
// ����������������������������������������������������������������������������������������������������
static bool __stdcall GetCrossRatioPoint(const VSDRatioLine& irLineA, const VSDRatioLine& irLineB, VSDRatioPoint& orPointC)
{
	if(IsOnLine(irLineA.pt1, irLineB))
	{
		orPointC = irLineA.pt1;
		return true;
	}
	if(IsOnLine(irLineA.pt2, irLineB))
	{
		orPointC = irLineA.pt2;
		return true;
	}

	const VSDRatioPoint a = irLineA.pt1;
	const VSDRatioPoint b = irLineA.pt2;
	const VSDRatioPoint c = irLineB.pt1;
	const VSDRatioPoint d = irLineB.pt2;

	Int64 area_abc = Int64((a.x - c.x)) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
	Int64 area_abd = Int64((a.x - d.x)) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);

	if(Int64(area_abc) * area_abd >= 0)
		return false;

	Int64 area_cda = Int64((c.x - a.x)) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
	Int64 area_cdb = Int64(area_cda) + area_abc - area_abd;
	if(Int64(area_cda) * area_cdb >= 0)
		return false;
	
	float t = (float)(area_cda) / (area_abd - area_abc);
	orPointC.x = int(a.x + t * (b.x - a.x));
	orPointC.y = a.y + t * (b.y - a.y);
	return true;
}


static inline bool __stdcall IsSame(const VSDRatioPoint& irPointA, const VSDRatioPoint& irPointB)
{
	return irPointA.x == irPointB.x && irPointA.y == irPointB.y;
}

static float __stdcall GetCrossRatio(const VSDRatioLine& irLine, const VSDRatioRECT& irRECT)
{
	VSDRatioLine lRectLineArray[4] = {{{irRECT.left, irRECT.top}, {irRECT.right, irRECT.top}},{{irRECT.right, irRECT.top}, {irRECT.right, irRECT.bottom}},
									 {{irRECT.right, irRECT.bottom}, {irRECT.left, irRECT.bottom}},{{irRECT.left, irRECT.bottom}, {irRECT.left, irRECT.top}}};


	VSDRatioPoint crossTmp;
	VSDRatioPoint crossPoint[2];
	int MAXCROSSPOINT = 2;
	int index = 0;
	bool isCross = false;

	for(int i = 0; i < sizeof(lRectLineArray)/sizeof(VSDRatioLine); ++i)
	{
		isCross = GetCrossRatioPoint(irLine, lRectLineArray[i], crossTmp);
		if(isCross)
		{
			if(index == 0)
			{
				crossPoint[0] = crossTmp;
				index = 1;
			}
			else if(index == 1)
			{
				if(!IsSame(crossPoint[0], crossTmp))
				{
					crossPoint[1] = crossTmp;
					index = 2;
					break;
				}
			}
		}
	}
	// ���ֻ��һ������
	if(1 == index)
	{
		if(IsInRect(irLine.pt1, irRECT))
		{
			crossPoint[1] = irLine.pt1;
			index = 2;
		}
		else if(IsInRect(irLine.pt2, irRECT))
		{
			crossPoint[1] = irLine.pt2;
			index = 2;
		}
	}

	if(2 != index)
		return 0;
	float middleX =  (float(crossPoint[0].x) + crossPoint[1].x) / 2;
	float middleY =  (float(crossPoint[0].y) + crossPoint[1].y) / 2;
	
	float areaA = (middleX - irRECT.left) * ( middleY - irRECT.top);
	float areaB = (irRECT.right - middleX) * (middleY - irRECT.top );
	float areaC = (irRECT.right - middleX) * (irRECT.bottom- middleY);
	float areaD = (middleX - irRECT.left) * (irRECT.bottom - middleY);

	float minArea = MinT(MinT(areaA,areaB), MinT(areaC, areaD));
	
	float areaRect = (irRECT.bottom - irRECT.top) * (irRECT.right - irRECT.left);
	return minArea * 4 / areaRect;
}


typedef std::map<int, int> StatusMap;

struct PoolData
{
	StatusMap mBreakRules;
	int mLoop;
	LPRImage* mpImage;
};

typedef std::deque<PoolData> ImagePool;
typedef std::map<int, LPRImage*> CaptureImageMap;
typedef std::map<int, wchar_t*> PlateMap; 
typedef std::list<EventAPPResult> ResultList;
typedef std::map<int, VSDRect> RectMap;
typedef std::map<int, int> RuleIndexMap;
typedef std::map<int, int> PriorityMap;
typedef std::map<int, int> RecordMap;
typedef std::map<int, int> TouchMap;


EventAPP::EventAPP()
{
	mObject = new int*[22];
	int** pValue = (int**)mObject;
	for(int i = 0; i < 22; ++i)
	{
		*(pValue + i) = NULL;
	}
}

APPRESULT EventAPP::Init(const EventAPPParam& irParam)
{
	int** pValue = (int**)mObject;
	*pValue = (int*)new EventAPPParam(irParam);								// EventAPP�ĵ�һ����ԱΪEventAPPParam
	*(pValue + 1) = (int*)new ImagePool;									// EventAPP�ĵڶ�����ԱΪһ��ImagePool������ͼƬ�����ϳ���Ƶ��ѡȡδΥ����ĸ��
	*(pValue + 2) = NULL;													// EventAPP�ĵ�������ԱΪÿ��������Ȧ��lanemark	
	int maxAhead = irParam.mRecordParam.mBreakRuleAhead[0];					// 
	int maxBehind = irParam.mRecordParam.mBreakRuleBehind[0];
	for(int i = 1; i < RULE_TYPES; ++i)
	{
		maxAhead = MaxT(maxAhead, irParam.mRecordParam.mBreakRuleAhead[i]);
		maxBehind = MaxT(maxBehind, irParam.mRecordParam.mBreakRuleBehind[i]);
	}
	*(pValue + 3) = new int(maxAhead + maxBehind + DEFAULT_EXTRA_IMAGE_BUFFER);	// EventAPP�ĵ�������Ա����ImagePool����󳤶�
	*(pValue + 4) = (int*)new CaptureImageMap;							// EventAPP�ĵ������Ա����ÿ����������ͣ���߸�������Ƭ
	*(pValue + 5) = (int*)new StatusMap;									// EventAPP�ĵ�������Ա����Ӹ��ٿ�ʼ����������ʷbreakrule״̬�ĵ���ֵ
	//*(pValue + 6) = new int(0);									// EventAPP�ĵ��߸���Ա����ÿ�������ĳ��breakrule�Ƿ��Ѿ�¼�ƹ���Ƶ����ֹ¼����ͬ�������ͬΥ��
	*(pValue + 6) = (int*)new MediaConverter((EventAPPVideoFormat)irParam.mRecordParam.mVideoFormat, irParam.mRecordParam.mFrameFrequent, irParam.mRecordParam.mBitFrequent);  // EventAPP�ĵڰ˸���Ա����MediaConvertoer������¼����Ƶ
	//SubtitleOverlay* pSubTitleOverlay = &SubtitleOverlay::getInstance();
	//pSubTitleOverlay->initialize(irParam.mFont.mCharactors, irParam.mFont);
	//*(pValue + 7) = (int*)pSubTitleOverlay;					                // EventAPP�ĵھŸ���Ա����������ĸ
	*(pValue + 8) = NULL;													// EventAPP�ĵ�ʮ����Ա����ʶ����
	*(pValue + 9) = (int*)new PlateMap;									// EventAPP�ĵ�ʮһ����Ա���˱���ÿ�����ĳ��ƺ�
	*(pValue + 10) = (int*)new CaptureImageMap;						// EventAPP�ĵ������Ա����ÿ����������ͣ���߸�������Ƭ
	*(pValue + 11) = NULL;													// EventAPP�ĵ�������Ա����ͼƬ�������Կ�ȵı�ֵ
	*(pValue + 12) = NULL;													// EventAPP�ĵ��߸���Ա����ͼƬ�߶�����Ը߶ȵı�ֵ
	*(pValue + 13) = (int*)new ImageSynthesis;				
	*(pValue + 14) = (int*)new RectMap;
	RuleIndexMap* pRuleIndexMap = new RuleIndexMap;
	*(pValue + 15) = (int*)pRuleIndexMap;
	(*pRuleIndexMap)[VSD_BR_NONE] = 0;
	(*pRuleIndexMap)[VSD_BR_TURN_LEFT] = 1;
	(*pRuleIndexMap)[VSD_BR_TURN_RIGHT] = 2;
	(*pRuleIndexMap)[VSD_BR_STRAIGHT_THROUGH] = 3;
	(*pRuleIndexMap)[VSD_BR_CROSS_LANE] = 4;
	(*pRuleIndexMap)[VSD_BR_REVERSE] = 5;
	(*pRuleIndexMap)[VSD_BR_RED_LIGHT] = 6;
	(*pRuleIndexMap)[VSD_BR_STOP] = 7;
	(*pRuleIndexMap)[VSD_BR_HIGH_SPEED] = 8;
	(*pRuleIndexMap)[VSD_BR_LOW_SPEED] = 9;
	*(pValue + 16) = (int*)new PriorityMap;
	*(pValue + 17) = (int*)new RecordMap;
	*(pValue + 18) = (int*)new CaptureImageMap;
	*(pValue + 19) = (int*)new CaptureImageMap;
	*(pValue + 20) = new int(maxAhead);
	*(pValue + 21) = (int*)new TouchMap;
	return APP_OK;
}

APPRESULT EventAPP::ProcessFrame(LPRImage *ipImage, const VSDObjectMulti* ipObjectMulti, const VSDObjectTrackMulti* ipObjectTrackMulti, int isRedLightOn[MAX_VIRTUAL_LOOPS], EventMultiAPPResult* opResult)
{
	opResult->mNumOfResult = 0;
	if(!ipImage || !ipObjectMulti || !ipObjectTrackMulti)
	{
#ifdef __DEBUG
		TRACE("EventAPP::ProcessFrame input fault");
#endif
		return APP_INPUT_NULL_POINTER;
	}

	if (ipImage->nColorMode != CS_JPEG)
	{
#ifdef __DEBUG
		TRACE("EventAPP::ProcessFrame input fault");
#endif
		return APP_IMAGE_FORMAT_FAULT;
	}

	if (ipObjectMulti->nObjects <= 0)
	{
#ifdef __DEBUG
		TRACE("EventAPP::ProcessFrame number of input ojbect is zero");
#endif
		return APP_OK;
	}

	// �õ�mObject��Ӧ�ĸ����Ա
	int** pValue = (int**)mObject;
	EventAPPParam* pAPPParam = (EventAPPParam*)(*pValue);
	ImagePool* pPool = (ImagePool*)(*(pValue + 1));
	VSDRatioLine* laneMark = (VSDRatioLine*)(*(pValue + 2));
	int* pPoolLength =(int*)(*(pValue + 3));
	CaptureImageMap* pTouchStopLineImage = (CaptureImageMap*)(*(pValue + 4));
	StatusMap* pStatusMap = (StatusMap*)(*(pValue + 5));
	//int* pPoolStartCheckIndex = *(pValue + 6);	
	MediaConverter* pMediaConverter = (MediaConverter*)(*(pValue + 6));
	//SubtitleOverlay* pSubtitleOverlay = (SubtitleOverlay*)(*(pValue + 7));
	LPR* pLPR = (LPR*)(*(pValue + 8));
	PlateMap* pPlateMap = (PlateMap*)(*(pValue + 9));
	CaptureImageMap* pLeaveStopLineImage = (CaptureImageMap*)(*(pValue + 10));
	int* pImageWidth = *(pValue + 11);
	int* pImageHeight = *(pValue + 12);
	RectMap* pRectMap = (RectMap*)(*(pValue + 14));
	RuleIndexMap* pRuleIndexMap = (RuleIndexMap*)(*(pValue + 15));
	PriorityMap* pPriorityMap = (PriorityMap*)(*(pValue + 16));
	RecordMap* pRecordMap = (RecordMap*)(*(pValue + 17));
	CaptureImageMap* pTouchCentreLineImage = (CaptureImageMap*)(*(pValue + 18));
	CaptureImageMap* pTouchVirtualLoopLineImage = (CaptureImageMap*)(*(pValue + 19));
	int* pStartFrameIndex = (int*)(*(pValue + 20));
	TouchMap* pTouchMap = (TouchMap*)(*(pValue + 21));
	

	// �õ�VSDEventParam�Ĳ���
	VSDEventParam lVSDParam = pAPPParam->mVSDParam;
	LPRRESULT lResult = LPR_OK;	

	// ͨ������õ�Image�ĳ���ֵ
	if(pImageHeight == NULL || pImageWidth == NULL)
	{
		LPRImage* pDecodeImage = NULL;
		lResult = LPRDecodeImage(&pDecodeImage, (const unsigned char*)ipImage->pData, ipImage->imageSize, LPR_ENCODE_FORMAT_JPG, 0);
		if(lResult != LPR_OK)
		{
#ifdef __DEBUG
			TRACE("EventAPP::ProcessFrame Fail to decode image");
#endif
			LPRReleaseImage(pDecodeImage);
			return APP_FAIL;
		}
		pImageHeight = new int(pDecodeImage->height);
		pImageWidth = new int(pDecodeImage->width);
		*(pValue + 11) = pImageWidth;
		*(pValue + 12) = pImageHeight;
		LPRReleaseImage(pDecodeImage);
	}

	// ��ʼ��ÿ��������Ȧ������VSDRatioLine��ֻ��ʼ��һ��
	if(laneMark == NULL)
	{
		laneMark = new VSDRatioLine[MAX_VIRTUAL_LOOPS*2];
		VSDRatioPoint lTmpPoint;
		if (pImageHeight == NULL || pImageWidth == NULL)
		{
#ifdef __DEBUG
			TRACE("EventAPP::ProcessFrame image height and width has not been inited");
#endif
			return APP_FAIL;
		}
		lResult = VSDEvent_GenerateLaneMark(lVSDParam.ptRoad, lVSDParam.virtualLoopLine, lVSDParam.nVirtualLoop, *pImageWidth, *pImageHeight, lVSDParam.nWidthBase, lVSDParam.nHeightBase, laneMark, &lTmpPoint);
		if(lResult != LPR_OK)
		{
#ifdef __DEBUG
			TRACE("EventAPP::ProcessFrame fail to generat lane mark");
#endif
			return APP_FAIL;
		}
		*(pValue + 2) = (int*)laneMark;
		// ��Ϊ�õ���laneMark̫��������ֻȡ������Ȧ�ڵ�Line
		for (int index = 0; index < lVSDParam.nVirtualLoop; ++index)
		{
			VSDRatioPoint lTmpPoint;
			GetCrossRatioPoint(laneMark[2 * index], pAPPParam->mStopLine,lTmpPoint);
			laneMark[2 * index].pt1 = lTmpPoint;
			GetCrossRatioPoint(laneMark[2 * index + 1], pAPPParam->mStopLine,lTmpPoint);
			laneMark[2 * index + 1].pt1 = lTmpPoint;
		}
	}


	// ��ʼ��LPRģ�飬��ʶ������
	if (pLPR == NULL && pAPPParam->mPlateCaptureSwitch == EVENT_APP_PLATE_SWITCH_ON)
	{
		LPRParam lLPRParam;
		lLPRParam.size = sizeof(lLPRParam);
		lLPRParam.nMaxPlate = DEFAULT_NUMBER_PLATE;
		lLPRParam.nMinPlateWidth = DEFAULT_PLATE_MIN_WIDTH;
		lLPRParam.nMaxPlateWidth = DEFAULT_PLATE_MAX_WIDTH;
		lLPRParam.plateModel.plateType = PT_CAR_NORMAL | PT_BIG_NORMAL;
		lLPRParam.plateModel.plateCharType[0] = LR_China;			// ��0λ��ȫ������ģ�͡�
		lLPRParam.plateModel.plateCharType[1] = LR_Alpha;			// ��1λ��ʶ����ĸ
		lLPRParam.plateModel.plateCharType[2] = LR_DigitAlpha;	// ��2λ��ʶ��������ĸ
		lLPRParam.plateModel.plateCharType[3] = LR_DigitAlpha;	// ��3λ��ʶ��������ĸ
		lLPRParam.plateModel.plateCharType[4] = LR_Digit;			// ��4λ��ʶ������
		lLPRParam.plateModel.plateCharType[5] = LR_Digit;			// ��5λ��ʶ������
		lLPRParam.plateModel.plateCharType[6] = LR_Digit;			// ��6λ��ʶ������
		pLPR = new LPR;
		pLPR->Fini();
		lResult = pLPR->Init2(lLPRParam, *pImageWidth, *pImageHeight, true);
		if(lResult != LPR_OK)
		{
#ifdef __DEBUG
			TRACE("EventAPP::ProcessFram fail to init LPR");
#endif
			return APP_FAIL;
		}
		*(pValue + 8) = (int*)pLPR;
	}
	
	// �õ�ѹ����ֵ
	float crossRatio = (float)pAPPParam->mRatioToCrossLine / 100;

	// �������Rectת��Ϊ RatioRect
	VSDRatioRECT objectRatioRECT;
	if(pImageHeight == NULL || pImageWidth == NULL || *pImageWidth == 0 || *pImageHeight == 0)
	{
#ifdef __DEBUG
		TRACE("ͼƬ�ĸ�ʽ�����޷��õ�ͼƬ�ĳ���ֵ");
#endif
		return APP_IMAGE_FORMAT_FAULT;
	}
	float imageXRatio = lVSDParam.nWidthBase / (float)(*pImageWidth);
	float imageYRatio = lVSDParam.nHeightBase / (float)(*pImageHeight);
	
	int lObjectCount = ipObjectMulti->nObjects;
	VSDObject lObject;
	
	// ��ΪipImageΪ�ⲿ���룬���ǲ��ܱ�֤ͼƬ��ʱ���ͷţ�������Ҫ�Լ�����һ�ݵ�ImagePool
	LPRImage *lpImage = LPRCloneImage(ipImage);

	// ����PoolData 
	PoolData lPoolData;
	lPoolData.mpImage = lpImage;
	
	// ���������Image��ObjectMuli�е�ÿ�����壬���ÿ�������Υ���������¼��mBreakRules��
	for(int index = 0; index < lObjectCount; ++index)
	{
		lObject = ipObjectMulti->objects[index];
		// objectRatioRECT ת����ľ���
		ToRatioRECT(lObject.rect, imageXRatio, imageYRatio, objectRatioRECT);
		// ץȡ����Ӵ�ͣ���ߵ�ͼƬ
		TouchMap::iterator itTouchStopLine = pTouchMap->find(lObject.uid);
		if( itTouchStopLine == pTouchMap->end() && GetCrossRatio(pAPPParam->mStopLine, objectRatioRECT) > 0)
		{
			pTouchMap->insert(std::make_pair(lObject.uid, 1));
			CaptureImageMap::iterator itVirtualImage = pTouchStopLineImage->find(lObject.uid);
			if(itVirtualImage == pTouchStopLineImage->end())
			{
				LPRImage *pImage = LPRCloneImage(ipImage);
				pTouchStopLineImage->insert(std::make_pair(lObject.uid, pImage));
				RectMap::iterator itRect = pRectMap->find(lObject.uid); 
				if(itRect == pRectMap->end())
					pRectMap->insert(std::make_pair(lObject.uid, lObject.rect));
			}
			
		}

		// ץȡ�����뿪ͣ���ߵ�ͼƬ 
		if (itTouchStopLine != pTouchMap->end() && itTouchStopLine->second == 1)
		{
			if (GetCrossRatio(pAPPParam->mStopLine, objectRatioRECT) == 0)
			{
				itTouchStopLine->second = 2;
				CaptureImageMap::iterator itVirtualImage = pLeaveStopLineImage->find(lObject.uid);
				if(itVirtualImage == pLeaveStopLineImage->end())
				{
					LPRImage *pImage = LPRCloneImage(ipImage);
					pLeaveStopLineImage->insert(std::make_pair(lObject.uid, pImage));
				}

			}
		}
		/*
		int lObjectMiddleLine = objectRatioRECT.top + (objectRatioRECT.bottom - objectRatioRECT.top) / 2; 
		int lMiddleStopLine = (pAPPParam->mStopLine.pt1.y + pAPPParam->mStopLine.pt2.y ) /2;
		if(lMiddleStopLine >= objectRatioRECT.top && lMiddleStopLine <= lObjectMiddleLine)
		{
			if(lVSDParam.nEventType == VSDEvent_VehicleTail)
			{
				CaptureImageMap::iterator itVirtualImage = pTouchStopLineImage->find(lObject.uid);
				if(itVirtualImage == pTouchStopLineImage->end())
				{
					LPRImage *pImage = LPRCloneImage(ipImage);
					pTouchStopLineImage->insert(std::make_pair(lObject.uid, pImage));
					RectMap::iterator itRect = pRectMap->find(lObject.uid); 
					if(itRect == pRectMap->end())
						pRectMap->insert(std::make_pair(lObject.uid, lObject.rect));
				}
			}
			else
			{
				CaptureImageMap::iterator itVirtualImage = pLeaveStopLineImage->find(lObject.uid);
				if(itVirtualImage == pLeaveStopLineImage->end())
				{
					LPRImage *pImage = LPRCloneImage(ipImage);
					pLeaveStopLineImage->insert(std::make_pair(lObject.uid, pImage));
				}
			}
		}
		// ץȡ�����뿪ͣ���ߵ�ͼƬ 
		if(lMiddleStopLine <= objectRatioRECT.bottom && lMiddleStopLine >= lObjectMiddleLine)
		{
			if(lVSDParam.nEventType == VSDEvent_VehicleHead)
			{
				CaptureImageMap::iterator itVirtualImage = pTouchStopLineImage->find(lObject.uid);
				if(itVirtualImage == pTouchStopLineImage->end())
				{
					LPRImage *pImage = LPRCloneImage(ipImage);
					pTouchStopLineImage->insert(std::make_pair(lObject.uid, pImage));
					RectMap::iterator itRect = pRectMap->find(lObject.uid); 
					if(itRect == pRectMap->end())
						pRectMap->insert(std::make_pair(lObject.uid, lObject.rect));
				}
			}
			else
			{
				CaptureImageMap::iterator itVirtualImage = pLeaveStopLineImage->find(lObject.uid);
				if(itVirtualImage == pLeaveStopLineImage->end())
				{
					LPRImage *pImage = LPRCloneImage(ipImage);
					pLeaveStopLineImage->insert(std::make_pair(lObject.uid, pImage));
				}
			}
		}
		*/
		// ץȡ����Ӵ��м��ߵ�ͼƬ, 
		int lMiddlCentreLine = (pAPPParam->mCentreLine.pt1.y + pAPPParam->mCentreLine.pt2.y) / 2;
		if(lMiddlCentreLine <= objectRatioRECT.bottom && lMiddlCentreLine >= objectRatioRECT.top)
		{
			CaptureImageMap::iterator itVirtualImage = pTouchCentreLineImage->find(lObject.uid);
			if(itVirtualImage == pTouchCentreLineImage->end())
			{
				LPRImage* pImage = LPRCloneImage(ipImage);
				pTouchCentreLineImage->insert(std::make_pair(lObject.uid, pImage));
			}
		}
		// ץȡ�Ӵ�������Ȧ��ͼƬ����������ͼƬ���г���ʶ��
		if(lObject.status & VSD_OBJ_STATUS_TOUCH_LINE)
		{
			CaptureImageMap::iterator itVirtualImage = pTouchVirtualLoopLineImage->find(lObject.uid);
			if(itVirtualImage == pTouchVirtualLoopLineImage->end())
			{
				LPRImage* pImage = LPRCloneImage(ipImage);
				pTouchVirtualLoopLineImage->insert(std::make_pair(lObject.uid, pImage));
			}

			if(pAPPParam->mPlateCaptureSwitch == EVENT_APP_PLATE_SWITCH_ON)
			{
			PlateMap::iterator itPlateMap = pPlateMap->find(lObject.uid);
			if(itPlateMap == pPlateMap->end())
			{
				// ��ʼ���ֲ�������
				LPRParamLocal	localParam;
				// ʶ��������Ϊ��ǰ����ľ��ο���Ϊ������ο���ʱ�Ƚ�С��û�а����������ڵķ�Χ����Ϊ��������������Χ
				int lEnlargeWidth = lObject.rect.width / 2;
				int lEnlargeHeight = lObject.rect.height / 2;
				localParam.m_rectRegion.left = MaxT(lObject.rect.x - lEnlargeWidth, 0);
				localParam.m_rectRegion.right = MinT(lObject.rect.x + lObject.rect.width + lEnlargeWidth, *pImageWidth);
				localParam.m_rectRegion.top = MaxT(lObject.rect.y - lEnlargeHeight, 0);
				localParam.m_rectRegion.bottom = MinT(lObject.rect.y + lObject.rect.height + lEnlargeHeight, *pImageHeight);
				localParam.m_nMinPlateWidth = DEFAULT_PLATE_MIN_WIDTH; 
				localParam.m_nMaxPlateWidth = DEFAULT_PLATE_MAX_WIDTH;
				localParam.m_fltReserved0 = 0;
				localParam.m_fltReserved1 = 0;
				localParam.m_fltReserved2 = 0;
				localParam.m_fltReserved3 = 0;

				// ��ʼ��LPRParamMulti�ṹ
				LPRParamMulti	multiParam;
				LPRParamMulti_Init( &multiParam );
				LPRParamMulti_Add( &multiParam, localParam );

				// ��ʼ��LPROutputMulti�ṹ
				LPROutputMulti	multiOutput;
				LPROutputMulti_Init( &multiOutput );

				// ���ʶ����
				LPRImage* pDecodeImage = NULL;
				lResult = LPRDecodeImage(&pDecodeImage, (const unsigned char*)ipImage->pData, ipImage->imageSize, LPR_ENCODE_FORMAT_JPG, 0);
				if(lResult != LPR_OK)
				{
	#ifdef __DEBUG
					TRACE("EventAPP::ProcessFrame Fail to decode image");
	#endif
					LPRReleaseImage(pDecodeImage);
					return APP_FAIL;
				}
				lResult =pLPR->ProcessImage(pDecodeImage, &multiOutput, multiParam, NULL );
				if (lResult != LPR_OK)
				{
	#ifdef __DEBUG
					TRACE("EventAPP::ProcessFrame fail to recognize plate");
	#endif
					LPRReleaseImage(pDecodeImage);
					return APP_FAIL;
				}
				wchar_t* pPlateCharactor = new wchar_t[LPR_PLATE_STR_LEN];
				if(multiOutput.m_nNumOutputs > 0)
				{
					for (int i = 0; i < LPR_PLATE_STR_LEN; ++i)
					{
						pPlateCharactor[i] = multiOutput.m_outputs[0].wszRec[i];
					}
				}
				else
					pPlateCharactor[0] = '\0';
				pPlateMap->insert(std::make_pair(lObject.uid, pPlateCharactor));
				LPRReleaseImage(pDecodeImage);
			}
			}
		}
		int lObjectHistoryStatus = VSD_BR_NONE;
		int lMaxPriority = (*pPriorityMap)[lObject.uid];
		lPoolData.mBreakRules[lObject.uid] = VSD_BR_NONE;
		lPoolData.mLoop = lObject.nLoopID;
		// �ж��Ƿ񴳺��
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_RED_LIGHT]] == EVENT_APP_RULE_SWITCH_ON && isRedLightOn[lObject.nLoopID] == EVENT_APP_LIGHT_RED  && GetCrossRatio(pAPPParam->mStopLine, objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid  << "����ƣ�����" << lObject.nLoopID << std::endl; 
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_RED_LIGHT;
			lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[6]);
		}
		// �ж��Ƿ�ѹ����
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_CROSS_LANE]] == EVENT_APP_RULE_SWITCH_ON && GetCrossRatio(laneMark[2 * lObject.nLoopID], objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "ѹ��" << lObject.nLoopID  << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_CROSS_LANE;
			lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[4]);
		}
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_CROSS_LANE]] == EVENT_APP_RULE_SWITCH_ON && GetCrossRatio(laneMark[2 * lObject.nLoopID + 1], objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "ѹ��" << lObject.nLoopID  <<  std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_CROSS_LANE;
			lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[4]);
		}

		// �ж��Ƿ���Υ����ת
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_TURN_LEFT]] == EVENT_APP_RULE_SWITCH_ON && !(lVSDParam.loopLaneProperty[lObject.nLoopID] & VSD_LANE_TURN_LEFT) && GetCrossRatio(pAPPParam->mLeftTurnLine, objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ�����" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_TURN_LEFT;
			lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[1]);
		}
		
		// �ж��Ƿ�Υ����ת
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_TURN_RIGHT]] == EVENT_APP_RULE_SWITCH_ON && !(lVSDParam.loopLaneProperty[lObject.nLoopID] & VSD_LANE_TURN_RIGHT) && GetCrossRatio(pAPPParam->mRightTurnLine, objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ����ת" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_TURN_RIGHT;
			lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[2]);
		}
		
		// �ж��Ƿ�Υ��ֱ��
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_STRAIGHT_THROUGH]] == EVENT_APP_RULE_SWITCH_ON && !(lVSDParam.loopLaneProperty[lObject.nLoopID] & VSD_LANE_STRAIGHT) && GetCrossRatio(pAPPParam->mStraightLine, objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ��ֱ��" << lObject.nLoopID  << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_STRAIGHT_THROUGH;
			lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[3]);
		}
		// �ж��Ƿ�����
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_REVERSE]] == EVENT_APP_RULE_SWITCH_ON)// && lObject.status & VSD_BR_REVERSE)
		{
			VSDObjectTrack lObjectTrack;
			lObjectTrack.uid = -1;
			for (int i= 0; i < ipObjectTrackMulti->nObjects; ++i)
			{
				if (ipObjectTrackMulti->objTracks[i].uid == lObject.uid)
				{
					lObjectTrack = ipObjectTrackMulti->objTracks[i];
					break;
				}
			}
			if (lObjectTrack.uid != -1)
			{
				int lTrackNum = lObjectTrack.nTracks;
				if (lTrackNum > 1)
				{
					if(lVSDParam.nEventType == VSDEvent_VehicleHead && lObjectTrack.tracks[0].y - lObject.rect.height * (float)pAPPParam->mReverseRatio / 100 > lObjectTrack.tracks[lTrackNum - 1].y)
					{
#ifdef __DEBUG
						lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "����" << lObject.nLoopID << std::endl;
#endif
						lPoolData.mBreakRules[lObject.uid] |= VSD_BR_REVERSE;
						lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[5]);
					}
					else if(lVSDParam.nEventType == VSDEvent_VehicleTail && lObjectTrack.tracks[0].y + lObject.rect.height *(float) pAPPParam->mReverseRatio / 100 < lObjectTrack.tracks[lTrackNum - 1].y)
					{
#ifdef __DEBUG
						lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "����" << lObject.nLoopID << std::endl;
#endif
						lPoolData.mBreakRules[lObject.uid] |= VSD_BR_REVERSE;
						lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[5]);
					}
				}
			}
		}
		// �ж��Ƿ�Υ��ͣ��
		if(pAPPParam->mRuleSwitch[(*pRuleIndexMap)[VSD_BR_STOP]] == EVENT_APP_RULE_SWITCH_ON)// && lObject.status & VSD_BR_STOP)
		{
			int lObjectMiddleX = (objectRatioRECT.left + objectRatioRECT.right ) / 2;//lObject.rect.width / 2 + lObject.rect.x;
			int lObjectMiddleY = (objectRatioRECT.bottom + objectRatioRECT.top) / 2;// lObject.rect.height / 2 + lObject.rect.y;
			VSDRatioPoint lTmpPoint = {lObjectMiddleX, lObjectMiddleY};
			if(IsInRect(lTmpPoint, pAPPParam->mStopForbidRect))
			{
				VSDObjectTrack lObjectTrack;
				lObjectTrack.uid = -1;
				for (int i= 0; i < ipObjectTrackMulti->nObjects; ++i)
				{
					if (ipObjectTrackMulti->objTracks[i].uid == lObject.uid)
					{
						lObjectTrack = ipObjectTrackMulti->objTracks[i];
						break;
					}
				}
				if (lObjectTrack.uid != -1)
				{
					int lTrackNum = lObjectTrack.nTracks;
					if (lTrackNum > 1)
					{
						int lCurrentX = lObject.rect.width / 2 + lObject.rect.x;
						int lCurrentY = lObject.rect.height / 2 + lObject.rect.y;

						int lPreviousX = lObjectTrack.tracks[lTrackNum - 2].width / 2 + lObjectTrack.tracks[lTrackNum - 2].x;
						int lPreviousY = lObjectTrack.tracks[lTrackNum - 2].height / 2 + lObjectTrack.tracks[lTrackNum - 2].y;
						if ((lCurrentX - lPreviousX) * (lCurrentX - lPreviousX) + (lCurrentY - lPreviousY) * (lCurrentY - lPreviousY) < ((float)pAPPParam->mStopRatio / 100 * lObject.rect.width) * (pAPPParam->mStopRatio*lObject.rect.width))
						{
#ifdef __DEBUG
							lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ��ͣ��" << lObject.nLoopID << std::endl;
#endif
							lPoolData.mBreakRules[lObject.uid] |= VSD_BR_STOP;
							lMaxPriority = MaxT(lMaxPriority, pAPPParam->mRulePriority[7]);
						}
					}

				}
			}
		}
		(*pStatusMap)[lObject.uid] |= lPoolData.mBreakRules[lObject.uid];
		(*pPriorityMap)[lObject.uid] = lMaxPriority;
	}
	int lResultCount = 0;

	// �ѵ�ǰPoolDataѹ���β
	pPool->push_back(lPoolData);
	if (pPool->size() > (*pPoolLength + 1))
	{
		// �ͷŵ�����ͷ�Ѳ�����Ҫ¼��Ļ����ͼƬ
		PoolData lCheckPoolData = pPool->front();
		LPRReleaseImage(lCheckPoolData.mpImage);
		pPool->pop_front();
		lCheckPoolData = pPool->at(*pStartFrameIndex);
		// ������ǰpoolData�е�����object��Υ�¼�¼
		for(std::map<int, int>::iterator itObject = lCheckPoolData.mBreakRules.begin(); itObject != lCheckPoolData.mBreakRules.end(); ++itObject)
		{
			ConstructResult(itObject->second, VSD_BR_NONE, itObject->first, opResult, lResultCount);
			ConstructResult(itObject->second, VSD_BR_TURN_LEFT, itObject->first, opResult, lResultCount);
			ConstructResult(itObject->second, VSD_BR_TURN_RIGHT, itObject->first, opResult, lResultCount);
			ConstructResult(itObject->second, VSD_BR_STRAIGHT_THROUGH, itObject->first, opResult, lResultCount);
			ConstructResult(itObject->second, VSD_BR_CROSS_LANE, itObject->first, opResult, lResultCount);
			ConstructResult(itObject->second, VSD_BR_REVERSE, itObject->first, opResult, lResultCount);
			ConstructResult(itObject->second, VSD_BR_RED_LIGHT, itObject->first, opResult, lResultCount);
			ConstructResult(itObject->second, VSD_BR_STOP, itObject->first, opResult, lResultCount);
			
			// ���������Ҫ������
			int lShowUp = 0;
			for(int i = *pStartFrameIndex + 1; i < pPool->size(); ++i)
			{
				PoolData lTmpPoolData = pPool->at(i);
				if(lTmpPoolData.mBreakRules.find(itObject->first) != lTmpPoolData.mBreakRules.end())
				{
					lShowUp = 1;
					break;
				}
			}
			if(0 == lShowUp)
			{
				StatusMap::iterator itStatusMap = pStatusMap->find(itObject->first);
				if(itStatusMap != pStatusMap->end())
					pStatusMap->erase(itStatusMap);
				PriorityMap::iterator itPirorityMap = pPriorityMap->find(itObject->first);
				if (itPirorityMap != pPriorityMap->end())
					pPriorityMap->erase(itPirorityMap);
				PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
				if(itPlate != pPlateMap->end())
				{
					delete[] itPlate->second;
					pPlateMap->erase(itPlate);
				}
				RectMap::iterator itRect = pRectMap->find(itObject->first);
				if (itRect != pRectMap->end())
					pRectMap->erase(itRect);
				RecordMap::iterator itRecord = pRecordMap->find(itObject->first);
				if(itRecord != pRecordMap->end())
					pRecordMap->erase(itRecord);

				TouchMap::iterator itTouchStopLine = pTouchMap->find(itObject->first);
				if (itTouchStopLine != pTouchMap->end())
					pTouchMap->erase(itTouchStopLine);

				CaptureImageMap::iterator itVirtualLoopImage = pTouchStopLineImage->find(itObject->first);
				if (itVirtualLoopImage != pTouchStopLineImage->end())
				{
					LPRReleaseImage(itVirtualLoopImage->second);
					pTouchStopLineImage->erase(itVirtualLoopImage);
				}

				itVirtualLoopImage = pLeaveStopLineImage->find(itObject->first);
				if(itVirtualLoopImage != pLeaveStopLineImage->end())
				{
					LPRReleaseImage(itVirtualLoopImage->second);
					pLeaveStopLineImage->erase(itVirtualLoopImage);
				}

				itVirtualLoopImage = pTouchCentreLineImage->find(itObject->first);
				if(itVirtualLoopImage != pTouchCentreLineImage->end())
				{
					LPRReleaseImage(itVirtualLoopImage->second);
					pTouchCentreLineImage->erase(itVirtualLoopImage);
				}
				itVirtualLoopImage = pTouchVirtualLoopLineImage->find(itObject->first);
				if (itVirtualLoopImage != pTouchVirtualLoopLineImage->end())
				{
					LPRReleaseImage(itVirtualLoopImage->second);
					pTouchVirtualLoopLineImage->erase(itVirtualLoopImage);
				}
			}
		}
	}
	return APP_OK;
}

APPRESULT EventAPP::ConstructResult(int iObjectBreakRule, int iRuleType, int uid, EventMultiAPPResult* opResultMulti, int& orResultCount)
{
	// �õ�EventAPP��Աָ��
	int** pValue = (int**)mObject;
	EventAPPParam* pAPPParam = (EventAPPParam*)(*pValue);
	ImagePool* pPool = (ImagePool*)(*(pValue + 1));
	int* pPoolLength =(int*)(*(pValue + 3));
	CaptureImageMap* pTouchStopLineImage = (CaptureImageMap*)(*(pValue + 4));
	StatusMap* pStatusMap = (StatusMap*)(*(pValue + 5));
	//int* pPoolStartCheckIndex = *(pValue + 6);	
	PlateMap* pPlateMap = (PlateMap*)(*(pValue + 9));
	CaptureImageMap* pLeaveStopLineImage = (CaptureImageMap*)(*(pValue + 10));
	RectMap* pRectMap = (RectMap*)(*(pValue + 14));
	RuleIndexMap* pRuleIndexMap = (RuleIndexMap*)(*(pValue + 15));
	PriorityMap* pPriorityMap = (PriorityMap*)(*(pValue + 16)); 
	RecordMap* pRecordMap = (RecordMap*)(*(pValue + 17));
	CaptureImageMap* pTouchVirtualLoopLineImage = (CaptureImageMap*)(*(pValue + 19));
	int* pStartFrameIndex = (int*)(*(pValue + 20));


	// objects�Ƿ���Υ������������Ƿ�Ҫ¼��iRuleType
	if((iObjectBreakRule == iRuleType) && (pAPPParam->mRuleSwitch[(*pRuleIndexMap)[iRuleType]]))
	{
		// ��������ȼ����ߵ�breakrule��������ͬ��breakrule�������Ǽ򵥵ķ���
		if(pRecordMap->find(uid) != pRecordMap->end() || ((pAPPParam->mRulePriority)[(*pRuleIndexMap)[iRuleType]]) < (*pPriorityMap)[uid] || (iRuleType == VSD_BR_NONE && (*pStatusMap)[uid] != VSD_BR_NONE))
			return APP_OK;
		// ���û�����ȼ����ߵ�breakrule�����ǹ���EventAPPResult
		(*pRecordMap)[uid] = 1;
		EventAPPResult lAPPResult;
		lAPPResult.mID = uid;
		lAPPResult.mBreakRule = iRuleType;
		lAPPResult.mLoopID = pPool->at(*pStartFrameIndex).mLoop;
		// ���EventAPPResult�ĳ�����Ϣ
		PlateMap::iterator itPlate = pPlateMap->find(uid);
		if (itPlate != pPlateMap->end())
		{
			for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
				lAPPResult.mPlate[i] = itPlate->second[i];
		}
		else
			lAPPResult.mPlate[0] = '\0';
		// ���EventAPPResult�ĳ���λ�þ�����Ϣ
		RectMap::iterator itRect = pRectMap->find(uid);
		if(itRect != pRectMap->end())
				lAPPResult.mRect = itRect->second;
		else
		{
			lAPPResult.mRect.height = 0;
			lAPPResult.mRect.width  = 0;
			lAPPResult.mRect.x = 0;
			lAPPResult.mRect.y = 0;
		}
		// ���EventAPPResult��mVideoImage��Ϣ
		int lBeginIndex =  MaxT(*pStartFrameIndex - pAPPParam->mRecordParam.mBreakRuleAhead[(*pRuleIndexMap)[iRuleType]], 0);
		int lEndIndex = MinT(pAPPParam->mRecordParam.mBreakRuleBehind[(*pRuleIndexMap)[iRuleType]] + *pStartFrameIndex, *pPoolLength - 1);
		int lSizeToCopy = lEndIndex - lBeginIndex + 1;
		LPRImage* lpImage = NULL;
		for (int i = 0; i < lSizeToCopy; ++i )
		{
			lpImage = LPRCloneImage(pPool->at(i + lBeginIndex).mpImage);
			lAPPResult.mVideoImage[i] = lpImage;
		}
		lAPPResult.mNumOfVideoImage = lSizeToCopy;
		// ���EventAPPResult��mSynthesisImage��Ϣ
		int lSynthesisNum = 0;
		CaptureImageMap::iterator itVirtualLoopTouch = pTouchStopLineImage->find(uid);
		if(itVirtualLoopTouch != pTouchStopLineImage->end())
		{
			lpImage = LPRCloneImage(itVirtualLoopTouch->second);
			lAPPResult.mSynthesisImage[lSynthesisNum++] = lpImage;
		}
		CaptureImageMap::iterator itVirtualLoopLeave = pLeaveStopLineImage->find(uid);
		if(itVirtualLoopLeave != pLeaveStopLineImage->end())
		{
			lpImage = LPRCloneImage(itVirtualLoopLeave->second);
			lAPPResult.mSynthesisImage[lSynthesisNum++] = lpImage;
		}
		if(iRuleType == VSD_BR_RED_LIGHT)
			lpImage = LPRCloneImage(lAPPResult.mVideoImage[lSizeToCopy - 1]);
		else
			lpImage = LPRCloneImage(pPool->at(*pStartFrameIndex).mpImage);
		lAPPResult.mSynthesisImage[lSynthesisNum++] = lpImage;
		lAPPResult.mNumOfSynthesisImage = lSynthesisNum;
		// ���EventAPPResult��mPlateImage��Ϣ
		CaptureImageMap::iterator itPlateImage = pTouchVirtualLoopLineImage->find(uid);
		if(itPlateImage != pTouchVirtualLoopLineImage->end())
		{
			lpImage = LPRCloneImage(itPlateImage->second);
			lAPPResult.mPlateImage = lpImage;
		}
		else
			lAPPResult.mPlateImage = NULL;
		// �����EventAPPResult��������뵽opResultMulti��
		opResultMulti->mppAPPResult[orResultCount++] = lAPPResult;
		opResultMulti->mNumOfResult = orResultCount;
#ifdef __DEBUG
		lBreakOutputLog << "��" << lAPPResult.mID << "������" << lAPPResult.mPlate << "��Υ������" << iRuleType << endl;
#endif
	}
	return APP_OK;
}

EventAPP::~EventAPP()
{
	int** pValue = (int**)mObject;
	EventAPPParam* pAPPParam = (EventAPPParam*)(*pValue);
	ImagePool* pPool = (ImagePool*)(*(pValue + 1));
	VSDRatioLine* laneMark = (VSDRatioLine*)(*(pValue + 2));
	int* pPoolLength =(int*)(*(pValue + 3));
	CaptureImageMap* pTouchStopLineImage = (CaptureImageMap*)(*(pValue + 4));
	StatusMap* pStatusMap = (StatusMap*)(*(pValue + 5));
	MediaConverter* pMediaConverter = (MediaConverter*)(*(pValue + 6));
	//SubtitleOverlay* pSubtitleOverlay = (SubtitleOverlay*)(*(pValue + 7));
	LPR* pLPR = (LPR*)(*(pValue + 8));
	PlateMap* pPlateMap = (PlateMap*)(*(pValue + 9));
	CaptureImageMap* pLeaveStopLineImage = (CaptureImageMap*)(*(pValue + 10));
	int* pImageWidth = *(pValue + 11);
	int* pImageHeight = *(pValue + 12);
	ImageSynthesis* pImageSynthesis = (ImageSynthesis*)*(pValue + 13);
	RectMap* pRectMap = (RectMap*)(*(pValue + 14));
	RuleIndexMap* pRuleIndexMap = (RuleIndexMap*)(*(pValue + 15));
	PriorityMap* pPriorityMap = (PriorityMap*)(*(pValue + 16));
	RecordMap* pRecordMap = (RecordMap*)(*(pValue + 17));
	CaptureImageMap* pTouchCentreLineImage = (CaptureImageMap*)(*(pValue + 18));
	CaptureImageMap* pTouchVirtualLoopLineImage = (CaptureImageMap*)(*(pValue + 19));
	int* pStartFrameIndex = (int*)(*(pValue + 20));
	TouchMap* pTouchMap = (TouchMap*)(*(pValue + 21));


	delete pAPPParam;

	if(pPool != NULL)
	{
		while(!pPool->empty())
		{
			LPRReleaseImage(pPool->front().mpImage);
			pPool->pop_front();
		}
		delete pPool;
	}

	delete[] laneMark;

	delete pPoolLength;

	if(pTouchStopLineImage != NULL)
	{
		for(CaptureImageMap::iterator it = pTouchStopLineImage->begin(); it != pTouchStopLineImage->end(); ++it)
			LPRReleaseImage(it->second);
		delete pTouchStopLineImage;
	}

	delete pStatusMap;
	delete pMediaConverter;
	
	if(pLPR != NULL)
	{
		pLPR->Fini();
		delete pLPR;
	}

	if(pPlateMap != NULL)
	{
		for (PlateMap::iterator it = pPlateMap->begin(); it != pPlateMap->end(); ++it)
		{
			delete[] it->second;
		}
		delete pPlateMap;
	}

	if(pLeaveStopLineImage != NULL)
	{
		for(CaptureImageMap::iterator it = pLeaveStopLineImage->begin(); it != pLeaveStopLineImage->end(); ++it)
			LPRReleaseImage(it->second);
		delete pLeaveStopLineImage;
	}


	delete pImageHeight;
	delete pImageWidth;
	delete pImageSynthesis;
	delete pRectMap;
	delete pRuleIndexMap;
	delete pPriorityMap;
	delete pRecordMap;


	if(pTouchCentreLineImage != NULL)
	{
		for(CaptureImageMap::iterator it = pTouchCentreLineImage->begin(); it != pTouchCentreLineImage->end(); ++it)
			LPRReleaseImage(it->second);
		delete pTouchCentreLineImage;
	}

	if (pTouchVirtualLoopLineImage != NULL)
	{
		for(CaptureImageMap::iterator it = pTouchVirtualLoopLineImage->begin(); it != pTouchVirtualLoopLineImage->end(); ++it)
			LPRReleaseImage(it->second);
		delete pTouchVirtualLoopLineImage;
	}
	delete pStartFrameIndex;
	delete pTouchMap;

}

APPRESULT EventAPP::AddSubTitle(LPRImage* ipImage, const wchar_t* ipString, const EventSubtitleImages* ipSubtitleImages, LPRImage** oppImage)
{
	if (ipImage == NULL || ipString == NULL || ipSubtitleImages == NULL || oppImage == NULL)
	{
#ifdef __DEBUG
		TRACE("EventAPP::AddSubTitle input null pointer");
#endif
		return APP_INPUT_NULL_POINTER;
	}
	int** pValue = (int**)mObject;
	EventAPPParam* pEventParam = (EventAPPParam*)(*pValue);
	//SubtitleOverlay* pSubtitleOverlay;
	//*oppImage = pSubtitleOverlay->overlaySubtitle(ipImage, ipString, pEventParam->mFont);
	*oppImage = LPROverlaySubtitle(ipImage, ipString, pEventParam->mFont, ipSubtitleImages);
	if(*oppImage == NULL)
		return APP_FAIL;
	return APP_OK;
}

APPRESULT EventAPP::SynthesisImages(LPRImage** ipImage, int iNumOfImages, const VSDRect& irRect, LPRImage** oppImage)
{
	if (ipImage == NULL || oppImage == NULL)
	{
#ifdef __DEBUG
		TRACE("EventAPP::SynthesisImages input null pointer");
#endif
		return APP_INPUT_NULL_POINTER;
	}

	int** pValue = (int**)mObject;
	EventAPPParam* pEventParam = (EventAPPParam*)(*pValue);
	ImageSynthesis* pImageSynthesis = (ImageSynthesis*)(*(pValue + 13));
	pEventParam->mImageSynthesis.mNumberofImage = iNumOfImages;
	*oppImage = pImageSynthesis->synthesis(ipImage, pEventParam->mImageSynthesis, irRect);
	if (*oppImage == NULL)
		return APP_FAIL;
	return APP_OK;
}

APPRESULT EventAPP::Convert2Media(LPRImage** ipImage, int iNumOfImages, EventMedia& orMedia)
{
	if (ipImage == NULL)
	{
#ifdef __DEBUG 
		TRACE("EventAPP::Convert2Media input null pointer");
#endif
	}
	int** pValue = (int**)mObject;
	MediaConverter* pMediaConverter = (MediaConverter*)(*(pValue + 6));
	bool ret = pMediaConverter->imgs2media(ipImage, iNumOfImages, orMedia);
	if(!ret)
		return APP_FAIL;
	return APP_OK;
}

 void __stdcall FreeAPPResult(EventAPPResult* ipAPPResult)
{
	if (!ipAPPResult)
	{
#ifdef __DEBUG
		TRACE("FreeAPPResult fault input param");
#endif
	}

	for(int i = 0; i < ipAPPResult->mNumOfVideoImage; ++i)
		LPRReleaseImage(ipAPPResult->mVideoImage[i]);

	for(int i = 0; i < ipAPPResult->mNumOfSynthesisImage; ++i)
		LPRReleaseImage(ipAPPResult->mSynthesisImage[i]);
	if (ipAPPResult->mPlateImage != NULL)
		LPRReleaseImage(ipAPPResult->mPlateImage);
}

void __stdcall FreeEventMedia(EventMedia *pMedia)
{
	av_free(pMedia->mBufferPtr);
}

void __stdcall FreeMultiAPPResult(EventMultiAPPResult* ipMultiAPPResult)
{
	if (!ipMultiAPPResult)
	{
#ifdef __DEBUG
		TRACE("FreeAPPResult fault input param");
#endif
	}
	for(int i = 0; i < ipMultiAPPResult->mNumOfResult; ++i)
	{
		FreeAPPResult(&(ipMultiAPPResult->mppAPPResult[i]));
	}
}

// ȥ��stringǰ�������Ŀո���Ʊ��
void __stdcall MyStrim(std::string& toTrimed)
{
	int start = 0;
	int end = toTrimed.size() - 1;
	while( start <= end && (toTrimed.at(start) == ' '|| toTrimed.at(start) == '\t'))
		++start;
	
	while( end > start && (toTrimed.at(end) == ' '|| toTrimed.at(end) == '\t'))
		--end;
	toTrimed = toTrimed.substr(start, end - start + 1);
}

static int str2wstr(wchar_t *dst, const char *src, int srcSize)
{
	int cchWideChar = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)src, srcSize, 0, 0);
	if(cchWideChar < 0) return false;
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)src, srcSize, dst, cchWideChar);
	dst[cchWideChar] = L'\0';
	return cchWideChar;
}

/*

static int wstr2str(const char *drc, wchar_t *src, int srcSize)
{
	int cchWideChar = WideCharToMultiByte(CP_ACP, 0, (LPCSTR)src, srcSize, 0, 0);
	if(cchWideChar < 0) return false;
	WideCharToMultiByte(CP_ACP, 0,(LPCSTR)src, srcSize, dst, cchWideChar);
	dst[cchWideChar] = L'\0';
	return cchWideChar;
}
*/

typedef std::map<std::string, std::string> KeyValue;

template<typename T>
APPRESULT __stdcall CheckAndSetValue(const KeyValue& irKeyValue, const std::string& irKey, T iMinVlaue, T iMaxValue, T& orValue)
{
	KeyValue::const_iterator it = irKeyValue.find(irKey);
	if (it == irKeyValue.end())
	{
#ifdef __DEBUG
		TRACE("EventAPP_LoadParam ��������ļ���ʽ����");
#endif
		return APP_CONFIG_PARAM_FAULT;
	}
	else
	{
		std::stringstream lValue(it->second);
		T number = -1;
		lValue >> number;
		if (number < iMinVlaue || number > iMaxValue)
		{
#ifdef __DEBUG
			TRACE("EventAPP_LoadParam ��������ļ���ʽ����");
#endif
			return APP_CONFIG_PARAM_FAULT;
		}
		orValue = number;
	}
	return APP_OK;
}

APPRESULT __stdcall EventAPP_LoadParam(const char* ipFileName, EventAPPParam* ipEventParam)
{
	if (!ipFileName || !ipEventParam)
	{
#ifdef __DEBUG
		TRACE("EventAPP_LoadParam input fault");
#endif
		return APP_INPUT_NULL_POINTER;
	}
	std::ifstream ifs(ipFileName);
	std::string lineString;
	KeyValue keyValue;
	string::size_type nPosition;
	string keyString;
	string valueString;
	while(std::getline(ifs,lineString))
	{
		MyStrim(lineString);
		if(lineString.empty() || lineString[0] == '#')
			continue;
		nPosition = lineString.find('=');
		if(nPosition == std::string::npos || nPosition == 0)
		{
#ifdef __DEBUG
			TRACE("EventAPP_LoadParam ��������ļ���ʽ����");
#endif
			return APP_CONFIG_PARAM_FAULT;
		}
		keyString = lineString.substr(0, nPosition);
		MyStrim(keyString);
		if(keyString.empty())
		{
#ifdef __DEBUG
			TRACE("EventAPP_LoadParam ��������ļ���ʽ����");
#endif
			return APP_CONFIG_PARAM_FAULT;
		}
		valueString = lineString.substr(nPosition + 1, lineString.size() - nPosition - 1);
		MyStrim(valueString);
		if(valueString.empty())
		{
#ifdef __DEBUG
			TRACE("EventAPP_LoadParam ��������ļ���ʽ����");
#endif
			return APP_CONFIG_PARAM_FAULT;
		}
		keyValue[keyString] = valueString;
	}

	LPRRESULT lResult = LPR_OK;
	// ��ʼ��EventAPPParam.mVSDEventParam
	KeyValue::iterator it = keyValue.find("VSDEvent_Param");
	if (it == keyValue.end())
	{
#ifdef __DEBUG
			TRACE("EventAPP_LoadParam ��������ļ���ʽ����");
#endif
			return APP_CONFIG_PARAM_FAULT;
	}
	else
	{
		VSDEventParam lVSDEventParam;
		lResult = VSDEvent_LoadParam(it->second.c_str(), &lVSDEventParam);
		if (lResult != LPR_OK)
		{
#ifdef __DEBUG
			TRACE("EventAPP_LoadParam ��������ļ���ʽ����");
#endif
			return APP_VSD_PARAM_FAULT;
		}
		ipEventParam->mVSDParam = lVSDEventParam;
	}

	// ��ʼ����ת��p1���x����
	APPRESULT lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt1.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mLeftTurnLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//��ʼ����ת��p1���y����
	lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt1.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mLeftTurnLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt2.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mLeftTurnLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt2.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mLeftTurnLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// 
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt1.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mRightTurnLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt1.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mRightTurnLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt2.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mRightTurnLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt2.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mRightTurnLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// 
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt1.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStraightLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt1.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStraightLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt2.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStraightLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt2.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStraightLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	
	lAPPResult = CheckAndSetValue(keyValue, "StopLine.pt1.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStopLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "StopLine.pt1.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStopLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "StopLine.pt2.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStopLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "StopLine.pt2.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStopLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "CentreLine.pt1.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mCentreLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "CentreLine.pt1.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mCentreLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "CentreLine.pt2.x", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mCentreLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "CentreLine.pt2.y", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mCentreLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "StopForbidRect.left", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStopForbidRect.left);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "StopForbidRect.top", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStopForbidRect.top);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "StopForbidRect.right", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStopForbidRect.right);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "StopForbidRect.bottom", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStopForbidRect.bottom);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// ��ʼ��ѹ����ֵ
	lAPPResult = CheckAndSetValue(keyValue, "CrossRatio", 0, 100, ipEventParam->mRatioToCrossLine);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "ReverseRatio", 0, 100, ipEventParam->mReverseRatio);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "StopRatio", 0, 100, ipEventParam->mStopRatio);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// ��ʼ������rule��¼��֡��Χ
	/*
	lAPPResult = CheckAndSetValue(keyValue, "RecordBreakNoneFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[0]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordBreakNoneFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[0]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
		*/
	ipEventParam->mRecordParam.mBreakRuleAhead[0] = 0;
	ipEventParam->mRecordParam.mBreakRuleBehind[0] = 0;

	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnLeftFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[1]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnLeftFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[1]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnRightFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[2]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnRightFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[2]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordStraightFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[3]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordStraightFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[3]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordCrossLineFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[4]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordCrossLineFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[4]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordReverseFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[5]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordReverseFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[5]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordRedLightFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[6]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordRedLightFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[6]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordStopFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[7]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordStopFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[7]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordHighSpeedFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[8]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordHighSpeedFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[8]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordLowSpeedFramAhead", 0, MAX_FRAME_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[9]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordLowSpeedFramBehind", 0, MAX_FRAME_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[9]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "VideoFormat", 0, 2, ipEventParam->mRecordParam.mVideoFormat);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "VideoFramFrequent", 0, 20, ipEventParam->mRecordParam.mFrameFrequent);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "VideoBitFrequent", 0, 1000000000, ipEventParam->mRecordParam.mBitFrequent);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "FontSize", 0, 10000, ipEventParam->mFont.mFontSize);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "FontFamily", 0, 1, ipEventParam->mFont.mFontFamily);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "FontRGB_R", 0, 256, ipEventParam->mFont.mFontRGB_R); 
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "FontRGB_G", 0, 256, ipEventParam->mFont.mFontRGB_G); 
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "FontRGB_B", 0, 256, ipEventParam->mFont.mFontRGB_B); 
	if (lAPPResult != APP_OK)
		return lAPPResult;


	lAPPResult = CheckAndSetValue(keyValue, "FontOrientation", 0, 2, ipEventParam->mFont.mFontOrientation);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "FontX", 0, ipEventParam->mVSDParam.nWidthBase, ipEventParam->mFont.mFontX);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "FontY", 0, ipEventParam->mVSDParam.nHeightBase, ipEventParam->mFont.mFontY);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "ImageSynthesisNum", 0, 10, ipEventParam->mImageSynthesis.mNumberofImage);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "ImageSynthesisOrientation", 0, 3, ipEventParam->mImageSynthesis.mPicOrientation);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "ImageSynthesisZoom", 0, 100, ipEventParam->mImageSynthesis.mZoomRatio);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "PlateCaptureSwitch", 0, 1, ipEventParam->mPlateCaptureSwitch);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakNoneSwitch", 0, 1, ipEventParam->mRuleSwitch[0]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakTurnLeftSwitch", 0, 1, ipEventParam->mRuleSwitch[1]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakTurnRightSwitch", 0, 1, ipEventParam->mRuleSwitch[2]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakStraightSwitch", 0, 1, ipEventParam->mRuleSwitch[3]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakCrossSwitch", 0, 1, ipEventParam->mRuleSwitch[4]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakReverseSwitch", 0, 1, ipEventParam->mRuleSwitch[5]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakRedLightSwitch", 0, 1, ipEventParam->mRuleSwitch[6]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakStopSwitch", 0, 1, ipEventParam->mRuleSwitch[7]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakHighSpeedSwitch", 0, 1, ipEventParam->mRuleSwitch[8]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakLowSpeedSwitch", 0, 1, ipEventParam->mRuleSwitch[9]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	/*
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakNonePriority", 0, 9, ipEventParam->mRulePriority[0]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
		*/

	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakTurnLeftPriority", 0, 9, ipEventParam->mRulePriority[1]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakTurnRightPriority", 0, 9, ipEventParam->mRulePriority[2]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakStraightPriority", 0, 9, ipEventParam->mRulePriority[3]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakCrossPriority", 0, 9, ipEventParam->mRulePriority[4]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakReversePriority", 0, 9, ipEventParam->mRulePriority[5]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakRedLightPriority", 0, 9, ipEventParam->mRulePriority[6]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakStopPriority", 0, 9, ipEventParam->mRulePriority[7]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakHighSpeedPriority", 0, 9, ipEventParam->mRulePriority[8]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RuleBreakLowSpeedPriority", 0, 9, ipEventParam->mRulePriority[9]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	return APP_OK;
}


static bool CmpFileName(const string& a, const string& b)
{
	return a.length() == b.length() ? a < b : a.length() < b.length();
}




void EmumAllJPGFileInFolder(std::wstring folder, std::vector<std::wstring>& files)
{
	wstring imgPath = folder;
	folder += L"\\*.*";
	WIN32_FIND_DATA		findData;
	//wchar_t pFolder[256];
	//str2wstr(pFolder, folder.c_str(), folder.size());
	HANDLE  hFirstFile = ::FindFirstFile(folder.c_str(), &findData );
	if( hFirstFile != INVALID_HANDLE_VALUE )
	{
		do
		{
			wstring fileName( findData.cFileName );
			int pos = fileName.find_last_of(L'.' );
			if( pos != -1 )
			{
				wstring ext = fileName.substr(pos+1, fileName.length()-pos);
				/// Ѱ��jpeg�ļ�
				if( ext == L"jpg" || ext == L"JPG" || ext == L"jpeg" || ext == L"JPEG")
				{
					files.push_back( imgPath + L"\\" + findData.cFileName );
				}
			}

		}while( FindNextFile( hFirstFile, &findData ) );

		::FindClose( hFirstFile );
	}
	//std::sort(files.begin(), files.end(), CmpFileName);
}

int main_b()
{
	ifstream ifs("F:\\EventData\\images\\120327\\0000000016.jpg", std::ios::binary);
	int nJpgLen = 0;
	char *pJpgBuf = NULL;
	if ( ifs.is_open() )
	{
		ifs.seekg( 0 , std::ios::end );
		nJpgLen = ifs.tellg();

		if ( nJpgLen > 0 )
		{
			pJpgBuf = new char [nJpgLen];
			ifs.seekg( 0 , std::ios::beg );
			ifs.read((char *)pJpgBuf, nJpgLen);
		}
		ifs.close();
	}
	std::string dir("F:\\EventData\\images\\stop\\");
	ofstream ofs;
	char buf[256];
	for (int i = 0; i < 2000; ++i)
	{
		_snprintf(buf, 256, "%s%d.jpg", dir.c_str(), i);
		ofs.open(buf, std::ios::binary);
		ofs.write((char*)pJpgBuf, nJpgLen);
		ofs.close();
	}
	return 0;
}
int main(int argc, char *argv[])
{
	VSDEventParam lParam;	
	VSDEvent_LoadParam("F:\\EventData\\images\\120327\\VSDEvent.ini", &lParam);
	VSDEvent lEvent;
	LPRRESULT l = lEvent.Init(lParam);
	EventAPP lEventApp;
	EventAPPParam lAPPParam;
	//////////////////////////////////////////////////////////////////////////
	EventAPP_LoadParam("F:\\EvenAPP_Param.ini", &lAPPParam);
	lEventApp.Init(lAPPParam);
	std::wstring fileDir(L"F:\\EventData\\images\\120327");
	std::vector<wstring> lFiles;
	EmumAllJPGFileInFolder(fileDir, lFiles);
	wstring fileName;
	time_t tBegin;
	tBegin = time(NULL);
	//////////////////////////////////////////////////////////////////////////
	int x = 0;
	EventSubtitleImages* pEventSubtitleIamge;
	int font[2] = {0,1};
	pEventSubtitleIamge = LPRGenerateCharacterImagesDat(L"1234567890km", font, 2, 128);
	for(vector<wstring>::iterator it = lFiles.begin(); it != lFiles.end(); ++it)
	{
		x++;
		fileName = *it;
		lCurrentPicName = *it;
		std::ifstream ifs(fileName.c_str(), std::ios::binary);
		int nJpgLen = 0;
		char *pJpgBuf = NULL;
		if ( ifs.is_open() )
		{
			ifs.seekg( 0 , std::ios::end );
			nJpgLen = ifs.tellg();

			if ( nJpgLen > 0 )
			{
				pJpgBuf = new char [nJpgLen];
				ifs.seekg( 0 , std::ios::beg );
				ifs.read((char *)pJpgBuf, nJpgLen);
			}
			ifs.close();
		}

		LPRImage imgJPG;
		imgJPG.nColorMode = CS_JPEG;
		imgJPG.pData = (unsigned char *)pJpgBuf;
		imgJPG.imageSize = nJpgLen;
		imgJPG.info.nCamID = 0;

		EventMultiAPPResult lAPPResult;
		VSDObjectMulti lObjectMulti;
		LPRRESULT lResult = lEvent.ProcessFrame(&imgJPG, &lObjectMulti);
		VSDObjectTrackMulti lObjectTrackMulti;
		VSDObjectTrackMulti_Init(&lObjectTrackMulti);
		int lLights[MAX_VIRTUAL_LOOPS] = {0, 0, 0, 0};
		lResult = lEvent.GetAllTracks(&lObjectTrackMulti);
		lEventApp.ProcessFrame(&imgJPG,&lObjectMulti, &lObjectTrackMulti, lLights, &lAPPResult);
		wchar_t resultFileName[256];
		char* buf;
		ofstream ofs;
		
		
		for(int i = 0; i < lAPPResult.mNumOfResult; ++i)
		{
			EventAPPResult lResult = lAPPResult.mppAPPResult[i];
			if (lResult.mBreakRule == VSD_BR_NONE)
			{
				if (lResult.mNumOfSynthesisImage > 0)
				{
					//wstr2str(lResult.mPlate, plateName);
					swprintf(resultFileName,256, L"F:\\resultdata\\%d_%S.jpg", lResult.mID, lResult.mPlate);
					ofs.open(resultFileName, std::ios::binary);
					ofs.write((const char*)lResult.mSynthesisImage[0]->pData, lResult.mSynthesisImage[0]->imageSize);
					ofs.close();
				}
			}
			else
			{
				if (lResult.mNumOfSynthesisImage == 3)
				{
					swprintf(resultFileName,256, L"F:\\resultdata\\%d_%S_%d.jpg", lResult.mID, lResult.mPlate, lResult.mBreakRule);
					ofs.open(resultFileName, std::ios::binary);
					LPRImage* lpImage = NULL;
					APPRESULT retCode = lEventApp.SynthesisImages(lResult.mSynthesisImage, lResult.mNumOfSynthesisImage, lResult.mRect, &lpImage);
					buf = new char[10 * nJpgLen]; 
					int length;
					LPREncodeImage(lpImage, (unsigned char*)buf, &length,LPR_ENCODE_FORMAT_JPG, 40);
					//buftitle = new char[10 * nJpgLen];
					LPRImage* pSubtitleImage = new LPRImage;

					pSubtitleImage->pData = (unsigned char*) buf;
					pSubtitleImage->imageSize = length;
					LPRImage* lpImageSubtile;
					lEventApp.AddSubTitle(pSubtitleImage,L"12345",pEventSubtitleIamge, &lpImageSubtile);
					LPREncodeImage(lpImageSubtile, (unsigned char*)buf, &length,LPR_ENCODE_FORMAT_JPG, 80);
					ofs.write((const char*)buf, length);
					ofs.close();
					delete[] buf;
					LPRReleaseImage(lpImage);
					LPRReleaseImage(lpImageSubtile);
					delete pSubtitleImage;
				}
				EventMedia lMedia;
				lEventApp.Convert2Media(lResult.mVideoImage, lResult.mNumOfVideoImage, lMedia);
				swprintf(resultFileName,256, L"F:\\resultdata\\%d_%S_%d.avi", lResult.mID, lResult.mPlate, lResult.mBreakRule);
				ofs.open(resultFileName, std::ios::binary);
				ofs.write((const char*)lMedia.mBufferPtr, lMedia.mBufferSize);
				ofs.close();
				//delete[] lMedia.mBufferPtr;
				FreeEventMedia(&lMedia);
			}
		}
		delete[] pJpgBuf;
		FreeMultiAPPResult(&lAPPResult);
	}
	time_t tEnd = time(NULL);
	cout << "����" << (tEnd - tBegin) / 60 << "��" << (tEnd - tBegin)%60 << "��" << endl;

	int i;
	std::cin >> i;
	return 0;
}
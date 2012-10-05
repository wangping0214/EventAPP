//*********************************************************
//This file defines the basic interface of EventAPP
//Author:WK
//Data:2012.09.22`
//**********************************************************
#define __DEBUG
#include "EventAPP.h"
#include "MediaConverter.h"
#include "SubtitleOverlay.h"
#include <cstring>
#include <deque>
#include <map>
#include <list>
#include <fstream>
#include <sstream>
#include <LPRVideo.h>
#include <LPR.h>



using namespace std;

#if defined(WIN32)
#define Int64 __int64
#elif  defined(LINUX)
#define Int64 long long
#else
#error "No platform specified!"
#endif

#ifdef __DEBUG

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>

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
static inline void __stdcall ToRatioRECT(const VSDRect& irRect, double iXRatio, double iYRatio, VSDRatioRECT& orRECT)
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
	
	double t = (double)(area_cda) / (area_abd - area_abc);
	orPointC.x = int(a.x + t * (b.x - a.x));
	orPointC.y = a.y + t * (b.y - a.y);
	return true;
}


static inline bool __stdcall IsSame(const VSDRatioPoint& irPointA, const VSDRatioPoint& irPointB)
{
	return irPointA.x == irPointB.x && irPointA.y == irPointB.y;
}

static double __stdcall GetCrossRatio(const VSDRatioLine& irLine, const VSDRatioRECT& irRECT)
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
	double middleX =  (double(crossPoint[0].x) + crossPoint[1].x) / 2;
	double middleY =  (double(crossPoint[0].y) + crossPoint[1].y) / 2;
	
	double areaA = (middleX - irRECT.left) * ( middleY - irRECT.top);
	double areaB = (irRECT.right - middleX) * (middleY - irRECT.top );
	double areaC = (irRECT.right - middleX) * (irRECT.bottom- middleY);
	double areaD = (middleX - irRECT.left) * (irRECT.bottom - middleY);

	double minArea = MinT(MinT(areaA,areaB), MinT(areaC, areaD));
	
	double areaRect = (irRECT.bottom - irRECT.top) * (irRECT.right - irRECT.left);
	return minArea * 4 / areaRect;
}




struct PoolData
{
	std::map<int, int> mBreakRules;
	LPRImage* mpImage;
};


typedef std::deque<PoolData> ImagePool;
typedef std::map<int, LPRImage*> VirtualLoopImagePool;
typedef std::map<int, int> StatusMap;
typedef std::map<int, wchar_t*> PlateMap; 
typedef std::list<EventAPPResult> ResultList;

EventAPP::EventAPP()
{
	mObject = new int*[12];
}

APPRESULT EventAPP::Init(const EventAPPParam& irParam)
{
	int** pValue = (int**)mObject;
	*pValue = (int*)new EventAPPParam(irParam);								// EventAPP�ĵ�һ����ԱΪEventAPPParam
	*(pValue + 1) = (int*)new ImagePool;									// EventAPP�ĵڶ�����ԱΪһ��ImagePool������ͼƬ�����ϳ���Ƶ��ѡȡδΥ����ĸ��
	*(pValue + 2) = NULL;													// EventAPP�ĵ�������ԱΪÿ��������Ȧ��lanemark	
	int maxAhead = irParam.mRecordParam.mBreakRuleAhead[0];
	int maxBehind = irParam.mRecordParam.mBreakRuleBehind[0];
	for(int i = 1; i < RULE_TYPES; ++i)
	{
		maxAhead = MaxT(maxAhead, irParam.mRecordParam.mBreakRuleAhead[i]);
		maxBehind = MaxT(maxBehind, irParam.mRecordParam.mBreakRuleBehind[i]);
	}
	*(pValue + 3) = new int(maxAhead + maxBehind);							// EventAPP�ĵ�������Ա����ImagePool����󳤶�
	*(pValue + 4) = (int*)new VirtualLoopImagePool;							// EventAPP�ĵ������Ա����ÿ����������ͣ���߸�������Ƭ
	*(pValue + 5) = (int*)new StatusMap;									// EventAPP�ĵ�������Ա����Ӹ��ٿ�ʼ����������ʷbreakrule״̬�ĵ���ֵ
	*(pValue + 6) = (int*)new StatusMap;									// EventAPP�ĵ��߸���Ա����ÿ�������ĳ��breakrule�Ƿ��Ѿ�¼�ƹ���Ƶ����ֹ¼����ͬ�������ͬΥ��
	*(pValue + 7) = (int*)new MediaConverter((EventAPPViedoFormat)irParam.mRecordParam.mViedoFormat);  // EventAPP�ĵڰ˸���Ա����MediaConvertoer������¼����Ƶ
	SubtitleOverlay* pSubTitleOverlay = &SubtitleOverlay::getInstance();
	pSubTitleOverlay->initialize("0123456789km", 40, 48, 48);
	*(pValue + 8) = (int*)pSubTitleOverlay;					                // EventAPP�ĵھŸ���Ա����������ĸ
	*(pValue + 9) = NULL;													// EventAPP�ĵ�ʮ����Ա����ʶ����
	*(pValue + 10) = (int*)new PlateMap;									// EventAPP�ĵ�ʮһ����Ա���˱���ÿ�����ĳ��ƺ�
	*(pValue + 11) = (int*)new VirtualLoopImagePool;						// EventAPP�ĵ������Ա����ÿ����������ͣ���߸�������Ƭ
	return APP_OK;
}

APPRESULT EventAPP::ProcessFram(LPRImage *ipImage, const VSDObjectMulti* ipObjectMulti,int isRedLightOn[MAX_VIRTUAL_LOOPS], EventMultiAPPResult* opResult)
{
	opResult->mNumOfResult = 0;
	if(!ipImage || !ipObjectMulti)
	{
#ifdef __DEBUG
		TRACE("EventAPP::ProcessFram input fault");
#endif
		return APP_INPUT_NULL_POINTER;
	}

	if (ipImage->nColorMode != CS_JPEG)
	{
#ifdef __DEBUG
		TRACE("EventAPP::ProcessFram input fault");
#endif
		return APP_IMAGE_FORMAT_FAULT;
	}

	if (ipObjectMulti->nObjects <= 0)
	{
#ifdef __DEBUG
		TRACE("EventAPP::ProcessFram number of input ojbect is zero");
#endif
		return APP_OK;
	}

	// �õ�mObject��Ӧ�ĸ����Ա
	int** pValue = (int**)mObject;
	EventAPPParam* pAPPParam = (EventAPPParam*)(*pValue);
	ImagePool* pPool = (ImagePool*)(*(pValue + 1));
	VSDRatioLine* laneMark = (VSDRatioLine*)(*(pValue + 2));
	int* pPoolLength =(int*)(*(pValue + 3));
	VirtualLoopImagePool* pVirtualLoopImage = (VirtualLoopImagePool*)(*(pValue + 4));
	StatusMap* pStatusMap = (StatusMap*)(*(pValue + 5));
	StatusMap* pRemoveStatusMap = (StatusMap*)(*(pValue + 6));
	MediaConverter* pMediaConverter = (MediaConverter*)(*(pValue + 7));
	SubtitleOverlay* pSubtitleOverlay = (SubtitleOverlay*)(*(pValue + 8));
	LPR* pLPR = (LPR*)(*(pValue + 9));
	PlateMap* pPlateMap = (PlateMap*)(*(pValue + 10));
	VirtualLoopImagePool* pVirtualLoopLeaveImage = (VirtualLoopImagePool*)(*(pValue + 11));

	// �õ�VSDEventParam�Ĳ���
	VSDEventParam lVSDParam = pAPPParam->mVSDParam;
	LPRRESULT lResult = LPR_OK;	

	// ����JPGͼƬΪRGBͼƬ��������ʶ����
	LPRImage* pDecodeImage = NULL;
	lResult = LPRDecodeImage(&pDecodeImage, (const unsigned char*)ipImage->pData, ipImage->imageSize, LPR_ENCODE_FORMAT_JPG, 0);
	if(lResult != LPR_OK)
	{
#ifdef __DEBUG
		TRACE("EventAPP::ProcessFram Fail to decode image");
#endif
		return APP_FAIL;
	}
	ipImage->width = pDecodeImage->width;
	ipImage->height = pDecodeImage->height;

	// ��ʼ��ÿ��������Ȧ������VSDRatioLine��ֻ��ʼ��һ��
	if(laneMark == NULL)
	{
		laneMark = new VSDRatioLine[MAX_VIRTUAL_LOOPS*2];
		VSDRatioPoint lTmpPoint;
		lResult = VSDEvent_GenerateLaneMark(lVSDParam.ptRoad, lVSDParam.virtualLoopLine, lVSDParam.nVirtualLoop, ipImage->width, ipImage->height, lVSDParam.nWidthBase, lVSDParam.nHeightBase, laneMark, &lTmpPoint);
		if(lResult != LPR_OK)
		{
	#ifdef __DEBUG
			TRACE("EventAPP::ProcessFram fail to generat lane mark");
	#endif
			return APP_FAIL;
			LPRReleaseImage(pDecodeImage);
		}
		*(pValue + 2) = (int*)laneMark;
		// ��Ϊ�õ���laneMark̫��������ֻȡ������Ȧ�ڵ�Line
		for (int index = 0; index < lVSDParam.nVirtualLoop; ++index)
		{
			laneMark[2 * index].pt1 = lVSDParam.virtualLoopLine[index].pt1;
			laneMark[2 * index + 1].pt1 = lVSDParam.virtualLoopLine[index].pt2;
		}
	}


	// ��ʼ��LPRģ�飬��ʶ������
	if (pLPR == NULL)
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
		lResult = pLPR->Init2(lLPRParam, ipImage->width, ipImage->height, true);
		if(lResult != LPR_OK)
		{
#ifdef __DEBUG
			TRACE("EventAPP::ProcessFram fail to init LPR");
#endif
			LPRReleaseImage(pDecodeImage);
			return APP_FAIL;
		}
		*(pValue + 9) = (int*)pLPR;
	}
	
	// �õ�ѹ����ֵ
	double crossRatio = pAPPParam->mRatioToCrossLine;
	// �������Rectת��Ϊ RatioRect
	VSDRatioRECT objectRatioRECT;
	double imageXRatio = lVSDParam.nWidthBase / (double)ipImage->width; 
	double imageYRatio = lVSDParam.nHeightBase / (double)ipImage->height;
	
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
		// ��virtualLoopImagePool �ﱣ��һ��ͼƬ����û��Υ�µĳ����á�ͬʱ������ͼƬ����ȡÿ������ĳ��ƺŵ�pPlateMap��
		if (lObject.status & VSD_OBJ_STATUS_TOUCH_LINE)
		{
			VirtualLoopImagePool::iterator itVirtualImage = pVirtualLoopImage->find(lObject.uid);
			PlateMap::iterator itPlateMap = pPlateMap->find(lObject.uid);
			if(itVirtualImage == pVirtualLoopImage->end())
			{
				LPRImage *pImage = LPRCloneImage(ipImage);
				pVirtualLoopImage->insert(make_pair(lObject.uid, pImage));
			}
			if(itPlateMap == pPlateMap->end())
			{
				// ��ʼ���ֲ�������
				LPRParamLocal	localParam;
				// ʶ��������Ϊ��ǰ����ľ��ο�
				localParam.m_rectRegion.left = lObject.rect.x; 
				localParam.m_rectRegion.right = lObject.rect.x + lObject.rect.width;
				localParam.m_rectRegion.top = lObject.rect.y; 
				localParam.m_rectRegion.bottom = lObject.rect.y + lObject.rect.height;
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
				lResult =pLPR->ProcessImage(pDecodeImage, &multiOutput, multiParam, NULL );
				if (lResult != LPR_OK)
				{
#ifdef __DEBUG
					TRACE("EventAPP::ProcessFram fail to recognize plate");
#endif
					LPRReleaseImage(pDecodeImage);
					return APP_FAIL;
				}
				if(multiOutput.m_nNumOutputs > 0)
				{
					wchar_t* pPlateCharactor = new wchar_t[LPR_PLATE_STR_LEN];
					for (int i = 0; i < LPR_PLATE_STR_LEN; ++i)
					{
						pPlateCharactor[i] = multiOutput.m_outputs[0].wszRec[i];
					}
					pPlateMap->insert(make_pair(lObject.uid, pPlateCharactor));
				}
			}
		}
		else if(lObject.status & VSD_OBJ_STATUS_LEAVE_LINE)
		{
			VirtualLoopImagePool::iterator itVirtualImage = pVirtualLoopLeaveImage->find(lObject.uid);
			if(itVirtualImage == pVirtualLoopLeaveImage->end())
			{
				LPRImage *pImage = LPRCloneImage(ipImage);
				pVirtualLoopLeaveImage->insert(make_pair(lObject.uid, pImage));
			}
		}
		
		int lObjectHistoryStatus = VSD_BR_NONE;
		lPoolData.mBreakRules[lObject.uid] = VSD_BR_NONE;
		// �ж��Ƿ񴳺��
		if(isRedLightOn[lObject.nLoopID]  && GetCrossRatio(lVSDParam.virtualLoopLine[lObject.nLoopID], objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid  << "����ƣ�����" << lObject.nLoopID << std::endl; 
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_RED_LIGHT;
		}
		// �ж��Ƿ�ѹ����
		if(GetCrossRatio(laneMark[2 * lObject.nLoopID], objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "ѹ��" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_CROSS_LANE;
		}
		if(GetCrossRatio(laneMark[2 * lObject.nLoopID + 1], objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "ѹ��" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_CROSS_LANE;
		}

		// �ж��Ƿ���Υ����ת
		if(!(lVSDParam.loopLaneProperty[lObject.nLoopID] & VSD_LANE_TURN_LEFT) && GetCrossRatio(pAPPParam->mleftTurnLine, objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ�����" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_TURN_LEFT;
		}
		
		// �ж��Ƿ�Υ����ת
		if( !(lVSDParam.loopLaneProperty[lObject.nLoopID] & VSD_LANE_TURN_RIGHT) && GetCrossRatio(pAPPParam->mRightTurnLine, objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ����ת" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_TURN_RIGHT;
		}
		
		// �ж��Ƿ�Υ��ֱ��
		if(!(lVSDParam.loopLaneProperty[lObject.nLoopID] & VSD_LANE_STRAIGHT) && GetCrossRatio(pAPPParam->mStraightLine, objectRatioRECT) >= crossRatio)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ��ֱ��" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_STRAIGHT_THROUGH;
		}
		// �ж��Ƿ�����
		if(lObject.status & VSD_BR_REVERSE)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "����" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_REVERSE;
		}
		// �ж��Ƿ�Υ��ͣ��
		if(lObject.status & VSD_BR_STOP)
		{
#ifdef __DEBUG
			lBreakRuleHistoryLog << lCurrentPicName << "��" << lObject.uid << "Υ��ͣ��" << lObject.nLoopID << std::endl;
#endif
			lPoolData.mBreakRules[lObject.uid] |= VSD_BR_STOP;
		}
		(*pStatusMap)[lObject.uid] |= lPoolData.mBreakRules[lObject.uid];
	}
	//ResultList lResultList;
	int lResultCount = 0;
	// �Ա��µ�PoolData��ԭ����β�����ݣ��������ԭ����β�͵��������µ�PoolData����ʧ����˵���������뿪���������������Ǹ�������ʷstatus���ж��Ǹ������Ƿ���Υ�£�
	// ���û��Υ�£������һ����ͣ���߸�����ͼƬ
	if (!pPool->empty())
	{
		std::map<int, int> lastObject = pPool->back().mBreakRules;
		for(std::map<int, int>::iterator it = lastObject.begin(); it != lastObject.end(); ++it)
		{
			std::map<int, int> thisObject = lPoolData.mBreakRules;
			std::map<int, int>::iterator thisIt = thisObject.find(it->first);

			if(thisIt == thisObject.end())
			{
				VirtualLoopImagePool::iterator itImage = pVirtualLoopImage->find(it->first);
				StatusMap::iterator itStatus = pStatusMap->find(it->first);
				PlateMap::iterator itPlate = pPlateMap->find(it->first);
				if(itImage != pVirtualLoopImage->end())
				{
					if(itStatus != pStatusMap->end() && itStatus->second == VSD_BR_NONE)
					{
						LPRImage* lpImage = LPRCloneImage(itImage->second);
						EventAPPResult lpResult;
						lpResult.mBreakRule = VSD_BR_NONE;
						lpResult.mID = it->first;
						lpResult.mImage[0] = lpImage;
						lpResult.mNumOfImage = 1;
						if(itPlate != pPlateMap->end())
						{
							for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
							{
								lpResult.mPlate[i] = itPlate->second[i];
							}
						}
						//lResultList.push_back(lpResult);
						opResult->mppAPPResult[lResultCount++] = lpResult;
						opResult->mNumOfResult = lResultCount;
						lNoBreakRuleLog << "��" << lpResult.mID << endl;
					}
					//LPRReleaseImage(itImage->second);
					//pVirtualLoopImage->erase(itImage);
				}
				if(itStatus != pStatusMap->end())
					pStatusMap->erase(itStatus);
				/*
				if(itPlate != pPlateMap->end())
				{
					delete[] itPlate->second;
					pPlateMap->erase(itPlate);
				}
				*/
			}
		}
	}

	// �ѵ�ǰPoolDataѹ���β
	pPool->push_back(lPoolData);
	if (pPool->size() > (*pPoolLength + 1))
	{
		// �ͷŵ�����ͷ�Ѳ�����Ҫ¼��Ļ����ͼƬ
		PoolData lCheckPoolData = pPool->front();
		LPRReleaseImage(lCheckPoolData.mpImage);
		pPool->pop_front();
		// �鿴�м����ݵ�Υ�����
		int middle = (*pPoolLength + 1)/2;
		lCheckPoolData = pPool->at(middle);
		// ������ǰpoolData�е�����object��Υ�¼�¼
		for(std::map<int, int>::iterator itObject = lCheckPoolData.mBreakRules.begin(); itObject != lCheckPoolData.mBreakRules.end(); ++itObject)
		{
			// itObjects��ָ��ĳ����Ƿ�Υ����ת
			if(itObject->second & VSD_BR_TURN_LEFT)
			{
				if(!((*pRemoveStatusMap)[itObject->first] & VSD_BR_TURN_LEFT))
				{
					(*pRemoveStatusMap)[itObject->first] |= VSD_BR_TURN_LEFT;
					int lBeginIndex = middle - pAPPParam->mRecordParam.mBreakRuleAhead[0];
					lBeginIndex = lBeginIndex > 0 ? lBeginIndex : 0;
					int lEndIndex = middle + pAPPParam->mRecordParam.mBreakRuleBehind[0];
					lEndIndex = lEndIndex > 0 ? lEndIndex : pPool->size();
					int lSizeToCopy = lEndIndex - lBeginIndex;
					int lImageCount = lSizeToCopy;
					EventAPPResult lpAPPResult;
					lpAPPResult.mID = itObject->first;
					lpAPPResult.mBreakRule = VSD_BR_TURN_LEFT;
					PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
					if(itPlate != pPlateMap->end())
					{
						for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
						{
							lpAPPResult.mPlate[i] = itPlate->second[i];
						}
					}
					LPRImage *lpImage = NULL;
					for (int j = 0; j < lSizeToCopy; ++j)
					{
						lpImage = LPRCloneImage(pPool->at(j + lBeginIndex).mpImage);
						lpAPPResult.mImage[j] = lpImage;
						//lpAPPResult.mNumOfImage = lSizeToCopy;
					}
					VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;

					}
					itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;
					}
					lpAPPResult.mNumOfImage = lImageCount;
					//lResultList.push_back(lpAPPResult);
					opResult->mppAPPResult[lResultCount++] = lpAPPResult;
					opResult->mNumOfResult = lResultCount;
					lBreakOutputLog << "��" <<lpAPPResult.mID << "Υ����ת" <<endl;
				}
			}
			if(itObject->second & VSD_BR_TURN_RIGHT)
			{
				if(!((*pRemoveStatusMap)[itObject->first] & VSD_BR_TURN_RIGHT))
				{
					(*pRemoveStatusMap)[itObject->first] |= VSD_BR_TURN_RIGHT;
					int lBeginIndex = middle - pAPPParam->mRecordParam.mBreakRuleAhead[1];
					lBeginIndex = lBeginIndex > 0 ? lBeginIndex : 0;
					int lEndIndex = middle + pAPPParam->mRecordParam.mBreakRuleBehind[1];
					lEndIndex = lEndIndex > 0 ? lEndIndex : pPool->size();
					int lSizeToCopy = lEndIndex - lBeginIndex;
					int lImageCount = lSizeToCopy;
					EventAPPResult lpAPPResult;
					lpAPPResult.mID = itObject->first;
					lpAPPResult.mBreakRule = VSD_BR_TURN_RIGHT;
					PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
					if(itPlate != pPlateMap->end())
					{
						for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
						{
							lpAPPResult.mPlate[i] = itPlate->second[i];
						}
					}
					LPRImage *lpImage = NULL;
					for (int j = 0; j < lSizeToCopy; ++j)
					{
						lpImage = LPRCloneImage(pPool->at(j + lBeginIndex).mpImage);
						lpAPPResult.mImage[j] = lpImage;
						//lpAPPResult.mNumOfImage = lSizeToCopy;
					}
					VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;

					}
					itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;
					}
					lpAPPResult.mNumOfImage = lImageCount;
					//lResultList.push_back(lpAPPResult);
					opResult->mppAPPResult[lResultCount++] = lpAPPResult;
					opResult->mNumOfResult = lResultCount;
					lBreakOutputLog << "��" <<lpAPPResult.mID << "Υ����ת" <<endl;
				}
			}
			if(itObject->second & VSD_BR_STRAIGHT_THROUGH)
			{
				if(!((*pRemoveStatusMap)[itObject->first] & VSD_BR_STRAIGHT_THROUGH))
				{
					(*pRemoveStatusMap)[itObject->first] |= VSD_BR_STRAIGHT_THROUGH;
					int lBeginIndex = middle - pAPPParam->mRecordParam.mBreakRuleAhead[2];
					lBeginIndex = lBeginIndex > 0 ? lBeginIndex : 0;
					int lEndIndex = middle + pAPPParam->mRecordParam.mBreakRuleBehind[2];
					lEndIndex = lEndIndex > 0 ? lEndIndex : pPool->size();
					int lSizeToCopy = lEndIndex - lBeginIndex;
					int lImageCount = lSizeToCopy;
					EventAPPResult lpAPPResult;
					lpAPPResult.mID = itObject->first;
					lpAPPResult.mBreakRule = VSD_BR_STRAIGHT_THROUGH;
					PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
					if(itPlate != pPlateMap->end())
					{
						for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
						{
							lpAPPResult.mPlate[i] = itPlate->second[i];
						}
					}
					LPRImage *lpImage = NULL;
					for (int j = 0; j < lSizeToCopy; ++j)
					{
						lpImage = LPRCloneImage(pPool->at(j + lBeginIndex).mpImage);
						lpAPPResult.mImage[j] = lpImage;
						//lpAPPResult.mNumOfImage = lSizeToCopy;
					}
					VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;

					}
					itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;
					}
					lpAPPResult.mNumOfImage = lImageCount;
					//lResultList.push_back(lpAPPResult);
					opResult->mppAPPResult[lResultCount++] = lpAPPResult;
					opResult->mNumOfResult = lResultCount;
					lBreakOutputLog << "��" <<lpAPPResult.mID << "Υ��ֱ��" <<endl;
				}
			}
			if(itObject->second & VSD_BR_CROSS_LANE)
			{
				if(!((*pRemoveStatusMap)[itObject->first] & VSD_BR_CROSS_LANE))
				{
					(*pRemoveStatusMap)[itObject->first] |= VSD_BR_CROSS_LANE;
					int lBeginIndex = middle - pAPPParam->mRecordParam.mBreakRuleAhead[3];
					lBeginIndex = lBeginIndex > 0 ? lBeginIndex : 0;
					int lEndIndex = middle + pAPPParam->mRecordParam.mBreakRuleBehind[3];
					lEndIndex = lEndIndex > 0 ? lEndIndex : pPool->size();
					int lSizeToCopy = lEndIndex - lBeginIndex;
					int lImageCount = lSizeToCopy;
					EventAPPResult lpAPPResult; 
					lpAPPResult.mID = itObject->first; 
					lpAPPResult.mBreakRule = VSD_BR_CROSS_LANE;
					PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
					if(itPlate != pPlateMap->end())
					{
						for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
						{
							lpAPPResult.mPlate[i] = itPlate->second[i];
						}
					}
					LPRImage *lpImage = NULL;
					for (int j = 0; j < lSizeToCopy; ++j)
					{
						lpImage = LPRCloneImage(pPool->at(j + lBeginIndex).mpImage);
						lpAPPResult.mImage[j] = lpImage;
						//lpAPPResult.mNumOfImage = lSizeToCopy;
					}
					VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;

					}
					itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;
					}
					lpAPPResult.mNumOfImage = lImageCount;
					opResult->mppAPPResult[lResultCount++] = lpAPPResult;
					opResult->mNumOfResult = lResultCount;
					//lResultList.push_back(lpAPPResult);
					lBreakOutputLog << "��" <<lpAPPResult.mID << "Υ��ѹ��" <<endl;
				}
			}
			if(itObject->second & VSD_BR_REVERSE)
			{
				if(!((*pRemoveStatusMap)[itObject->first] & VSD_BR_REVERSE))
				{
					(*pRemoveStatusMap)[itObject->first] |= VSD_BR_REVERSE;
					int lBeginIndex = middle - pAPPParam->mRecordParam.mBreakRuleAhead[4];
					lBeginIndex = lBeginIndex > 0 ? lBeginIndex : 0;
					int lEndIndex = middle + pAPPParam->mRecordParam.mBreakRuleBehind[4];
					lEndIndex = lEndIndex > 0 ? lEndIndex : pPool->size();
					int lSizeToCopy = lEndIndex - lBeginIndex;
					int lImageCount = lSizeToCopy;
					EventAPPResult lpAPPResult;
					lpAPPResult.mID = itObject->first;
					lpAPPResult.mBreakRule = VSD_BR_REVERSE;
					PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
					if(itPlate != pPlateMap->end())
					{
						for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
						{
							lpAPPResult.mPlate[i] = itPlate->second[i];
						}
					}
					LPRImage *lpImage = NULL;
					for (int j = 0; j < lSizeToCopy; ++j)
					{
						lpImage = LPRCloneImage(pPool->at(j + lBeginIndex).mpImage);
						lpAPPResult.mImage[j] = lpImage;
						//lpAPPResult.mNumOfImage = lSizeToCopy;
					}
					VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;

					}
					itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;
					}
					lpAPPResult.mNumOfImage = lImageCount;
					//lResultList.push_back(lpAPPResult);
					opResult->mppAPPResult[lResultCount++] = lpAPPResult;
					opResult->mNumOfResult = lResultCount;
					lBreakOutputLog << "��" <<lpAPPResult.mID << "Υ������" <<endl;
				}
			}
			if(itObject->second & VSD_BR_RED_LIGHT)
			{
				if(!((*pRemoveStatusMap)[itObject->first] & VSD_BR_RED_LIGHT))
				{
					(*pRemoveStatusMap)[itObject->first] |= VSD_BR_RED_LIGHT;
					int lBeginIndex = middle - pAPPParam->mRecordParam.mBreakRuleAhead[5];
					lBeginIndex = lBeginIndex > 0 ? lBeginIndex : 0;
					int lEndIndex = middle + pAPPParam->mRecordParam.mBreakRuleBehind[5];
					lEndIndex = lEndIndex > 0 ? lEndIndex : pPool->size();
					int lSizeToCopy = lEndIndex - lBeginIndex;
					int lImageCount = lSizeToCopy;
					EventAPPResult lpAPPResult;
					lpAPPResult.mID = itObject->first;
					lpAPPResult.mBreakRule = VSD_BR_RED_LIGHT;
					PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
					if(itPlate != pPlateMap->end())
					{
						for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
						{
							lpAPPResult.mPlate[i] = itPlate->second[i];
						}
					}
					LPRImage *lpImage = NULL;
					for (int j = 0; j < lSizeToCopy; ++j)
					{
						lpImage = LPRCloneImage(pPool->at(j + lBeginIndex).mpImage);
						lpAPPResult.mImage[j] = lpImage;
						//lpAPPResult.mNumOfImage = lSizeToCopy;
					}
					VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;

					}
					itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;
					}
					lpAPPResult.mNumOfImage = lImageCount;
					//lResultList.push_back(lpAPPResult);
					opResult->mppAPPResult[lResultCount++] = lpAPPResult;
					opResult->mNumOfResult = lResultCount;
					lBreakOutputLog << "��" <<lpAPPResult.mID << "Υ�����" <<endl;
				}
			}
			if(itObject->second & VSD_BR_STOP)
			{
				if(!((*pRemoveStatusMap)[itObject->first] & VSD_BR_STOP))
				{
					(*pRemoveStatusMap)[itObject->first] |= VSD_BR_STOP;
					int lBeginIndex = middle - pAPPParam->mRecordParam.mBreakRuleAhead[6];
					lBeginIndex = lBeginIndex > 0 ? lBeginIndex : 0;
					int lEndIndex = middle + pAPPParam->mRecordParam.mBreakRuleBehind[6];
					lEndIndex = lEndIndex > 0 ? lEndIndex : pPool->size();
					int lSizeToCopy = lEndIndex - lBeginIndex;
					int lImageCount = lSizeToCopy;
					EventAPPResult lpAPPResult;
					lpAPPResult.mID = itObject->first;
					lpAPPResult.mBreakRule = VSD_BR_STOP;
					PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
					if(itPlate != pPlateMap->end())
					{
						for(int i = 0; i < LPR_PLATE_STR_LEN; ++i)
						{
							lpAPPResult.mPlate[i] = itPlate->second[i];
						}
					}
					LPRImage *lpImage = NULL;
					for (int j = 0; j < lSizeToCopy; ++j)
					{
						lpImage = LPRCloneImage(pPool->at(j + lBeginIndex).mpImage);
						lpAPPResult.mImage[j] = lpImage;
						//lpAPPResult.mNumOfImage = lSizeToCopy;
					}
					VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;

					}
					itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
					if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
					{
						lpImage = LPRCloneImage(itVirtualLoopImage->second);
						lpAPPResult.mImage[lImageCount++] = lpImage;
					}
					lpAPPResult.mNumOfImage = lImageCount;
					//lResultList.push_back(lpAPPResult);
					opResult->mppAPPResult[lResultCount++] = lpAPPResult;
					opResult->mNumOfResult = lResultCount;
					lBreakOutputLog << "��" <<lpAPPResult.mID << "Υ��ͣ��" <<endl;
				}
			}
			// ���pRemoveStatsu�ﲻ��Ҫ�ļ�¼
			int lShowUp = 0;
			for(int i = middle + 1; i < pPool->size(); ++i)
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
				StatusMap::iterator itStatusMap = pRemoveStatusMap->find(itObject->first);
				if(itStatusMap != pRemoveStatusMap->end())
					pRemoveStatusMap->erase(itStatusMap);
				PlateMap::iterator itPlate = pPlateMap->find(itObject->first);
				if(itPlate != pPlateMap->end())
				{
					delete[] itPlate->second;
					pPlateMap->erase(itPlate);
				}

				VirtualLoopImagePool::iterator itVirtualLoopImage = pVirtualLoopImage->find(itObject->first);
				if (itVirtualLoopImage != pVirtualLoopImage->end())
				{
					LPRReleaseImage(itVirtualLoopImage->second);
					pVirtualLoopImage->erase(itVirtualLoopImage);
				}

				itVirtualLoopImage = pVirtualLoopLeaveImage->find(itObject->first);
				if(itVirtualLoopImage != pVirtualLoopLeaveImage->end())
				{
					LPRReleaseImage(itVirtualLoopImage->second);
					pVirtualLoopLeaveImage->erase(itVirtualLoopImage);
				}
			}
		}
	}

	if (lResultCount > 0)
	{
		char buf[256];
		char img[256];
		for(int i = 0; i < lResultCount;++i)
		{
			_snprintf(buf, 256, "test_%d_%d.avi", opResult->mppAPPResult[i].mID, opResult->mppAPPResult[i].mBreakRule);
			_snprintf(img, 256, "F:\\test_%d_%d.jpg", opResult->mppAPPResult[i].mID, opResult->mppAPPResult[i].mBreakRule);
			if (opResult->mppAPPResult[i].mBreakRule != VSD_BR_NONE)
			{
				pMediaConverter->imgs2media((opResult->mppAPPResult[i].mImage), opResult->mppAPPResult[i].mNumOfImage, buf);
			}
			else
			{
				pSubtitleOverlay->overlaySubtitle((opResult->mppAPPResult[i].mImage[0]), img, "123445km");
			}
		}
		/*
		for(ResultList::iterator it = lResultList.begin(); it != lResultList.end(); ++it)
		{
			_snprintf(buf, 256, "test_%d_%d.avi", it->mID, it->mBreakRule);
			_snprintf(img, 256, "F:\\test_%d_%d.jpg", it->mID, it->mBreakRule);
			if(it->mBreakRule != VSD_BR_NONE)
			{
				pMediaConverter->imgs2media((it->mImage), it->mNumOfImage, buf);
			}
			else
			{
				pSubtitleOverlay->overlaySubtitle((it->mImage[0]), img, "123445km");
			}
		}
		*/
	}


	LPRReleaseImage(pDecodeImage);

	return APP_OK;
}

EventAPP::~EventAPP()
{
	int** pValue = (int**)mObject;
	EventAPPParam* pAPPParam = (EventAPPParam*)(*pValue);
	ImagePool* pPool = (ImagePool*)(*(pValue + 1));
	VSDRatioLine* laneMark = (VSDRatioLine*)(*(pValue + 2));
	int* pPoolLength =(int*)(*(pValue + 3));
	VirtualLoopImagePool* pVirtualLoopImage = (VirtualLoopImagePool*)(*(pValue + 4));
	StatusMap* pStatusMap = (StatusMap*)(*(pValue + 5));
	StatusMap* pRemoveStatusMap = (StatusMap*)(*(pValue + 6));
	MediaConverter* pMediaConverter = (MediaConverter*)(*(pValue + 7));
	LPR* pLPR = (LPR*)(*(pValue + 9));
	PlateMap* pPlateMap = (PlateMap*)(*(pValue + 10));
	VirtualLoopImagePool* pVirtualLoopLeaveImage = (VirtualLoopImagePool*)(*(pValue + 11));

	delete pAPPParam;

	while(!pPool->empty())
	{
		LPRReleaseImage(pPool->front().mpImage);
		pPool->pop_front();
	}
	delete pPool;

	delete[] laneMark;

	delete pPoolLength;

	for(VirtualLoopImagePool::iterator it = pVirtualLoopImage->begin(); it != pVirtualLoopImage->end(); ++it)
		LPRReleaseImage(it->second);
	delete pVirtualLoopImage;

	for(VirtualLoopImagePool::iterator it = pVirtualLoopLeaveImage->begin(); it != pVirtualLoopLeaveImage->end(); ++it)
		LPRReleaseImage(it->second);
	delete pVirtualLoopLeaveImage;

	delete pStatusMap;

	delete pRemoveStatusMap;

	delete pMediaConverter;

	pLPR->Fini();
	delete pLPR;

	for (PlateMap::iterator it = pPlateMap->begin(); it != pPlateMap->end(); ++it)
	{
		delete[] it->second;
	}
	delete pPlateMap;
}

 void __stdcall FreeAPPResult(EventAPPResult* ipAPPResult)
{
	if (!ipAPPResult)
	{
#ifdef __DEBUG
		TRACE("FreeAPPResult fault input param");
#endif
	}

	for(int i = 0; i < ipAPPResult->mNumOfImage; ++i)
		LPRReleaseImage(ipAPPResult->mImage[i]);
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

typedef std::map<std::string, std::string> KeyValue;

template<typename T>
APPRESULT __stdcall CheckAndSetValue(const KeyValue& irKeyValue, const std::string& irKey, T iMaxValue, T& orValue)
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
		if (number < 0 || number > iMaxValue)
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
	std::string::size_type nPosition;
	std::string keyString;
	std:;string valueString;
	while(::getline(ifs,lineString))
	{
		if(lineString.empty())
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
	APPRESULT lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt1.x", ipEventParam->mVSDParam.nWidthBase, ipEventParam->mleftTurnLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//��ʼ����ת��p1���y����
	lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt1.y", ipEventParam->mVSDParam.nHeightBase, ipEventParam->mleftTurnLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt2.x", ipEventParam->mVSDParam.nWidthBase, ipEventParam->mleftTurnLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnLeft.pt2.y", ipEventParam->mVSDParam.nHeightBase, ipEventParam->mleftTurnLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// 
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt1.x", ipEventParam->mVSDParam.nWidthBase, ipEventParam->mRightTurnLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt1.y", ipEventParam->mVSDParam.nHeightBase, ipEventParam->mRightTurnLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt2.x", ipEventParam->mVSDParam.nWidthBase, ipEventParam->mRightTurnLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "TurnRight.pt2.y", ipEventParam->mVSDParam.nHeightBase, ipEventParam->mRightTurnLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// 
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt1.x", ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStraightLine.pt1.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt1.y", ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStraightLine.pt1.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt2.x", ipEventParam->mVSDParam.nWidthBase, ipEventParam->mStraightLine.pt2.x);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	//
	lAPPResult = CheckAndSetValue(keyValue, "Straight.pt2.y", ipEventParam->mVSDParam.nHeightBase, ipEventParam->mStraightLine.pt2.y);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// ��ʼ��ѹ����ֵ
	lAPPResult = CheckAndSetValue(keyValue, "CrossRatio", 1.0, ipEventParam->mRatioToCrossLine);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	// ��ʼ������rule��¼��֡��Χ
	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnLeftFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[0]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnLeftFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[0]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnRightFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[1]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordTurnRightFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[1]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordStraightFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[2]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordStraightFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[2]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordCrossLineFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[3]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordCrossLineFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[3]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordReverseFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[4]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordReverseFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[4]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordRedLightFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[5]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordRedLightFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[5]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordStopFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[6]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordStopFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[6]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordHighSpeedFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[7]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordHighSpeedFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[7]);
	if (lAPPResult != APP_OK)
		return lAPPResult;

	//
	lAPPResult = CheckAndSetValue(keyValue, "RecordLowSpeedFramAhead", MAX_FRAM_AHEAD, ipEventParam->mRecordParam.mBreakRuleAhead[8]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	lAPPResult = CheckAndSetValue(keyValue, "RecordLowSpeedFramBehind", MAX_FRAM_BHEIND, ipEventParam->mRecordParam.mBreakRuleBehind[8]);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	lAPPResult = CheckAndSetValue(keyValue, "ViedoFormat", 1, ipEventParam->mRecordParam.mViedoFormat);
	if (lAPPResult != APP_OK)
		return lAPPResult;
	
	return APP_OK;
}

static bool CmpFileName(const string& a, const string& b)
{
	return a.length() == b.length() ? a < b : a.length() < b.length();
}

static int str2wstr(wchar_t *dst, const char *src, int srcSize)
{
	int cchWideChar = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)src, srcSize, 0, 0);
	if(cchWideChar < 0) return false;
	return MultiByteToWideChar(CP_ACP, 0,(LPCSTR)src, srcSize, dst, cchWideChar);
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

int main(int argc, char *argv[])
{
	VSDRatioLine line = {{2,1},{2,3}};
	VSDRatioRECT rect = {1,3,3,1};

	double length = GetCrossRatio(line, rect);
	std::cout << length << std::endl;

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
	for(vector<wstring>::iterator it = lFiles.begin(); it != lFiles.end(); ++it)
	{
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
		/*
		LPRImage* lpImage = NULL;
		LPRRESULT lResult = LPRDecodeImage(&lpImage, (const unsigned char*)pJpgBuf, nJpgLen, LPR_ENCODE_FORMAT_JPG, 0);
		*/
		LPRImage imgJPG;
		imgJPG.nColorMode = CS_JPEG;
		imgJPG.pData = (unsigned char *)pJpgBuf;
		imgJPG.imageSize = nJpgLen;
		imgJPG.info.nCamID = 0;
		//imgJPG.timeStamp = sysTime;
		//LPRImage* lEncodeImage = LPRCreateImage(lpImage->width, lpImage->height, lpImage->depth, lpImage->nChannels);
	/*	char* pData = new char[5*nJpgLen];
		int length = 5*nJpgLen;
		lResult = LPREncodeImage(lpImage, (unsigned char*)pData, &length, LPR_ENCODE_FORMAT_JPG, 80);*/
		EventMultiAPPResult lAPPResult;
		VSDObjectMulti lObjectMulti;
		LPRRESULT lResult = lEvent.ProcessFrame(&imgJPG, &lObjectMulti);
		int lLights[MAX_VIRTUAL_LOOPS] = {1, 1, 1, 1};
		lEventApp.ProcessFram(&imgJPG,&lObjectMulti, lLights, &lAPPResult);
		delete[] pJpgBuf;
		//FreeMultiAPPResult(&lAPPResult);
	}
	time_t tEnd = time(NULL);
	cout << "����" << (tEnd - tBegin) / 60 << "��" << (tEnd - tBegin)%60 << "��" << endl;

	int i;
	std::cin >> i;
	return 0;
}

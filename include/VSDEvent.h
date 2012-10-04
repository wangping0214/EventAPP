#ifndef __VSDEVENT_H__
#define __VSDEVENT_H__

#include "LPRStructure.h"
#include "VSDEventStructure.h"

#if defined(WIN32)
#ifdef VSDEVENT_EXPORTS
#define VSDEVENT_API __declspec(dllexport)
#else
#define VSDEVENT_API __declspec(dllimport)
#endif
#elif defined(LPR_LINUX)
#ifdef VSDEVENT_EXPORTS
#define VSDEVENT_API __attribute__ ((visibility("default")))
#else
#define VSDEVENT_API
#endif
#else
#error "No platform specified!"
#endif

struct LPRImage;

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
/// C�ӿڵĹ��ܺ�����װ
//////////////////////////////////////////////////////////////////////////

/// ��ʼ��VSDObject���ݽṹ
VSDEVENT_API void __stdcall VSDObject_Init(VSDObject* pObject);
/// ��ʼ��VSDObjectMulti���ݽṹ
VSDEVENT_API void __stdcall VSDObjectMulti_Init(VSDObjectMulti* pObjectMulti);

/// ��ʼ��VSDObjectTrack���ݽṹ
VSDEVENT_API void __stdcall VSDObjectTrack_Init(VSDObjectTrack* pObjectMulti);
/// ��ʼ��VSDObjectTrackMulti���ݽṹ
VSDEVENT_API void __stdcall VSDObjectTrackMulti_Init(VSDObjectTrackMulti* pObjectMulti);

/// ��ó�ʼ����Ĭ�ϲ���
VSDEVENT_API VSDEventParam __stdcall VSDEvent_GetDefaultParam();
/// ���ļ����ز���
VSDEVENT_API LPRRESULT __stdcall VSDEvent_LoadParam(const char* paramFile, VSDEventParam *pParam);
/// ������д���ļ�
VSDEVENT_API LPRRESULT __stdcall VSDEvent_SaveParam(const char* paramFile, VSDEventParam *pParam);

/// ��������, ����·��궨��, ��ȡ����ͼ��
//  ����:	pImage			--- ������ͼ��
//  ����:	ptRoad[4]		--- ·��궨�㣨������꣩
//  ����:	nWidthBase		--- x�������
//  ����:   nHeightBase		--- y�������
//  ����:	nImCaliWidth	--- ����Ľ���ͼ��Ŀ��
//  ����:	nImCaliHeight	--- ����Ľ���ͼ��ĸ߶�
//  ������  nMargin			--- ����Ľ���ͼ��ı�Ե����
//  ����ֵ��LPRImage*		--- ����Ľ���ͼ�񣬿��ΪnImCaliWidth+nMargin*2���߶�ΪnImCaliHeight+nMargin*2
//								��ΪNULL, ��ʾ��ȡ����ͼ��ʧ��
VSDEVENT_API LPRImage * __stdcall VSDGetCalibrateImage(const LPRImage* pImage,
													   VSDRatioPoint ptRoad[4], int nWidthBase, int nHeightBase,
													   int nImCaliWidth, int nImCaliHeight,
													   int nMargin = 0);

/// ��������������·��궨���������Ȧ�Ķ˵㣬��ȡ��������Ϣ
//  ������  ptRoad[4]		--- ·��궨��
//  ������  virtualLoopLine[MAX_VIRTUAL_LOOPS]
//							--- ������Ȧλ��(�������)
//  ����:	nVirtualLoop	--- ������Ȧ����
//  ����:   nImWidth		--- ͼ����
//  ����:   nImHeight		--- ͼ��߶�
//  ����:	nWidthBase		--- x�������
//  ����:   nHeightBase		--- y�������
//  ���:   laneMark[MAX_VIRTUAL_LOOPS*2]
//							--- �����ߵĶ˵�(�ֱ���ͼ�񶥶˺�ͼ��׶�)
//							--- ÿ����Ȧ��Ӧ��������������, ��Ȧi�ĳ����߷ֱ�ΪlaneMark[2*i], laneMark[2*i+1]
//  ���:	pPtLaneCross	--- ���г������ӳ��ߵĽ����
VSDEVENT_API LPRRESULT __stdcall VSDEvent_GenerateLaneMark(const VSDRatioPoint ptRoad[4],
														  const VSDRatioLine virtualLoopLine[MAX_VIRTUAL_LOOPS], const int nVirtualLoop,
														  int nImWidth, int nImHeight,
														  int nWidthBase, int nHeightBase,
														  VSDRatioLine laneMark[MAX_VIRTUAL_LOOPS*2], VSDRatioPoint *pPtLaneCross);
/// [��ɾ��]��������
VSDEVENT_API LPRRESULT __stdcall VSDCopySubImage(const LPRImage* pImSmall, LPRImage* pImLarge, const VSDRect& roi);



//////////////////////////////////////////////////////////////////////////
/// ���º����ṩ�Ľӿڣ���Ҫ��VSDEvent�������ã����Ƽ�ֱ��ʹ��
//////////////////////////////////////////////////////////////////////////

VSDEVENT_API void* __stdcall VSDEvent_Create();
VSDEVENT_API void __stdcall VSDEvent_Destroy(void* pObject);
VSDEVENT_API LPRRESULT __stdcall VSDEvent_Init(void* pObject, const VSDEventParam& param);
VSDEVENT_API void __stdcall VSDEvent_Fini(void* pObject);
VSDEVENT_API bool __stdcall VSDEvent_IsInited(void* pObject);
VSDEVENT_API void __stdcall VSDEvent_ClearObjects(void* pObject);
VSDEVENT_API void __stdcall VSDEvent_ClearOneObject(void* pObject, int nID);

VSDEVENT_API LPRRESULT __stdcall VSDEvent_GetParam(void* pObject, VSDEventParam *pParam);
VSDEVENT_API LPRRESULT __stdcall VSDEvent_UpdateParam(void* pObject, const VSDEventParam& param);

VSDEVENT_API LPRRESULT __stdcall VSDEvent_AddLightMatcher(void* pObject, int nLightID,
														  LPRImage *pLightStatusTemplate[MAX_LIGHT_STATUS], int nNumTemplate,
														  const VSDRatioRECT& searchRect);
VSDEVENT_API void __stdcall VSDEvent_RemoveLightMatcher(void* pObject, int nLightID);
VSDEVENT_API void __stdcall VSDEvent_RemoveAllLightMatcher(void* pObject);
VSDEVENT_API LPRRESULT __stdcall VSDEvent_GetLightStatus(void* pObject, int nLightID, int *pLightStatus, VSDRect *pLightRect, int *pConfid);

VSDEVENT_API LPRRESULT __stdcall VSDEvent_ProcessFrame(void* pObject, const LPRImage* pImage, VSDObjectMulti* vsdObjects, const VSDEventControlParam* param = NULL);

VSDEVENT_API LPRRESULT __stdcall VSDEvent_GetTrack(void* pObject, int uid, VSDObjectTrack* pTrack);
VSDEVENT_API LPRRESULT __stdcall VSDEvent_GetAllTracks(void* pObject, VSDObjectTrackMulti* pAllTracks);

VSDEVENT_API LPRRESULT __stdcall VSDEvent_GetEventStatus(void *pObject, VSDEventStatus *pEventStatus);
/// ��δ����ļ���API
//VSDEVENT_API LPRRESULT __stdcall VSDEvent_AddCaptureLine(void *pObject, const VSDRatioLine *pLine, bool bDoubleShoot);
//
//VSDEVENT_API LPRRESULT __stdcall VSDEVent_AddSpeedDetector(void* pObject,
//														   VSDRatioPoint ptMarkerOnImage[4],
//														   VSDPoint ptMarkerOnRoad[4]);
//VSDEVENT_API LPRRESULT __stdcall VSDEVent_GetSpeed(void* pObject, int uid, int* pSpeed);
///

//////////////////////////////////////////////////////////////////////////
/// ��װ�õĵ�����Ƶ��⹦�ܵĺ����࣬����ʹ��
//////////////////////////////////////////////////////////////////////////
class VSDEvent
{
public:
	VSDEvent() { m_pObject = VSDEvent_Create(); }
	~VSDEvent() { VSDEvent_Destroy(m_pObject); }

	/// ��ʼ��
	LPRRESULT Init(const VSDEventParam &param)
	{
		return VSDEvent_Init(m_pObject, param);
	}

	/// ��ȡ��ʼ��״̬
	bool IsInited()
	{
		return VSDEvent_IsInited(m_pObject);
	}

	/// �ͷ��ڴ�
	void Fini()
	{
		VSDEvent_Fini(m_pObject);
	}

	/// ��Ƶ�����ĺ���
	/// PC�汾֧��CS_JPEG��CS_RGB24��LPRImage
	LPRRESULT ProcessFrame(const LPRImage *pImage, VSDObjectMulti *trackObjects, const VSDEventControlParam* param = NULL)
	{
		return VSDEvent_ProcessFrame(m_pObject, pImage, trackObjects, param);
	}

	/// ��������
	/// ��ȡ��ǰʹ�õĲ���
	LPRRESULT GetParam(VSDEventParam *pParam)
	{
		return VSDEvent_GetParam(m_pObject, pParam);
	}

	/// ��������
	/// ����Event�ڲ�����
	/// ע�⣺�����ֲ����ɱ�����
	/// Ŀǰ֧�֣�nEnhanceImage
	LPRRESULT UpdateParam(const VSDEventParam &param)
	{
		return VSDEvent_UpdateParam(m_pObject, param);
	}

	/// ��������
	/// ��ȡ��ǰ�����ض�ID���������ʷ�켣
	LPRRESULT GetTrack(int uid, VSDObjectTrack* pTrack)
	{
		return VSDEvent_GetTrack(m_pObject, uid, pTrack);
	}

	/// ������������ȡ��ǰ֡����Ŀ�����ʷ�켣
	LPRRESULT GetAllTracks(VSDObjectTrackMulti* pAllTracks)
	{
		return VSDEvent_GetAllTracks(m_pObject, pAllTracks);
	}

	/// ������������յ�ǰ���м�¼������
	void ClearObjects()
	{
		VSDEvent_ClearObjects(m_pObject);
	}

	/// ����������ɾ��ָ��������
	void ClearOneObject(int nID)
	{
		VSDEvent_ClearOneObject(m_pObject, nID);
	}


	//////////////////////////////////////////////////////////////////////////
	/// ����Ϊ���̵�ʶ��������
	/// ���̵�ʶ������ͨ��ͼ��ȶ��㷨��ɵġ�
	/// ʹ��ǰ��Ҫ���ÿһ���źŵƣ��ɼ��źŵ���ʾ�����źŵ�ͼ��ģ�壬
	/// Ȼ��ָ����һ����Χ�ڽ��������ȶԡ�
	/// ��ģ���������ȶԽ�����������һ��״̬��ͼ��ģ�������ƣ���������Ƶ�ȷ��λ�á�

	//*********************************************************************
	// ������:	AddTrafficLightMatcher
	// ˵��:	���ò����һ���źŵ�ƥ����
	// ����ֵ:	LPRRESULT
	// ����:	nLightID		--- ��ǰ��ӵ��źŵƵ����
	// ����:	pLightTemplate	--- �Ƶĸ���״̬��ͼ��ģ��
	// ����:	nNumTemplate	--- �Ƶ�ͼ��ģ����Ŀ
	// ����:	searchRect		--- �źŵƱȶԵ�������Χ
	//*********************************************************************
	LPRRESULT AddTrafficLightMatcher(int nLightID, LPRImage *pLightStatusTemplate[MAX_LIGHT_STATUS], int nNumTemplate,
									const VSDRatioRECT& searchRect)
	{
		return VSDEvent_AddLightMatcher(m_pObject, nLightID, pLightStatusTemplate, nNumTemplate, searchRect);
	}

	//*********************************************************************
	// ��������	GetLightStatus
	// ˵��:	��ȡ�źŵ�ƥ����
	// ����ֵ:	LPRRESULT
	// ����:	nLightID		--- �źŵƵ����
	// ����:	pLightStatus	--- �����״̬��ȡֵΪ0~nNumTemplate - 1
	//						(nNumTemplateΪAddTrafficLightMatcher�����������
	// ����:	pLightRect		--- ����Ƶľ�ȷλ�ã����ܲ�ͬ�ڳ�ʼλ��
	// ����:	pConfid			--- ���ƥ�����Ŷȣ�ȡֵ0~100
	//*********************************************************************
	LPRRESULT GetLightStatus(int nLightID, int *pLightStatus, VSDRect *pLightRect, int *pConfid)
	{
		return VSDEvent_GetLightStatus(m_pObject, nLightID, pLightStatus, pLightRect, pConfid);
	}
	
	//*********************************************************************
	// ��������	RemoveLightMatcher
	// ˵��:	ɾ��һ���źŵ�ƥ����
	// ����ֵ:  void
	// ����:	nLightID	--- ��ɾ���ĵ����
	//*********************************************************************
	void RemoveLightMatcher(int nLightID)
	{
		VSDEvent_RemoveLightMatcher(m_pObject, nLightID);
	}

	//*********************************************************************
	// ��������	RemoveAllLightMatcher
	// ˵��:	ɾ�����е��źŵ�ƥ����
	// ����ֵ:  void
	// ����:	void
	//*********************************************************************
	void RemoveAllLightMatcher()
	{
		VSDEvent_RemoveAllLightMatcher(m_pObject);
	}

	//*********************************************************************
	// ��������	GetEventStatus
	// ˵��:	��ȡEvent����״̬��Ϣ
	// ����ֵ:  LPRRESULT
	// ����:	pEventStatus	--- ���Event����״̬��Ϣ
	//*********************************************************************
	void GetEventStatus(VSDEventStatus *pEventStatus)
	{
		VSDEvent_GetEventStatus(m_pObject, pEventStatus);
	}


	/// ���»�δ����
	//////////////////////////////////////////////////////////////////////////
	/// ����Ϊ���ٸ�������
	/// ��ʼ��VSDEventģ��󣬵���AddSpeedDetector��ʼ�����ٹ��ܣ�
	/// ��ϵͳ���е�����ʱ�̣�����GetSpeed���Ի��ָ��id��������ٶ���Ϣ
	/// 
	/// ע�⣺VSDEventģ���ṩ���Ƚϵ͵���ʵʱ���ٶ���Ϣ��
	///       VSDEventCaptureģ���ṩ����ȷ�Ĳ��٣������ͺ�������
	/// 
	/// Ϊ�˽��в��٣�ÿһ��ProcessFrame��������ʱ��
	/// �����LPRImage* pImage��ҪЯ��׼ȷ��ʱ�����Ϣ
	///

	//*********************************************************************
	// ��������	AddSpeedDetector
	// ˵��:	VSDEvent::AddSpeedDetector
	// ����ֵ:	LPRRESULT 
	// ����:	VSDRatioPoint ptMarkerOnImage[4]
	//			ͼ�����ĸ��궨����������
	// ����:	VSDPoint ptMarkerOnRoad[4]
	//			·�����ĸ��궨���ʵ�ʾ��룬��λΪ����
	//          �ݲ�����
	//*********************************************************************

	//LPRRESULT AddSpeedDetector(VSDRatioPoint ptMarkerOnImage[4], VSDPoint ptMarkerOnRoad[4])
	//{
	//	return VSDEVent_AddSpeedDetector(m_pObject, ptMarkerOnImage, ptMarkerOnRoad);
	//}

	//*********************************************************************
	// ��������	GetSpeed
	// ˵��:	VSDEvent::GetSpeed
	// ����ֵ:	LPRRESULT 
	// ����:	int uid
	//			��Ҫ���ٵ�����id
	// ����:	int* nSpeed
	//			������ٶȣ���λΪ����ÿСʱ
	//          �ݲ�����
	//*********************************************************************
	//LPRRESULT GetSpeed(int uid, int* pSpeed)
	//{
	//	return VSDEVent_GetSpeed(m_pObject, uid, pSpeed);
	//}


private:
	/// �ڲ�ָ�룬���ڷ�װ
	void *m_pObject;
};

#ifdef __cplusplus
}
#endif


#endif	/// end of __VSDEVENT_H__


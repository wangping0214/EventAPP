#ifndef __VSDEVENT_CAPTURE_H__
#define __VSDEVENT_CAPTURE_H__

/// ��Ƶ���ץ�ĺϳ�ģ�飬��ɶ�ץ��ͼ���ʶ���ƴͼ����

#include <VSDEvent.h>

#if defined(WIN32)
#ifdef VSDEVENTCAPTURE_EXPORTS
#define VSDEVENTCAPTURE_API __declspec(dllexport)
#else
#define VSDEVENTCAPTURE_API __declspec(dllimport)
#endif
#elif defined(LPR_LINUX)
#ifdef VSDEVENTCAPTURE_EXPORTS
#define VSDEVENTCAPTURE_API __attribute__ ((visibility("default")))
#else
#define VSDEVENTCAPTURE_API
#endif
#else
#error "No platform specified!"
#endif

enum VSDEventCapMode
{
	CAPMOD_LOCAL = 0,				/// ����ģʽ����������汾
	CAPMOD_NETWORK = 1				/// ����ģʽ����������汾
};

#ifdef __cplusplus
extern "C" {
#endif

struct VSDCaptureParam
{
	int			nMode;				/// ȡֵΪVSDEventCapMode
	int			nCombineMode;		/// ͼ��ϳ�ģʽ
									/// 0:�ر� 1:�ĸ��ɫ 2:�ĸ����
	int			nRec;				/// �Ƿ��ץ�Ľ���ʶ��
									/// 0:��ʶ�� 1:ʶ��

	int			pReserved[32];		/// ����
};

/// ����ץ�ĵ����ݽṹ
struct CaptureRec 
{
	int			nObjID;				/// ����ID
	int			nBreakRule;			/// Υ������
	LPRImage*	pCombinedCapture;	/// pCombinedCapture��nColorModeΪCS_RGB24
	LPRImage*	pCaptures[MAX_CAPTURE_PER_OBJ];	/// pCapture��nColorModeΪCS_JPEG
	LPROutput	recOutput;			/// ʶ����
	float		fSpeed;				/// �ٶȣ�����/Сʱ
	int			pReserved[32];		/// ����
};

struct CaptureRec_Multi
{
	int nCapRecs;
	CaptureRec capRec[MAX_OBJECTS];
};

VSDEVENTCAPTURE_API void* __stdcall VSDEventCapture_Create();
VSDEVENTCAPTURE_API void __stdcall VSDEventCapture_Destroy(void* pObject);
VSDEVENTCAPTURE_API LPRRESULT __stdcall VSDEventCapture_Init(void* pObject, const VSDEventParam &eventParam, const VSDCaptureParam &capParam);
VSDEVENTCAPTURE_API bool __stdcall VSDEventCapture_IsInited(void* pObject);
VSDEVENTCAPTURE_API void __stdcall VSDEventCapture_Fini(void* pObject);

/// �����VSDEventCapture�Ľӿ�
/// LPRImage���������ͣ�DSP�汾ΪCS_RGB24��PC�汾ΪCS_JPEG
/// ����pTrackObjectsֻ������VSDEvent_ProcessFrame������������
/// �ڲ������VSDObject��status�ֶ��ж��Ƿ񻺴�ͼ��
VSDEVENTCAPTURE_API LPRRESULT __stdcall VSDEventCapture_FeedCapture(void* pObject, const LPRImage* pImage, VSDObjectMulti *pTrackObjects);
VSDEVENTCAPTURE_API LPRRESULT __stdcall VSDEventCapture_RequestCapRec(void* pObject, int nObjectID, int nTotalCaps);
VSDEVENTCAPTURE_API LPRRESULT __stdcall VSDEventCapture_GetCapRec(void* pObject, CaptureRec_Multi *pCapRecMulti);

VSDEVENTCAPTURE_API void __stdcall VSDEventCapture_EnableRec(void* pObject, int nEableRec);

VSDEVENTCAPTURE_API LPRRESULT __stdcall VSDEventCapture_AddSpeedDetector(void* pObject, VSDRatioPoint ptMarkerOnImage[4], VSDPoint ptMarkerOnRoad[4]);

class VSDEventCapture
{
public:
	VSDEventCapture() { m_pObject = VSDEventCapture_Create(); }
	~VSDEventCapture() { VSDEventCapture_Destroy(m_pObject); }

	LPRRESULT Init(const VSDEventParam &eventParam, const VSDCaptureParam &capParam)
	{
		return VSDEventCapture_Init(m_pObject, eventParam, capParam);
	}

	void IsInited()
	{
		VSDEventCapture_IsInited(m_pObject);
	}

	void Fini()
	{
		VSDEventCapture_Fini(m_pObject);
	}

	LPRRESULT FeedCapture(const LPRImage *pImage, VSDObjectMulti *pTrackObjects)
	{
		return VSDEventCapture_FeedCapture(m_pObject, pImage, pTrackObjects);
	}
	LPRRESULT RequestCapRec(int nObjectID, int nTotalCaps)
	{
		return VSDEventCapture_RequestCapRec(m_pObject, nObjectID, nTotalCaps);
	}

	LPRRESULT GetCaptureRec(CaptureRec_Multi *pCapRecMulti)
	{
		return VSDEventCapture_GetCapRec(m_pObject, pCapRecMulti);
	}

	void EnableRec(int nEnableRec)
	{
		VSDEventCapture_EnableRec(m_pObject, nEnableRec);
	}

	LPRRESULT AddSpeedDetector(VSDRatioPoint ptMarkerOnImage[4],
								VSDPoint ptMarkerOnRoad[4])
	{
		return VSDEventCapture_AddSpeedDetector(m_pObject, ptMarkerOnImage, ptMarkerOnRoad);
	}

private:
	void*	m_pObject;
};

#if 0
class VSDEventEx
{
public:
	VSDEventEx() {}
	~VSDEventEx() {}

	LPRRESULT Init(const VSDEventParam &eventParam, const VSDCaptureParam &capParam)
	{
		LPRRESULT lr = LPR_OK;
		lr = m_event.Init(eventParam);
		if ( lr != LPR_OK )
			return lr;

		lr = m_capture.Init(eventParam, capParam);
		if ( lr != LPR_OK )
			return lr;

		return lr;
	}

	void Fini()
	{
		m_event.Fini();
		m_capture.Fini();
	}

	LPRRESULT ProcessFrame(const LPRImage *pImage, const VSDEventControlParam *param, VSDObjectMulti *trackObjects)
	{
		LPRRESULT lr = m_event.ProcessFrame(pImage, trackObjects, param);

		if ( lr == LPR_OK )
		{
			lr = m_capture.FeedCapture(pImage, trackObjects);
			
			for ( int i = 0; i < trackObjects->nObjects; i++ )
			{
				if ( trackObjects->objects[i].status & VSD_OBJ_STATUS_DELETE )
				{
					m_capture.RequestCapRec(trackObjects->objects[i].uid);
				}
			}
		}

		return lr;
	}

	LPRRESULT GetCapRec(CaptureRec_Multi *pCapRec)
	{
		return m_capture.GetCaptureRec(pCapRec);
	}
public:
	VSDEvent		m_event;
	VSDEventCapture m_capture;
};

#endif

#ifdef __cplusplus
}
#endif

#endif

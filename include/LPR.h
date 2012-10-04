#ifndef __LPR_H__
#define __LPR_H__

#include "LPRStructure.h"
#include "LPRResult.h"

#if defined(WIN32)
	#ifdef LPR_EXPORTS
		#define LPR_API __declspec(dllexport)
	#else
		#define LPR_API __declspec(dllimport)
	#endif
#elif defined(LPR_LINUX)
	#ifdef LPR_EXPORTS
		#define LPR_API __attribute__ ((visibility("default")))
	#else
		#define LPR_API
	#endif
#else
	#error "No platform specified!"
#endif



#ifdef __cplusplus
extern "C" {
#endif


#define LPR_MAX_REGIONS_PER_FRAME		32

/// ����ͼ��ĳ��������ʽ
/// ��ָ�����ڲ��Զ��ж�
#define LPR_IMAGE_TYPE_AUTO				0
/// ����ͼ��Ϊ֡ģʽ��ͼ�񳤿�Ⱥ�ʵ�ʻ���һ��
#define LPR_IMAGE_TYPE_FRAME			1
/// ����ͼ��Ϊ��ģʽ���߶ȱ�ʵ�ʻ�������
#define LPR_IMAGE_TYPE_FIELD			2

// �ֲ�����
// ��������ͼ���е�һ����ʶ��ľ�������
struct LPRParamLocal
{
	RECT		m_rectRegion;		// ��һ�����ο��ʾ��ʶ����������ͼ���е���Ҫʶ��ʶ�����򣬸�������Ĳ��ֽ�����ʶ��
									// ���ο���������ұ߽�ֵ��������Ϊ��λ��
	int			m_nMinPlateWidth;	// ��С���ƿ�ȣ�С�ڸÿ�ȵĳ��ƽ�����ʶ��
	int			m_nMaxPlateWidth;	// ����ƿ�ȣ����ڸÿ�ȵĳ��ƽ�����ʶ��

	float		m_fltReserved0;		// ������������Ϊ0��
	float		m_fltReserved1;		// ������������Ϊ0��
	float		m_fltReserved2;		// ������������Ϊ0��
	float		m_fltReserved3;		// ������������Ϊ0��
};

struct LPRParamMulti
{
	LPRParamLocal	m_params[LPR_MAX_REGIONS_PER_FRAME];	// ����ֲ�������һ�������ʹ��һ���ֲ��������ɡ�
	int				m_nNumParams;							// m_params�����оֲ�������ʵ����Ŀ��
};

// ��������
LPR_API void*	  __stdcall LPR_Create();
LPR_API void	  __stdcall LPR_Destroy( void* pObject );
LPR_API LPRRESULT __stdcall LPR_Init( void* pObject, const LPRParam& param, int width, int height );
LPR_API LPRRESULT __stdcall LPR_Init2( void* pObject, const LPRParam& param, int width, int height, bool bPicVersion );
LPR_API LPRRESULT __stdcall LPR_Fini( void* pObject );
LPR_API bool	  __stdcall LPR_IsInited( void* pObject );
LPR_API LPRRESULT __stdcall LPR_ProcessImage( void* pObject, LPRImage* pInput, LPROutputMulti* pOutput, LPRParamMulti& params, LPRImage* pMask );
LPR_API LPRRESULT __stdcall LPR_ProcessImage2( void* pObject, LPRImage* pInput, LPROutputMulti* pOutput, LPRParamMulti& params, LPRImage* pMask, int imageType );

// ��ʼ��LPROutput�ṹ
LPR_API void	  __stdcall LPROutput_Init( LPROutput* pOutput );
// ��ʼ��LPROutputMulti�ṹ
LPR_API void	  __stdcall LPROutputMulti_Init( LPROutputMulti* pOutputM );
// ��LPROutputMulti�ṹ�����һ��ʶ����
LPR_API bool	  __stdcall LPROutputMulti_Add( LPROutputMulti* pOutputM, const LPROutput& record );
// ��LPROutputMulti�ṹ�л�ȡ���ʶ����
LPR_API bool	  __stdcall LPROutputMulti_GetBest( const LPROutputMulti* pOutputM, LPROutput* pOutput );
LPR_API bool	  __stdcall LPROutputMulti_GetBest2( const LPROutputMulti* pOutputM, LPROutputMulti* pOutputUser, int nMaxPlate );
// ��ʼ��LPRParamLocal�ṹ
LPR_API void	  __stdcall LPRParamLocal_Init( LPRParamLocal* pParamL );
// ��ʼ��LPRParamMulti�ṹ
LPR_API void	  __stdcall LPRParamMulti_Init( LPRParamMulti* pParamM );
// ��LPRParamMulti�ṹ�����һ���ֲ�����
LPR_API bool	  __stdcall LPRParamMulti_Add( LPRParamMulti* pParamM, const LPRParamLocal& param );


// LPR�࣬�ṩ���ų���ʶ��Ĺ��ܡ�
class LPR
{
public:
	LPR() { m_pObject = LPR_Create(); }
	~LPR() { LPR_Destroy( m_pObject ); }

	// ��������ʼ��������Ӧ�ó�����Ҫ�ڵ�����������֮ǰ���ô˺������г�ʼ����
	//       ���������������û�ʧ�ܡ�
	// ������param - ����ʶ��Ĳ���������ṹ˵����LPRParam�ṹע�͡�
	//		 width - ��Ҫ�����ͼƬ�Ŀ�ȡ�
	//		 height - ��Ҫ�����ͼƬ�ĸ߶ȡ�
	// ����ֵ�� ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	// ˵����Init������LPR��ʼ��Ϊ��Ƶģʽ�������Ҫʹ�õ���ģʽ�������Init2��������ʼ����
	LPRRESULT Init( const LPRParam& param, int width, int height )
	{
		return LPR_Init( m_pObject, param, width, height );
	}

	// ��������ʼ��������Ӧ�ó�����Ҫ�ڵ�����������֮ǰ���ô˺������г�ʼ����
	//       ���������������û�ʧ�ܡ�
	// ������param - ����ʶ��Ĳ���������ṹ˵����LPRParam�ṹע�͡�
	//		 width - ��Ҫ�����ͼƬ�Ŀ�ȡ�
	//		 height - ��Ҫ�����ͼƬ�ĸ߶ȡ�
	//		 bPicMode - ����ģʽ������Ƶģʽ,trueΪ����ģʽ,falseΪ��Ƶģʽ
	// ����ֵ�� ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT Init2( const LPRParam& param, int width, int height, bool bPicMode )
	{
		return LPR_Init2( m_pObject, param, width, height, bPicMode );
	}

	// ����������������Ӧ�ó�������Ҫ���г���ʶ��ʱ��Ӧ���ô˺������ͷ�LPR��
	//       ռ�õ���Դ��
	// ��������
	// ����ֵ�� ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT Fini()
	{
		return LPR_Fini( m_pObject );
	}

	bool IsInited() { return LPR_IsInited( m_pObject ); }

	// ������ʶ����
	// ������pImage - ����ʶ�������ͼ�����ݣ�Ӧ�ó����Ѿ�Ϊ�����ݷ�����ڴ档ͼ�����ΪRGB24��ʽ�������ߴ硣
	//                LPRImage��˵���μ�LPRStructure.hͷ�ļ���
	//       pOutput - ָ��LPROutput��ָ�룬�������ִ�гɹ�����LPROutput�ᱻ���Ϊ
	//                 ����ʶ��Ľ����ע�⴫���LPROutputMulti�ṹ�����Ѿ�����LPROutputMulti_Init��ʼ����
	//		 params - ��Ҫʶ��ľ�������
	//		 pImageMask - �������룬һ���������ΪNULL���ɡ�
	// ����ֵ�� ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT ProcessImage( LPRImage* pImage, LPROutputMulti* pOutput, LPRParamMulti& params, LPRImage* pImageMask = NULL )
	{
		return LPR_ProcessImage( m_pObject, pImage, pOutput, params, pImageMask );
	}

	LPRRESULT ProcessImage2( LPRImage* pImage, LPROutputMulti* pOutput, LPRParamMulti params, LPRImage* pImageMask, int imageType )
	{
		return LPR_ProcessImage2( m_pObject, pImage, pOutput, params, pImageMask, imageType );
	}
private:
	LPR( const LPR& );				// not implemented
	LPR& operator= ( const LPR& );	// not implemented
	void*	m_pObject;
};

//
// ���ܹ���غ���
//

// ��ü��ܹ����кţ����ܺ�LPREngine.h�е�LPRGetKeySerial������ͬ��Ϊ�������������һ���ṩ
LPR_API LPRRESULT  __stdcall LPRGetKeySerialNumber( unsigned long* pSerial );
// ��ȡ���ܹ��е��û�����
LPR_API LPRRESULT  __stdcall LPRGetUserData( LPRUserPasswd* pPassword, LPRUserData* pData );
// ���û�����д����ܹ�
LPR_API LPRRESULT  __stdcall LPRSetUserData( LPRUserPasswd* pPassword, LPRUserData* pData );
// �޸ļ��ܹ����û�����
LPR_API LPRRESULT  __stdcall LPRSetUserPassword( LPRUserPasswd* pOldPassword, LPRUserPasswd* pNewPassword );


#ifdef __cplusplus
}
#endif


#endif // __LPR_H__


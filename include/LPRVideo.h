#ifndef __LPR_VIDEO_H__
#define __LPR_VIDEO_H__

#include "LPRStructure.h"
#include "LPRResult.h"

#if defined(WIN32)
	#ifdef LPRVIDEO_EXPORTS
		#define LPRVIDEO_API __declspec(dllexport)
	#else
		#define LPRVIDEO_API __declspec(dllimport)
	#endif
#elif defined(LPR_LINUX)
	#ifdef LPRVIDEO_EXPORTS
		#define LPRVIDEO_API __attribute__ ((visibility("default")))
	#else
		#define LPRVIDEO_API
	#endif
#else
	#error "No platform specified!"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LPR_VIDEOSRC_INVALID		0
#define LPR_VIDEOSRC_IMAGEFILE		1
#define LPR_VIDEOSRC_VIDEOFILE		2
#define LPR_VIDEOSRC_CAPTURE		3

#define LPR_CAPTURE_WIDTH_DEFAULT			-1
#define LPR_CAPTURE_HEIGHT_DEFAULT			-1

#define LPR_CAPTURE_COLOR_FORMAT_DEFAULT	-1
#define LPR_CAPTURE_COLOR_FORMAT_RGB24		0
#define LPR_CAPTURE_COLOR_FORMAT_RGB555		1
#define LPR_CAPTURE_COLOR_FORMAT_YUV422		2
#define LPR_CAPTURE_COLOR_FORMAT_YV12		3
#define LPR_CAPTURE_COLOR_FORMAT_MJPG		4
#define LPR_CAPTURE_COLOR_FORMAT_MAX		4

#define LPR_CAPTURE_STANDARD_DEFAULT		-1
#define LPR_CAPTURE_STANDARD_PAL			0
#define	LPR_CAPTURE_STANDARD_NTSC			1
#define	LPR_CAPTURE_STANDARD_MAX			1

#define LPR_CAPTURE_SCANMODE_DEFAULT		-1
#define LPR_CAPTURE_SCANMODE_FIELD			0
#define	LPR_CAPTURE_SCANMODE_FRAME			1
#define	LPR_CAPTURE_SCANMODE_MAX			1

#define LPR_CAPTURE_INPUT_TYPE_DEFAULT		-1
#define LPR_CAPTURE_INPUT_TYPE_COMPOSITE	0
#define	LPR_CAPTURE_INPUT_TYPE_SVIDEO		1
#define	LPR_CAPTURE_INPUT_TYPE_MAX			1

#define LPR_CAPTURE_BRIGHTNESS_DEFAULT		-1
#define LPR_CAPTURE_BRIGHTNESS_MAX			255
#define LPR_CAPTURE_CONTRAST_DEFAULT		-1
#define LPR_CAPTURE_CONTRAST_MAX			255
#define LPR_CAPTURE_HUE_DEFAULT				-1
#define LPR_CAPTURE_HUE_MAX					255
#define LPR_CAPTURE_SATURATION_DEFAULT		-1
#define LPR_CAPTURE_SATURATION_MAX			255

#define LPR_CAPTURE_OSC_DEFAULT				-1
#define LPR_CAPTURE_OSC_28M					0
#define	LPR_CAPTURE_OSC_35M					1
#define	LPR_CAPTURE_OSC_MAX					1

#define LPR_CAPTURE_CHANNEL_DEFAULT			-1
#define LPR_CAPTURE_CHANNEL_MAX				8

#if defined(WIN32)

#define LPR_VIDEO_SRC_NAME_MAX_LEN		512
struct LPRVideoSrcItem
{
	int		type;
	char	name[LPR_VIDEO_SRC_NAME_MAX_LEN];
};


LPRVIDEO_API void*		__stdcall LPRVideoSource_Create();
LPRVIDEO_API void		__stdcall LPRVideoSource_Destroy( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_Init( void* pObject, int srcType, const char* pSrcName );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_Fini( void* pObject );
LPRVIDEO_API bool		__stdcall LPRVideoSource_IsInited( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_GetNextImage( void* pObject, LPRImage** ppImage );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_ReleaseImage( void* pObject, LPRImage* pImage );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_GetCaptureSourceList( LPRVideoSrcItem* pItemArray, int* pArraySize );

LPRVIDEO_API int		__stdcall LPRVideoSource_GetWidth( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetWidth( void* pObject, int width );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetHeight( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetHeight( void* pObject, int height );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetColorFormat( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetColorFormat( void* pObject, int format );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetStandard( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetStandard( void* pObject, int standard );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetScanMode( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetScanMode( void* pObject, int scanMode );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetInputType( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetInputType( void* pObject, int inputType );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetBrightness( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetBrightness( void* pObject, int brightness );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetContrast( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetContrast( void* pObject, int contrast );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetHue( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetHue( void* pObject, int hue );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetSaturation( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetSaturation( void* pObject, int saturation );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetOSC( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetOSC( void* pObject, int osc );
LPRVIDEO_API int		__stdcall LPRVideoSource_GetChannel( void* pObject );
LPRVIDEO_API LPRRESULT	__stdcall LPRVideoSource_SetChannel( void* pObject, int channel );


class LPRVideoSource
{
public:
	LPRVideoSource() { m_pObject = LPRVideoSource_Create(); }
	~LPRVideoSource() { LPRVideoSource_Destroy( m_pObject ); }

	// ��ʼ��
	// srcType - ��ƵԴ����
	// pSrcName - ��ƵԴ����
	LPRRESULT Init( int srcType, const char* pSrcName )
	{
		return LPRVideoSource_Init( m_pObject, srcType, pSrcName );
	}
	// ����
	LPRRESULT Fini()
	{
		return LPRVideoSource_Fini( m_pObject );
	}

	bool IsInited()
	{
		return LPRVideoSource_IsInited( m_pObject );
	}

	// ����ƵԴ��ȡ����
	// û�������� *ppImage ��Ϊ NULL
	LPRRESULT GetNextImage( LPRImage** ppImage )
	{
		return LPRVideoSource_GetNextImage( m_pObject, ppImage ); 
	}

	// �ͷ���ƵԴ����
	LPRRESULT ReleaseImage( LPRImage* pImage )
	{
		return LPRVideoSource_ReleaseImage( m_pObject, pImage );
	}

	static LPRRESULT GetCaptureSourceList( LPRVideoSrcItem* pItemArray, int* pArraySize )
	{
		return LPRVideoSource_GetCaptureSourceList( pItemArray, pArraySize );
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	
	// ��ȡ�ɼ�ͼ����
	// ������
	// ����ֵ��
	//   ���زɼ��豸����Ŀ�ȣ������-1��ʾȱʡ��ȣ�ÿ���豸��ȱʡ��Ȳ�ͬ������μ���Ӧ�豸��˵����
	int GetWidth() { return LPRVideoSource_GetWidth( m_pObject ); }

	// ���òɼ�ͼ����
	// ������
	//   width - �µ�ͼ���ȣ�-1��ʾʹ��ȱʡ��ȡ�
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetWidth( int width ) { return LPRVideoSource_SetWidth( m_pObject, width ); }

	// ��ȡ�ɼ�ͼ��߶�
	// ������
	// ����ֵ��
	//   ���زɼ��豸����ĸ߶ȣ������-1��ʾȱʡ�߶ȣ�ÿ���豸��ȱʡ�߶Ȳ�ͬ������μ���Ӧ�豸��˵����
	int GetHeight() { return LPRVideoSource_GetHeight( m_pObject ); }

	// ���òɼ�ͼ��߶�
	// ������
	//   height - �µ�ͼ��߶ȣ�-1��ʾʹ��ȱʡ�߶ȡ�
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetHeight( int height ) { return LPRVideoSource_SetHeight( m_pObject, height ); }

	// ��ȡ��ɫ��ʽ
	// ������
	// ����ֵ��
	//   ���زɼ��豸ʹ�õ���ɫ��ʽ�����ܵķ���ֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_COLOR_FORMAT��ͷ�ĺ궨�壬
	//   �����-1��ʾȱʡ��ɫ��ʽ��ÿ���豸��ȱʡ��ɫ��ʽ��ͬ������μ���Ӧ�豸��˵����
	int GetColorFormat() { return LPRVideoSource_GetColorFormat( m_pObject ); }

	// ������ɫ��ʽ
	// ������
	//   format - �µ���ɫ��ʽ����ȡֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_COLOR_FORMAT��ͷ�ĺ궨�塣
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetColorFormat( int format ) { return LPRVideoSource_SetColorFormat( m_pObject, format ); }

	// ��ȡͼ����ʽ��PAL��NTSC��
	// ������
	// ����ֵ��
	//   ���زɼ��豸ʹ�õ�ͼ����ʽ�����ܵķ���ֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_STANDARD��ͷ�ĺ궨�壬
	//   �����-1��ʾȱʡͼ����ʽ��ÿ���豸��ȱʡͼ����ʽ��ͬ������μ���Ӧ�豸��˵����
	int GetStandard() { return LPRVideoSource_GetStandard( m_pObject ); }

	// ����ͼ����ʽ��PAL��NTSC��
	// ������
	//   standard - �µ�ͼ����ʽ����ȡֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_STANDARD��ͷ�ĺ궨�塣
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetStandard( int standard ) { return LPRVideoSource_SetStandard( m_pObject, standard ); }

	// ��ȡɨ��ģʽ��֡��ģʽ��
	// ������
	// ����ֵ��
	//   ���زɼ��豸ʹ�õ�ɨ��ģʽ�����ܵķ���ֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_SCANMODE��ͷ�ĺ궨�壬
	//   �����-1��ʾȱʡɨ��ģʽ��ÿ���豸��ȱʡɨ��ģʽ��ͬ������μ���Ӧ�豸��˵����
	int GetScanMode() { return LPRVideoSource_GetScanMode( m_pObject ); }

	// ����ɨ��ģʽ��PAL��NTSC��
	// ������
	//   scanMode - �µ�ɨ��ģʽ����ȡֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_SCANMODE��ͷ�ĺ궨�塣
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetScanMode( int scanMode ) { return LPRVideoSource_SetScanMode( m_pObject, scanMode ); }

	// ��ȡ�����ź����ͣ�Composite��SVideo��
	// ������
	// ����ֵ��
	//   ���زɼ��豸ʹ�õ������ź����ͣ����ܵķ���ֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_INPUT_TYPE��ͷ�ĺ궨�壬
	//   �����-1��ʾȱʡ�����ź����ͣ�ÿ���豸��ȱʡ�����ź����Ͳ�ͬ������μ���Ӧ�豸��˵����
	int GetInputType() { return LPRVideoSource_GetInputType( m_pObject ); }

	// ���������ź����ͣ�Composite��SVideo��
	// ������
	//   inputType - �µ������ź����ͣ���ȡֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_INPUT_TYPE��ͷ�ĺ궨�塣
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetInputType( int inputType ) { return LPRVideoSource_SetInputType( m_pObject, inputType ); }

	// ��ȡͼ������
	// ������
	// ����ֵ��
	//   ���زɼ��豸��ͼ�����ȣ������-1��ʾȱʡ���ȣ�ÿ���豸��ȱʡ���Ȳ�ͬ������μ���Ӧ�豸��˵����
	int GetBrightness() { return LPRVideoSource_GetBrightness( m_pObject ); }

	// ����ͼ������
	// ������
	//   brightness - �µ�ͼ�����ȣ�ȡֵ��Χ0-255��-1��ʾʹ��ȱʡ���ȡ�
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetBrightness( int brightness ) { return LPRVideoSource_SetBrightness( m_pObject, brightness ); }

	// ��ȡͼ��Աȶ�
	// ������
	// ����ֵ��
	//   ���زɼ��豸��ͼ��Աȶȣ������-1��ʾȱʡ�Աȶȣ�ÿ���豸��ȱʡ�ԱȶȲ�ͬ������μ���Ӧ�豸��˵����
	int GetContrast() { return LPRVideoSource_GetContrast( m_pObject ); }

	// ����ͼ��Աȶ�
	// ������
	//   contrast - �µ�ͼ��Աȶȣ�ȡֵ��Χ0-255��-1��ʾʹ��ȱʡ�Աȶȡ�
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetContrast( int contrast ) { return LPRVideoSource_SetContrast( m_pObject, contrast ); }

	// ��ȡͼ��ɫ��
	// ������
	// ����ֵ��
	//   ���زɼ��豸��ͼ��ɫ�ȣ������-1��ʾȱʡɫ�ȣ�ÿ���豸��ȱʡɫ�Ȳ�ͬ������μ���Ӧ�豸��˵����
	int GetHue() { return LPRVideoSource_GetHue( m_pObject ); }
	
	// ����ͼ��ɫ��
	// ������
	//   hue - �µ�ͼ��ɫ�ȣ�ȡֵ��Χ0-255��-1��ʾʹ��ȱʡɫ�ȡ�
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetHue( int hue ) { return LPRVideoSource_SetHue( m_pObject, hue ); }

	// ��ȡͼ�񱥺Ͷ�
	// ������
	// ����ֵ��
	//   ���زɼ��豸��ͼ�񱥺Ͷȣ������-1��ʾȱʡ���Ͷȣ�ÿ���豸��ȱʡ���ͶȲ�ͬ������μ���Ӧ�豸��˵����
	int GetSaturation() { return LPRVideoSource_GetSaturation( m_pObject ); }

	// ����ͼ�񱥺Ͷ�
	// ������
	//   saturation - �µ�ͼ�񱥺Ͷȣ�ȡֵ��Χ0-255��-1��ʾʹ��ȱʡ���Ͷȡ�
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetSaturation( int saturation ) { return LPRVideoSource_SetSaturation( m_pObject, saturation ); }

	// ��ȡ�ɼ�������Ƶ��
	// ������
	// ����ֵ��
	//   ���زɼ�������Ƶ�ʣ����ܵķ���ֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_OSC��ͷ�ĺ궨�壬
	//   �˲���Ŀǰ���Դ�㿨��Ч��
	int GetOSC() { return LPRVideoSource_GetOSC( m_pObject ); }

	// ���òɼ�������Ƶ��
	// ������
	//   osc - �µĲɼ�������Ƶ�ʣ���ȡֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_OSC��ͷ�ĺ궨�塣
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	LPRRESULT SetOSC( int osc ) { return LPRVideoSource_SetOSC( m_pObject, osc ); }

	// ��ȡ�ɼ�����ǰͨ����
	// ������
	// ����ֵ��
	//   ���زɼ�����ǰͨ���ţ��˲�������֧��ͨ��ѡ��Ĳɼ�����Ч������μ���Ӧ�豸��˵����
	int GetChannel() { return LPRVideoSource_GetChannel( m_pObject ); }

	// ���òɼ�����ǰͨ���ţ�Ӧ�ó������ͨ���˺����ڲɼ����Ķ��ͨ�����л���
	// ������
	//   channel - �µ�ͨ���ţ���ȡֵ�μ���ͷ�ļ���ʼ������LPR_CAPTURE_OSC��ͷ�ĺ궨�塣
	// ����ֵ��
	//   ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�����ֵ����μ�LPRResult.h��
	//   �˲�������֧��ͨ��ѡ��Ĳɼ�����Ч������μ���Ӧ�豸��˵����
	LPRRESULT SetChannel( int channel ) { return LPRVideoSource_SetChannel( m_pObject, channel ); }

private:
	LPRVideoSource( const LPRVideoSource& );			// not implemented
	LPRVideoSource& operator=( const LPRVideoSource& );	// not implemented
	void*		m_pObject;
};

#endif


/// ����ͼ���һ������������һ��ͼ��
/// ���� pSrcImage --- ԭͼ����ͼ
/// ���� rect	   --- ָ���ľ�������
/// ��� pDstImage --- Ŀ��ͼ�����ڴ�ſ�ȡ��ͼ��
LPRVIDEO_API void __stdcall LPRCopySubImage( const LPRImage* pSrcImage, LPRImage* pDstImage, RECT rect );

/// ����ͼ��
LPRVIDEO_API LPRImage* __stdcall LPRCreateImage( int nWidth, int nHeight, int nDepth, int nChannels );
/// ����Ԥ�����ʽ��ͼ��֧��CS_GRAY, CS_RGB24, �Լ�LPRColorSpace�а�����YUV��ʽ
LPRVIDEO_API LPRImage* __stdcall LPRCreateImage2( int nWidth, int nHeight, LPRColorSpace nColorMode );
LPRVIDEO_API void		__stdcall LPRReleaseImage( LPRImage* pImage );
LPRVIDEO_API LPRImage* __stdcall LPRCloneImage( const LPRImage* pImage );

/// YUV ��ʽת������
LPRVIDEO_API LPRRESULT __stdcall LPRConvertYUVToGRAY(const LPRImage *pImYUV, LPRImage *pImGray, bool bFlip = false);
LPRVIDEO_API LPRRESULT __stdcall LPRConvertYUVToRGB(const LPRImage *pImYUV, LPRImage *pImRGB, bool bFlip = false);

/// ע�⣺���º�����֧��YUV��ʽͼ��Ĵ���
LPRVIDEO_API LPRImage* __stdcall LPRCropImage( LPRImage* pImage, RECT rect );
LPRVIDEO_API LPRRESULT __stdcall LPRFlipImage( const LPRImage* pSrcImage, LPRImage* pDstImage );
LPRVIDEO_API void		__stdcall LPRResizeImage( const LPRImage* pSrcImage, LPRImage* pDstImage );
LPRVIDEO_API void		__stdcall LPRResizeImage2( const LPRImage* pSrcImage, LPRImage* pDstImage );
LPRVIDEO_API LPRRESULT __stdcall LPRSaveImage( LPRImage* pImage, const char* pFilePath );
LPRVIDEO_API LPRImage* __stdcall LPRConvertImageToGray( LPRImage* pImage );
LPRVIDEO_API LPRRESULT __stdcall LPRFillImage( LPRImage* pDest, RECT rect, LPRImage* pSrc );


/// ����һ��LPRImage���ݽṹ����ʾԭͼ��һ����ͼ����û���·����ڴ棬ֻ�ǵ�����ָ�뵽��ͼ���ʵ�λ��
/// Ŀǰֻ����������ΪLPRImage::nColorMode == CS_RGB24��
/// ���� pSrcImage --- ԭͼ����ͼ
/// ���� rect	   --- ָ���ľ�������
/// ���		   --- �趨�˺���ָ����ڲ�������LPRImage��������ݽṹ��Ҫ�ͷ��ڴ�
LPRVIDEO_API LPRImage __stdcall LPRGetSubImageStruct( const LPRImage* pImage, RECT rect );

/// ����Ϊѹ������ѹ��ָ����format����
#define LPR_ENCODE_FORMAT_NONE			1	// ����ѹ��
#define LPR_ENCODE_FORMAT_JPG			2	// ʹ��JPEG��ʽѹ��
#define LPR_ENCODE_FORMAT_J2K			3	// ʹ��JPEG2000��ʽѹ��

//
// ѹ��ͼ�����ڴ�
//
// ������
//	pImage - ��Ҫѹ����ͼ��Ŀǰֻ֧��BGR 3ͨ���Ĳ�ɫͼ��
//  pBuffer - ���ݻ����������ڱ���ѹ�����ͼ�����ݡ�
//  pLen   - ָ��һ��int�ͱ�������int�����ڱ��������ݻ������ĳ��ȡ�
//  format - ͼ���ѹ����ʽ����ȡ��ֵ�μ�ǰ��LPR_ENCODE_FORMAT_��ͷ�ĺꡣ
//  param  - ѹ���㷨�Ĳ��������formatȡֵΪLPR_ENCODE_FORMAT_JPG���˲�����ʾͼ��ѹ����������ȡֵ��ΧΪ1-100��ֵԽ���ļ����Խ�󣬵�ͼ������Խ�á�
//			 ���formatȡֵΪLPR_ENCODE_FORMAT_J2K����ô�˲���ȡֵ�������£�
//			 paramȡֵӦС��0   �� ָ��ѹ��������ݴ�СΪparam�ľ���ֵ����λΪByte������-2000��ʾͼ��ѹ�����ԼΪ2000�ֽڡ�
// ����ֵ��
//  ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ��������ֵΪLPR_BUFFER_TOO_SMALL����ʾ�ṩ�Ļ�����̫С��pLenָ���int�����ڱ���������Ҫ����С
//	���������ȡ�
//  
LPRVIDEO_API LPRRESULT __stdcall LPREncodeImage( const LPRImage* pImage, unsigned char* pBuffer, int* pLen, int format, int param );

//
// ���ڴ��ѹͼ��
//
// ������
//	ppImage - ���ڱ����ѹ��ͼ���ָ���ַ��
//           ע�⣺���*ppImageΪNULL��LPRDecodeImage�ڲ�������ڴ棬��ô�ⲿ��Ҫ����LPRReleaseImage�ͷ��ڴ�
//                 ���*ppImage��ΪNULL�����Һ͵�ǰ��Ҫ�����ͼ���С��������ͬ����ô�ڲ��Ͳ����·����ڴ棬��ֱ�Ӹ���*ppImage
//                 ����������£����Ա������ڴ������ͷţ�ֻ��Ҫ����ͷ�һ�μ��ɡ�
//  pBuffer - ���ݻ�����������Ŵ���ѹ�����ݡ�
//  len     - ���ݻ������ĳ��ȡ�
//  format - ͼ���ѹ����ʽ����ȡ��ֵ�μ�ǰ��LPR_ENCODE_FORMAT_��ͷ�ĺꡣ
//  param  - ��������Ϊ0��
//
// ����ֵ��
//  ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�
//
LPRVIDEO_API LPRRESULT __stdcall LPRDecodeImage( LPRImage** ppImage, const unsigned char* pBuffer, int len, int format, int param );


#define LPR_ENCODE_PLATE_FORMAT_NONE			1		// ����ѹ��
#define LPR_ENCODE_PLATE_FORMAT_J2K_COLOR		2		// �ѳ���ѹ��ΪJPEG200��ͼ
#define LPR_ENCODE_PLATE_FORMAT_J2K_GRAY		3		// �ѳ���ѹ��ΪJPEG200�Ҷ�ͼ

//
// ѹ������ͼ��
//
// ������
//	pImage - ��Ƶͼ��
//  output - pImageͼ���ʶ����
//  pBuffer - ���ݻ����������ڱ���ѹ����ĳ���ͼ�����ݡ�
//  pLen   - ָ��һ��int�ͱ�������int�����ڱ��������ݻ������ĳ��ȡ�
//  width  - ���ƵĿ�ȣ�LPREncodePlate���Զ��ѳ���У��Ϊָ����ȡ�
//  height - ���Ƶĸ߶ȣ�LPREncodePlate���Զ��ѳ���У��Ϊָ���߶ȡ�
//  format - ����ͼ���ѹ����ʽ����ȡ��ֵ�μ�ǰ��LPR_ENCODE_PLATE_FORMAT_��ͷ�ĺꡣ
//  param  - ����ѹ���㷨�Ĳ��������formatȡֵΪLPR_ENCODE_PLATE_FORMAT_J2K_COLOR��LPR_ENCODE_PLATE_FORMAT_J2K_GRAY��
//			 �˲���ȡֵ�������£�
//				1-100�� ��ʾָ��ͼ��ѹ����������ֵԽ��ѹ��Ч��Խ�á���Ŀǰ��δ֧�֣�����LPR_NOT_SUPPORTED��
//				<0   �� ��ʾָ��ѹ��������ݴ�С����ָ���Ĵ�СΪparam�ľ���ֵ����λΪByte������-400��ʾ����ѹ�����ԼΪ400�ֽڡ�
// ����ֵ��
//  ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ��������ֵΪLPR_BUFFER_TOO_SMALL����ʾ�ṩ�Ļ�����̫С��pLenָ���int�����ڱ���������Ҫ����С
//	���������ȡ�
//  
LPRVIDEO_API LPRRESULT __stdcall LPREncodePlate( const LPRImage* pImage, const LPROutput& output, unsigned char* pBuffer, int* pLen, 
												int width, int height, int format, int param );

//
// ����ͼ��
//
//	pImage - ��Ҫ�����ͼ��
//  pFilePath - ������ļ�·�����ļ������ļ�����Ҫ������չ����ָ��ͼ��ѹ����ʽ��Ŀǰ֧�����¸�ʽ��
//					.bmp - ����ѹ��������Ϊλͼ�ļ���
//					.jpg - ѹ��������Ϊjpeg�ļ���
//					.j2k - ѹ��������Ϊjpeg2000�ļ���
//  param  - ѹ���㷨�Ĳ������������Ϊbmp�ļ����˲������ԡ��������Ϊjpg�ļ����˲�����ʾͼ��ѹ����������ȡֵ��ΧΪ1-100��ֵԽ��ѹ��Ч��Խ�á�
//			 �������Ϊjpeg2000�ļ�����ô�˲���ȡֵ�������£�
//				1-100�� ��ʾָ��ͼ��ѹ����������ֵԽ��ѹ��Ч��Խ�á���Ŀǰ��δ֧�֣�����LPR_NOT_SUPPORTED��
//				<0   �� ��ʾָ��ѹ������ļ���С����ָ���Ĵ�СΪparam�ľ���ֵ����λΪByte������-2000��ʾѹ���󱣴���ļ���ԼΪ2000�ֽڡ�
// ����ֵ��
//  ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ�
// 
LPRVIDEO_API LPRRESULT __stdcall LPRSaveImage2( LPRImage* pImage, const char* pFilePath, int param );
LPRVIDEO_API LPRRESULT __stdcall LPRLoadImage( const char* pFilePath, LPRImage** ppImage );
LPRVIDEO_API LPRRESULT __stdcall LPRLoadImage2( const char* pFilePath, LPRImage** ppImage, int param );
LPRVIDEO_API LPRRESULT __stdcall LPRGenerateMask( LPRImage* pImage, LPRRatioLine line1, LPRRatioLine line2 );

// ��������������ı��Σ�ע��pt1,pt2,pt3,pt4��������Ϊ�ı��ε�˳ʱ�����ʱ��Ķ���
struct LPRQuadrangle
{
	POINT		pt1;
	POINT		pt2;
	POINT		pt3;
	POINT		pt4;
};

//
// ����ʶ����������ͼ��
//
//	ppMask - ���ڱ�������ͼ���ָ���ַ��
//  width - ���ɵ�����ͼ����
//  height - ���ɵ�����ͼ��߶�
//  pQuadrangles - һ�������ı���ʶ��������Щ�ı���ʶ������ġ��򡱼�Ϊ���ɵ�����ͼ���е�ʶ������
//  num  - ��pQuadranglesָ����������ı��εĸ���
// ����ֵ��
//  ���ڵ���0��ʾ�ɹ���С��0��ʾʧ�ܡ��������ִ�гɹ�����ô*ppMask�ڴ�������ɵ�����ͼ��Ӧ�ó���ʹ����
//  ��ͼ�����Ҫ����LPRReleaseImage�ͷŸ�ͼ��
LPRVIDEO_API LPRRESULT __stdcall LPRGenerateMask2( LPRImage** ppMask, int width, int height, const LPRQuadrangle* pQuadrangles, int num );

/************************************************************************/
/* ͳ�Ƴ���ͼ������ȶԱȶ�                                             */
/************************************************************************/
//����
//
// pImageFull - ץ�ĵ���ͼ��һ���LPRInput�ṹ��ã�ʹ��ImageFromInput��������
// lpr		  - ��Ӧ��ץ��ͼ���ʶ����
// �����
// mean		- ����ͼ�����ȵľ�ֵ
// var		- ����ͼ�����ȵķ���
//
LPRVIDEO_API void __stdcall LPRGetPlateLighting( const LPRImage* pImageFull, const LPROutput& lpr, double* mean, double* var);

/************************************************************************/
/* ��ȡ���Ƶ�ɫ                                                         */
/************************************************************************/
//����
//
// pImageFull - ץ�ĵ���ͼ��һ���LPRInput�ṹ��ã�ʹ��ImageFromInput��������
// lpr		  - ��Ӧ��ץ��ͼ���ʶ����
// �����
// r, g, b	  - ���Ƶ�ɫ��rgbֵ
// 
//
LPRVIDEO_API void __stdcall LPRGetPlateRGB( const LPRImage* pImageFull, const LPROutput& lpr, int* r, int* g, int* b);


/************************************************************************/
/* ��ȡ������ɫ                                                         */
/************************************************************************/
//����
//
// pImageFull - ץ�ĵ���ͼ��һ���LPRInput�ṹ��ã�ʹ��ImageFromInput��������
// lpr		  - ��Ӧ��ץ��ͼ���ʶ����
// �����
// r, g, b	  - ������ɫ��rgbֵ
// 
//
LPRVIDEO_API void __stdcall LPRGetCarRGB( const LPRImage* pImageFull, const LPROutput& lpr, int* r, int* g, int* b);

/************************************************************************/
/* ����ͼ������Լ���ֵ��                                               */
/************************************************************************/
//����
//
// pImageFull - ץ�ĵ���ͼ��һ���LPRInput�ṹ��ã�ʹ��ImageFromInput��������
// lpr		  - ��Ӧ��ץ��ͼ���ʶ����
// width	  - �����׼��ȣ��ڲ���ͼ�����š�
// height	  - �����׼�߶ȣ��ڲ���ͼ�����š�
//
// �����
// *ppImgPlate	  - ����֮��ĳ���ͼ�񣬲�ɫ��
//					�����ڲ������ڴ棬��������LPRReleaseImage�ͷţ�ԭʼ��С��
// *ppImgBinPlate - ����֮��Ķ�ֵ����ͼ����0��255��ʾ��һ������һ��byte������һ��bit����
//					��ͼ������ں�����JBigѹ����
//					�����ڲ������ڴ棬��������LPRReleaseImage�ͷţ�ָ����С(width, height)��
//
LPRVIDEO_API void __stdcall LPRRectifyPlate( const LPRImage* pImageFull, const LPROutput& lpr, LPRImage** ppImgPlate, LPRImage** ppImgBinPlate, int width, int height );

/************************************************************************/
/* JBig ͼ��ѹ���������API                                                 */
/************************************************************************/
// LPREncodeJBig, ����JBig
// ���룺
//	pImage - ��0��255��ʾ�Ķ�ֵͼ��ÿһ������һ��byte��
//			 ͨ���Ǿ���LPRRectifyPlate����� *ppImgBinPlate��
//	pBuffer - Ϊ������JBigͼ�񻺳�������Ҫ�ɵ����߷��䡣
//
// ������
//	nBufferLength - pBuffer�ĳ��ȡ�
//
// ����ֵ��
//	> 0  - �����JBig���ݵ�ʵ�ʳ���
//	= 0  - ����ṩ��pBuffer����̫С��û�б���ɹ�
//  < 0  - ��������
//
LPRVIDEO_API int __stdcall LPREncodeJBig( const LPRImage* pImage, unsigned char* pBuffer, int nBufferLength );

// LPRDecodeJBig, ����JBig
// ���룺
//
//	pBuffer - JBigͼ�񻺳������ɵ����ߴ��ļ������洢�еõ���
//	nBufferLength - pBuffer�ĳ��ȡ�
//
// �����
//
//	pImage - ��0��255��ʾ�Ķ�ֵͼ��ÿһ������һ��byte������һ��bitһ�����أ�������������ʾ������bmp/jpg�ļ���
//
// ����ֵ��
//	NULL - ����ʧ�ܡ�
//	�Ϸ��ĵ�ַ - �ɹ����벢�Ҵ�����һ��LPRImage����������Ҫ������ʹ����֮����LPRReleaseImage()�ͷ�
//
LPRVIDEO_API LPRImage* __stdcall LPRDecodeJBig( const unsigned char* pBuffer, int nBufferLength );

/************************************************************************/
/* ����ͼ��������API                                                 */
/************************************************************************/
// LPREncodePointArray, �����Ϊ�����ʽ��
// 
// 112 X 20�ĵ���ÿ������һ��bit��һ���ַ�8�����أ���280���ֽڵ��ַ�����
// ���е������й������£�ͼƬ��λ���Ϸ�������������λ��ǰ�棬��ǰ14���ַ����������ֵ��ͼ��һ�е����ض�Ӧ��
// һ���ַ���8λ���ض�Ӧ�������ֽڵ�λ������λ��ͼ�ε��󷽣�������һ���ַ�ֵΪ0x01��
// ����ַ���Ӧ��8�������У�ֻ�������һ����Ϊ��ɫ�������Ϊ��ɫ��
//
// ���룺
//	pImage - ��0��255��ʾ�Ķ�ֵͼ��ÿһ������һ��byte��
//			 ͨ���Ǿ���LPRRectifyPlate����� *ppImgBinPlate��
//
// �����
//	pBuffer - �����ĵ���ͼ�񻺳�������Ҫ�ɵ����߷��䡣
//	nBufferLength - pBuffer�ĳ��ȡ�
//
// ����ֵ��
//	> 0  - �����������ݵ�ʵ�ʳ���
//	= 0  - ����ṩ��pBuffer����̫С��û�б���ɹ�
//  < 0  - ��������
//
LPRVIDEO_API int __stdcall LPREncodePointArray( const LPRImage* pImage, unsigned char* pBuffer, int nBufferLength );

// LPRDecodePointArray, ��������ʽͼ��
// ���룺
//
//	pBuffer - ����ͼ�񻺳������ɵ����ߴ��ļ������洢�еõ���
//	nBufferLength - pBuffer�ĳ��ȡ�
//  width	- ϣ�������ͼ���ȡ�
//  height  - ϣ�������ͼ��߶ȡ�
//  nBufferLength == width * height;
//
// �����
//
//	pImage - ��0��255��ʾ�Ķ�ֵͼ��ÿһ������һ��byte������һ��bitһ�����أ�������������ʾ������bmp/jpg�ļ���
//
// ����ֵ��
//	NULL - ����ʧ�ܡ�
//	�Ϸ��ĵ�ַ - �ɹ����벢�Ҵ�����һ��LPRImage����������Ҫ������ʹ����֮����LPRReleaseImage()�ͷ�
//
LPRVIDEO_API LPRImage* __stdcall LPRDecodePointArray( const unsigned char* pBuffer, int nBufferLength, int width, int height );

#if defined(WIN32)


LPRVIDEO_API HANDLE __stdcall LPRCreateVideoWriter( const char* fileName, int width, int height, int frameRate );
LPRVIDEO_API void __stdcall LPRWriteVideoFrame( HANDLE handle, LPRImage* pImage );
LPRVIDEO_API void __stdcall LPRReleaseVideoWriter( HANDLE handle );


#define LPRRENDER_MAX_LINES				16
#define LPRRENDER_MAX_RECTS				16
#define LPRRENDER_MAX_TEXTS				16

#define LPRRENDER_TEXT_EFFECT_NONE		0x0
#define LPRRENDER_TEXT_EFFECT_OUTLINE	0x1

struct LPRRenderLine
{
	POINT		pt1;
	POINT		pt2;
	HPEN		hPen;
};

struct LPRRenderRect
{
	RECT		rect;
	HPEN		hPen;
};

struct LPRRenderText
{
	char		text[256];
	POINT		pos;
	SIZE		scale;
	HFONT		hFont;
	COLORREF	textColor;
	DWORD		bkMode;
	COLORREF	bkColor;
	COLORREF	outlineColor;
	INT			effect;
};

struct LPRRenderLine2
{
	POINT		pt1;
	POINT		pt2;
	int			penStyle;
	int			penWidth;
	COLORREF	penColor;
};

struct LPRRenderRect2
{
	RECT		rect;
	int			penStyle;
	int			penWidth;
	COLORREF	penColor;
};

struct LPRRenderText2
{
	char		text[256];
	POINT		pos;
	SIZE		scale;
	int			fontSize;
	char		fontName[256];
	COLORREF	textColor;
	DWORD		bkMode;
	COLORREF	bkColor;
	COLORREF	outlineColor;
	INT			effect;	
};

LPRVIDEO_API void* __stdcall LPRRender_Create();
LPRVIDEO_API void  __stdcall LPRRender_Destroy( void* pObject );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_SetDestWindow( void* pObject, HWND hWnd );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_SetDestRect( void* pObject, RECT rect );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_Reset( void* pObject );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_SetImage( void* pObject, LPRImage* pImage, SIZE actualSize, LPRImage** ppOldImage );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_AddLine( void* pObject, const LPRRenderLine& item );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_AddRect( void* pObject, const LPRRenderRect& item );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_AddText( void* pObject, const LPRRenderText& item );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_ClearLines( void* pObject );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_ClearRects( void* pObject );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_ClearTexts( void* pObject );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_Update( void* pObject );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_UpdateToImage( void* pObject, LPRImage* pImage );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_SetLine( void* pObject, int index, const LPRRenderLine2& item );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_SetRect( void* pObject, int index, const LPRRenderRect2& item );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_SetText( void* pObject, int index, const LPRRenderText2& item );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_ClearLine( void* pObject, int index );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_ClearRect( void* pObject, int index );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_ClearText( void* pObject, int index );
LPRVIDEO_API LPRRESULT __stdcall LPRRender_SetBkgrdColor( void* pObject, COLORREF color );


class LPRRender
{
public:
	LPRRender() { m_pObject = LPRRender_Create(); }
	~LPRRender() { LPRRender_Destroy( m_pObject ); }

	LPRRESULT SetDestWindow( HWND hWnd )
	{
		return LPRRender_SetDestWindow( m_pObject, hWnd );
	}
	LPRRESULT SetDestRect( RECT rect )
	{
		return LPRRender_SetDestRect( m_pObject, rect );
	}
	LPRRESULT SetBkgrdColor( COLORREF color )
	{
		return LPRRender_SetBkgrdColor( m_pObject, color );
	}
	LPRRESULT Reset()
	{
		return LPRRender_Reset( m_pObject );
	}
	LPRRESULT SetImage( LPRImage* pImage, SIZE actualSize, LPRImage** ppOldImage )
	{
		return LPRRender_SetImage( m_pObject, pImage, actualSize, ppOldImage );
	}
	LPRRESULT AddLine( const LPRRenderLine& item )
	{
		return LPRRender_AddLine( m_pObject, item );
	}
	LPRRESULT AddRect( const LPRRenderRect& item )
	{
		return LPRRender_AddRect( m_pObject, item );
	}
	LPRRESULT AddText( const LPRRenderText& item )
	{
		return LPRRender_AddText( m_pObject, item );
	}
	LPRRESULT ClearLines()
	{
		return LPRRender_ClearLines( m_pObject );
	}
	LPRRESULT ClearRects()
	{
		return LPRRender_ClearRects( m_pObject );
	}
	LPRRESULT ClearTexts()
	{
		return LPRRender_ClearTexts( m_pObject );
	}
	LPRRESULT Update()
	{
		return LPRRender_Update( m_pObject );
	}
	LPRRESULT UpdateToImage( LPRImage* pImage )
	{
		return LPRRender_UpdateToImage( m_pObject, pImage );
	}
	LPRRESULT SetLine( int index, const LPRRenderLine2& item )
	{
		return LPRRender_SetLine( m_pObject, index, item );
	}
	LPRRESULT SetRect( int index, const LPRRenderRect2& item )
	{
		return LPRRender_SetRect( m_pObject, index, item );
	}
	LPRRESULT SetText( int index, const LPRRenderText2& item )
	{
		return LPRRender_SetText( m_pObject, index, item );
	}
	LPRRESULT ClearLine( int index )
	{
		return LPRRender_ClearLine( m_pObject, index );
	}
	LPRRESULT ClearRect( int index )
	{
		return LPRRender_ClearRect( m_pObject, index );
	}
	LPRRESULT ClearText( int index )
	{
		return LPRRender_ClearText( m_pObject, index );
	}

private:
	void*	m_pObject;
};

LPRVIDEO_API void* __stdcall LPRAviWriter_Create();
LPRVIDEO_API void  __stdcall LPRAviWriter_Destroy( void* pObject );
LPRVIDEO_API LPRRESULT __stdcall LPRAviWriter_Init( void* pObject, const char* pFileName, int frameRate, DWORD codec );
LPRVIDEO_API LPRRESULT __stdcall LPRAviWriter_WriteFrame( void* pObject, LPRImage* pImage, bool bFlip );
LPRVIDEO_API LPRRESULT __stdcall LPRAviWriter_Fini( void* pObject );
LPRVIDEO_API bool	   __stdcall LPRAviWriter_IsInited( void* pObject );

#define LPR_MAKE_FOURCC( ch0, ch1, ch2, ch3 ) \
	( (DWORD)(BYTE)(ch0) | ( (DWORD)(BYTE)(ch1) << 8 ) |	\
	( (DWORD)(BYTE)(ch2) << 16 ) | ( (DWORD)(BYTE)(ch3) << 24 ) )

class LPRAviWriter
{
public:
	LPRAviWriter() { m_pObject = LPRAviWriter_Create(); }
	~LPRAviWriter() { LPRAviWriter_Destroy( m_pObject ); }

	LPRRESULT Init( const char* pFileName, int frameRate, DWORD codec )
	{
		return LPRAviWriter_Init( m_pObject, pFileName, frameRate, codec );
	}
	LPRRESULT WriteFrame( LPRImage* pImage, bool bFlip )
	{
		return LPRAviWriter_WriteFrame( m_pObject, pImage, bFlip );
	}	
	LPRRESULT Fini()
	{
		return LPRAviWriter_Fini( m_pObject );
	}
	bool IsInited()
	{
		return LPRAviWriter_IsInited( m_pObject );
	}
private:
	void*	m_pObject;
};

#endif


#ifdef __cplusplus
}
#endif
	
#endif //__LPR_VIDEO_H__


#ifndef __LPR_EX_H__
#define __LPR_EX_H__

#include "LPR.h"
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


// ��������
/************************************************************************/
/* ��ȡ������ɫ                                                    */
/************************************************************************/
// ���룺
// pImageFull - ץ�ĵ���ͼ��һ���LPRInput�ṹ��ã�ʹ��ImageFromInput��������
// lpr		  - ��Ӧ��ץ��ͼ���ʶ����
// 
// �����
// r, g, b	  - ������ɫ��rgbֵ
// 
//
LPR_API void __stdcall LPRGetCarRGBEx( const LPRImage* pImageFull, const LPROutput& lpr, int* r, int* g, int* b);

/************************************************************************/
/* ���ݳ���ʶ��������⳵�ꡢ��������ֻ֧��RGB��ʽ                    */
/************************************************************************/
// ���룺
// pInput     - ץ�ĵ���ͼ��һ���LPRInput�ṹ��ã�ʹ��ImageFromInput��������
// nImageType - ץ��ͼ���ǳ�ģʽ��LPR_IMAGE_TYPE_FIELD������֡ģʽ��LPR_IMAGE_TYPE_FRAME����
// bRecBrand  - �Ƿ�ʶ�𳵱�
// bDetFace   - �Ƿ�������
// pLPROutput - ��Ӧ��pInput�ĳ���ʶ����
// 
// �����
// pLPROutput - �ú����Ὣ���ꡢ�����������䵽pLPROutput
// 
//
LPR_API LPRRESULT __stdcall LPRGetBrandFaceEx( const LPRImage* pInput, int nImageType, LPROutput* pLPROutput, bool bRecBrand, bool bDetFace );

#ifdef __cplusplus
}
#endif

#endif //__LPR_EX_H__

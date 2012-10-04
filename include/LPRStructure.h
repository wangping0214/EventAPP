#ifndef __LPR_STRUCTURE_H__
#define __LPR_STRUCTURE_H__

#include "LPRResult.h"
#include "LPRDefines.h"

struct LPRLine
{
	POINT	from;
	POINT	to;
};

struct LPRRatioRect
{
	int		leftRatio;
	int		rightRatio;
	int		topRatio;
	int		bottomRatio;
};

struct LPRRatioPoint
{
	int		xRatio;
	int		yRatio;
};

struct LPRRatioLine
{
	LPRRatioPoint	from;
	LPRRatioPoint	to;
};

/// ��ʱû�п���ʹ��
struct LPRImageInfo
{
	int nCamID;				/// ����ͷID
	int nRoadID;			/// ·��ID
	int nDirID;				/// ����ID
	int pReserved[61];
};

/// ��ʱû�п���ʹ��
struct LPROutputInfo
{
	int nMotionDir;			/// �˶�����
	int pReserved[63];		/// �����ֶ�
};

struct LPRImage
{
	int  nSize;         // LPRImage �ṹ��С
	int  nChannels;     // ͨ����Ŀ��1��2��3��4
	int  depth;         // ��ɫ��ȣ�8��16
	int  nColorMode;	// ȡֵΪ LPRColorSpace ��ĳһ��
	int  dataOrder;     // 0
	int  origin;        // 0
	int  width;         // ���
	int  height;        // �߶�
	int  imageSize;     // ����buffer��С
	unsigned char *pData;// ����bufferָ��
	int  step;			// ÿһ�еĳ��ȣ���һ���Ϳ����ȣ�
	SYSTEMTIME	timeStamp;  // ʱ���
	LPRImageInfo info;
};

/// ���Ƽ��������Ϣ
struct LPROutput
{
	LPRPlateColor	nColor;						// ������ɫ
	LPRPlateType	nType;						// ��������
	RECT			rectPlate;					// ����λ�þ���
	RECT			charRect[LPR_MAX_NUM_CHAR];	// ���Ƹ�λ�ַ���λ�þ��Σ���ȫͼ�е�����
	wchar_t			wszRec[LPR_PLATE_STR_LEN];	// ���ƺ�
	int				pRec[LPR_MAX_NUM_CHAR];		// ����
	float			pConfid[LPR_MAX_NUM_CHAR];	// ���Ƹ�λ�ַ������Ŷ�
	float			fltConfid;					// �������Ƶ����Ŷ�
	float			currConfid;					// ����
	int				nNumChar;					// �������ַ���Ŀ
	bool			bNewCar;					// �����Ƿ��һ������Ƶ�г���
#if ENABLE_SEPARATOR_POS_FIELD
	int				nSeparatorPos;				// �������ּ����λ��
#endif
#if REC_VEHICLE_BRAND
	int				nBrand;						// ����ʶ���������ó���ʶ����Ҫ��LPR.ini������
	RECT			rectBrand;					// RecogBrand����Ϊ1��������Ҫ���ܹ�֧�ֳ���ʶ���ܡ�
												// rectBrandΪ����ľ��ο�
#endif
	int				numOfFaces;					// �����������faces�ֶδ�ż�⵽������������
	RECT			faces[LPR_MAX_FACES];		// numOfFaces��ʾfaces����������������ĸ�����
												// ���������������������ô��һ�����Ϊ��ʻԱ����
												// ���ó����������Ҫ��LPR.ini������DetectFace����Ϊ1��
												// ������Ҫ���ܹ�֧��������⹦�ܡ�
	RECT			rectWindow;					// ����ǰ�����Ĵ���λ��

	LPROutputInfo	info;						// ���������Ϣ
};

/// LPR ���Է��ض�����
/// ������Ľӿ�
struct LPROutputMulti
{
	LPROutput	m_outputs[LPR_MAX_OUTPUT_NUM];	// �����������
	int			m_nNumOutputs;					// ����������Ŀ��
	RECT  		m_motionRegion;					// �˶�����
};


// ����ģ��
struct LPRPlateModel
{
	int		plateType;
	int		plateCharType[LPR_MAX_NUM_CHAR];	// CharType��ÿһλ������
};

#define		LPR_MAX_PLATE_NUM		12

// LPR����
struct LPRParam
{
	int				size;
	int				nMaxPlate;
	int				nMinPlateWidth;
	int				nMaxPlateWidth;
	LPRPlateModel	plateModel;
	int				pReserved[64];	/// �����ֶΣ����ڽ����汾�޷�����
};

// LPREngine����
struct LPREngineParam
{
	int				size;			// �ṹ��С
	LPRParam		lprParam;		// LPR����
	LPRRatioRect	recogRegion;	// ����ʶ������

	LPRRatioLine	line1;			// ˫��ʶ������
	LPRRatioLine	line2;

	POINT			tlvRegion[4];	

	int				maxSkip;		// ���Ϊǰ����֮�����С�����
									// ʶ��һ����֮�����maxSkip֡û�н����
									// �������һ�����Ľ��
									// ���������Ŀ���ǣ�������Ƶʶ��ʱ���
									// ��ⲻ������

	float			minValid;		// ͶƱ�������������Ŷ�֮��������
	float			minConfid;		// ����ͶƱ�ĵ���ʶ������������Ŷ�

	int				framePercent;			// ʶ�����
	bool			bAdaptiveFP;			// ����
	int 			motionDetectionMode;	// �˶����ģʽ��0-�����˶���⣬1-���˶��������ϲ���ʶ��ʶ��
	bool			bCapture;				// �Ƿ�򿪳���ץ�Ĺ���
	int				minCaptureDiff;			// 
	LPRRatioRect	preferCapRegion;		// ����ץͼ����
	LPRRatioRect	rectEntrance;			// ����ģʽΪ3ʱ���������������
	int				pReserved[512];	/// �����ֶΣ����ڽ����汾�޷�����
};

// LPREngine����
struct LPREngineInput
{
	int				size;		// �ṹ��С
	LPRImage*		pInput;		// ��ʶ��ͼ���ָ��
	bool			bForce;		// ��������Ϊfalse
	bool			bRedLight;
	long			context;	// �Զ����ֶ�
};

class LPREngine;

// ץͼ
struct LPRCarItem
{
	LPRImage*		pCapture;	// ����ץ��ͼƬ
	LPROutput		output;		// ������Ϣ
};

// TLV for "Traffic Light Violation"
#define LPR_MAX_TLV_NUM			16
struct LPRTLVItem
{
	LPRCarItem		captures[3];
};

// LPREngine������
struct LPREngineOutput
{
	int				size;							// �ṹ��С
	LPREngine*		pEngine;						// LPREngine��ָ��
	LPRImage*		pInput;							// ���ʶ���ͼƬ
	bool			bForce;							// ������Ϊfalse
	long			context;						// �Զ����ֶ�
	LPRRESULT		lr;								// ʶ��Ĵ���ֵ
	LPROutputMulti	outputs;						// ʶ����
	int				numOfCarItems;					// ���������Ŀ
	LPRCarItem		carItems[LPR_MAX_OUTPUT_NUM];	// �����������
	int				numOfTlvItems;
	LPRTLVItem		tlvItems[LPR_MAX_TLV_NUM];
};	


struct LPRUserPasswd
{
	unsigned long	data1;
	unsigned long	data2;
};
struct LPRUserData
{
	unsigned long	data1;
	unsigned long	data2;
	unsigned long	data3;
	unsigned long	data4;
};

#endif //__LPR_STRUCTURE_H__


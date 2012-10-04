#ifndef __LPR_DEFINES_H__
#define __LPR_DEFINES_H__

#define LPR_REGION_TYPE					0

#define ENABLE_SEPARATOR_POS_FIELD		1
#define REC_VEHICLE_BRAND				1

// �����ĳ���

// �����ַ�������
#define LPR_PLATE_STR_LEN		11

// �����ַ���Ŀ
#define LPR_NUM_CHAR			7
#define LPR_MAX_NUM_CHAR		7

// ������������Ŀ��Ԥ��
#define LPR_MAX_OUTPUT_NUM		32
#define LPR_MAX_FACES			4

#ifdef __BORLANDC__
#pragma option push -b  // treat enum as integer
#endif

// ÿһλ�ַ���ʶ������
enum LPRRecType
{
	LR_INVALID		= -1,
	LR_DigitAlpha	= 0,	// ���ּ���ĸ
	LR_Digit		= 1,	// ����
	LR_Alpha		= 2,	// ��ĸ
	// ����ʶ��ĵ�������	  
	LR_China		= 20,	// ȫ��ͨ��
	LR_HeiLongJiang	= 21,	// ������
	LR_ShangHai		= 22,	// �Ϻ�
	LR_LiaoNing		= 23,	// ����
	LR_ShanDong		= 24,	// ɽ��
	LR_GuangDong	= 25,	// �㶫
	LR_GuangXi		= 26,	// ����
	LR_TianJin		= 27,	// ���
	LR_BeiJing		= 28,	// ����
	LR_JiLin		= 29,	// ����
	LR_HeNan		= 30,	// ����
	LR_HuBei		= 31,	// ����
	LR_HuNan		= 32,	// ����
	LR_JiangXi		= 33,	// ����
	LR_GanSu		= 34,	// ����
	LR_ZheJiang		= 35,	// �㽭
	LR_JiangSu		= 36,	// ����
	LR_HuaDong		= 37,	// ����
	LR_XiNan		= 38,	// ����
	LR_XiBei		= 39,	// ����
	LR_DongBei		= 40,	// ����
	LR_HuaBei		= 41,	// ����
	LR_HuaNan		= 42,	// ����
	LR_HuaZhong		= 43	// ����
};							

// ������ɫ
enum LPRPlateColor
{
	PC_UNKNOWN	= 0,	// δ֪
	PC_BLUE		= 1,	// ���װ���
	PC_YELLOW	= 2,	// �Ƶ׺���
	PC_WHITE	= 3,	// �׵׺���
	PC_BLACK	= 4,	// �ڵװ���	
	PC_GREEN	= 5		// �̵װ���
};

// ��������
enum LPRPlateType
{
	PT_UNKOWN		= 0,	// δ֪
	PT_CAR_NORMAL	= 1,	// ��ͨС����˽�ҳ�����ɫ����ɫ
	PT_BIG_NORMAL	= 2,	// ��ͨ�󳵣���������ɫ
	PT_POLICE		= 4,    // �������ָ���ڵ�һλ�͵ڶ�λ�ַ�֮�䣩����ɫ
	PT_WJ			= 8,	// �侯����
	PT_HK			= 16,	// �۰ĳ���
	PT_DOUBLELINE	= 32,	// ˫�����
	PT_MILITARY		= 64,	// ����
	PT_AMBASSADOR	= 128,	// ʹ�ݳ���
	PT_POLICE_TYPE2	= 512,  // �ڶ��ྯ�����ָ���ڵڶ�λ�͵���λ�ַ�֮�䣩����ɫ
	PT_XUE			= 1024,	// ��ѧ������
	PT_MOTOR		= 2048,	// Ħ�г�
	PT_LIAOTEMP		= 4096, // ���ֳ���
	PT_GREEN		= 8192,	// ˫�����ƣ�ũ�ó�
	PT_GUA			= 16384	// ���ҡ�����
};

enum LPRVersion
{
	LV_VIDEO	= 0,		/// ��Ƶ�汾��ֻ������LPREngine����
	LV_FAST_VID = 1,		/// Ԥ��
	LV_ACC_VIDEO = 2,		/// ����ͨ��Ƶ�汾���ܸ��������Ǹ���ȷ
	LV_PICTURE	= 8,		/// ���Ű汾
	LV_FAST_PIC = 9,		/// ���ٵ��Ű汾
	LV_FAST_PIC2= 10,		/// �����ٵ��Ű汾
	LV_FAST_PIC3= 11,		/// �ǳ����ٵ��Ű汾
	LV_FAST_PIC4= 12,		/// �����ٵ��Ű汾
};

enum LPRVehicleBrand
{
	BRAND_UNKOWN	= -1,	/// δ֪
	BRAND_CSVW		= 0,	/// ����
	BRAND_AUDI		= 1,	/// �µ�
	BRAND_HONDA		= 2,	/// ����
	BRAND_BUICK		= 3,	/// ���
	BRAND_TOYOTA	= 4,	/// ����
	BRAND_CITROEN	= 5,	/// ѩ����/����
	BRAND_MAZDA		= 6,	/// ���Դ�
	BRAND_HYUNDAI	= 7,	/// �ִ�
};

// ����ͼ�����ɫ�ռ�
enum LPRColorSpace
{
	CS_RGB24	= 0,
	CS_RGB565	= 1,
	CS_RGB555	= 2,
	CS_RGB32	= 3,
	CS_JPEG		= 4,	/// ����jpeg����
	CS_YUYV		= 5,
	CS_UYVY		= 6,
	CS_GRAY		= 4,	/// �����ݲ�֧��
	CS_YV12		= 8,
	CS_YV16		= 16,
	CS_YUY2		= 32,
};


#if defined( LPR_LINUX )

typedef long				BOOL;
typedef unsigned short     	WORD;
typedef int					INT;
typedef long				LONG;
typedef unsigned long		DWORD;
typedef unsigned long		ULONG;
#ifndef USE_ICC
typedef long				__int64;
#endif
typedef char				TCHAR;
typedef char				_TCHAR;
typedef unsigned char		UCHAR;


#define TRUE	1
#define FALSE   0

#ifndef NULL
#define NULL 0
#endif /* NULL */

typedef void *HANDLE;
typedef void *HPEN;
typedef void *HFONT;
typedef void *HWND;
typedef DWORD COLORREF;
typedef char  TCHAR;

#ifdef RTM
#undef assert
#define assert(exp)		(true)
#endif

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, *LPPOINT;

typedef struct  tagSIZE
{
    LONG cx;
    LONG cy;
} SIZE, *PSIZE, *LPSIZE;

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT, *LPRECT;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

#define  __stdcall
#define _MAX_PATH   260
#define _snprintf	snprintf
#define _stricmp	strcasecmp
#define stricmp	strcasecmp
#define _cprintf	printf
#define wcscpy_s(a,b,c)  wcscpy(a,c)
#define _countof(array)  (sizeof(array)/sizeof(array[0]))

#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))


#elif defined(WIN32)
#include <windows.h>
#endif

#ifdef __BORLANDC__
#pragma option pop
#endif

#endif //__LPR_DEFINES_H__

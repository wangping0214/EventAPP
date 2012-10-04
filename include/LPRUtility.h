#ifndef __LPR_UTILITY_H__
#define __LPR_UTILITY_H__

#include "LPRResult.h"
#include "LPRStructure.h"
#include "stdarg.h"
#include <string>
#include <vector>

#if defined(WIN32)
	#ifdef LPRUTILITY_EXPORTS
		#define LPRUTILITY_API __declspec(dllexport)
	#else
		#define LPRUTILITY_API __declspec(dllimport)
	#endif
#elif defined(LPR_LINUX)
	#ifdef LPRUTILITY_EXPORTS
		#define LPRUTILITY_API __attribute__ ((visibility("default")))
	#else
		#define LPRUTILITY_API
	#endif
#else
	#error "No platform specified!"
#endif

#ifdef __cplusplus
extern "C" {
#endif

LPRUTILITY_API void*	  __stdcall LPRSetting_Create();
LPRUTILITY_API void		  __stdcall LPRSetting_Destroy( void* pObject );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_LoadFromFile( void* pObject, const char* pFileName, const char* pReserved );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_SaveToFile( const void* pObject, const char* pFileName, const char* pReserved );
LPRUTILITY_API bool		  __stdcall LPRSetting_HasKey( const void* pObject, const char* pKeyName );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_GetIntValue( const void* pObject, const char* pKeyName, int* pValue );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_GetFloatValue( const void* pObject, const char* pKeyName, float* pValue );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_GetBoolValue( const void* pObject, const char* pKeyName, bool* pValue );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_GetStringValue( const void* pObject, const char* pKeyName, char* pBuf, int* pLen );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_SetIntValue( void* pObject, const char* pKeyName, int value );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_SetFloatValue( void* pObject, const char* pKeyName, float value );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_SetBoolValue( void* pObject, const char* pKeyName, bool value );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_SetStringValue( void* pObject, const char* pKeyName, const char* pStr );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_AddIntKey( void* pObject, const char* pKeyName, int value, bool bOverwrite );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_AddFloatKey( void* pObject, const char* pKeyName, float value, bool bOverwrite );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_AddBoolKey( void* pObject, const char* pKeyName, bool value, bool bOverwrite );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_AddStringKey( void* pObject, const char* pKeyName, const char* pStr, bool bOverwrite );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_DeleteKey( void* pObject, const char* pKeyName );
LPRUTILITY_API LPRRESULT  __stdcall LPRSetting_GetAllKeys( const void* pObject, char* pBuffer, int* pBufLen );

class LPRSetting
{
public:
	LPRSetting() { m_pObject = LPRSetting_Create(); }
	~LPRSetting() { LPRSetting_Destroy( m_pObject ); }

	// ����:   ���ļ���������
	// ����:   pFileName - �ļ���, �����Ǿ���·����Ҳ���������·����
	//         ��������·���Ļ�����Ӧ�ó�������Ŀ¼��Ϊ��׼·����
	//         pReserved - ������������ΪNULL��   
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT LoadFromFile( const char* pFileName, const char* pReserved )
	{
		return LPRSetting_LoadFromFile( m_pObject, pFileName, pReserved );
	}

	// ����:   �������õ��ļ�
	// ����:   pFileName - �ļ���, �����Ǿ���·����Ҳ���������·����
	//         ��������·���Ļ�����Ӧ�ó�������Ŀ¼��Ϊ��׼·����
	//         pReserved - ������������ΪNULL��   
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT SaveToFile( const char* pFileName, const char* pReserved ) const
	{
		return LPRSetting_SaveToFile( m_pObject, pFileName, pReserved );
	}

	// ����:   ��ѯ�Ƿ����ض������ò���
	// ����:   pKeyName - ������
	// ����ֵ: true��ʾ�д˲�����false��ʾ�޴˲���
	bool HasKey( const char* pKeyName )
	{
		return LPRSetting_HasKey( m_pObject, pKeyName );
	}
	
	// ����:   ������в���������
	// ����:   pBuffer - ���ڴ�Ų������Ļ�����
	//		   pBufLen - ����������
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	// ˵��:   �������ִ�гɹ���pBuffer�ڽ���˳���ŵĸ��������������ַ���
	//			��������β��'\0'���������ַ����Կ��ַ�����'\0'�� ��β��pBufLen
	//			�ڴ�ŵ������ݵ�ʵ�ʳ��ȡ����������Ϊ�������������ʧ�ܣ���ô
	//			pBufLen�ڴ�ŵ�����Ҫ����С���������ȡ�
	LPRRESULT GetAllKeys( char* pBuffer, int* pBufLen ) const
	{
		return LPRSetting_GetAllKeys( m_pObject, pBuffer, pBufLen );
	}

	// ����:   ��������������ͬGetAllKeys( char* pBuffer, int* pBufLen )��ֻ�����ݵ�
	//		   �������ַ���������ʽ���
	std::vector<std::string> GetAllKeys() const
	{
		std::vector<std::string>	result;

		int len = 0;
		LPRRESULT lr = LPRSetting_GetAllKeys( m_pObject, NULL, &len );
		if( lr != LPR_BUFFER_TOO_SMALL )
			return result;
		
		char* pBuf = new char[len];
		if( !pBuf )
			return result;

		lr = LPRSetting_GetAllKeys( m_pObject, pBuf, &len );
		if( lr < 0 )
		{
			delete[] pBuf;
			return result;
		}

		char* pCur = pBuf;
		std::string str = pCur;
		pCur += str.length()+1;
		while( !str.empty() )
		{
			result.push_back( str );
			str = pCur;
			pCur += str.length()+1;
		}

		delete[] pBuf;
		return result;
	}
	// ����:   ��ȡ�������͵����ò���
	// ����:   pKeyName - ������
	//         pValue - ���ڴ�Ŷ�ȡ������ֵ��
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT GetIntValue( const char* pKeyName, int* pValue ) const
	{
		return LPRSetting_GetIntValue( m_pObject, pKeyName, pValue );
	}

	// ����:   ��ȡ�������͵����ò���
	// ����:   pKeyName - ������
	//         pValue - ���ڴ�Ŷ�ȡ�ĸ���ֵ��
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT GetFloatValue( const char* pKeyName, float* pValue ) const
	{
		return LPRSetting_GetFloatValue( m_pObject, pKeyName, pValue );
	}

	// ����:   ��ȡ�������͵����ò���
	// ����:   pKeyName - ������
	//         pValue - ���ڴ�Ŷ�ȡ�Ĳ���ֵ��
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT GetBoolValue( const char* pKeyName, bool* pValue ) const
	{
		return LPRSetting_GetBoolValue( m_pObject, pKeyName, pValue );
	}
	
	// ����:   ��ȡ�ַ������͵����ò���
	// ����:   pKeyName - ������
	//         pBuf - �����ַ������ݵ����ݿ顣
	//         pLen - pBuf���ݿ�ĳ��ȣ��������ִ�гɹ���*pLen����ַ����ĳ���
	//         ��������β����'\0'�����������ʧ�ܲ�����LPR_BUFFER_TOO_SMALL����ʾ
	//         ���ݿ�̫С����ʱ*pLen�д������С���ݿ鳤�ȵĸ�ֵ������ -128����
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT GetStringValue( const char* pKeyName, char* pBuf, int* pLen ) const
	{
		return LPRSetting_GetStringValue( m_pObject, pKeyName, pBuf, pLen );
	}

	// ����:   �޸��������͵����ò���
	// ����:   pKeyName - ������
	//         value - �µĲ���ֵ��
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT SetIntValue( const char* pKeyName, int value )
	{
		return LPRSetting_SetIntValue( m_pObject, pKeyName, value );
	}

	// ����:   �޸ĸ������͵����ò���
	// ����:   pKeyName - ������
	//         value - �µĲ���ֵ��
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT SetFloatValue( const char* pKeyName, float value )
	{
		return LPRSetting_SetFloatValue( m_pObject, pKeyName, value );
	}

	// ����:   �޸Ĳ������͵����ò���
	// ����:   pKeyName - ������
	//         value - �µĲ���ֵ��
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT SetBoolValue( const char* pKeyName, bool value )
	{
		return LPRSetting_SetBoolValue( m_pObject, pKeyName, value );
	}
	
	// ����:   �޸��ַ������͵����ò���
	// ����:   pKeyName - ������
	//         pStr - �µ��ַ���ֵ���ַ�����NULL��β��
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT SetStringValue( const char* pKeyName, const char* pStr )
	{
		return LPRSetting_SetStringValue( m_pObject, pKeyName, pStr );
	}

	// ����:   �����������͵����ò���
	// ����:   pKeyName - ������
	//         value - ����ֵ��
	//         bOverWrite - ���ԭ�ȴ���pKeyName�����Ĳ����Ļ�,�Ƿ񸲸�ԭ���Ĳ�����
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT AddIntKey( const char* pKeyName, int value, bool bOverWrite )
	{
		return LPRSetting_AddIntKey( m_pObject, pKeyName, value, bOverWrite );
	}

	// ����:   ���Ӹ������͵����ò���
	// ����:   pKeyName - ������
	//         value - ����ֵ��
	//         bOverWrite - ���ԭ�ȴ���pKeyName�����Ĳ����Ļ�,�Ƿ񸲸�ԭ���Ĳ�����
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT AddFloatKey( const char* pKeyName, float value, bool bOverWrite )
	{
		return LPRSetting_AddFloatKey( m_pObject, pKeyName, value, bOverWrite );
	}

	// ����:   ���Ӳ������͵����ò���
	// ����:   pKeyName - ������
	//         value - ����ֵ��
	//         bOverWrite - ���ԭ�ȴ���pKeyName�����Ĳ����Ļ�,�Ƿ񸲸�ԭ���Ĳ�����
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT AddBoolKey( const char* pKeyName, bool value, bool bOverWrite )
	{
		return LPRSetting_AddBoolKey( m_pObject, pKeyName, value, bOverWrite );
	}
	
	// ����:   �����ַ������͵����ò���
	// ����:   pKeyName - ������
	//         pStr - �ַ���ֵ���ַ�����NULL��β��
	//         bOverWrite - ���ԭ�ȴ���pKeyName�����Ĳ����Ļ�,�Ƿ񸲸�ԭ���Ĳ�����
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT AddStringKey( const char* pKeyName, const char* pStr, bool bOverWrite )
	{
		return LPRSetting_AddStringKey( m_pObject, pKeyName, pStr, bOverWrite );
	}

	// ����:   ɾ�����ò���
	// ����:   pKeyName - ������
	// ����ֵ: ���ڵ���0��ʾִ�гɹ���С��0��ʾʧ�ܣ�ʧ��ֵ�μ�LPRResult.h
	LPRRESULT DeleteKey( const char* pKeyName )
	{
		return LPRSetting_DeleteKey( m_pObject, pKeyName );
	}
private:
	void*	m_pObject;
};

LPRUTILITY_API void*	  __stdcall LPRLog_Create();
LPRUTILITY_API void		  __stdcall LPRLog_Destroy( void* pObject );
LPRUTILITY_API LPRRESULT  __stdcall LPRLog_Init( void* pObject, const char* pFileName );
LPRUTILITY_API LPRRESULT  __stdcall LPRLog_Fini( const void* pObject );
LPRUTILITY_API LPRRESULT  __stdcall LPRLog_Log( const void* pObject, const char* str, va_list args );

class LPRLog
{
public:
	LPRLog() { m_pObject = LPRLog_Create(); }
	~LPRLog() { LPRLog_Destroy( m_pObject ); }
	LPRRESULT Init( const char* pFileName ) { return LPRLog_Init( m_pObject, pFileName ); }
	LPRRESULT Fini() { return LPRLog_Fini( m_pObject ); }
	LPRRESULT Log( const char* str, ... ) 
	{ 
		va_list args;
		va_start( args, str );
		LPRRESULT lr = LPRLog_Log( m_pObject, str, args ); 
		va_end( args );
		return lr;
	}
private:
	void*	m_pObject;
};

// һЩ��������
LPRUTILITY_API LPRRESULT __stdcall LPRRatioPointToPoint( LPRRatioPoint* pRPt, int width, int height, POINT* pPt );
LPRUTILITY_API LPRRESULT __stdcall PointToLPRRatioPoint( POINT* pPt, int width, int height, LPRRatioPoint* pRPt );
LPRUTILITY_API LPRRESULT __stdcall LPRGetExeFilePath( char* pPathBuf, int* pLen, bool needEndSlash );
LPRUTILITY_API LPRRESULT __stdcall LPRGetModuleFilePath( const char* pModuleName, char* pPathBuf, int* pLen, bool needEndSlash );


LPRUTILITY_API const char*		__stdcall LPRGetPlateTypeStr( int type );
LPRUTILITY_API const wchar_t*	__stdcall LPRGetPlateTypeStrW( int type );
LPRUTILITY_API const char*		__stdcall LPRGetPlateColorStr( int color );
LPRUTILITY_API const wchar_t*	__stdcall LPRGetPlateColorStrW( int color );

LPRUTILITY_API const char*		__stdcall LPRGetErrorStr( LPRRESULT err );
LPRUTILITY_API const wchar_t*	__stdcall LPRGetErrorStrW( LPRRESULT err );



#ifdef __cplusplus
}
#endif

#endif //__LPR_UTILITY_H__


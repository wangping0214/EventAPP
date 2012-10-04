#ifndef  __LPR_RESULT_H__
#define  __LPR_RESULT_H__

typedef long	LPRRESULT;

#define	LPR_OK								0		// ����ִ�гɹ�
#define LPR_SETTING_OVERWRITE				1		// �����Ӳ�������ʱ������ͬ������
#define LPR_NO_DEST_WINDOW					2		// û��ָ��Ŀ�괰��
#define LPR_ENGINE_SHORTCUT					3		// ʶ��ģ�鱻����
#define LPR_FRAME_SKIPPED					4		// ��֡����֡����
#define LPR_USER_ABORT						5		// �û�ȡ������
#define LPR_S_ALREADY_INITED				6		// ģ���Ѿ���ʼ����
#define LPR_S_NOT_INITED					7		// ģ����δ��ʼ����
#define LPR_RECOGNITION_DISABLED			8		// ʶ���ܱ��ر�
#define LPR_MOTION_DISABLED					9		// û�������˶����
#define LPR_MOTION_NO						10		// �����˶���⣬���˶�����
#define LPR_MOTION_YES						11		// �����˶���⣬���˶�����
#define LPR_EMPTY_FRAME						12		// ����ͼ��Ϊ��


#define LPR_FAIL							-1		// ����ִ��ʧ��
#define LPR_INVALID_PARAM					-2		// ��Ч����
#define LPR_NOT_INITED						-3		// ģ��û�г�ʼ��
#define LPR_KEY_NOT_FOUND					-4		// û���ҵ����ܹ�
#define LPR_END_OF_STREAM					-5		// ��Ƶ������
#define LPR_EXCEPTION						-6		// ����ʱ�����쳣
#define LPR_BUFFER_FULL						-7		// ��������
#define LPR_BUFFER_EMPTY					-8		// ��������
#define LPR_JPG_DATA_TOO_LARGE				-9		// JPG����̫��
#define LPR_INVALID_DATA					-10		// ��Ч����
#define LPR_CORRUPTED_SETTINGS				-11		// ��Ч����
#define LPR_SETTING_KEY_NOT_FOUND			-12		// û���ҵ�ָ���Ĳ�������
#define LPR_INVALID_SETTING_VALUE			-13		// ��Ч�Ĳ�������ֵ
#define LPR_BUFFER_TOO_SMALL				-14		// ������̫С
#define LPR_SETTING_KEY_EXIST				-15		// �Ѿ�����ָ���Ĳ�������
#define LPR_CLEANUP							-16		// ģ��������
#define LPR_ENGINE_STOPPED					-17		// LPREngine�Ѿ�ֹͣ����
#define LPR_NOT_SUPPORTED					-18		// ��֧��ָ�����ܻ�����
#define LPR_ALREADY_INITED					-19		// ģ���Ѿ���ʼ��
#define LPR_EXCEED_ENGINE_LICENCE			-20		// �����������󲢷�LPREngine��
#define LPR_WRONG_PASSWD					-21		// �������
#define LPR_CANNOT_OPEN_FILE				-22		// �޷����ļ�
#define LPR_KEY_VER_UNSUPPORTED				-23		// ��֧�ָü��ܹ��汾�������Ǽ��ܹ��汾̫�£�Ӧ�ó���汾̫��
#define LPR_NO_OUTPUT_AVAILABLE				-24		// ��ǰû��ʶ����
#define LPR_INVALID_INDEX					-25		// ��Ч������ֵ
#define LPR_CREATE_PEN_FAIL					-26		// ��������ʧ��
#define LPR_CREATE_FONT_FAIL				-27		// ��������ʧ��
#define LPR_DAHENG_STATIC_MEM_NEED_REBOOT	-28		// ���������˴�㿨������̬�ڴ棬��Ҫ����ʹ������Ч
#define LPR_KEY_READ_FAIL					-29		// �޷���ȡ������
#define LPR_KEY_WRITE_FAIL					-30		// �޷�д�������
#define LPR_KEY_INVALID						-31 	// ��Ч�ļ�����
#define LPR_KEY_EXPIRED						-32		// �������ѹ���
#define LPR_MASK_SIZE_NOT_MATCH				-33		// ʶ��������ͼ��ߴ粻ƥ��
#define LPR_INVALID_PLATE_WIDTH_PARAM		-34		// ���ƿ�Ȳ�����Ч
#define LPR_MODULE_NOT_FOUND				-35		// �Ҳ��������ģ���ļ�
#define LPR_NOT_LICENCED					-36		// ָ���Ĺ���û�б���Ȩ��������ͼ��ʼ����Ƶʶ��ģ�鵫�Ǽ��ܹ���û����Ƶʶ���Licence��
#define LPR_NOT_SUFFICIENT_LICENCE			-37		// ָ���Ĺ��ܳ����˵�ǰ��Ȩ��Χ��������ͼʶ�����ͼƬ���Ǽ��ܹ���ֻ��ʶ����ͨͼƬ��Licence��
#define LPR_ALREADY_INITED_IN_DIFF_MODE		-38		// ģ���Ѿ���ʼ��Ϊ����ģʽ
#define LPR_NOT_SUPPORTED_IN_CURR_MODE		-39		// ��ǰģʽ��֧�ִ˹���
#define LPR_INVALID_WND_HANDLE				-40 	// ��Ч�Ĵ��ھ��
#define LPR_CREATE_DDRAW_SURFACE_FAIL		-41 	// ����DirectDraw����ʧ��
#define LPR_CREATE_DDRAW_PALETTE_FAIL		-42 	// ����DirectDraw��ɫ��ʧ��
#define LPR_PARAM_OVERRIDDEN_BY_INI			-43 	// �ò�������INI�ļ������ã�Ӧ�ó����޷��޸ģ������Ҫ��Ӧ�ó������޸Ĵ˲�������Ҫ��INI�е���Ӧ����ɾ��
#define LPR_ALLOC_MEM_FAIL					-44 	// �����ڴ�ʧ��
#define LPR_CALIB_INFO_ERR					-45		// �궨��Ϣ����

#endif //__LPR_RESULT_H__


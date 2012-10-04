#ifndef _VSD_EVENT_DEFINES_H__
#define _VSD_EVENT_DEFINES_H__

/// ͬһ�������ץ��ͼ����Ŀ
#define MAX_CAPTURE_PER_OBJ 4

/// ͬһ�����������ܵ�Ŀ����Ŀ
#define MAX_OBJECTS	32

/// һ��Ŀ�����ʷ�켣�����ܵ�λ����Ŀ
#define MAX_TRACK_NUM 256

/// ������Ȧ�����ܵ���Ŀ
#define MAX_VIRTUAL_LOOPS 16

/// �źŵ��������ܵ���Ŀ
#define MAX_LIGHT_NUM 8

/// ����һ���źŵ������ܵ�״̬��Ŀ
#define MAX_LIGHT_STATUS 32

/// ��Ƶ�������ģʽ��ֻ�ܴ���ѡ��һ��
enum VSDEventType
{
	VSDEvent_VehicleHead = 0,		/// ��ͷģʽ, ���ڳ�ͷ����
	VSDEvent_VehicleTail = 1		/// ��βģʽ, ���ں��̵ƺͳ�β����
};

/// ��Ҫ���ĳ������ͣ��������Ϳ����ۼ�
enum VSDVehicleType
{
	VHT_SMALL		= 1,		/// �����ͨС�����аͣ�Ҳ���ܼ�⵽�󳵣��ܼ�����Ƴ�
	VHT_BIG			= 2,		/// ����ʿ���󳵣��ܼ�����Ƴ�
	VHT_MOTOR		= 4,		/// Ħ�г����ݲ�֧�֣�
	VHT_ENHANCE		= 1024,		/// �������ԣ���ǿ�����Ƴ��ļ�⣬����������
	VHT_REDUCE		= 2048		/// �������ԣ���������������ܽ��ͼ����
};

/// ����״̬���������Ϳ����ۼ�
enum VSDLightMode
{
	VSD_LIGHT_MODE_DAYLIGHT		= 1,	/// �������
	VSD_LIGHT_MODE_NIGHT		= 2,	/// ҹ�����
	VSD_LIGHT_MODE_AUTO			= 4		/// �Զ����գ��������Զ����գ�Ҫ��ProcessFrame������ȷ��ʱ��
};

/// ���ٵ������״̬���������Ϳ����ۼ�
enum VSDObjectStatus
{
	VSD_OBJ_STATUS_NORMAL			= 0,		/// ����״̬
	VSD_OBJ_STATUS_NEW				= 1,		/// ������������������
	VSD_OBJ_STATUS_DELETE			= 2,		/// �����뿪��������, Event�ڲ����ٴ��������
	VSD_OBJ_STATUS_LOW_CONFID		= 4,		/// �����������Ŷȵ�
	VSD_OBJ_STATUS_TOUCH_LINE		= 1024,		/// ����״̬�����崥����Ȧ
	VSD_OBJ_STATUS_LEAVE_LINE		= 2048,		/// ����״̬�������뿪��Ȧ
};

/// �����Ƿ�Ҫץ��, �Լ�ץ�ĵ�����, �������Ϳ����ۼ�
enum VSDObjectCapType
{
	VSD_OBJ_CAP_NONE			= 0,		/// ����Ҫץ��
	VSD_OBJ_CAP_DETECT			= 1,		/// ��⵽�����ץ��
	VSD_OBJ_CAP_TOUCH_LINE		= 2,		/// ������Ȧ��ץ��
	VSD_OBJ_CAP_LEAVE_LINE		= 4,		/// �뿪��Ȧ��ץ��
	VSD_OBJ_CAP_BREAK_RULE		= 8,		/// Υ�µ�ץ��
	VSD_OBJ_CAP_AWAY			= 16		/// �뿪���������ץ��
};


/// ������Ȧ���ڳ����ĵ����ԣ��������Ϳ����ۼ�
enum VSDLaneProperty
{
	VSD_LANE_STRAIGHT			= 1,		/// ����ֱ��
	VSD_LANE_TURN_LEFT			= 2,		/// ������ת
	VSD_LANE_TURN_RIGHT			= 4,		/// ������ת
	VSD_LANE_BAN_CROSS_LEFT		= 8,		/// ��ֹ���󲢵�
	VSD_LANE_BAN_CROSS_RIGHT	= 16		/// ��ֹ���Ҳ���
};

/// Υ�����򣬸������Ϳ����ۼ�
enum VSDBreakRule
{
	VSD_BR_NONE				= 0,		/// û��Υ��
	VSD_BR_TURN_LEFT		= 1,		/// Υ����ת
	VSD_BR_TURN_RIGHT		= 2,		/// Υ����ת
	VSD_BR_STRAIGHT_THROUGH	= 4,		/// Υ��ֱ��
	VSD_BR_CROSS_LANE		= 8,		/// Υ�²���
	VSD_BR_REVERSE			= 16,		/// ����
	VSD_BR_RED_LIGHT		= 32,		/// �����
	VSD_BR_STOP				= 64,		/// Υ��ͣ��
	VSD_BR_HIGH_SPEED		= 128,		/// ����
	VSD_BR_LOW_SPEED		= 256		/// ����
};

#endif // _VSD_EVENT_DEFINES_H__


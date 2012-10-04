#ifndef __VSD_EVENT_STRUCTURE_H__
#define __VSD_EVENT_STRUCTURE_H__

#include "VSDEventDefines.h"

/// ��
struct VSDPoint
{
	int x;
	int y;
};

/// ���ο�
struct VSDRect
{
	int x;
	int y;
	int width;
	int height;
};

/// ע�⣬��ģ���ڵ��������
/// ˮƽ�������������Ϳ�ȵĻ�׼��VSDEventParam::nWidthBase
/// ��ֱ�������������͸߶ȵĻ�׼��VSDEventParam::nHeightBase

/// �㣬�������
struct VSDRatioPoint
{
	int x;
	int y;
};

/// �ߣ��������
struct VSDRatioLine
{
	VSDRatioPoint	pt1;	/// �˵�1
	VSDRatioPoint	pt2;	/// �˵�2/
};

/// ���Σ��������
struct VSDRatioRECT 
{
	int left;
	int top;
	int right;
	int bottom;
};

/// ��ʼ������
struct VSDEventParam
{
	int				nEventType;		/// ��Ƶ�����������, ȡֵΪVSDEventType֮һ�����ֳ�ͷ����βģʽ
	int				nVehicleType;	/// �����������ͣ�ȡֵΪVSDVehicleType�и����͵��ۼ�

	/// ������������˵�ǰ����ʼ����VSDEvent���������������������ı�����������ĸ����
	/// ������VSDEventParam��Ĳ������Լ�������������������������ı�������
	int				nWidthBase;		/// x�������
	int				nHeightBase;	/// y�������
	/// ����һ�����������������1000�����ʾǧ�ֱ�����
	/// ���Ӷ���������������ֱ���Ϊʵ��ͼ�񳤿����൱��ʹ��ʵ������

	int				nVirtualLoop;	/// ʹ�õ�������Ȧ����
	VSDRatioLine	virtualLoopLine[MAX_VIRTUAL_LOOPS];		/// ������Ȧ
	int				loopLaneProperty[MAX_VIRTUAL_LOOPS];	/// ��Ȧ���ڳ��������ԣ�ȡֵΪVSDVLProperty�и����͵��ۼ�

	VSDRatioRECT	trackRegion;	/// ��������������꣩
									/// ���������������ֻ���и��٣��뿪�����ɾ��
									/// ����������, ���������ֹͣ�ĳ���

	VSDRatioPoint	ptRoad[4];		/// ·��궨�㣬Ҫ���ĸ���������ռ�����һ������

	VSDRatioPoint	ptPlate[2];		/// ������Ȧ����һ������λ�õĳ��ƿ��

	int				nLightMode;		/// ����ģ��, ȡֵΪVSDLightMode�и��������ۼ�ֵ
									/// �������VSD_LIGHT_MODE_AUTO��
									///	   nLightMode���������������ģ�ͣ���Ϊ��ʼ������ģ��
									///    VSDEvent�ڲ�����ݹ���ģ�͡���������ǿ�ȡ�ʱ����Ϣ�Զ�����

	int				nFrameRate;		/// ֡�ʣ���ÿ��֡��

	int				nAllowStopInTR;	/// �Ƿ�������ͣ�ڸ��������ڣ��Ƽ�������

	VSDRatioRECT	regionTurnLeft;	/// ��ת��ת���򣨸�����������ֹͣ��

	int				nIDMax;			/// VSDObject��uid�����ֵ������ȡֵ��nIDMax��֮���0���¼���

	int				pReserved[147];	/// ����
};

/// ÿ�ε��õ�����ʱ���Ʋ���
struct VSDEventControlParam
{
	int nLaneLightStatus[MAX_VIRTUAL_LOOPS];	/// ���г�����صĺ��̵�״̬��0: ��ƽ���, 1���̵ƿ�ͨ��
												/// nLaneLightStatus�е��±꣬������VSDEventParam��virtualLoopLine���±��Ӧ
	int pReserved[32];				/// ����
};

/// VSDEventϵͳ�ڲ�״̬��Ϣ
struct VSDEventStatus
{
	int				nLightMode;		/// VSDEvent�ڲ���ǰʹ�õĹ���ģ��
									/// ��VSD_LIGHT_MODE_DAYLIGHT��VSD_LIGHT_MODE_NIGHT���϶���
	int				nFrameBrightness;	/// ��ǰ֡������
	int				pReserved[126];	/// ����
};

/// ����Ŀ��
struct VSDObject
{
	int				uid;			/// Ŀ���id�ţ�ȡֵ[0, VSDEventParam::nIDMax]֮��ѭ��
	int				nLoopID;		/// Ŀ�������ĸ�������⵽��
	int				status;			/// ��ǰ֡Ŀ��Ĵ���״̬, ȡֵΪVSDObjectStatus�и����͵��ۼ�
	int				nBreakRule;		/// Ŀ��Υ���Ĺ���, ȡֵΪVSDBreakRule
	int				nCapType;		/// Ŀ�굱ǰ֡�Ƿ�ץ�ļ�ץ�����ͣ�ȡֵΪVSDObjectCapType�и����͵��ۼ�
	int				nTotalCaps;		/// Ŀ���ֹ����ǰ֡�ܹ�ץ���˶���֡
	VSDRect			rect;			/// Ŀ��λ��
};

/// ���Ŀ��
struct VSDObjectMulti
{
	int				nObjects;					/// Ŀ����Ŀ
	VSDObject		objects[MAX_OBJECTS];		/// ����Ŀ�������
};


/// Ŀ��켣
struct VSDObjectTrack
{
	int				uid;						/// Ŀ��id
	int				nLoopID;					/// Ŀ�������ĸ�������⵽��
	int				nBreakRule;					/// Ŀ��Υ���Ĺ���, ȡֵΪVSDBreakRule
	int				nTracks;					/// �켣��λ�ã�����Ŀ
	int				status[MAX_TRACK_NUM];		/// ��ǰ֡Ŀ��Ĵ���״̬, ȡֵΪVSDObjectStatus�и����͵��ۼ�
	VSDRect			tracks[MAX_TRACK_NUM];		/// ��֡��λ��
};

/// ���Ŀ��켣
struct VSDObjectTrackMulti
{
	int				nObjects;					/// Ŀ����Ŀ
	VSDObjectTrack	objTracks[MAX_OBJECTS];		/// ����Ŀ��Ĺ켣
};


#endif //__VSDEvent_STRUCTURE_H__


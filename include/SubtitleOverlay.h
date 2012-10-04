#ifndef SUBTITLEOVERLAY_H
#define SUBTITLEOVERLAY_H

#include <LPRVideo.h>
#include <string>
#include <map>

using std::map;
using std::string;

bool LPRCopySubImageToLarge(const LPRImage* pImSub, LPRImage* pImLarge, RECT rect);
bool LPROverlay(LPRImage* pImForeground, LPRImage* pImBackGround, int x, int y);

class SubtitleOverlay
{
public:
	virtual ~SubtitleOverlay(void);
	static SubtitleOverlay& getInstance();
	/**
	 * ��ָ���ֿ��е��ְ��ո�����С������ָ����С��ͼƬ�У���������ͼƬ�С�Ϊ��Ч�������
	 * ��ȷ���ֿ��е����Ѿ��������Ժ�Ҫ�õ��������֡�
	 *
	 * param str - �ֿ�
	 * param fontSize - �����С��������Ϊ��λ
	 * param fontImageWidth - ����ͼƬ�Ŀ�ȣ�������Ϊ��λ
	 * param fontImageHeight - ����ͼƬ�ĸ߶ȣ�������Ϊ��λ
	 */
	void initialize(const string &str, int fontSize, int fontImageWidth, int fontImageHeight);
	/**
	 * ��ָ����ͼƬ��ָ����λ�����ָ������Ļ�� 
	 *
	 * param imageFilePath - ͼƬ·����֧��LPRImage֧�ֵ�����ͼƬ���ͣ�
	 * param subtitle - ��Ļ����
	 * param startX - ��Ļ��ʼ��x���꣬������Ļ����ϵ
	 * param startY - ��Ļ��ʼ��y���꣬������Ļ����ϵ
	 */
	bool overlaySubtitle(const string &imageFilePath, const string &subtitle, int startX = 0, int startY = 0);
	/**
	 * ��ָ����ͼƬ��ָ����λ�����ָ������Ļ�������Ϊһ���µ�ͼƬ���÷�������ı�ԭ��ͼƬ�����ݡ� 
	 *
	 * param imageFilePath - ͼƬ·����֧��LPRImage֧�ֵ�����ͼƬ���ͣ�
	 * param saveAsPath - ͼƬ���Ϊ·��
	 * param subtitle - ��Ļ����
	 * param startX - ��Ļ��ʼ��x���꣬������Ļ����ϵ
	 * param startY - ��Ļ��ʼ��y���꣬������Ļ����ϵ
	 */
	bool overlaySubtitle(const string &imageFilePath, const string &saveAsPath, const string &subtitle, int startX = 0, int startY = 0);
	/**
	 * ��ָ����ͼƬ��ָ����λ�����ָ������Ļ�������Ϊһ���µ�ͼƬ���÷�������ı�ԭ��ͼƬ�����ݡ� 
	 *
	 * param pRawImage - ��JPG��ʽ�������ͼ��Ŀǰ��֧��JPG�������룩
	 * param saveAsPath - ͼƬ���Ϊ·��
	 * param subtitle - ��Ļ����
	 * param startX - ��Ļ��ʼ��x���꣬������Ļ����ϵ
	 * param startY - ��Ļ��ʼ��y���꣬������Ļ����ϵ
	 */
	bool overlaySubtitle(LPRImage *pRawImage, const string &saveAsPath, const string &subtitle, int startX = 0, int startY = 0);
private:
	SubtitleOverlay(void) {}
	SubtitleOverlay(const SubtitleOverlay &other) {}
	SubtitleOverlay& operator=(const SubtitleOverlay &other) { return *this; }
	bool bitmapToLPRImage(HDC hdc, HBITMAP hbm, LPRImage **pImagePtr);
	LPRImage* characterImage(wchar_t ch);
private:
	map<wchar_t, LPRImage*> mCharImageMap;	// �ַ���ͼƬ��ӳ��

	// �������
	int mFontSize;
	int mFontImgWidth;
	int mFontImgHeight;
};

#endif //SUBTITLEOVERLAY_H

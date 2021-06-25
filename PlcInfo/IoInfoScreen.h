#ifndef IOINFOSCREEN_H
#define IOINFOSCREEN_H

#include "ioinfo.h"


enum ITEMREDRAWTYPE { IRD_FULL, IRD_ADDR, IRD_VALUE, IRD_LABEL};

class IoInfoScreen : public  CStatic
{
public: 
	IoInfoScreen();
	~IoInfoScreen();
	void Draw(CDC *dc);

public:
	CPaintDC *m_dc;

private:
	HBRUSH hbBkBrush;
	CRect m_InitRectangle;

public:
	IoInfo *m_pIoInfos[MAXIOINFOS];

private:
	int m_iIoInfoCount;

public:
	bool InitScreen();
	int GetCount() { return m_iIoInfoCount; }
	int GetCurrentHeight();
	void RedrawItem(int ItemPos,ITEMREDRAWTYPE RedrawType=::IRD_VALUE);

public:
	void InsertBit(int VirtualPos, CString sAddressInfo, BYTE *pValue, BYTE LatchMask, int OffsetX, int OffsetY, CString sLabel=_T(""));
	void InsertBit(int VirtualPos, CString sAddressInfo, BYTE *pValue, BYTE LatchMask, CString sLabel=_T(""));

	void InsertByte(int VirtualPos, CString sAddressInfo, BYTE *pValue, int OffsetX, int OffsetY, CString sLabel=_T(""));
	void InsertByte(int VirtualPos, CString sAddressInfo, BYTE *pValue, CString sLabel=_T(""));

	void InsertByteArray(int VirtualPos ,CString sAddressInfo, BYTE *pValue,int Length, int OffsetX, int OffsetY, CString sLabel=_T(""));
	void InsertByteArray(int VirtualPos, CString sAddressInfo, BYTE *pValue,int Length, CString sLabel=_T(""));

	void InsertWord(int VirtualPos, CString sAddressInfo, WORD *pValue, CString sLabel=_T(""));
	void InsertWord(int VirtualPos, CString sAddressInfo, WORD *pValue, int OffsetX, int OffsetY, CString sLabel=_T(""));
	
	void InsertAddiDigDef(int VirtualPos, CString sAddressInfo, unsigned int *pValue, int iInfoCounterStart,bool IsDigitalInput);
	void InsertAddiDigDef(int VirtualPos, CString sAddressInfo, unsigned int *pValue, int OffsetX, int OffsetY, int iInfoCounterStart,bool IsDigitalInput);

	void UpdateScreenSize();

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};

#endif
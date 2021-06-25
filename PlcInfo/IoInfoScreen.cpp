#include "stdafx.h"
#include "ioinfoscreen.h"

BEGIN_MESSAGE_MAP(IoInfoScreen, CStatic)
	//{{AFX_MSG_MAP(IoInfoScreen)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IoInfoScreen::IoInfoScreen()
{
	m_iIoInfoCount = 0;
	memset(m_pIoInfos,0,sizeof(m_pIoInfos));
	hbBkBrush = CreateSolidBrush(RGB(212, 208, 200));
}

IoInfoScreen::~IoInfoScreen()
{
	//delete m_dc;
}

bool IoInfoScreen::InitScreen()
{
	GetWindowRect(&m_InitRectangle);
	m_dc = new CPaintDC(this); // device context for painting
	return true;
}


void IoInfoScreen::OnPaint() 
{
	CRect m_rect;
	GetClientRect(&m_rect);
	
	CPaintDC dc(this); // device context for painting
	FillRect(dc.m_hDC, &m_rect, hbBkBrush);
	Draw(&dc);
}

BOOL IoInfoScreen::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void IoInfoScreen::RedrawItem(int ItemPos, ITEMREDRAWTYPE RedrawType)
{
	if (ItemPos>m_iIoInfoCount) return;
	if (m_pIoInfos[ItemPos]== NULL) return;

	switch(RedrawType)
	{
	case ::IRD_FULL:
		RedrawWindow(&m_pIoInfos[ItemPos]->m_rect_complete);
		break;

	case ::IRD_ADDR:
		RedrawWindow(&m_pIoInfos[ItemPos]->m_rect_address);
		break;

	case ::IRD_VALUE:
		RedrawWindow(&m_pIoInfos[ItemPos]->m_rect_value);
		break;

	case ::IRD_LABEL:
		RedrawWindow(&m_pIoInfos[ItemPos]->m_rect_label);
		break;
	}
}

void IoInfoScreen::Draw(CDC *dc)
{
	for(int i=0; i<m_iIoInfoCount; i++)
	{
		m_pIoInfos[i]->Draw(dc);
	}
}

void IoInfoScreen::InsertBit(int VirtualPos, CString sAddressInfo, BYTE *pValue, BYTE LatchMask, CString sLabel)
{
	InsertBit(VirtualPos, sAddressInfo,pValue,LatchMask,STD_OFFSET_X,STD_OFFSET_Y+GetCurrentHeight()/*((STD_OFFSET_Y+STD_LINE_HEIGHT)*m_iIoInfoCount)*/,sLabel);
}

void IoInfoScreen::InsertBit(int VirtualPos, CString sAddressInfo, BYTE *pValue, BYTE LatchMask, int OffsetX, int OffsetY, CString sLabel)
{
	m_pIoInfos[m_iIoInfoCount] = new IoInfo(VirtualPos, ::IOT_BIT,pValue,LatchMask,OffsetX,OffsetY,sLabel,sAddressInfo);
	(*m_pIoInfos[m_iIoInfoCount]).UpdateLabelDimensions(m_dc);
	m_iIoInfoCount++;
	UpdateScreenSize();
}

void IoInfoScreen::InsertByte(int VirtualPos, CString sAddressInfo, BYTE *pValue, CString sLabel)
{
	InsertByte(VirtualPos, sAddressInfo,pValue,STD_OFFSET_X,STD_OFFSET_Y+GetCurrentHeight()/*((STD_OFFSET_Y+STD_LINE_HEIGHT)*m_iIoInfoCount)*/,sLabel);
}

void IoInfoScreen::InsertByte(int VirtualPos, CString sAddressInfo, BYTE *pValue, int OffsetX, int OffsetY, CString sLabel)
{
	m_pIoInfos[m_iIoInfoCount] = new IoInfo(VirtualPos, ::IOT_BYTE,pValue,OffsetX,OffsetY,sLabel,sAddressInfo);
	(*m_pIoInfos[m_iIoInfoCount]).UpdateLabelDimensions(m_dc);

	m_iIoInfoCount++;
	UpdateScreenSize();
}

void IoInfoScreen::InsertByteArray(int VirtualPos, CString sAddressInfo, BYTE *pValue,int Length, int OffsetX, int OffsetY, CString sLabel)
{
	m_pIoInfos[m_iIoInfoCount] = new IoInfo(VirtualPos, ::IOT_BYTEARRAY,pValue,Length,OffsetX,OffsetY,sLabel,sAddressInfo);
	(*m_pIoInfos[m_iIoInfoCount]).UpdateLabelDimensions(m_dc);

	m_iIoInfoCount++;
	UpdateScreenSize();
}

void IoInfoScreen::InsertByteArray(int VirtualPos, CString sAddressInfo, BYTE *pValue,int Length, CString sLabel)
{
	InsertByteArray(VirtualPos, sAddressInfo,pValue,Length,STD_OFFSET_X,STD_OFFSET_Y+GetCurrentHeight()/*((STD_OFFSET_Y+STD_LINE_HEIGHT)*m_iIoInfoCount)*/,sLabel);
}

void IoInfoScreen::InsertWord(int VirtualPos, CString sAddressInfo, WORD *pValue, CString sLabel)
{
	InsertWord(VirtualPos, sAddressInfo, pValue,STD_OFFSET_X,STD_OFFSET_Y+GetCurrentHeight()/*((STD_OFFSET_Y+STD_LINE_HEIGHT)*m_iIoInfoCount)*/,sLabel);
}

void IoInfoScreen::InsertWord(int VirtualPos, CString sAddressInfo, WORD *pValue, int OffsetX, int OffsetY, CString sLabel)
{
	m_pIoInfos[m_iIoInfoCount] = new IoInfo(VirtualPos, ::IOT_WORD,pValue,OffsetX,OffsetY,sLabel,sAddressInfo);
	(*m_pIoInfos[m_iIoInfoCount]).UpdateLabelDimensions(m_dc);

	m_iIoInfoCount++;
	UpdateScreenSize();
}

void IoInfoScreen::InsertAddiDigDef(int VirtualPos, CString sAddressInfo,unsigned int *pValue,int iInfoCounterStart, bool IsDigitalInput)
{
	InsertAddiDigDef(VirtualPos, sAddressInfo,pValue, STD_OFFSET_X,STD_OFFSET_Y+GetCurrentHeight()/*((STD_OFFSET_Y+STD_LINE_HEIGHT)*m_iIoInfoCount)*/,iInfoCounterStart,IsDigitalInput);
}

void IoInfoScreen::InsertAddiDigDef(int VirtualPos, CString sAddressInfo,unsigned int *pValue, int OffsetX, int OffsetY,int iInfoCounterStart,bool IsDigitalInput)
{
	m_pIoInfos[m_iIoInfoCount] = new IoInfo(VirtualPos, ::IOT_ADDI_DIG_IO,pValue,OffsetX,OffsetY,sAddressInfo,iInfoCounterStart,IsDigitalInput);
	m_iIoInfoCount++;
	UpdateScreenSize();
}

int IoInfoScreen::GetCurrentHeight()
{
	if (m_iIoInfoCount<=0) return 0;
	return m_pIoInfos[m_iIoInfoCount-1]->GetEndY();
}

void IoInfoScreen::UpdateScreenSize()
{
	if (m_iIoInfoCount<=0) return;

	int FullHeight= GetCurrentHeight();
//	MoveWindow(m_InitRectangle.left,m_InitRectangle.top,920,FullHeight+STD_OFFSET_Y);
	MoveWindow(0,12,920,FullHeight+STD_OFFSET_Y);
}

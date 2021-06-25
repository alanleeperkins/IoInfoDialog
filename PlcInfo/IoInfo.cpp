#include "stdafx.h"
#include "ioinfo.h"
#include <math.h>     

IoInfo::IoInfo(int VirtualPos, IOINFOTYPE type, unsigned int *xLatch, unsigned int xlatchMask, int xOffset, int yOffset, CString sLabel, CString sAddressInfo)
{
	m_iVirtualPos = VirtualPos;
	m_IoType = type;
	m_iColor = COLOR_BLUE;
	m_piLatch = xLatch;
	m_iLatchMask = xlatchMask;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_sLabel = sLabel;
	m_sAddressInfo = sAddressInfo;

	Init();
}

IoInfo::IoInfo(int VirtualPos, IOINFOTYPE type,BYTE *xLatch, BYTE xlatchMask, int xOffset, int yOffset, CString sLabel, CString sAddressInfo)
{
	m_iVirtualPos = VirtualPos;
	m_pbLatch = xLatch;
	m_bLatchMask = (unsigned int)xlatchMask;
	m_iColor = COLOR_BLUE;
	m_IoType = type;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_sLabel = sLabel;
	m_sAddressInfo = sAddressInfo;

	m_rect_complete = new CRect(xOffset,yOffset,500,yOffset+STD_LINE_HEIGHT);
	Init();

}

IoInfo::IoInfo(int VirtualPos, IOINFOTYPE type, BYTE *bLatch, int xOffset, int yOffset, CString sLabel, CString sAddressInfo)
{
	m_iVirtualPos = VirtualPos;
	m_pbLatch = bLatch;
	m_iColor = COLOR_BLUE;
	m_IoType = type;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_sLabel = sLabel;
	m_sAddressInfo = sAddressInfo;

	m_rect_complete = new CRect(xOffset,yOffset,500,yOffset+STD_LINE_HEIGHT);
	Init();
}

IoInfo::IoInfo(int VirtualPos, IOINFOTYPE type, BYTE *bLatches, int iLength, int xOffset, int yOffset, CString sLabel, CString sAddressInfo)
{
	m_iVirtualPos = VirtualPos;
	m_iLatchesCount = iLength;
	m_pbLatch = bLatches;
	m_iColor = COLOR_BLUE;
	m_IoType = type;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_sLabel = sLabel;
	m_sAddressInfo = sAddressInfo;

	m_rect_complete = new CRect(xOffset,yOffset,500,yOffset+STD_LINE_HEIGHT);
	Init();
}

IoInfo::IoInfo(int VirtualPos, IOINFOTYPE type, WORD *wLatch, int xOffset, int yOffset, CString sLabel, CString sAddressInfo)
{
	m_iVirtualPos = VirtualPos;
	m_pwLatch = wLatch;
	m_iColor = COLOR_BLUE;
	m_IoType = type;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_sLabel = sLabel;
	m_sAddressInfo = sAddressInfo;

	m_rect_complete = new CRect(xOffset,yOffset,1000,yOffset+STD_LINE_HEIGHT);
	Init();
}

IoInfo::IoInfo(int VirtualPos, IOINFOTYPE type, unsigned int *pLatch, int xOffset, int yOffset, CString sAddressInfo,int iInfoCounterStart,bool IsDigitalInput)
{
	m_iVirtualPos = VirtualPos;
	m_piLatch = pLatch;
	m_iColor = COLOR_BLUE;
	m_IoType = type;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_bIsDigitalInput = IsDigitalInput;
	m_iInfoCounterStart = iInfoCounterStart;
	m_sAddressInfo = sAddressInfo;

	m_rect_complete = new CRect(xOffset,yOffset,1000,yOffset+15);	
	Init();
}

void IoInfo::Init()
{
	// add the virtual addres to the address info (if legal)
	if(m_iVirtualPos!=-2)
	{
		CString sTmp;
		sTmp.Format(_T("[%.2i] %s"),m_iVirtualPos,m_sAddressInfo);
		m_sAddressInfo = sTmp;
	}

	m_SingleBit_Height = STD_LINE_HEIGHT;
	m_SingleBit_AddressInfoWidth = 170;
	m_SingleBit_SingleBitSymbolWidth = 50;
	m_SingleBit_LabelWidth = STD_LABEL_WIDTH;

	m_SingleByte_Height = STD_LINE_HEIGHT;
	m_SingleByte_AddressInfoWidth = 170;
	m_SingleByte_SingleByteBitSymbolsWidth = 150;
	m_SingleByte_SingleByteValueWidth = 110;
	m_SingleByte_LabelWidth = STD_LABEL_WIDTH;

	m_SingleByteArray_Height = 150;
	m_SingleByteArray_AddressInfoWidth = 170;
	m_SingleByteArray_SingleByteBitSymbolsWidth = 150;
	m_SingleByteArray_SingleByteValueWidth = 50;
	m_SingleByteArray_MaxValuesPerLine = 5;
	m_SingleByteArray_LabelWidth = STD_LABEL_WIDTH;
	m_SingleByteArray_LineCount = 1;

	m_SingleWord_Height = STD_LINE_HEIGHT;
	m_SingleWord_AddressInfoWidth = 170;
	m_SingleWord_SingleWordValueWidth = 110;
	m_SingleWord_LabelWidth = STD_LABEL_WIDTH;

	m_AddiDigInfo_Height = STD_LINE_HEIGHT;
	m_AddiDigInfo_AddressInfoWidth = 170;
	m_AddiDigInfo_SingleLineBitSymbolsWidth = 740; // each line shows 32 i/o

	m_PenBitDefActive.CreatePen(PS_SOLID,1,colors[COLOR_WHITE]);
	m_PenBitDefInActive.CreatePen(PS_SOLID,1,colors[COLOR_BLACK]);

	m_BrushTransparent.CreateSolidBrush(colors[COLOR_TRANSPARENT]);
	m_BrushChoosenColor.CreateSolidBrush(colors[m_iColor]);
	m_BrushWhite.CreateSolidBrush(colors[COLOR_WHITE]);
	
	InitFonts();

	CalculateRectangles();
}

void IoInfo::InitFonts()
{
	// FontStandard
	LOGFONT LogFontStandard;
	memset(&LogFontStandard, 0, sizeof(LOGFONT));
	LogFontStandard.lfHeight =16;
	LogFontStandard.lfWeight = 500;
	_tcscpy_s(LogFontStandard.lfFaceName, _T("Microsoft Sans Serif"));
	m_FontStandard.CreateFontIndirect(&LogFontStandard);

	// FontLabel
	m_FontLabel.CreatePointFont( 90,_T("Microsoft Sans Serif"));

	m_FontValue.CreatePointFont( 100,_T("Consolas"));
	m_FontValueSmall.CreatePointFont( 80,_T("Consolas"));

	m_FontSingleBitTitle.CreatePointFont( 80,_T("Consolas"));
}


void IoInfo::UpdateLabelDimensions(CDC *dc)
{
	CFont *pOldFont;
	CRect rect(m_xOffset,m_yOffset,STD_LABEL_WIDTH,0);
	int iLength = m_sLabel.GetLength();
	HDC hdc = *dc;	
	pOldFont=dc->SelectObject(&m_FontLabel);
	DrawText(hdc,m_sLabel,iLength,&rect,DT_WORDBREAK|DT_CALCRECT);
	dc->SelectObject(pOldFont);

	m_iActiveLabelHeight = (rect.bottom-rect.top)+10;

	CalculateRectangles();
}

void IoInfo::CalculateRectangles()
{
	switch(m_IoType)
	{
	case ::IOT_BIT:
		m_rect_address.left = m_xOffset;
		m_rect_address.right = m_rect_address.left+m_SingleBit_AddressInfoWidth;
		m_rect_address.top = m_yOffset;
		m_rect_address.bottom = m_rect_address.top+m_SingleBit_Height;

		m_rect_value.left = m_rect_address.right;
		m_rect_value.right = m_rect_value.left+m_SingleBit_SingleBitSymbolWidth;
		m_rect_value.top = m_yOffset;
		m_rect_value.bottom = m_rect_value.top+m_SingleBit_Height;

		m_rect_label.left = m_rect_value.right;
		m_rect_label.right = m_rect_label.left+m_SingleBit_LabelWidth;
		m_rect_label.top = m_yOffset;
		m_rect_label.bottom = m_rect_label.top+m_iActiveLabelHeight;
	
		m_rect_complete.left = m_xOffset;
		m_rect_complete.right = m_rect_complete.left+STD_LABEL_START_X+m_SingleBit_LabelWidth;//m_rect_complete.left+STD_LABEL_START_X+m_SingleBit_LabelWidth;//m_SingleBit_AddressInfoWidth+m_SingleBit_SingleBitSymbolWidth+m_SingleBit_LabelWidth;
		m_rect_complete.top = m_yOffset;
		m_rect_complete.bottom = m_rect_complete.top+m_SingleBit_Height;

		if (m_rect_address.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_address.bottom;
		if (m_rect_value.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_value.bottom;
		if (m_rect_label.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_label.bottom;
		break;

	case ::IOT_BYTE:		
		m_rect_address.left = m_xOffset;
		m_rect_address.right = m_rect_address.left+m_SingleByte_AddressInfoWidth;
		m_rect_address.top = m_yOffset;
		m_rect_address.bottom = m_rect_address.top+m_SingleByte_Height;

		m_rect_value.left = m_rect_address.right;
		m_rect_value.right = m_rect_value.left+m_SingleByte_SingleByteBitSymbolsWidth+m_SingleByte_SingleByteValueWidth;
		m_rect_value.top = m_yOffset;
		m_rect_value.bottom = m_rect_value.top+m_SingleByte_Height;	
		
		m_rect_label.left = m_rect_value.right;
		m_rect_label.right = m_rect_label.left+m_SingleByte_LabelWidth;
		m_rect_label.top = m_yOffset;
		m_rect_label.bottom = m_rect_label.top+m_iActiveLabelHeight;
	
		m_rect_complete.left = m_xOffset;
		m_rect_complete.right = m_rect_complete.left+STD_LABEL_START_X+m_SingleBit_LabelWidth;//m_rect_complete.left+m_SingleByte_AddressInfoWidth+m_SingleByte_SingleByteBitSymbolsWidth+m_SingleByte_SingleByteValueWidth+m_SingleByte_LabelWidth;
		m_rect_complete.top = m_yOffset;
		m_rect_complete.bottom = m_rect_complete.top+m_SingleByte_Height;

		if (m_rect_address.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_address.bottom;
		if (m_rect_value.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_value.bottom;
		if (m_rect_label.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_label.bottom;

	break;

	case ::IOT_BYTEARRAY:
		m_rect_address.left = m_xOffset;
		m_rect_address.right = m_rect_address.left+m_SingleByteArray_AddressInfoWidth;
		m_rect_address.top = m_yOffset;
		m_rect_address.bottom = m_rect_address.top+STD_TEXT_HEIGHT;

		m_SingleByteArray_LineCount = ceil((double)m_iLatchesCount/(double)m_SingleByteArray_MaxValuesPerLine);
		m_rect_value.left = m_rect_address.right;
		m_rect_value.right = m_rect_value.left+(m_SingleByteArray_SingleByteValueWidth*m_SingleByteArray_MaxValuesPerLine);
		m_rect_value.top = m_yOffset;
		m_rect_value.bottom = m_rect_value.top+(((STD_TEXT_HEIGHT)+(STD_TEXT_LINE_SEP))*m_SingleByteArray_LineCount)+STD_TEXT_HEIGHT;
	
		m_rect_label.left = m_rect_value.right;
		m_rect_label.right = m_rect_label.left+m_SingleByteArray_LabelWidth;
		m_rect_label.top = m_yOffset;
		m_rect_label.bottom = m_rect_label.top+m_iActiveLabelHeight;

		m_rect_complete.left = m_xOffset;
		m_rect_complete.right = m_rect_complete.left+STD_LABEL_START_X+m_SingleBit_LabelWidth;//m_rect_complete.left+m_SingleByteArray_AddressInfoWidth+m_SingleByteArray_SingleByteBitSymbolsWidth+m_SingleByteArray_SingleByteValueWidth+m_SingleByteArray_LabelWidth;
		m_rect_complete.top = m_yOffset;
		m_rect_complete.bottom = m_rect_value.bottom;
	
		if (m_rect_address.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_address.bottom;
		if (m_rect_value.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_value.bottom;
		if (m_rect_label.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_label.bottom;
		break;

	case ::IOT_WORD:		
		m_rect_address.left = m_xOffset;
		m_rect_address.right = m_rect_address.left+m_SingleWord_AddressInfoWidth;
		m_rect_address.top = m_yOffset;
		m_rect_address.bottom = m_rect_address.top+m_SingleWord_Height;

		m_rect_value.left = m_rect_address.right;
		m_rect_value.right = m_rect_value.left+m_SingleWord_SingleWordValueWidth;
		m_rect_value.top = m_yOffset;
		m_rect_value.bottom = m_rect_value.top+m_SingleWord_Height;
	
		m_rect_label.left = m_rect_value.right;
		m_rect_label.right = m_rect_label.left+m_SingleWord_LabelWidth;
		m_rect_label.top = m_yOffset;
		m_rect_label.bottom = m_rect_label.top+m_iActiveLabelHeight;

		m_rect_complete.left = m_xOffset;
		m_rect_complete.right = m_rect_complete.left+m_SingleWord_AddressInfoWidth+m_SingleWord_SingleWordValueWidth+m_SingleWord_LabelWidth;
		m_rect_complete.top = m_yOffset;
		m_rect_complete.bottom = m_rect_complete.top+m_SingleWord_Height;

		if (m_rect_address.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_address.bottom;
		if (m_rect_value.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_value.bottom;
		if (m_rect_label.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_label.bottom;
	
		break;

	case ::IOT_ADDI_DIG_IO:
	
		m_rect_address.left = m_xOffset;
		m_rect_address.right = m_rect_address.left+m_SingleWord_AddressInfoWidth;
		m_rect_address.top = m_yOffset;
		m_rect_address.bottom = m_rect_address.top+m_SingleWord_Height;

		m_rect_value.left = m_rect_address.right;
		m_rect_value.right = m_rect_value.left+m_AddiDigInfo_SingleLineBitSymbolsWidth;
		m_rect_value.top = m_yOffset;
		m_rect_value.bottom = m_rect_value.top+m_AddiDigInfo_Height;
	
		m_rect_complete.left = m_xOffset;
		m_rect_complete.right = m_rect_complete.left+m_AddiDigInfo_AddressInfoWidth+m_AddiDigInfo_SingleLineBitSymbolsWidth;
		m_rect_complete.top = m_yOffset;
		m_rect_complete.bottom = m_rect_complete.top+m_AddiDigInfo_Height;

		if (m_rect_address.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_address.bottom;
		if (m_rect_value.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_value.bottom;
		if (m_rect_label.bottom>m_rect_complete.bottom)m_rect_complete.bottom = m_rect_label.bottom;
		break; 

	default: 
		break;
	}
}

void IoInfo::Draw(CDC *dc)
{
	bool bItemFound = true;
	switch(m_IoType)
	{
	case ::IOT_BIT:
		DrawSingleBit(dc);
		break;

	case ::IOT_BYTE:
		DrawSingleByte(dc);
		break;

	case ::IOT_BYTEARRAY:
		DrawSingleByteArray(dc);
		break;

	case ::IOT_WORD:
		DrawSingleWord(dc);
		break;

	case ::IOT_ADDI_DIG_IO:
		DrawSingleAddiDigIO(dc);
		break; 
	default: 
		bItemFound = false;
		break;
	}

	if (bItemFound)
	{
		DrawVerticalSeperator(dc);
	}

}

void IoInfo::DrawVerticalSeperator(CDC *dc)
{
    CPen *pOldPen;
	CBrush *pOldBrush;

	int VerticalSeperatorWidth = STD_VERT_SEP_WITH;

	int StartX =STD_VERT_SEP_OFFSET_X;
	int StartY =m_yOffset+GetHeight()+1;
	int EndX =StartX+VerticalSeperatorWidth;
	int EndY =StartY;


	pOldPen=dc->SelectObject(&m_PenBitDefInActive);
	pOldBrush=dc->SelectObject(&m_BrushTransparent);

	dc->MoveTo(StartX,StartY);
	dc->LineTo(EndX,EndY);

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
}

int IoInfo::GetHeight()
{
	switch(m_IoType)
	{
	case ::IOT_BIT:
		return m_rect_complete.bottom-m_rect_complete.top;
		break;

	case ::IOT_BYTE:
		return m_rect_complete.bottom-m_rect_complete.top;
		break;

	case ::IOT_BYTEARRAY:
		return m_rect_complete.bottom-m_rect_complete.top;
		break;

	case ::IOT_WORD:
		return m_rect_complete.bottom-m_rect_complete.top;
		break;

	case ::IOT_ADDI_DIG_IO:
		return m_rect_complete.bottom-m_rect_complete.top;
		break; 

	default: 
		return STD_LINE_HEIGHT;
		break;
	}
}

bool IoInfo::IsPointInLed(CPoint point) 
{
	CRect rec(5+m_xOffset, 5+m_xOffset,m_xOffset+20, m_xOffset+20);
	return rec.PtInRect(point)!=0; 
}

void IoInfo::DrawSingleBit(CDC *dc)
{
    CPen *pOldPen;
	CBrush *pOldBrush;
	CString sTempText;	

	int StartAddressX = m_xOffset+0;
	int StartAddressY = m_yOffset;

	int StartBitSymbolX = m_xOffset+m_SingleBit_AddressInfoWidth;
	int StartBitSymbolY = m_yOffset+2;

	int StartLabelX = m_xOffset+StartBitSymbolX+m_SingleBit_SingleBitSymbolWidth;
	int StartLabelY = m_yOffset;

	// draw address info
	DrawSingleAddressInfo(dc,m_sAddressInfo,StartAddressX,StartAddressY);

	// draw bit state
	DrawSingleBitBoxBig(dc, StartBitSymbolX, StartBitSymbolY, (*m_pbLatch)&m_bLatchMask);

	// draw label
	StartLabelX = STD_LABEL_START_X;
	StartLabelY-=4;
	int iLength = m_sLabel.GetLength();
	CRect rect(StartLabelX,StartLabelY,StartLabelX+STD_LABEL_WIDTH,0);
	DrawLabel(dc,m_sLabel,iLength,&rect);

	pOldPen=dc->SelectObject(&m_PenBitDefInActive);
	pOldBrush=dc->SelectObject(&m_BrushTransparent);

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
}

void IoInfo::DrawSingleByte(CDC *dc)
{
    CPen *pOldPen;
	CBrush *pOldBrush;
	CString sTempText;	
	
	pOldPen=dc->SelectObject(&m_PenBitDefInActive);
	pOldBrush=dc->SelectObject(&m_BrushTransparent);

	int StartAddressX = m_xOffset+0;
	int StartAddressY = m_yOffset;

	int StartBitSymbolsX = m_xOffset+m_SingleByte_AddressInfoWidth;
	int StartBitSymbolsY = m_yOffset+2;

	int StartValueX = m_xOffset+StartBitSymbolsX+m_SingleByte_SingleByteBitSymbolsWidth;
	int StartValueY = m_yOffset;

	int StartLabelX = m_xOffset+StartValueX+m_SingleByte_SingleByteValueWidth;
	int StartLabelY = m_yOffset;

	// draw address info
	DrawSingleAddressInfo(dc,m_sAddressInfo,StartAddressX,StartAddressY);

	// draw single byte box
	DrawSingleByteBox(dc,m_pbLatch,StartBitSymbolsX,StartBitSymbolsY);
	
	// draw value
	if((*m_pbLatch)>=0x21 && (*m_pbLatch)<=0x7E) sTempText.Format(_T("d%.3i [h%.2X] '%c'"),(*m_pbLatch),(*m_pbLatch),(*m_pbLatch));
	else sTempText.Format(_T("d%.3i [h%.2X]"),(*m_pbLatch),(*m_pbLatch));
	DrawValueInfo(dc,sTempText,StartValueX,StartValueY);

	// draw label
	StartLabelX = STD_LABEL_START_X;
	StartLabelY-=4;
	int iLength = m_sLabel.GetLength();
	CRect rect(StartLabelX,StartLabelY,StartLabelX+STD_LABEL_WIDTH,0);
	DrawLabel(dc,m_sLabel,iLength,&rect);

	CalculateRectangles();

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
}

void IoInfo::DrawSingleByteArray(CDC *dc)
{
    CFont *pOldFont;
    CPen *pOldPen;
	CBrush *pOldBrush;
	CString sTempText;	
	
	pOldPen=dc->SelectObject(&m_PenBitDefInActive);
	pOldBrush=dc->SelectObject(&m_BrushTransparent);

	int StartAddressX = m_xOffset+0;
	int StartAddressY = m_yOffset;

	int StartValueX = m_xOffset+m_SingleByteArray_AddressInfoWidth;
	int StartValueY = m_yOffset;

	int StartLabelX = m_xOffset+StartValueX+(m_SingleByteArray_SingleByteValueWidth*m_SingleByteArray_MaxValuesPerLine);
	int StartLabelY = m_yOffset;

	// draw address info
	DrawSingleAddressInfo(dc,m_sAddressInfo,StartAddressX,StartAddressY);

	int PosValueX = StartValueX;
	int PosValueY = StartValueY;

	int LineCount = 1;
	for(int i=0; i<m_iLatchesCount; i++)
	{
		byte value = *(m_pbLatch+i);
		// draw value
		sTempText.Format(_T(" d%.3i"),value);
		DrawValueInfo(dc,sTempText,PosValueX,PosValueY);
		sTempText.Format(_T("[h%.3X]"),value);
		DrawValueInfo(dc,sTempText,PosValueX,PosValueY+STD_TEXT_HEIGHT);
		PosValueX+=m_SingleByteArray_SingleByteValueWidth;

		if((i>0) && ((i+1)%(m_SingleByteArray_MaxValuesPerLine) == 0))
		{
			PosValueY+=STD_TEXT_HEIGHT+STD_TEXT_LINE_SEP;
			PosValueX = StartValueX;
			LineCount++;
		}
	}

	m_SingleByteArray_LineCount = LineCount;

	// recalc the rectangles
	CalculateRectangles();

	StartLabelX = STD_LABEL_START_X;
	StartLabelY-=4;
	int iLength = m_sLabel.GetLength();
	CRect rect(StartLabelX,StartLabelY,StartLabelX+STD_LABEL_WIDTH,0);
	DrawLabel(dc,m_sLabel,iLength,&rect);

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
}

void IoInfo::DrawSingleWord(CDC *dc)
{
    CPen *pOldPen;
	CBrush *pOldBrush;
	CString sTempText;	

	// a legal color must be set
	if (!m_iColor) return;

	pOldPen=dc->SelectObject(&m_PenBitDefInActive);
	pOldBrush=dc->SelectObject(&m_BrushTransparent);

	// Y
	int StartAddressX = m_xOffset+0;
	int StartAddressY = m_yOffset;

	int StartValueX = m_xOffset+m_SingleWord_AddressInfoWidth;
	int StartValueY = m_yOffset;

	int StartLabelX = m_xOffset+StartValueX+m_SingleWord_SingleWordValueWidth;
	int StartLabelY = m_yOffset;

	// draw address info
	DrawSingleAddressInfo(dc,m_sAddressInfo,StartAddressX,StartAddressY);

	// draw value
	sTempText.Format(_T("d%.5i [h%.4X]"),(*m_pwLatch),(*m_pwLatch));
	DrawValueInfo(dc,sTempText,StartValueX,StartValueY);

	// draw label
	StartLabelX = STD_LABEL_START_X;
	StartLabelY-=4;
	int iLength = m_sLabel.GetLength();
	CRect rect(StartLabelX,StartLabelY,StartLabelX+STD_LABEL_WIDTH,0);
	DrawLabel(dc,m_sLabel,iLength,&rect);

	CalculateRectangles();

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
}

void IoInfo::DrawSingleAddiDigIO(CDC *dc)
{
    CPen *pOldPen;
	CBrush *pOldBrush;
	
	pOldPen=dc->SelectObject(&m_PenBitDefInActive);
	pOldBrush=dc->SelectObject(&m_BrushTransparent);

	int StartAddressX = m_xOffset;
	int StartAddressY = m_yOffset;

	int StartSingleLineBitSymbolsX = m_xOffset+m_AddiDigInfo_AddressInfoWidth;
	int StartSingleLineBitSymbolsY = m_yOffset+2;

	// draw address info
	DrawSingleAddressInfo(dc,m_sAddressInfo,StartAddressX,StartAddressY);

	// draw single bit array box
	DrawSingleBitArrayBox(dc,m_piLatch,StartSingleLineBitSymbolsX,StartSingleLineBitSymbolsY);

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
}

void IoInfo::DrawSingleAddressInfo(CDC *dc, CString sAddressInfo, int iStartX, int StartY)
{
	dc->SelectObject(&m_BrushTransparent);
	dc->SetTextColor(colors[COLOR_ADDRESS]);

	dc->SetBkMode(TRANSPARENT);
	dc->TextOutW(iStartX,StartY,sAddressInfo);
	dc->SetTextColor(colors[COLOR_BLACK]);

}

void IoInfo::DrawSingleByteBox(CDC *dc, BYTE* pValue, int iStartX, int StartY)
{
	// draw the single bits
	int SeperatorSize = 3; 	
	int TempX=iStartX;
	dc->SelectObject(&m_BrushTransparent);
	BYTE tester = 0x80;
	CString BitDef;
	for(int i=8; i>0; i--)
	{
		BitDef.Format(_T("b%i"),i);
		DrawSingleBitBox(dc,BitDef,TempX,StartY,((BYTE)*pValue)&tester);
	
		TempX +=(15+SeperatorSize);
		tester = tester>>1;
	}
}

void IoInfo::DrawSingleBitBox(CDC *dc, CString BitDef, int iStartX, int StartY, bool IsActive)
{
	CPen *pOldPen;
	CBrush *pOldBrush;
	CFont *pOldFont;
	
	pOldFont=dc->SelectObject(&m_FontSingleBitTitle);

	int Height = 5;
	int Width = 15;

	if (IsActive)
	{
		pOldBrush=dc->SelectObject(&m_BrushChoosenColor);	
	}
	else
	{
		pOldBrush=dc->SelectObject(&m_BrushWhite);	
	}

	dc->Rectangle(iStartX,StartY,iStartX+Width,StartY+Height);
	dc->TextOutW(iStartX+2,StartY+4,BitDef,BitDef.GetLength());

	dc->SelectObject(pOldBrush);
	dc->SelectObject(pOldFont);
}

void IoInfo::DrawSingleBitBoxBig(CDC *dc, int iStartX, int StartY, bool IsActive)
{
	CPen *pOldPen;
	CBrush *pOldBrush;
	CFont *pOldFont;
	CString sStateInfo;

	pOldFont=dc->SelectObject(&m_FontSingleBitTitle);

	int Height = 4;
	int Width = 40;

	if (IsActive)
	{
		pOldBrush=dc->SelectObject(&m_BrushChoosenColor);	
		sStateInfo = _T("TRUE");
	}
	else
	{
		pOldBrush=dc->SelectObject(&m_BrushWhite);	
		sStateInfo = _T("FALSE");
	}

	dc->Rectangle(iStartX,StartY,iStartX+Width,StartY+Height);
	dc->TextOutW(iStartX+5,StartY+3,sStateInfo,sStateInfo.GetLength());

	dc->SelectObject(pOldBrush);
	dc->SelectObject(pOldFont);

}

void IoInfo::DrawValueInfo(CDC *dc, CString sValueInfo,int iStartX, int StartY)
{
	CFont *pOldFont;
	pOldFont=dc->SelectObject(&m_FontValue);

	dc->SetTextColor(colors[COLOR_VALUE]);
	dc->TextOutW(iStartX,StartY,sValueInfo);
	
	dc->SetTextColor(colors[COLOR_BLACK]);
	dc->SelectObject(pOldFont);
}

void IoInfo::DrawSmallValueInfo(CDC *dc, CString sValueInfo,int iStartX, int StartY)
{
	CFont *pOldFont;
	pOldFont=dc->SelectObject(&m_FontValueSmall);
	dc->TextOutW(iStartX,StartY,sValueInfo);
	dc->SelectObject(pOldFont);
}

void IoInfo::DrawLabel(CDC *dc, CString sLabel, int iLength, CRect *pRect)
{
	CFont *pOldFont;
	HDC hdc = *dc;	
	pOldFont=dc->SelectObject(&m_FontLabel);
	dc->SetTextColor(colors[COLOR_LABEL]);
	DrawText(hdc,sLabel,iLength,pRect,DT_WORDBREAK|DT_CALCRECT);
	DrawText(hdc,sLabel,iLength,pRect,DT_WORDBREAK);
	
	dc->SetTextColor(colors[COLOR_BLACK]);
	dc->SelectObject(pOldFont);
}

void IoInfo::DrawSingleBitArrayBox(CDC *dc, unsigned int *pValue, int iStartX, int StartY)
{
	// draw the single bits
	int TempX=iStartX;
	dc->SelectObject(&m_BrushTransparent);
	unsigned int tester = 0x00000001;
	CString BitDef;

	int iSingleBitHeight = 5;
	int SeperatorWidth = 3; 	
	int iSingleBitWidth = 20;

	int iInfoCounter = m_iInfoCounterStart;

	for(int i=0; i<32; i++)
	{
		if (m_bIsDigitalInput)
		{
			BitDef.Format(_T("E%i"),iInfoCounter);
		}
		else 
		{
			BitDef.Format(_T("A%i"),iInfoCounter);
		}
		DrawSingleBitArrayBitBox(dc,BitDef,TempX,StartY,iSingleBitWidth,iSingleBitHeight,(*pValue)&tester);
	
		TempX +=(iSingleBitWidth+SeperatorWidth);
		tester = tester<<1;
		iInfoCounter++;
	}		
}

void IoInfo::DrawSingleBitArrayBitBox(CDC *dc, CString BitDef, int iStartX, int StartY,int iSingleBitWidth, int iSingleBitHeight, bool IsActive)
{
	CPen *pOldPen;
	CBrush *pOldBrush;
	CFont *pOldFont;

	pOldFont=dc->SelectObject(&m_FontSingleBitTitle);

	if (IsActive)
	{
		pOldBrush=dc->SelectObject(&m_BrushChoosenColor);	
	}
	else
	{
		pOldBrush=dc->SelectObject(&m_BrushWhite);	
	}

	dc->Rectangle(iStartX,StartY,iStartX+iSingleBitWidth,StartY+iSingleBitHeight);
	dc->TextOutW(iStartX+2,StartY+4,BitDef,BitDef.GetLength());

	dc->SelectObject(pOldBrush);
	dc->SelectObject(pOldFont);
}
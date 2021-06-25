#ifndef IOINFO_H
#define IOINFO_H

#define COLOR_TRANSPARENT	0 
#define COLOR_BLACK			1
#define COLOR_BLUE			2
#define COLOR_GREEN			3	
#define COLOR_CYAN			4
#define COLOR_RED			5	
#define COLOR_PURPLE		6 
#define COLOR_YELLOW		7
#define COLOR_WHITE			8
#define COLOR_LABEL			9
#define COLOR_ADDRESS		10
#define COLOR_VALUE			11

#define MAXIOINFOS 500

#define STD_TEXT_HEIGHT			15
#define STD_TEXT_LINE_SEP		20
#define STD_LINE_HEIGHT			30
#define STD_LABEL_START_X		450
#define STD_LABEL_WIDTH			400
#define STD_OFFSET_X			10
#define STD_OFFSET_Y			10
#define STD_VERT_SEP_OFFSET_X	0
#define STD_VERT_SEP_WITH		945

enum IOINFOTYPE {IOT_ADDI_DIG_IO, IOT_BYTE, IOT_WORD, IOT_BYTEARRAY, IOT_BIT };

const COLORREF colors[12] =
{ 
	RGB(0,0,0),    // transparent
	RGB(0,0,0),    // black
	RGB(0,0,255),  // blue
	RGB(0,255,0),  // green
	RGB(0,255,255),  // cyan
	RGB(255,0,0),  // red
	RGB(255,0,255),  // purple
	RGB(255,255,0),  // yellow
	RGB(255,255,255),  // white
	RGB(18, 97, 128),  // label
	RGB(61, 43, 31),  // address
	RGB(80,80,80),  // value
};

class IoInfo
{
public: 
	IoInfo(int VirtualPos, IOINFOTYPE type,unsigned int *xLatch, unsigned int xlatchMask, int xOffset, int yOffset, CString sLabel, CString sAddressInfo);
	IoInfo(int VirtualPos, IOINFOTYPE type,BYTE *xLatch, BYTE xlatchMask, int xOffset, int yOffset, CString sLabel, CString sAddressInfo);
	IoInfo(int VirtualPos, IOINFOTYPE type, BYTE *bLatch, int xOffset, int yOffset, CString sLabel, CString sAddressInfo);
	IoInfo(int VirtualPos, IOINFOTYPE type, BYTE *bLatches, int iLength, int xOffset, int yOffset, CString sLabel, CString sAddressInfo);
	IoInfo(int VirtualPos, IOINFOTYPE type, WORD *bLatch, int xOffset, int yOffset, CString sLabel, CString sAddressInfo);
	IoInfo(int VirtualPos, IOINFOTYPE type, unsigned int *pLatch, int xOffset, int yOffset, CString sAddressInfo,int iInfoCounterStart,bool IsDigitalInput);

public:
	CRect m_rect_complete;
	CRect m_rect_address;
	CRect m_rect_value;
	CRect m_rect_label;
private:
	int m_iActiveLabelHeight;

private:
	int m_SingleBit_Height;
	int m_SingleBit_AddressInfoWidth;
	int m_SingleBit_SingleBitSymbolWidth;
	int m_SingleBit_LabelWidth;

	int m_SingleByte_Height;
	int m_SingleByte_AddressInfoWidth;
	int m_SingleByte_SingleByteBitSymbolsWidth;
	int m_SingleByte_SingleByteValueWidth;
	int m_SingleByte_LabelWidth;

	int m_SingleByteArray_Height;
	int m_SingleByteArray_AddressInfoWidth;
	int m_SingleByteArray_SingleByteBitSymbolsWidth;
	int m_SingleByteArray_SingleByteValueWidth;
	int m_SingleByteArray_MaxValuesPerLine;
	int m_SingleByteArray_LabelWidth;
	int m_SingleByteArray_LineCount;

	int m_SingleWord_Height;
	int m_SingleWord_AddressInfoWidth;
	int m_SingleWord_SingleWordValueWidth;
	int m_SingleWord_LabelWidth;

	int m_AddiDigInfo_Height;	
	int m_AddiDigInfo_AddressInfoWidth;
	int m_AddiDigInfo_SingleLineBitSymbolsWidth;

public: 
	int	m_iColor;

public:
	int GetVirtualPosition() { return m_iVirtualPos; }
	int GetOffsetX() { return m_xOffset;}
	int GetOffsetY() { return m_yOffset;}
	int GetEndY() { return m_rect_complete.bottom;}
	int GetHeight();
private:
	int m_xOffset;
	int m_yOffset;

private:
	int m_iVirtualPos;
	unsigned int m_iLatchMask;
	BYTE m_bLatchMask;
	WORD m_wLatchMask;

public:
	IOINFOTYPE GetIoType() { return m_IoType;}
	unsigned int *m_piLatch;
	BYTE *m_pbLatch;
	WORD *m_pwLatch;

	int m_iLatchesCount;
private:
	IOINFOTYPE m_IoType;
	CString m_sLabel;
	CString m_sAddressInfo;
	bool m_bIsDigitalInput;
	int m_iInfoCounterStart;


private:
	CFont  m_FontStandard;
	CFont  m_FontLabel;
	CFont  m_FontValue;
	CFont  m_FontValueSmall;
	CFont  m_FontSingleBitTitle;
	CPen   m_PenBitDefActive;
	CPen   m_PenBitDefInActive;
	CBrush m_BrushTransparent;
	CBrush m_BrushChoosenColor;
	CBrush m_BrushWhite;
	
public:
	void Draw(CDC *dc);
	void Init();
	void InitFonts();
	bool IsPointInLed(CPoint point);
	void GetRect(CRect &rect);
	void UpdateLabelDimensions(CDC *dc);

private:
	void CalculateRectangles();

private:
	void DrawVerticalSeperator(CDC *dc);

private:
	void DrawSingleAddiDigIO(CDC *dc);
	void DrawSingleByte(CDC *dc);
	void DrawSingleByteArray(CDC *dc);
	void DrawSingleWord(CDC *dc);
	void DrawSingleBit(CDC *dc);

private:
	void DrawSingleAddressInfo(CDC *dc, CString sAddressInfo, int iStartX, int StartY);
	void DrawSingleByteBox(CDC *dc, BYTE* pValue, int iStartX, int StartY);
	void DrawSingleBitBox(CDC *dc, CString BitDef,int iStartX, int StartY, bool IsActive);
	void DrawSingleBitBoxBig(CDC *dc, int iStartX, int StartY, bool IsActive);
	void DrawValueInfo(CDC *dc, CString BitDef,int iStartX, int StartY);
	void DrawSmallValueInfo(CDC *dc, CString BitDef,int iStartX, int StartY);
	void DrawLabel(CDC *dc, CString sLabel, int iLength, CRect *pRect);

	void DrawSingleBitArrayBox(CDC *dc, unsigned int *pValue, int iStartX, int StartY);
	void DrawSingleBitArrayBitBox(CDC *dc, CString BitDef,int iStartX, int StartY, int iSingleBitWidth, int iSingleBitHeight, bool IsActive);

};
#endif
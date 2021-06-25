#pragma once

#include "ioinfoscreen.h"
#include "ScrollHelper.h"

class CScrollHelper;    // Forward class declaration.

class CDlgInfoIO : public CDialog
{
	DECLARE_DYNAMIC(CDlgInfoIO)

public:
	CDlgInfoIO(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInfoIO();

// Dialog Data
	enum { IDD = IDD_DLG_IO_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	void InitInfoItems();
	void AddItemsIntoInfoScreen();
	void UpdateDisplaySize();

private:
	bool StartReaderThread();
	void KillReaderThread();

public:
	IoInfoScreen m_IoInfoScreen;

private:
    CScrollHelper* m_scrollHelper;

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode,UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode,UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx,int cy);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

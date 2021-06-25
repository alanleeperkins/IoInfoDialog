// PlcInfoDlg.h : header file
//
#pragma once

#include "CDlgInfoIO.h"

// CPlcInfoDlg dialog
class CPlcInfoDlg : public CDialog
{
// Construction
public:
	CPlcInfoDlg(CWnd* pParent = NULL);	// standard constructor
	~CPlcInfoDlg();

// Dialog Data
	enum { IDD = IDD_PLCINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	CDlgInfoIO m_DlgPlcEditor;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
};

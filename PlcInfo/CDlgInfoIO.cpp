// CDlgInfoIO.cpp : implementation file
//

#include "stdafx.h"
#include "PlcInfo.h"
#include "CDlgInfoIO.h"
#include "IoInfo.h"
#include "ioinfoscreen.h"

/// TESTING
#define MAXINFOITEMS 32 
BYTE bLatches[MAXINFOITEMS];
WORD wLatches[MAXINFOITEMS];
// addi-data i/o
#define MAXADDILATCHES 4
UINT iLatches[MAXADDILATCHES];
// bit array
#define BYTEARRYLENGTH 15
BYTE bArray1[BYTEARRYLENGTH];
// TESTING END

IMPLEMENT_DYNAMIC(CDlgInfoIO, CDialog)
CWinThread *pThreadReadExternalIO;
bool bExitThreadReadExternalIO = false;
bool bHasExitedThreadReadExternalIO = false;

UINT ThreadReadExternalIO( LPVOID pParam )
{
	CDlgInfoIO *parent = (CDlgInfoIO*)pParam;

	bool RunUpdate = false;
	BYTE *pbUpdateLatch = 0;
	bExitThreadReadExternalIO = false;
	bHasExitedThreadReadExternalIO = false;

	if (parent == NULL) 
	{
		bExitThreadReadExternalIO = true;
		bHasExitedThreadReadExternalIO = true; 
	}

	while(!bExitThreadReadExternalIO)
	{
		int ItemCount = (*parent).m_IoInfoScreen.GetCount();
		
		for (int ItemPos=0; ItemPos<ItemCount; ItemPos++)
		{
			if (bExitThreadReadExternalIO) break;

			pbUpdateLatch = 0;
			RunUpdate = false;
			switch((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->GetIoType())
			{
				case ::IOT_BIT:
				{
					BYTE newbyte = rand() % 0xFF + 1;
					if(*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pbLatch) !=newbyte)
					{
						*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pbLatch) = newbyte;
						pbUpdateLatch = (*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pbLatch;
						RunUpdate = true;
					}
				}
				break;

				case ::IOT_WORD:
				{
					WORD newword = rand() % 0xFFFF + 1;
//						WORD newword = (*pMasterDevCom).m_pVariables->GetWordVariable((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->GetVirtualPosition());
					if (*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pwLatch) !=newword)
					{
						*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pwLatch) = newword;
						RunUpdate=true;
					}
				}
				break;

				case ::IOT_BYTE:
				{
					BYTE newbyte = rand() % 0xFF + 1;
//							BYTE newbyte = (*pMasterDevCom).m_pVariables->GetByteVariable((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->GetVirtualPosition());
					if(*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pbLatch) !=newbyte)
					{
						*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pbLatch) = newbyte;
						pbUpdateLatch = (*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_pbLatch;
						RunUpdate=true;
					}
				}
				break;
				case ::IOT_BYTEARRAY:
				{
					for(int i=0;i<BYTEARRYLENGTH;i++)
					{
						if (bExitThreadReadExternalIO) break;

						BYTE newbyte = rand() % 0xFF + 1;
						if (bArray1[i] !=newbyte)
						{
							bArray1[i] = newbyte;
							RunUpdate = true;
						}
					}
				}
				break;

				case ::IOT_ADDI_DIG_IO:
				{
					unsigned int newnumber = rand() % 0xFFFF + 1;
					newnumber = newnumber << 16;
					newnumber+=rand() % 0xFFFF + 0;

					if (*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_piLatch) !=newnumber)
					{
						*((*parent).m_IoInfoScreen.m_pIoInfos[ItemPos]->m_piLatch) = newnumber;
						RunUpdate = true;
					}
				}
				break;

			}

			if (RunUpdate == true)
			{
				if(pbUpdateLatch!=0)
				{
					for(int i=0;i<(*parent).m_IoInfoScreen.GetCount();i++)
					{
						if (bExitThreadReadExternalIO) break;

						if((*parent).m_IoInfoScreen.m_pIoInfos[i]->m_pbLatch == pbUpdateLatch)
						{
							(*parent).m_IoInfoScreen.RedrawItem(i);
						}
					}
				}
				else 
				{
					(*parent).m_IoInfoScreen.RedrawItem(ItemPos);
				}	
			}
		}
		Sleep(50);
	}

	return 0;
}

CDlgInfoIO::CDlgInfoIO(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfoIO::IDD, pParent)
{
	m_scrollHelper = new CScrollHelper;
    m_scrollHelper->AttachWnd(this);
	m_scrollHelper->SetDisplaySize(930, 1800);
}

CDlgInfoIO::~CDlgInfoIO()
{
	delete m_scrollHelper;
}	

void CDlgInfoIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CONTENT, m_IoInfoScreen);
}

BEGIN_MESSAGE_MAP(CDlgInfoIO, CDialog)
	ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


BOOL CDlgInfoIO::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitInfoItems();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgInfoIO::InitInfoItems()
{		
	m_IoInfoScreen.InitScreen();

	AddItemsIntoInfoScreen();
	
	// Update the Display Size
	UpdateDisplaySize();
}

void CDlgInfoIO::AddItemsIntoInfoScreen()
{
	CString Label;
	CString Address;
	
	memset(bLatches,0,sizeof(bLatches));
	memset(wLatches,0,sizeof(wLatches));
	memset(iLatches,0,sizeof(iLatches));
	memset(bArray1,0,sizeof(bArray1));
	
	// addi dig i/o
	m_IoInfoScreen.InsertAddiDigDef(-1,_T("APCI-3500-1 (I)"), &iLatches[0],0,true);
	m_IoInfoScreen.InsertAddiDigDef(-1,_T("APCI-3500-2 (I)"), &iLatches[1],32,true);
	m_IoInfoScreen.InsertAddiDigDef(-1,_T("APCI-3500-2 (O)"), &iLatches[2],0,false);
	m_IoInfoScreen.InsertAddiDigDef(-1,_T("APCI-3500-2 (O)"), &iLatches[3],32,false);
		
	// single bytes
	for(int i=0; i<8; i++)
	{
		Label.Format(_T("LoremMorbi eget tellus efficitur, posuere arcu quis..."),i+50);
		Address.Format(_T("DB1000.DBB%.4i"),50 + i);
		m_IoInfoScreen.InsertByte(-1,Address, &bLatches[i],Label);	
	}


	// single words
	for(int i=0; i<3; i++)
	{
		Label.Format(_T("Lorem ipsum dolor sit amet, consectetur adipiscing elit.WORD Nr.%i"),i+1);
		Address.Format(_T("DB1000.DBW%.2i"),rand() % 99 + 1);
		m_IoInfoScreen.InsertWord(-1,Address, &wLatches[i], Label);
	}

	// byte array
	Label.Format(_T("Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum  Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum  Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum  Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum  Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum "));	
	Address.Format(_T("DB1000.DBB12"));
	m_IoInfoScreen.InsertByteArray(-1,Address, bArray1,BYTEARRYLENGTH, Label);

	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.0"), &bLatches[0],0x01,_T("Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, "));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.1"), &bLatches[0],0x02,_T(".Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum  Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum  Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum  Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum dolor sit amet, consectetur adipiscing elit.Lorem ipsum "));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.2"), &bLatches[0],0x04,_T("Taster Befuellung weiter"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.3"), &bLatches[0],0x08,_T("Lorem ipsum dolor sit amet, consec "));
/*	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.4"), &bLatches[0],0x10,_T("Impulsueberwachung"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.5"), &bLatches[0],0x20,_T("Reset"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.6"), &bLatches[0],0x40,_T("Im Job-Fenster mit Automatischer Befüllung"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB50.7"), &bLatches[0],0x80,_T("Mehr als eine Sollmenge im Auftrag (Getriebe)"));

	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.0"), &bLatches[1],0x01,_T("Bit 1"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.1"), &bLatches[1],0x02,_T("Bit 2"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.2"), &bLatches[1],0x04,_T("Bit 3"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.3"), &bLatches[1],0x08,_T("Bit 4"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.4"), &bLatches[1],0x10,_T("Bit 5"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.5"), &bLatches[1],0x20,_T("Bit 6"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.6"), &bLatches[1],0x40,_T("Bit 7"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB51.7"), &bLatches[1],0x80,_T("Bit 8"));

	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.0"), &bLatches[2],0x01,_T("Bit 1"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.1"), &bLatches[2],0x02,_T("Bit 2"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.2"), &bLatches[2],0x04,_T("Bit 3"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.3"), &bLatches[2],0x08,_T("Bit 4"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.4"), &bLatches[2],0x10,_T("Bit 5"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.5"), &bLatches[2],0x20,_T("Bit 6"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.6"), &bLatches[2],0x40,_T("Bit 7"));
	m_IoInfoScreen.InsertBit(-1,_T("DB1000.DBB52.7"), &bLatches[2],0x80,_T("Bit 8"));
*/
}

void CDlgInfoIO::UpdateDisplaySize()
{
	CRect rect;
	m_IoInfoScreen.GetWindowRect(&rect);
	int width = rect.right +20;
	int height = rect.bottom +20;
	m_scrollHelper->SetDisplaySize(width, height);
}

void CDlgInfoIO::OnHScroll(UINT nSBCode,UINT nPos, CScrollBar* pScrollBar)
{
	if (m_IoInfoScreen.GetCount()<=0) return;
	m_scrollHelper->OnHScroll(nSBCode,nPos, pScrollBar);
	m_IoInfoScreen.RedrawWindow();
}

void CDlgInfoIO::OnVScroll(UINT nSBCode,UINT nPos, CScrollBar* pScrollBar)
{
	if (m_IoInfoScreen.GetCount()<=0) return;
	m_scrollHelper->OnVScroll(nSBCode,nPos, pScrollBar);
	m_IoInfoScreen.RedrawWindow();
}

BOOL CDlgInfoIO::OnMouseWheel(UINT nFlags,short zDelta, CPoint pt)
{ 
	if (m_IoInfoScreen.GetCount()<=0) return false;
	BOOL wasScrolled =m_scrollHelper->OnMouseWheel(nFlags,zDelta, pt);
	m_IoInfoScreen.RedrawWindow();
    return wasScrolled;
}

void CDlgInfoIO::OnSize(UINT nType, int cx, int cy)
{
 	if (m_IoInfoScreen.GetCount()<=0) return;
 
	CDialog::OnSize(nType, cx, cy);
	m_scrollHelper->OnSize(nType, cx, cy);
	m_IoInfoScreen.RedrawWindow();
}

bool CDlgInfoIO::StartReaderThread()
{
	bExitThreadReadExternalIO = false;
	pThreadReadExternalIO = AfxBeginThread(ThreadReadExternalIO,(void*)this);
	return (pThreadReadExternalIO!=NULL);
}

void CDlgInfoIO::KillReaderThread()
{
	DWORD exit_code= NULL;
	if (pThreadReadExternalIO->m_hThread!=NULL)
	{
		GetExitCodeThread(pThreadReadExternalIO->m_hThread, &exit_code);
		if(exit_code == STILL_ACTIVE)
		{
			::TerminateThread(pThreadReadExternalIO->m_hThread, 0);
			CloseHandle(pThreadReadExternalIO->m_hThread);
		}
	}

	pThreadReadExternalIO->m_hThread = NULL;
    pThreadReadExternalIO = NULL;
}

void CDlgInfoIO::OnClose()
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.
	KillReaderThread();
	CDialog::OnClose();
}

void CDlgInfoIO::OnDestroy()
{
	KillReaderThread();
	CDialog::OnDestroy();
}

void CDlgInfoIO::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		StartReaderThread();
	}
}

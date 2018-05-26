
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDlg.h"
#include "ClientCon.h"
#include "FileUtil.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg dialog




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent),
	m_pClient(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Message, m_Textbox);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_LogIn, &CClientDlg::OnClickedBtLogin)
	ON_BN_CLICKED(IDC_BT_LogOut, &CClientDlg::OnClickedBtLogout)
	ON_BN_CLICKED(IDC_BT_Send, &CClientDlg::OnClickedBtSend)
	ON_BN_CLICKED(IDC_BT_CLEAR, &CClientDlg::OnClickedBtClear)
	ON_BN_CLICKED(IDC_BT_FILE, &CClientDlg::OnClickedBtFile)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::ShowServerInfo(string sValue)
{
	CString strLine(sValue.c_str());
	// add CR/LF to text
	//MessageBox(sValue.c_str());
	AppendTextToEditCtrl(m_Textbox, strLine);
	//DoModal();
	//UpdateData(TRUE);
}

void CClientDlg::AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText)
{
   // get the initial text length
   int nLength = edit.GetWindowTextLength();
   // put the selection at the end of text
   edit.SetSel(nLength, nLength);
   // replace the selection
   edit.ReplaceSel(pszText);
}

UINT __cdecl CClientDlg::StaticThreadFunc(LPVOID pParam)
{
	CClientDlg *pYourClass = reinterpret_cast<CClientDlg*>(pParam);
    UINT retCode = pYourClass->ThreadFunc();

    return retCode;
}

UINT CClientDlg::ThreadFunc()
{ 
	CString IPname; 
	GetDlgItemText(IDC_EDIT_IP, IPname);

	CString portname;
	GetDlgItemText(IDC_EDIT_Port, portname);
	int iPort = _wtoi( portname.GetString() );
	
	CString username;
	GetDlgItemText(IDC_EDIT_Name, username);

	m_pClient = new ClientCon(this);

	CT2CA CStringToAscii(IPname);

    //construct a std::string using the LPCSTR input
    std::string IPAddress (CStringToAscii);
     
	CT2CA CStringToAscii2(username);

	std::string UserName (CStringToAscii2);

	m_pClient->StartConnect(IPAddress, iPort, UserName);
	return 0;
}

void CClientDlg::OnBnClickedOk()
{
	OnClickedBtLogin();
}

void CClientDlg::OnClickedBtLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cTh = AfxBeginThread(StaticThreadFunc, this);
	m_Thread_handle = cTh->m_hThread;
}

void CClientDlg::OnClickedBtLogout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pClient != NULL)
	{
		std::string sResultedString(m_pClient->m_pUser + " is logged out\n");
		m_pClient->SendData(sResultedString);
	    ShowServerInfo(sResultedString);
		delete m_pClient;
	}
	else
	{
		ShowServerInfo("You haven't log in yet.\n");
	}
}


void CClientDlg::OnClickedBtSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sTextData;
	GetDlgItemText(IDC_EDIT_SendText, sTextData);

	CT2CA CStringToAscii(sTextData);

	// construct a std::string using the LPCSTR input
	std::string sResultedString(CStringToAscii);
	if (m_pClient != NULL) 
	{
		m_pClient->SendData(sResultedString);
	}
	CWnd* pWnd = GetDlgItem(IDC_EDIT_SendText);
	pWnd->SetWindowText(_T(""));//��շ�����
}


void CClientDlg::OnClickedBtClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd* pWnd = GetDlgItem(IDC_EDIT_Message);
	pWnd->SetWindowText(_T("Welcome to Magic TCP chat room!!!"));//��ս�����
}


void CClientDlg::OnClickedBtFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���ù�����     
	TCHAR szFilter[] = _T("���±��ļ�(*.txt*)|*.txt*|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���     
	CFileDialog fileDlg(TRUE, _T("."), NULL, 0, szFilter, this);
	CString strFilePath;
	CString strFileName;

	// ��ʾ���ļ��Ի���     
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����     
		strFilePath = fileDlg.GetPathName();
		strFileName = fileDlg.GetFileTitle();
		FileUtil fu;
		FILE *f = fu.openFile(strFilePath);
		if ((f != NULL) && (m_pClient != NULL))
		{
			m_pClient->SendFile(f, strFileName);
		}
	}
}


BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		if (GetDlgItem(IDC_EDIT_SendText) == GetFocus())
		{
			OnClickedBtSend();
			return false;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

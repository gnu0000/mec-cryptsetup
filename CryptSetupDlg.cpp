// CryptSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CryptSetup.h"
#include "CryptSetupDlg.h"
#include "ExFileDialog.h"
#include "gnudir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define STR_DEFAULT_PATH		"C:\\Program Files\\ITICrypt"
#define STR_REG_PROG_DIR		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion"
#define STR_REG_PROG_DIR_KEY	"ProgramFilesDir"
#define STR_DEF_PROG_DIR		"D:\\Program Files\\ITICrypt"

#define MAX_MESSAGE_LENGTH		255



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCryptSetupDlg dialog

CCryptSetupDlg::CCryptSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCryptSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCryptSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCryptSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCryptSetupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCryptSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CCryptSetupDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_INSTALL, OnInstall)
	ON_BN_CLICKED(ID_BTN_CHANGE, OnBtnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCryptSetupDlg message handlers

BOOL CCryptSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	// Set default installation path ---
	SetDlgItemText(IDC_INSTALL_DIR, GetProgFilesDir());
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCryptSetupDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCryptSetupDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCryptSetupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// --- ExtractExeFile() ---------------------------------------------
// 
BOOL CCryptSetupDlg::ExtractExeFile(LPCTSTR strPath)
{
	// ITICrypt is in resource file ---
	HRSRC hSrc = NULL;
	HGLOBAL hRes = NULL;
	LPVOID lpData = NULL;
	if (   ((hSrc = FindResource(NULL, MAKEINTRESOURCE(IDR_PROG1), "PROG")) == NULL)
		|| ((hRes = LoadResource(NULL, hSrc))== NULL)
		|| ((lpData = LockResource(hRes)) == NULL))
	{
		AfxMessageBox("An error occured while installing ITICrypt");
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Copy file to target folder ---
	CFile targetFile;
	CFileException fileErr;
	if (!targetFile.Open(strPath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary, &fileErr))
	{
		char lpszErr[MAX_MESSAGE_LENGTH];
		fileErr.GetErrorMessage(lpszErr, MAX_MESSAGE_LENGTH);

		CString strMes;
		strMes.Format("An error occured while creating file '%s' (%s)", strPath, lpszErr);
		AfxMessageBox(strMes);
		return FALSE;
	}

	try
	{
		DWORD nLen = SizeofResource(NULL, hSrc);
		targetFile.Write(lpData, (UINT) nLen);
	}
	catch(CFileException* pFileErr)
	{
		char lpszErr[MAX_MESSAGE_LENGTH];
		pFileErr->GetErrorMessage(lpszErr, MAX_MESSAGE_LENGTH);

		CString strMes;
		strMes.Format("An error occured while writing file '%s' (%s)", strPath, lpszErr);
		AfxMessageBox(strMes);
		return FALSE;
	}
	targetFile.Close();
	return TRUE;
}


// --- OnInstall() --------------------------------------------------
// 
void CCryptSetupDlg::OnInstall() 
{
	TCHAR strPath[_MAX_PATH+1];
	if (GetDlgItemText(IDC_INSTALL_DIR, strPath, sizeof(strPath)) == 0)
	{
		AfxMessageBox("No directory name is provided");
		return;
	}

	// Check that the path points to a directory ---
	CFileStatus fileStatus;
	if (CFile::GetStatus(strPath, fileStatus))
	{
		if ((fileStatus.m_attribute & CFile::directory) == 0 )
		{
			// Error: not a directory ---
			CString strErr;
			strErr.Format("A file with the name '%s' already exists", strPath);
			AfxMessageBox(strErr);
			return;
		}
	}
	else
	{
		// Directory does not exist: ask user about creating it ---
		CString strMes;
		strMes.Format("The folder '%s' does not exist. Do you want to create it ?", strPath);
		if (AfxMessageBox(strMes, MB_YESNO) != IDYES)
		{
			return;
		}
		DirMakePath(strPath);
	}

	strcat(strPath, "\\");
	strcat(strPath, "ITICrypt.exe");
	if (CFile::GetStatus(strPath, fileStatus))
	{
		// The file already exists ---
		CString strMes;
		strMes.Format("The file '%s' already exist, do you want to override it ?", strPath);
		if (AfxMessageBox(strMes, MB_YESNO) != IDYES)
			return;
	}

	if (!ExtractExeFile(strPath))
		return;

	// Register ITICrypt ---
	int nStatus = (int) ShellExecute(NULL, "open", strPath, "/register", NULL, SW_HIDE);

	if (nStatus > 32)
	{
		AfxMessageBox("ITICrypt has been installed successfully. To encrypt a file, you can right-click on it in Windows" \
			" Explorer, choose the entry ‘Send To’ then ‘ITI Encryption Tool’.");
		EndDialog(IDOK);
	}
	else
	{
		AfxMessageBox("An error occured while registering ITICrypt");
		EndDialog(IDCANCEL);
		return;
	}
}


// --- OnBtnChange() ------------------------------------------------
// User wants to change installation directory.
//
void CCryptSetupDlg::OnBtnChange() 
{
	HRESULT hres = CoInitialize(NULL);
	BOOL bCallCoUninit = (hres == S_OK) || (hres == S_FALSE);
	
	LPMALLOC pMalloc;    

	/* Gets the Shell's default allocator */
	if (::SHGetMalloc(&pMalloc) == NOERROR)    
	{        
		BROWSEINFO bi;
		TCHAR pszBuffer[_MAX_PATH];        
		LPITEMIDLIST pidl;

		// Get help on BROWSEINFO struct - it's got all the bit settings.
		bi.hwndOwner = GetSafeHwnd();        
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("Select an Installation Directory for ITICrypt:");
		bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;        
		bi.lParam = 0;

		// This next call issues the dialog box.
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)        
		{
			if (::SHGetPathFromIDList(pidl, pszBuffer))            
			{ 
				// At this point pszBuffer contains the selected path */.
				SetDlgItemText(IDC_INSTALL_DIR, pszBuffer);            
			}

			// Free the PIDL allocated by SHBrowseForFolder.
			pMalloc->Free(pidl);        
		}
		// Release the shell's allocator.        
		pMalloc->Release();
	}

	if (bCallCoUninit)
		CoUninitialize();
}


CString CCryptSetupDlg::GetProgFilesDir()
{
	HKEY hProgFilesDirKey = 0;
	DWORD dwBufLen = MAX_PATH;
	TCHAR szProgFilesDir[MAX_PATH];

	if (   (RegOpenKeyEx(HKEY_LOCAL_MACHINE, STR_REG_PROG_DIR, 0, KEY_READ, &hProgFilesDirKey) != ERROR_SUCCESS)
		|| (RegQueryValueEx(hProgFilesDirKey, STR_REG_PROG_DIR_KEY, NULL, NULL, (LPBYTE) szProgFilesDir, &dwBufLen) != ERROR_SUCCESS))
	{
		return STR_DEF_PROG_DIR;
	}
	else
	{
		return strcat(szProgFilesDir, "\\ITICrypt");
	}
}

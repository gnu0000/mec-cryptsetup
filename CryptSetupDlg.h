// CryptSetupDlg.h : header file
//

#if !defined(AFX_CRYPTSETUPDLG_H__8577004E_A8C4_11D3_B4B4_005004D39EC7__INCLUDED_)
#define AFX_CRYPTSETUPDLG_H__8577004E_A8C4_11D3_B4B4_005004D39EC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCryptSetupDlg dialog

class CCryptSetupDlg : public CDialog
{
// Construction
public:
	CCryptSetupDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCryptSetupDlg)
	enum { IDD = IDD_CRYPTSETUP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCryptSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCryptSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInstall();
	afx_msg void OnBtnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString		GetProgFilesDir();
	BOOL		ExtractExeFile(LPCTSTR strPath);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRYPTSETUPDLG_H__8577004E_A8C4_11D3_B4B4_005004D39EC7__INCLUDED_)

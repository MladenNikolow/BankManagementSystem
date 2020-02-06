#include "Database.h"

// BankManagementSystemMFCDlg.h : header file


#pragma once


// CBankManagementSystemMFCDlg dialog
class CBankManagementSystemMFCDlg : public CDialogEx
{
// Construction
public:
	CBankManagementSystemMFCDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BANKMANAGEMENTSYSTEMMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:

	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonLogin();

	// Email input for login
	CString strEmailLogin;
	// Password for the login
	CString strPasswordLogin;
	//Database
	Database database;

	// Check box to show/hide password
	CButton ctrCheckBox;
	// Control the state of the password edit box (show/hide)
	CEdit ctrPass;
	afx_msg void OnBnClickedCheckPass();

	BOOL checkPassword(const CString& strPass);
};

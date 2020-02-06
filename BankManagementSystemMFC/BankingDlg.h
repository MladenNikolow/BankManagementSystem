#pragma once
#include "Database.h"
#include <vector>
#include <afxtempl.h>


// Banking dialog

class Banking : public CDialogEx
{
	DECLARE_DYNAMIC(Banking)



public:
	Banking(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Banking();
	//Account email
	CString strEmailAcc;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BANKING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();

	// All bank accounts for the user
	CComboBox comboBoxAccounts;

	//All bank accounts
	std::vector<CString> allAccounts;

	//Account id
	int nAccId;

	// History of the transactions for the bank account	// History of the transactions for the bank account
	CString strTransHistory;

	//Current bank account id
	CString strBankAccName;

	// The amount of money in the current bank account
	CString strBalance;

	Database database;

	void setAccountId();
	void updateComboBox();

	afx_msg void OnBnClickedButtonAddmoney();
	afx_msg void OnBnClickedButtonWithdraw();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonHistory();
	afx_msg void OnBnClickedButtonAddaccount();

	void Banking::setAllBankAccounts();

};

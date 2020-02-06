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

	// All bank accounts for the user
	CComboBox comboBoxAccounts;
	afx_msg void OnBnClickedButtonAddaccount();

	//All bank accounts
	std::vector<CString> allAccounts;
	void setAllBankAccounts();

	//Account id
	int nAccId;
	void setAccountId();





	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddmoney();
	afx_msg void OnBnClickedButtonWithdraw();


	afx_msg void OnBnClickedButtonHistory();
	// History of the transactions for the bank account	// History of the transactions for the bank account
	CString strTransHistory;

	//Current bank account id
	CString strBankAccName;
	afx_msg void OnBnClickedButtonSend();
	// The amount of money in the current bank account
	CString strBalance;

	Database database;



};

#pragma once

#include <vector>
#include "Database.h"

// WithdrawDlg dialog

class WithdrawDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WithdrawDlg)

public:
	WithdrawDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WithdrawDlg();

	//Account email
	CString strEmailAcc;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WITHDRAW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
private:

	//Account id
	int nAccId;
	void setAccountId();

	//Stores all available bank accounts of the user
	std::vector <CString> allAccounts;
	void setAllBankAccounts();

	// Stores the available bank accounts for the user
	CComboBox comboBoxBankAcc;
	// The amount of money that the user wants to withdraw
	CString strAmount;
	afx_msg void OnBnClickedButtonWithdraw();

	//Stores the selected bank account from the combo box
	CString strCurrentAccount;

	//database
	Database database;
	//Validate the bank account
	BOOL validation(const CString& strBankAcc);

};

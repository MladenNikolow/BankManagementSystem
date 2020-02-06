#pragma once

#include <vector>
#include "Database.h"

// AddMoneyDlg dialog

class AddMoneyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddMoneyDlg)

public:
	AddMoneyDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddMoneyDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_MONEY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Amount of money to add to the account
	CString strAmount;
	std::vector <CString> allAccounts;

	void setAllAccounts(const std::vector <CString>& allAcc)
	{
		for (size_t i = 0; i < allAcc.size(); ++i)
		{
			allAccounts.push_back(allAcc[i]);
		}
	}
	//Initialize the Combo box
	virtual BOOL OnInitDialog();
	// All accounts for the user
	CComboBox comboBoxAccountsList;
	//Button add money
	afx_msg void OnBnClickedButtonAddMoney();
	//Store the current bank account
	CString strCurrentAccount;

	//database
	Database database;

	BOOL AddMoneyDlg::validation(const CString& strBankAcc);

};

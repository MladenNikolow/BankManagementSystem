#pragma once
#include <vector>
#include "Database.h"

// SendMoneyDlg dialog

class SendMoneyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SendMoneyDlg)

public:
	SendMoneyDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SendMoneyDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// The available bank accounts for the user
	CComboBox comboBoxAccounts;
	// The number of the receiver's bank account
	CString strReceiverBankAcc;
	// The amount of the bank transfer
	CString strAmount;
	virtual BOOL OnInitDialog();

	//The sender bank account
	CString strCurrentBankAcc;

	//All available bank accounts for the user
	std::vector<CString> allAccounts;
	//Set all available accounts
	void setAllAccounts(const std::vector <CString>& allAcc)
	{
		for (size_t i = 0; i < allAcc.size(); ++i)
		{
			allAccounts.push_back(allAcc[i]);
		}
	}


	Database database;

	BOOL validation(const CString& strBankAcc);
	afx_msg void OnBnClickedButtonSendMoney();
};

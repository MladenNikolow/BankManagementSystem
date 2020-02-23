#pragma once

#include "Database.h"

// AddBankAccDLG dialog

class AddBankAccDLG : public CDialogEx
{
	DECLARE_DYNAMIC(AddBankAccDLG)

public:
	AddBankAccDLG(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddBankAccDLG();
	//Account email
	CString strEmail;



// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_BANK_ACC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:

	// Currency of the new bank account
	CString strCurrency;

	// Initial amount of money for the new account
	CString strAmount;

	Database database;

	//Login Account Id
	int nAccId;

	//The new bank account number
	CString bankAccNumber;


	afx_msg void OnBnClickedButtonAddBankAcc();

	void setAccountId();


public:
	virtual BOOL OnInitDialog();
};

// Banking.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "BankingDlg.h"
#include "afxdialogex.h"
#include "afxtempl.h"
#include "Database.h"
#include "AddBankAccDLG.h"
#include "AddMoneyDlg.h"
#include "WithdrawDlg.h"
#include"SendMoneyDlg.h"
#include "Currency.h"
#include "Transaction.h"
#include"BankAccount.h"
#include <vector>



// Banking dialog

IMPLEMENT_DYNAMIC(Banking, CDialogEx)

Banking::Banking(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BANKING, pParent)
	, strTransHistory(_T(""))
	, strBalance(_T(""))
{
	
}

Banking::~Banking()
{

}


void Banking::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ALLACCOUNTS, comboBoxAccounts);

	DDX_Text(pDX, IDC_EDIT_TRANSACTION_HISTORY, strTransHistory);
	DDX_Text(pDX, IDC_EDIT_BALANCE, strBalance);
}


BEGIN_MESSAGE_MAP(Banking, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_ADDACCOUNT, &Banking::OnBnClickedButtonAddaccount)
	ON_BN_CLICKED(IDC_BUTTON_ADDMONEY, &Banking::OnBnClickedButtonAddmoney)
	ON_BN_CLICKED(IDC_BUTTON_WITHDRAW, &Banking::OnBnClickedButtonWithdraw)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY, &Banking::OnBnClickedButtonHistory)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Banking::OnBnClickedButtonSend)
END_MESSAGE_MAP()


void Banking::setAccountId()
{
	//Get the current account id
	CString selectQuery = L"SELECT id FROM LoginAccounts";
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmailAcc);

	nAccId = database.selectInt(selectQuery, L"id", whereStatement);
}

void Banking::setAllBankAccounts()
{
	//Get all bank accounts of this user
	CString selectQuery;
	selectQuery.Format(L"SELECT bank_acc_number FROM BankAccounts");
	CString whereStatement;
	whereStatement.Format(L"log_acc_id = '%d'", nAccId);

	CString fieldName = _T("bank_acc_number");
	allAccounts = database.selectMultipleStrings(selectQuery, fieldName, whereStatement);
}

// Banking message handlers

void Banking::updateComboBox()
{

	//Get all bank accounts of this user
	setAllBankAccounts();

	comboBoxAccounts.ResetContent();

	for (size_t i = 0; i < allAccounts.size(); ++i)
	{
		comboBoxAccounts.AddString(allAccounts[i]);
	}
}

BOOL Banking::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setAccountId();
	setAllBankAccounts();

	for (size_t i = 0; i < allAccounts.size(); ++i)
	{
		comboBoxAccounts.AddString(allAccounts[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


//Add Bank Account
void Banking::OnBnClickedButtonAddaccount()
{
	// TODO: Add your control notification handler code here
	AddBankAccDLG addBankAccDlg;
	addBankAccDlg.strEmail = strEmailAcc;

	INT_PTR result = addBankAccDlg.DoModal();

	if (result == IDOK)
	{

		updateComboBox();

	}

}

//Add money into a bank account
void Banking::OnBnClickedButtonAddmoney()
{

	AddMoneyDlg addMoneyDlg;
	//Set the account email
	addMoneyDlg.strEmail = strEmailAcc;
	//Open window
	INT_PTR result = addMoneyDlg.DoModal();

}

//Withdraw a money from a bank account
void Banking::OnBnClickedButtonWithdraw()
{
	// TODO: Add your control notification handler code here
	WithdrawDlg withdrawDlg;
	//Set the account email
	withdrawDlg.strEmailAcc = strEmailAcc;
	//Open the windows
	INT_PTR result = withdrawDlg.DoModal();

}



//Button to show the histroty of the transactions and the balance of the bank account
void Banking::OnBnClickedButtonHistory()
{

	//Get the current bank account number
	comboBoxAccounts.GetLBText(comboBoxAccounts.GetCurSel(), strBankAccName);

	//Display All Transactions
	Transaction currentTrans(strBankAccName);
	strTransHistory = currentTrans.getTransactionsToString();


	//Get the balance
	BankAccount currentAcc(strBankAccName);
	double balance = currentAcc.getBalance(strBankAccName);

	strBalance.Format(L"%.*f", 2, balance);

	//Update the balance edit box and the transaction history edit box
	UpdateData(FALSE);
	
	
}


void Banking::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	SendMoneyDlg sendMoneyDlg;

	//Set the account email
	sendMoneyDlg.strEmail = strEmailAcc;

	INT_PTR result = sendMoneyDlg.DoModal();

}

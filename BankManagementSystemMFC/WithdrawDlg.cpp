// WithdrawDlg.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "WithdrawDlg.h"
#include "afxdialogex.h"


// WithdrawDlg dialog

IMPLEMENT_DYNAMIC(WithdrawDlg, CDialogEx)

WithdrawDlg::WithdrawDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_WITHDRAW, pParent)
	, strAmount(_T(""))
{

}

WithdrawDlg::~WithdrawDlg()
{
}

void WithdrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BANK_ACC, comboBoxBankAcc);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, strAmount);
}


BEGIN_MESSAGE_MAP(WithdrawDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_WITHDRAW, &WithdrawDlg::OnBnClickedButtonWithdraw)
END_MESSAGE_MAP()


// WithdrawDlg message handlers
void WithdrawDlg::setAccountId()
{
	//Get the current account id
	CString selectQuery = L"SELECT id FROM LoginAccounts";
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmailAcc);

	nAccId = database.selectInt(selectQuery, L"id", whereStatement);
}

void WithdrawDlg::setAllBankAccounts()
{
	//Get all bank accounts of this user
	CString selectQuery;
	selectQuery.Format(L"SELECT bank_acc_number FROM BankAccounts");
	CString whereStatement;
	whereStatement.Format(L"log_acc_id = '%d'", nAccId);

	CString fieldName = _T("bank_acc_number");
	allAccounts = database.selectMultipleStrings(selectQuery, fieldName, whereStatement);
}

BOOL WithdrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setAccountId();
	setAllBankAccounts();

	//Initialize the Combo Box
	for (size_t i = 0; i < allAccounts.size(); ++i)
	{
		comboBoxBankAcc.AddString(allAccounts[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL WithdrawDlg::validation(const CString& strBankAcc)
{
	CString strSqlSelect;
	strSqlSelect.Format(L"SELECT bank_acc_number FROM BankAccounts");
	CString strField = L"bank_acc_number";
	CString strWhereStatement;
	strWhereStatement.Format(L"bank_acc_number = '%s'", strBankAcc);

	if (database.selectString(strSqlSelect, strField, strWhereStatement) == "No Data")
	{
		AfxMessageBox(L"Enter a valid bank account!");
		return FALSE;
	}

	return TRUE;
}
void WithdrawDlg::OnBnClickedButtonWithdraw()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	comboBoxBankAcc.GetLBText(comboBoxBankAcc.GetCurSel(), strCurrentAccount);


	//Validate the bank account
	if (validation(strCurrentAccount))
	{
		//Get the current balance of the bank account
		CString strSelectQuery;
		strSelectQuery.Format(L"SELECT balance FROM BankAccounts");
		CString fieldName = _T("balance");
		CString whereStatement;
		whereStatement.Format(
			L"bank_acc_number = '%s'",
			strCurrentAccount
		);

		double currentBalance = database.selectDouble(strSelectQuery, fieldName, whereStatement);

		//Get the id of the bank account
		CString SelectQuery;
		SelectQuery.Format(L"SELECT id FROM BankAccounts");
		CString field = _T("id");
		CString strWhereStatement;
		strWhereStatement.Format(
			L"bank_acc_number = '%s'",
			strCurrentAccount
		);

		int bankAccId = database.selectInt(SelectQuery, field, strWhereStatement);


		//check if there is enough money
		if (currentBalance < _wtof(strAmount))
		{
			//there isn't enough money
			int precision = 2;
			CString error;
			error.Format(L"Not enough money!\nThe current balance of the bank account is: %.*f", precision, currentBalance);
			AfxMessageBox(error);
		}
		else
		{
			//there is enough money to perform the operation
			//Insert query for the transactions table
			CString strInsertQuery;
			strInsertQuery.Format(
				L"INSERT INTO Transactions(bank_acc_id, amount, bank_number_sender, bank_number_recipient, transaction_time) VALUES ('%d', '%f', '%s', '%s', GETDATE())",
				bankAccId,
				_wtof(strAmount),
				strCurrentAccount,
				L"out"
			);

			//Insert query for the amount field in bank account table
			CString strUpdateQuery;
			strUpdateQuery.Format(
				L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
				currentBalance - _wtof(strAmount),
				strCurrentAccount
			);

			if (database.Execute(strInsertQuery) && database.Execute(strUpdateQuery))
			{
				AfxMessageBox(L"Money withdrawal successful!");
			}
			else
			{
				//do nothing
				//Database::Execute() will throw a message automatically, if something go wrong
			}
		}
		CDialogEx::OnOK();
	}
}




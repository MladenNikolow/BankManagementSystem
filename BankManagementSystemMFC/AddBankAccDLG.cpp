// AddBankAccDLG.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "AddBankAccDLG.h"
#include "afxdialogex.h"
#include "Currency.h"
#include "BankAccountNumberGenerator.h"


// AddBankAccDLG dialog

IMPLEMENT_DYNAMIC(AddBankAccDLG, CDialogEx)

AddBankAccDLG::AddBankAccDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD_BANK_ACC, pParent)

	, strCurrency(_T(""))
	, strAmount(_T(""))
{
	
}

AddBankAccDLG::~AddBankAccDLG()
{
}

void AddBankAccDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_CBString(pDX, IDC_COMBO_CURRENCY_ADDACC, strCurrency);
	DDV_MaxChars(pDX, strCurrency, 3);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, strAmount);
	DDV_MaxChars(pDX, strAmount, 5);

}


BEGIN_MESSAGE_MAP(AddBankAccDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_BANK_ACC, &AddBankAccDLG::OnBnClickedButtonAddBankAcc)
END_MESSAGE_MAP()


// AddBankAccDLG message handlers
void AddBankAccDLG::setLoginAccId()
{
	//Get the current account id
	CString selectQuery = L"SELECT id FROM LoginAccounts";
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmailAcc);

	nAccId = database.selectInt(selectQuery, L"id", whereStatement);
}


void AddBankAccDLG::OnBnClickedButtonAddBankAcc()
{
	UpdateData(TRUE);

	//Check if the input currency is valid
	if (isValidCurrency(strCurrency))
	{
		double moneyInput = _wtof(strAmount);


		//Generate a unique bank account number starting including the currency and 6 random numbers
		CString bankAccNumber;
		
		bankAccNumber.Append(strCurrency); //add the currency
		bankAccNumber.Append(genRandomBankNumber()); //generate the numbers

		CString insertQuery;
		insertQuery.Format(L"INSERT INTO BankAccounts (log_acc_id, bank_acc_number, currency, balance, date_of_creation) VALUES ('%i', '%s', '%s', '%f', GETDATE())",
			nAccId,
			bankAccNumber,
			strCurrency,
			moneyInput
		);

		database.Execute(insertQuery);

		//Get the id of the bank account
		CString strSelectQuery;
		strSelectQuery.Format(L"SELECT id FROM BankAccounts");
		CString fieldName = _T("id");
		CString strWhereStatement;
		strWhereStatement.Format(
			L"bank_acc_number = '%s'",
			bankAccNumber
		);

		int bankAccId = database.selectInt(strSelectQuery, fieldName, strWhereStatement);

		if (moneyInput > 0.00)
		{
			//Insert Query for the Transaction
			CString insertQuery;
			insertQuery.Format(L"INSERT INTO Transactions (bank_acc_id, amount, bank_number_sender, bank_number_recipient, transaction_time) VALUES ('%d', '%f', '%s', '%s', GETDATE())",
				bankAccId,
				moneyInput,
				L"out",
				bankAccNumber
			);
			database.Execute(insertQuery);
		}
		CDialogEx::OnOK();
	}
}

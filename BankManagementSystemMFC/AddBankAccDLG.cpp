// AddBankAccDLG.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "BankAccountNumberGenerator.h"
#include "AddBankAccDLG.h"
#include "afxdialogex.h"
#include "Currency.h"
#include "BankAccount.h"



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
void AddBankAccDLG::setAccountId()
{
	//Get the current account id
	CString selectQuery = L"SELECT id FROM LoginAccounts";
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmail);

	nAccId = database.selectInt(selectQuery, L"id", whereStatement);
}



BOOL AddBankAccDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setAccountId();

		return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX Property Pages should return FALSE
}

void AddBankAccDLG::OnBnClickedButtonAddBankAcc()
{
	UpdateData(TRUE);

	//Check if the input currency is valid
	if (isValidCurrency(strCurrency))
	{


		//Generate a unique bank account number starting including the currency and 6 random numbers
		CString bankAccNumber;
		
		bankAccNumber.Append(strCurrency); //add the currency
		bankAccNumber.Append(genRandomBankNumber()); //generate the numbers

		BankAccount current(bankAccNumber);

		//Create the new account (insert into the database)
		//Generate the transaction if it's needed
		current.addNewAccount(nAccId, _wtof(strAmount), strCurrency);


		CDialogEx::OnOK();
	}
}




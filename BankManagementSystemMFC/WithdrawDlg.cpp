// WithdrawDlg.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "WithdrawDlg.h"
#include "BankAccount.h"
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


void WithdrawDlg::OnBnClickedButtonWithdraw()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	comboBoxBankAcc.GetLBText(comboBoxBankAcc.GetCurSel(), strCurrentAccount);

	BankAccount current(strCurrentAccount);

	//Validate that the bank account exists in the database
	//If not exists, throw a message
	if (current.validate(strCurrentAccount))
	{
		//Change the balance of the bank account
		//Generate the transaction
		if (current.withdrawMoney(_wtof(strAmount)))
		{
			AfxMessageBox(L"Money withdrawal successful!");
		}
		
		CDialogEx::OnOK();
	}
}




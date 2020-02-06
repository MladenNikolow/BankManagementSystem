// AddMoneyDlg.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "AddMoneyDlg.h"
#include "BankAccount.h"
#include "afxdialogex.h"


// AddMoneyDlg dialog

IMPLEMENT_DYNAMIC(AddMoneyDlg, CDialogEx)

AddMoneyDlg::AddMoneyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD_MONEY, pParent)
	, strAmount(_T(""))
{

}

AddMoneyDlg::~AddMoneyDlg()
{
}

void AddMoneyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, strAmount);
	DDV_MaxChars(pDX, strAmount, 9);
	DDX_Control(pDX, IDC_COMBO_BANK_ACCOUNTS, comboBoxAccountsList);
}


BEGIN_MESSAGE_MAP(AddMoneyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MONEY, &AddMoneyDlg::OnBnClickedButtonAddMoney)
END_MESSAGE_MAP()


// AddMoneyDlg message handlers
void AddMoneyDlg::setLoginAccId()
{
	
	//Get the current account id
	CString selectQuery = L"SELECT id FROM LoginAccounts";
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmail);

	nAccId = database.selectInt(selectQuery, L"id", whereStatement);
	
}

void AddMoneyDlg::setAllBankAccounts()
{
	//Get all bank accounts of this user
	CString selectQuery;
	selectQuery.Format(L"SELECT bank_acc_number FROM BankAccounts");
	CString whereStatement;
	whereStatement.Format(L"log_acc_id = '%d'", nAccId);

	CString fieldName = _T("bank_acc_number");
	allAccounts = database.selectMultipleStrings(selectQuery, fieldName, whereStatement);
}

BOOL AddMoneyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	setLoginAccId();

	setAllBankAccounts();

	for (size_t i = 0; i < allAccounts.size(); ++i)
	{
		comboBoxAccountsList.AddString(allAccounts[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void AddMoneyDlg::OnBnClickedButtonAddMoney()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	comboBoxAccountsList.GetLBText(comboBoxAccountsList.GetCurSel(), strCurrentAccount);

	BankAccount current (strCurrentAccount);

	//Validate that the bank account exists in the database
	//If not exists, throw a message
	if (current.validate(strCurrentAccount))
	{
		if (current.addMoney(_wtof(strAmount)))
		{
			AfxMessageBox(L"Money added successfully!");
		}

		CDialogEx::OnOK();
	}


}

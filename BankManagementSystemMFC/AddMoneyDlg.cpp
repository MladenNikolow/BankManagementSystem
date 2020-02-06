// AddMoneyDlg.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "AddMoneyDlg.h"
#include"ValidateBankAccount.h"
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


BOOL AddMoneyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (size_t i = 0; i < allAccounts.size(); ++i)
	{
		comboBoxAccountsList.AddString(allAccounts[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL AddMoneyDlg::validation(const CString& strBankAcc)
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
void AddMoneyDlg::OnBnClickedButtonAddMoney()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	comboBoxAccountsList.GetLBText(comboBoxAccountsList.GetCurSel(), strCurrentAccount);

	if (validation(strCurrentAccount))
	{
		CDialogEx::OnOK();
	}

}

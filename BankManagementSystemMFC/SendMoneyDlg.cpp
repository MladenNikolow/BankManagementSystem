// SendMoneyDlg.cpp : implementation file


#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "SendMoneyDlg.h"
#include "ValidateBankAccount.h"
#include "afxdialogex.h"


// SendMoneyDlg dialog

IMPLEMENT_DYNAMIC(SendMoneyDlg, CDialogEx)

SendMoneyDlg::SendMoneyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SEND, pParent)
	, strReceiverBankAcc(_T(""))
	, strAmount(_T(""))
{

}

SendMoneyDlg::~SendMoneyDlg()
{
}

void SendMoneyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SENDER_BA, comboBoxAccounts);
	DDX_Text(pDX, IDC_EDIT_RECIPIENT_BA, strReceiverBankAcc);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, strAmount);
}


BEGIN_MESSAGE_MAP(SendMoneyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND_MONEY, &SendMoneyDlg::OnBnClickedButtonSendMoney)
END_MESSAGE_MAP()


// SendMoneyDlg message handlers


BOOL SendMoneyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (size_t i = 0; i < allAccounts.size(); ++i)
	{
		comboBoxAccounts.AddString(allAccounts[i]);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL SendMoneyDlg::validation(const CString& strBankAcc)
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

void SendMoneyDlg::OnBnClickedButtonSendMoney()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//get
	comboBoxAccounts.GetLBText(comboBoxAccounts.GetCurSel(), strCurrentBankAcc);

	//Validate the bank accounts of the sender and the receiver

	if (validation(strReceiverBankAcc) && validation(strCurrentBankAcc))
	{
		CDialogEx::OnOK();
	}
}

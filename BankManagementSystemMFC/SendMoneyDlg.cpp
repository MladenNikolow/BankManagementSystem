// SendMoneyDlg.cpp : implementation file


#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "SendMoneyDlg.h"
#include "Currency.h"
#include "BankAccount.h"
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

void SendMoneyDlg::setLoginAccId()
{
	//Get the current account id
	CString selectQuery = L"SELECT id FROM LoginAccounts";
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmail);

	nAccId = database.selectInt(selectQuery, L"id", whereStatement);
}


void SendMoneyDlg::setAllBankAccounts()
{
	//Get all bank accounts of this user
	CString selectQuery;
	selectQuery.Format(L"SELECT bank_acc_number FROM BankAccounts");
	CString whereStatement;
	whereStatement.Format(L"log_acc_id = '%d'", nAccId);

	CString fieldName = _T("bank_acc_number");
	allAccounts = database.selectMultipleStrings(selectQuery, fieldName, whereStatement);
}

BOOL SendMoneyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setLoginAccId();
	setAllBankAccounts();

	for (size_t i = 0; i < allAccounts.size(); ++i)
	{
		comboBoxAccounts.AddString(allAccounts[i]);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void SendMoneyDlg::OnBnClickedButtonSendMoney()
{

	UpdateData(TRUE);

	//Get the current selection of the combo box
	comboBoxAccounts.GetLBText(comboBoxAccounts.GetCurSel(), strCurrentBankAcc);

	//Validate the bank accounts of the sender and the receiver
	BankAccount sender (strCurrentBankAcc);
	BankAccount receiver (strReceiverBankAcc);

	if (sender.validate(strCurrentBankAcc) && receiver.validate(strReceiverBankAcc))
	{
		if (sender.sendMoney(strReceiverBankAcc, _wtof(strAmount)))
		{
			AfxMessageBox(L"Successful bank transfer");
		}
		
		CDialogEx::OnOK();
	}
}

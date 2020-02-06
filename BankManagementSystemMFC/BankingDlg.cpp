// Banking.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "BankingDlg.h"
#include "afxdialogex.h"
#include "afxtempl.h"
#include "Database.h"
#include "BankAccountNumberGenerator.h"
#include "AddBankAccDLG.h"
#include "AddMoneyDlg.h"
#include "WithdrawDlg.h"
#include"SendMoneyDlg.h"
#include "Currency.h"
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

BOOL Banking::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Set the login account id 
	setAccountId();

	//Get all bank accounts of this user
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
	addBankAccDlg.strEmailAcc = strEmailAcc;

	INT_PTR result = addBankAccDlg.DoModal();

	if (result == IDOK)
	{
		CString bankAccNumber = addBankAccDlg.getBankAccNumber();

		//add the account to the combo box in the banking window
		comboBoxAccounts.AddString(bankAccNumber);
		//add the account to the local member
		allAccounts.push_back(bankAccNumber);
	}

}

//Add money into a bank account
void Banking::OnBnClickedButtonAddmoney()
{
	// TODO: Add your control notification handler code here
	AddMoneyDlg addMoneyDlg;

	//Pass all available bank accounts to the Withdraw Windows
	addMoneyDlg.setAllAccounts(allAccounts);


	INT_PTR result = addMoneyDlg.DoModal();

	if (result == IDOK)
	{
		//Get the current balance of the bank account
		CString strSelectQuery;
		strSelectQuery.Format(L"SELECT balance FROM BankAccounts");
		CString fieldName = _T("balance");
		CString whereStatement;
		whereStatement.Format(
			L"bank_acc_number = '%s'",
			addMoneyDlg.strCurrentAccount
		);

		double currentBalance = database.selectDouble(strSelectQuery, fieldName, whereStatement);

		//Get the id of the bank account
		CString SelectQuery;
		SelectQuery.Format(L"SELECT id FROM BankAccounts");
		CString field = _T("id");
		CString strWhereStatement;
		strWhereStatement.Format(
			L"bank_acc_number = '%s'",
			addMoneyDlg.strCurrentAccount
		);
		
		int bankAccId = database.selectInt(SelectQuery, field, strWhereStatement);


		//Insert query for the transactions table
		CString strInsertQuery;
		strInsertQuery.Format(
			L"INSERT INTO Transactions(bank_acc_id, amount, bank_number_sender, bank_number_recipient, transaction_time) VALUES ('%d', '%f', '%s', '%s', GETDATE())",
			bankAccId,
			_wtof(addMoneyDlg.strAmount),
			L"out",
			addMoneyDlg.strCurrentAccount
		);


		//Insert query for the amount field in bank account table
		CString strUpdateQuery;
		strUpdateQuery.Format(
			L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
			currentBalance + _wtof(addMoneyDlg.strAmount),
			addMoneyDlg.strCurrentAccount
		);

		if (database.Execute(strInsertQuery) && database.Execute(strUpdateQuery))
		{
			AfxMessageBox(L"Money added successfully!");
		}
		else
		{
			//do nothing
			//Database::Execute() will throw a message automatically, if something go wrong
		}
	}
}

//Withdraw a money from a bank account
void Banking::OnBnClickedButtonWithdraw()
{
	// TODO: Add your control notification handler code here
	WithdrawDlg withdrawDlg;
	withdrawDlg.strEmailAcc = strEmailAcc;


	//Open the windows
	INT_PTR result = withdrawDlg.DoModal();

}



//Button to show the histroty of the transactions and the balance of the bank account
void Banking::OnBnClickedButtonHistory()
{

	
	//Clear the history string
	strTransHistory = L"";

	//Get the current bank account number
	comboBoxAccounts.GetLBText(comboBoxAccounts.GetCurSel(), strBankAccName);


		//Get the id of the bank account
		CString strSelectQuery;
		strSelectQuery.Format(L"SELECT id FROM BankAccounts");
		CString strfield = _T("id");
		CString strWhereStatement;
		strWhereStatement.Format(
			L"bank_acc_number = '%s'",
			strBankAccName
		);
		CString selectQuery;
		selectQuery.Format(L"SELECT ");

		int bankAccId = database.selectInt(strSelectQuery, strfield, strWhereStatement);

		//Get the amounts of the transactions
		strSelectQuery.Format(L"SELECT amount FROM Transactions");
		strfield = _T("amount");
		strWhereStatement.Format(
			L"bank_acc_id = '%d'",
			bankAccId
		);

		std::vector <double> allAmounts = database.selectMultipleDoubles(strSelectQuery, strfield, strWhereStatement);

		//Get the sender of the transactions
		strSelectQuery.Format(L"SELECT bank_number_sender FROM Transactions");
		strfield = _T("bank_number_sender");
		strWhereStatement.Format(
			L"bank_acc_id = '%d'",
			bankAccId
		);

		std::vector <CString> allSenders = database.selectMultipleStrings(strSelectQuery, strfield, strWhereStatement);

		//Get the receipients of the transactions
		strSelectQuery.Format(L"SELECT bank_number_recipient FROM Transactions");
		strfield = _T("bank_number_recipient");
		strWhereStatement.Format(
			L"bank_acc_id = '%d'",
			bankAccId
		);

		std::vector <CString> allRecipients = database.selectMultipleStrings(strSelectQuery, strfield, strWhereStatement);

		//Get the dates of the transactions
		strSelectQuery.Format(L"SELECT transaction_time FROM Transactions");
		strfield = _T("transaction_time");
		strWhereStatement.Format(
			L"bank_acc_id = '%d'",
			bankAccId
		);

		std::vector <CString> allDates = database.selectMultipleStrings(strSelectQuery, strfield, strWhereStatement);
		CString current;

		for (size_t i = 0; (i < 10) && (i < allAmounts.size()); ++i)
		{
			if (allSenders[i] == "out")
			{
				//Added money
				current.Format(L"Your bank transfer --- Amount: %.*f --- Date: %s\r\n",
					2,//precision
					allAmounts[i],
					allDates[i]
				);
				strTransHistory.Append(current);

			}
			else if (allRecipients[i] == "out")
			{
				//Money withdrawal
				current.Format(L"Money withdrawal --- Amount: %.*f --- Date: %s\r\n",
					2, //precision
					allAmounts[i],
					allDates[i]
				);

				strTransHistory.Append(current);
			}
			else
			{
				//Send or received money

				if (allSenders[i] == strBankAccName)
				{
					//Send money
					current.Format(L"Bank Transfer (sended money) --- Amount: %.*f --- Recipient: %s --- Date: %s\r\n",
						2,
						allAmounts[i],
						allRecipients[i],
						allDates[i]
					);

					strTransHistory.Append(current);

				}
				else
				{
					//Receive money

					//MUST CHANGE THE AMOUNT OF THE TRANSFER IF THE CURRENCIES ARE DIFFRENET

					//Get the currency of the sender
					CString SelectQuery;
					SelectQuery.Format(L"SELECT currency FROM BankAccounts");
					CString field = _T("currency");
					strWhereStatement.Format(
						L"bank_acc_number = '%s'",
						allSenders[i]
					);

					CString senderCurrency = database.selectString(SelectQuery, field, strWhereStatement);

					//Get the currency of the receiver
					SelectQuery.Format(L"SELECT currency FROM BankAccounts");
					field = _T("currency");
					strWhereStatement.Format(
						L"bank_acc_number = '%s'",
						strBankAccName
					);

					CString receiverCurrency = database.selectString(SelectQuery, field, strWhereStatement);

					//check the currencies of the two bank accounts
					double amount;
					if (senderCurrency == receiverCurrency) //the two accounts are in the same currency
					{
						amount = allAmounts[i];
					}
					else //the two accounts are in different currency
					{
						amount = exchangeRateCalculator(allAmounts[i], senderCurrency, receiverCurrency);
					}
					current.Format(L"Bank Transfer (Received money) --- Amount: %.*f --- Sender: %s --- Date: %s\r\n",
						2,
						amount,
						allSenders[i],
						allDates[i]
					);

					strTransHistory.Append(current);
				}
			}
		}

		//Get the balance of the bank account
		strSelectQuery.Format(L"SELECT balance FROM BankAccounts");
		strfield = _T("balance");
		strWhereStatement.Format(
			L"bank_acc_number = '%s'",
			strBankAccName
		);


		double balance = database.selectDouble(strSelectQuery, strfield, strWhereStatement);
		strBalance.Format(L"%.*f", 2, balance);

		//Update the info in the balance editbox and transaction history editbox
		UpdateData(FALSE);
	
	
	
}


void Banking::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	SendMoneyDlg sendMoneyDlg;
	sendMoneyDlg.setAllAccounts(allAccounts);

	INT_PTR result = sendMoneyDlg.DoModal();

	if (result == IDOK)
	{
		//Get the current balance of the sender bank account
		CString strSelectQuery;
		strSelectQuery.Format(L"SELECT balance FROM BankAccounts");
		CString fieldName = _T("balance");
		CString whereStatement;
		whereStatement.Format(
			L"bank_acc_number = '%s'",
			sendMoneyDlg.strCurrentBankAcc
		);

		double currentBalance = database.selectDouble(strSelectQuery, fieldName, whereStatement);

		//Get the current balance of the receiver bank account
		strSelectQuery;
		strSelectQuery.Format(L"SELECT balance FROM BankAccounts");
		fieldName = _T("balance");
		whereStatement;
		whereStatement.Format(
			L"bank_acc_number = '%s'",
			sendMoneyDlg.strReceiverBankAcc
		);

		double currentBalanceReceiver = database.selectDouble(strSelectQuery, fieldName, whereStatement);
		//Get the id of the sender bank account
		CString SelectQuery;
		SelectQuery.Format(L"SELECT id FROM BankAccounts");
		CString field = _T("id");
		CString strWhereStatement;
		strWhereStatement.Format(
			L"bank_acc_number = '%s'",
			sendMoneyDlg.strCurrentBankAcc
		);

		int bankAccId = database.selectInt(SelectQuery, field, strWhereStatement);

		//Get the id of the receiver bank account
		SelectQuery.Format(L"SELECT id FROM BankAccounts");
		strWhereStatement.Format(
			L"bank_acc_number = '%s'",
			sendMoneyDlg.strReceiverBankAcc
		);

		int bankAccIdReceiver = database.selectInt(SelectQuery, field, strWhereStatement);

		if (currentBalance < _wtof(sendMoneyDlg.strAmount))
		{
			//there isn't enough money
			int precision = 2;
			CString error;
			error.Format(L"Not enough money!\nThe current balance of your bank account is: %.*f", precision, currentBalance);
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
				_wtof(sendMoneyDlg.strAmount),
				sendMoneyDlg.strCurrentBankAcc,
				sendMoneyDlg.strReceiverBankAcc
			);

			//Update of the sender bank account
			CString strUpdateQuery;
			strUpdateQuery.Format(
				L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
				currentBalance - _wtof(sendMoneyDlg.strAmount),
				sendMoneyDlg.strCurrentBankAcc
			);

			if (database.Execute(strInsertQuery) && database.Execute(strUpdateQuery))
			{
				AfxMessageBox(L"The bank transfer successful!");
			}
			else
			{
				//do nothing
				//Database::Execute() will throw a message automatically, if something go wrong
			}


			//Get the currency of the sender
			SelectQuery.Format(L"SELECT currency FROM BankAccounts");
			field = _T("currency");
			strWhereStatement.Format(
				L"bank_acc_number = '%s'",
				sendMoneyDlg.strCurrentBankAcc
			);

			CString senderCurrency = database.selectString(SelectQuery, field, strWhereStatement);

			//Get the currency of the receiver
			SelectQuery.Format(L"SELECT currency FROM BankAccounts");
			field = _T("currency");
			strWhereStatement.Format(
				L"bank_acc_number = '%s'",
				sendMoneyDlg.strReceiverBankAcc
			);

			CString receiverCurrency = database.selectString(SelectQuery, field, strWhereStatement);

			//check the currencies of the two bank accounts
			double amount;
			if (senderCurrency == receiverCurrency) //the two accounts are in the same currency
			{
				amount = _wtof(sendMoneyDlg.strAmount);
			}
			else //the two accounts are in different currency
			{
				amount = exchangeRateCalculator(_wtof(sendMoneyDlg.strAmount), senderCurrency, receiverCurrency);
			}

			//Insert query for the update of the receiver bank account
			strUpdateQuery;
			strUpdateQuery.Format(
				L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
				currentBalanceReceiver + amount,
				sendMoneyDlg.strReceiverBankAcc
			);

			database.Execute(strUpdateQuery);

			//Insert transaction for the receiver
			strInsertQuery.Format(
				L"INSERT INTO Transactions(bank_acc_id, amount, bank_number_sender, bank_number_recipient, transaction_time) VALUES ('%d', '%f', '%s', '%s', GETDATE())",
				bankAccIdReceiver,
				_wtof(sendMoneyDlg.strAmount),
				sendMoneyDlg.strCurrentBankAcc,
				sendMoneyDlg.strReceiverBankAcc
			);

			database.Execute(strInsertQuery);

		}


	}
}

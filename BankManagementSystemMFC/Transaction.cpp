#include "pch.h"
#include "Transaction.h"
#include "BankAccount.h"
#include "Currency.h"


void Transaction::generateTransaction(const CString& sender, const CString& receiver, const double& amount)
{
	BankAccount current(sender);

	if (current.getBankId(sender) == -1)
	{
		CString strInsertQuery;
		strInsertQuery.Format(
			L"INSERT INTO Transactions(bank_acc_id, amount, bank_number_sender, bank_number_recipient, transaction_time) VALUES ('%d', '%f', '%s', '%s', GETDATE())",
			current.getBankId(receiver),
			amount,
			sender,
			receiver
		);
		database.Execute(strInsertQuery);
	}
	else 
	{
		CString strInsertQuery;
		strInsertQuery.Format(
			L"INSERT INTO Transactions(bank_acc_id, amount, bank_number_sender, bank_number_recipient, transaction_time) VALUES ('%d', '%f', '%s', '%s', GETDATE())",
			current.getBankId(sender),
			amount,
			sender,
			receiver
		);
		database.Execute(strInsertQuery);
	}




}
CString Transaction::getTransactionsToString()
{
	CString result;

	BankAccount current(bankAccName);

	//Get the id of the bank account
	int bankAccId = current.getBankId(bankAccName);


	//Get the amount of all input transactions
	CString strSelectQuery;
	strSelectQuery.Format(L"SELECT amount FROM Transactions");
	CString strfield;
	strfield = _T("amount");
	CString strWhereStatement;
	strWhereStatement.Format(
		L"bank_number_recipient = '%s'",
		bankAccName
	);

	std::vector <double> allInputAmounts = database.selectMultipleDoubles(strSelectQuery, strfield, strWhereStatement);

	//Get the senders of all input transactions
	strSelectQuery.Format(L"SELECT bank_number_sender FROM Transactions");
	strfield = _T("bank_number_sender");
	strWhereStatement.Format(
		L"bank_number_recipient = '%s'",
		bankAccName
	);

	std::vector <CString> allInputSenders = database.selectMultipleStrings(strSelectQuery, strfield, strWhereStatement);

	//Get the date of all input transactions
	strSelectQuery.Format(L"SELECT transaction_time FROM Transactions");
	strfield = _T("transaction_time");
	strWhereStatement.Format(
		L"bank_number_recipient = '%s'",
		bankAccName
	);

	std::vector <CString> allInputDate = database.selectMultipleStrings(strSelectQuery, strfield, strWhereStatement);
	CString temp;
	for (size_t i = 0; i < allInputSenders.size(); i++)
	{
		if (allInputSenders[i] == "out")
		{
			temp.Format(L"Your bank transfer --- Amount: %.*f --- Date: %s\r\n",
				2,//precision
				allInputAmounts[i],
				allInputDate[i]
			);

			result.Append(temp);
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
				allInputSenders[i]
			);

			CString senderCurrency = database.selectString(SelectQuery, field, strWhereStatement);

			//Get the currency of the receiver
			CString receiverCurrency = current.getCurrency(bankAccName);

			//check the currencies of the two bank accounts
			double amount;

			if (senderCurrency == receiverCurrency) //the two accounts are in the same currency
			{
				amount = allInputAmounts[i];
			}
			else //the two accounts are in different currency
			{
				amount = exchangeRateCalculator(allInputAmounts[i], senderCurrency, receiverCurrency);
			}

			temp.Format(L"Bank Transfer (Received money) --- Amount: %.*f --- Sender: %s --- Date: %s\r\n",
				2,
				amount,
				allInputSenders[i],
				allInputDate[i]
			);

			result.Append(temp);
		}
	}
	

	//Get the amount of all output transactions
	strSelectQuery.Format(L"SELECT amount FROM Transactions");
	strfield = _T("amount");
	strWhereStatement.Format(
		L"bank_number_sender = '%s'",
		bankAccName
	);

	std::vector <double> allOutputAmounts = database.selectMultipleDoubles(strSelectQuery, strfield, strWhereStatement);

	//Get the receiver of all input transactions
	strSelectQuery.Format(L"SELECT bank_number_recipient FROM Transactions");
	strfield = _T("bank_number_recipient");
	strWhereStatement.Format(
		L"bank_number_sender = '%s'",
		bankAccName
	);

	std::vector <CString> allOutputReceivers = database.selectMultipleStrings(strSelectQuery, strfield, strWhereStatement);

	//Get the date of all input transactions
	strSelectQuery.Format(L"SELECT transaction_time FROM Transactions");
	strfield = _T("transaction_time");
	strWhereStatement.Format(
		L"bank_number_sender = '%s'",
		bankAccName
	);

	std::vector <CString> allOutputDate = database.selectMultipleStrings(strSelectQuery, strfield, strWhereStatement);

	for (size_t i = 0; i < allOutputReceivers.size(); i++)
	{
		if (allOutputReceivers[i] == "out")
		{
			//Withdrawal money

			temp.Format(L"Withdrawal money --- Amount: %.*f --- Date: %s\r\n",
				2,//precision
				allOutputAmounts[i],
				allOutputDate[i]
			);

			result.Append(temp);
		}
		else
		{
			//Send money
			//MUST CHANGE THE AMOUNT OF THE TRANSFER IF THE CURRENCIES ARE DIFFRENET

			temp.Format(L"Bank Transfer (Sended money) --- Amount: %.*f --- Receiver: %s --- Date: %s\r\n",
				2,
				allOutputAmounts[i],
				allOutputReceivers[i],
				allOutputDate[i]
			);

			result.Append(temp);
		}
	}

	return result;

}

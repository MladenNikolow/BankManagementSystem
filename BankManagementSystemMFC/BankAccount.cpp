#include "pch.h"
#include "BankAccount.h"
#include "Currency.h"
#include "Transaction.h"


BOOL BankAccount::validate(const CString& bankAccName)
{
	CString strSqlSelect;
	strSqlSelect.Format(L"SELECT bank_acc_number FROM BankAccounts");
	CString strField = L"bank_acc_number";
	CString strWhereStatement;
	strWhereStatement.Format(L"bank_acc_number = '%s'", bankAccName);

	if (database.selectString(strSqlSelect, strField, strWhereStatement) == "No Data")
	{
		//AfxMessageBox(L"Enter a valid bank account!");
		return FALSE;
	}

	return TRUE;
}

int BankAccount::getLoginId(const CString& bankAccName) 
{

		//Get bank id
		CString selectQuery = L"SELECT log_acc_id FROM BankAccounts";
		CString whereStatement;
		whereStatement.Format(L"bank_acc_number = '%s'", bankAccName);

		return database.selectInt(selectQuery, L"log_acc_id", whereStatement);

}

int BankAccount::getBankId(const CString& bankAccName)
{
		//Get bank id
		CString selectQuery = L"SELECT id FROM BankAccounts";
		CString whereStatement;
		whereStatement.Format(L"bank_acc_number = '%s'", bankAccName);

		return database.selectInt(selectQuery, L"id", whereStatement);
}

double BankAccount::getBalance(const CString& bankAccName)
{

	//Get balance
	CString selectQuery = L"SELECT balance FROM BankAccounts";
	CString whereStatement;
	whereStatement.Format(L"bank_acc_number = '%s'", bankAccName);

	return database.selectDouble(selectQuery, L"balance", whereStatement);
}

CString BankAccount::getCurrency(const CString& bankAccName)
{
	//Get the currency
	CString selectQuery = L"SELECT currency FROM BankAccounts";
	CString whereStatement;
	whereStatement.Format(L"bank_acc_number = '%s'", bankAccName);

	return database.selectString(selectQuery, L"currency", whereStatement);
}

BOOL BankAccount::addMoney(const double& amount)
{
	double balance = getBalance(name);
	int bankId = getBankId(name);

	//Generate the transaction
	Transaction current(name);
	current.generateTransaction(L"out", name, amount);

	//Insert query for the amount field in bank account table
	CString strUpdateQuery;
	strUpdateQuery.Format(
		L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
		balance + amount,
		name
	);

	if (database.Execute(strUpdateQuery))
	{
		return TRUE;
	}
	else
	{
		//Database::Execute() will throw a message automatically, if something go wrong
		return FALSE;
	}

}


BOOL BankAccount::withdrawMoney(const double& amount)
{
	double balance = getBalance(name);
	int bankId = getBankId(name);

	//Check if there is enough money
	if (balance < amount)
	{
		//there isn't enough money
		int precision = 2;
		CString error;
		error.Format(L"Not enough money!\nThe current balance of the bank account is: %.*f", precision, balance);
		AfxMessageBox(error);
	}
	else
	{
		//there is enough money to perform the operation
		//Generate the transaction
		Transaction current(name);
		current.generateTransaction(name, L"out", amount);


		//Insert query for the amount field in bank account table
		CString strUpdateQuery;
		strUpdateQuery.Format(
			L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
			balance - amount,
			name
		);

		if (database.Execute(strUpdateQuery))
		{
			return TRUE;
		}
		else
		{
			//Database::Execute() will throw a message automatically, if something go wrong
			return FALSE;
		}
	}
}

void BankAccount::addNewAccount(const int& loginAccId, const double& amount, const CString& currency)
{
	//Create the bank account (Insert into BankAccounts)
	CString insertQuery;
	insertQuery.Format(L"INSERT INTO BankAccounts (log_acc_id, bank_acc_number, currency, balance, date_of_creation) VALUES ('%i', '%s', '%s', '%f', GETDATE())",
		loginAccId,
		name,
		currency,
		amount
	);

	bool isCreated = database.Execute(insertQuery);

	

	int bankAccId = getBankId(name);

	if (amount > 0.00)
	{
		//Generate the transaction
		Transaction current(name);
		current.generateTransaction(L"out", name, amount);
	}

		if (isCreated)
		{
			AfxMessageBox(L"The new bank account was created successfully!");
		}

}

BOOL BankAccount::sendMoney(const CString& receiver, const double& amount)
{
	double senderBalance = getBalance(name);
	double receiverBalance = getBalance(receiver);

	int senderBankId = getBankId(name);
	int receiverBankId = getBankId(receiver);

	if (senderBalance < amount)
	{
		//there isn't enough money
		int precision = 2;
		CString error;
		error.Format(L"Not enough money!\nThe current balance of your bank account is: %.*f", precision, senderBalance);
		AfxMessageBox(error);

		return FALSE;
	}
	else
	{
		//there is enough money to perform the operation

		//Generate the transaction
		Transaction current(name);
		current.generateTransaction(name, receiver, amount);

		//Update of the sender bank account
		CString strUpdateQuery;
		strUpdateQuery.Format(
			L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
			senderBalance - amount,
			name
		);

		database.Execute(strUpdateQuery);

		//Get the currency of the sender account
		CString senderCurrency = getCurrency(name);

		//Get the currency of the receiver account
		CString receiverCurrency = getCurrency(receiver);

		//check the currencies of the two bank accounts
		double exchangedAmount;
		if (senderCurrency == receiverCurrency) //the two accounts are in the same currency
		{
			exchangedAmount = amount;
		}
		else //the two accounts are in different currency
		{
			exchangedAmount = exchangeRateCalculator(amount, senderCurrency, receiverCurrency);
		}

		//Insert query for the update of the receiver bank account
		strUpdateQuery;
		strUpdateQuery.Format(
			L"UPDATE BankAccounts SET balance = '%f' WHERE bank_acc_number = '%s'",
			receiverBalance + exchangedAmount,
			receiver
		);

		database.Execute(strUpdateQuery);
	}

	return TRUE;

}


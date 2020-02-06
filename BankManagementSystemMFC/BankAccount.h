#pragma once

#include "Database.h"

class BankAccount
{
private:
	Database database;
	CString name;

public:

	//Default constructor unavailable
	BankAccount() = delete;
	//Custom constructor
	BankAccount(const CString& bankAccName) : name(bankAccName) {}

	//Gettters
	double getBalance(const CString& bankAccName);
	int getBankId(const CString& bankAccName);
	int getLoginId(const CString& bankAccName);
	CString getCurrency(const CString& bankAccName);

	//Validate if exists account with that name
	BOOL validate(const CString& bankAccName);

	//Change the balance of the bank account
	//Generate the transaction
	BOOL addMoney(const double& amount);

	//Change the balance of the bank account
	//Generate the transaction
	BOOL withdrawMoney(const double& amount);

	//Create the new account (insert into the database)
	//Generate a transaction if it's needed
	void addNewAccount(const int& loginAccId, const double& amount, const CString& currency);

	//Send the money from one bank account to another
	//Generate the transaction and update the balance of the bank accounts
	BOOL sendMoney(const CString& receiver, const double& amount);

	//Virtual Destructor
	virtual ~BankAccount() = default;


};


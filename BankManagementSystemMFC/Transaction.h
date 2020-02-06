#pragma once
#include "pch.h"
#include "Database.h"

class Transaction
{
private:
	Database database;
	CString bankAccName;

public:
	Transaction() = delete;
	Transaction(const CString& accName): bankAccName(accName) {}

	//Generate a transaction and insert it into the database
	void generateTransaction(const CString& sender, const CString& receiver, const double& amount);

	//Return CString containting all transactions for the account
	CString getTransactionsToString();

};


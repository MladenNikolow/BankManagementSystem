#pragma once
#include "Database.h"
class ValidateBankAccount
{
private:
	Database database;

public:
	BOOL validation(const CString& strBankAcc);
};


#include "pch.h"
#include "ValidateBankAccount.h"
#include "Database.h"

BOOL ValidateBankAccount::validation(const CString& strBankAcc)
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
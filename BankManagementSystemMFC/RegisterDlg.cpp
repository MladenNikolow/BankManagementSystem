// Register.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "Database.h"
#include "Currency.h"
#include "BankAccountNumberGenerator.h"
#include <regex>



// Register dialog

IMPLEMENT_DYNAMIC(Register, CDialogEx)

Register::Register(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REGISTER, pParent)
	, strFirstName(_T("Enter First Name"))
	, strSecondName(_T(""))
	, strLastName(_T(""))
	, strEmailReg(_T(""))
	, strPhone(_T(""))
	, strCity(_T(""))
	, strAddress(_T(""))
	, strPasswordReg(_T(""))

	, strAge(_T(""))
	, strCurrency(_T(""))
{

}

Register::~Register()
{
}

BOOL Register::validateAge()
{
	CString inputAge;
	GetDlgItemText(IDC_COMBO_AGE, inputAge);

	for (int i = 0; i < inputAge.GetLength(); ++i)
	{
		if (!std::isdigit(inputAge[i]))
		{
			AfxMessageBox(L"Enter a valid age!");
			return FALSE;
		}
	}

	if (_wtoi(inputAge) < 18)
	{
		AfxMessageBox(L"The minimum age is 18!");
		return FALSE;
	}

	return TRUE;
}

BOOL Register::validateName()
{
	CString fullName = strFirstName;
	fullName.Append(strSecondName);
	fullName.Append(strLastName);
	if (strFirstName.GetLength() < 2 || strFirstName.GetLength() < 2 || strFirstName.GetLength() < 2)
	{
		AfxMessageBox(L"The name must be at lest 2 letters!");
		return FALSE;
	}
	for (int i = 0; i < fullName.GetLength(); ++i)
	{
		if (!std::isalpha(fullName[i]))
		{
			AfxMessageBox(L"The name must contain only uppercase and lowercase letters!");
			return FALSE;
		}
	}

	return TRUE;


}

BOOL Register::validatePassword()
{
	if (strPasswordReg.GetLength() < 8)
	{
		AfxMessageBox(L"Your password must be at least 8 characters long!");
		return FALSE;
	}

	BOOL upperChar = FALSE;
	BOOL numChar = FALSE;

	for (int i = 0; i < strPasswordReg.GetLength(); ++i)
	{
		if (std::isupper(strPasswordReg[i]))
		{
			upperChar = TRUE;
		}
		else if (std::isdigit(strPasswordReg[i]))
		{
			numChar = TRUE;
		}
	}

	if (!numChar)
	{
		AfxMessageBox(L"Your password must contain at least one numeric character!");
		return FALSE;
	}

	if (!upperChar)
	{
		AfxMessageBox(L"Your password must contain at least one uppercase letter!");
		return FALSE;
	}
	
	return TRUE;

}
BOOL Register::validateNumber()
{

	if (strPhone[0] == '+') //+359
	{
		if (strPhone.GetLength() != 13)
		{
			AfxMessageBox(L"The number must be a valid bulgarian number starting with 0 or +359!");
			return FALSE;
		}
		else
		{
			for (size_t i = 1; i < 12; ++i)
			{
				if (!std::isdigit(strPhone[i]))
				{
					AfxMessageBox(L"The number must be a valid bulgarian number starting with 0 or +359!");
					return FALSE;
				}
			}
		}
	}
	else if (strPhone[0] == '0')
	{

		if (strPhone.GetLength() != 10)
		{
			AfxMessageBox(L"The number must be a valid bulgarian number starting with 0 or +359!");
			return FALSE;
		}
		else 
		{

			for (size_t i = 1; i < 9; ++i)
			{
				if (!std::isdigit(strPhone[i]))
				{
					AfxMessageBox(L"The number must be a valid bulgarian number starting with 0 or +359!");
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
BOOL Register::validateEmail()
{
	//Check for '@' symbol
	int foundIndex = strEmailReg.Find('@', 0);

	if (foundIndex < 1)
	{
		AfxMessageBox(L"Enter valid email!");
		return FALSE;
	}
	else
	{
		//Check for '.' after the '@' symbol
		int foundIndexPoint = strEmailReg.Find('.', foundIndex);

		if (foundIndexPoint < foundIndex + 2)
		{
			AfxMessageBox(L"Enter valid email!");
			return FALSE;
		}
	}

	//Check if already there is a registration with that email
	CString strSqlSelect;
	strSqlSelect.Format(L"SELECT email FROM LoginAccounts");
	CString strField = _T("email");
	CString strWhereStatement;
	strWhereStatement.Format(L"email = '%s'", strEmailReg);
	int result = database.selectInt(strSqlSelect, strField, strWhereStatement);
	if (result != -1)
	{
		AfxMessageBox(L"An account with such an email already exists!");
		return FALSE;
	}

	return TRUE;
	
}

BOOL Register::validateCity()
{
	if (strCity.GetLength() < 2)
	{
		AfxMessageBox(L"Enter a valid city!");
		return FALSE;
	}
	else
	{
		for (size_t i = 0; i < strCity.GetLength(); ++i)
		{
			if (!std::isalpha(strCity[i]))
			{
				AfxMessageBox(L"Enter a valid city!");
				return FALSE;
			}
		}
	}

	return TRUE;
}

void Register::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIRSTNAME, strFirstName);
	DDX_Text(pDX, IDC_EDIT_SECONDNAME, strSecondName);
	DDX_Text(pDX, IDC_EDIT_LASTNAME, strLastName);
	DDX_Text(pDX, IDC_EDIT_EMAILREG, strEmailReg);

	DDX_Text(pDX, IDC_EDIT_PHONE, strPhone);
	DDX_Text(pDX, IDC_EDIT_CITY, strCity);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, strAddress);
	DDX_Text(pDX, IDC_EDIT_PASSWORDREG, strPasswordReg);

	DDX_CBString(pDX, IDC_COMBO_AGE, strAge);
	DDV_MaxChars(pDX, strAge, 2);
	DDX_CBString(pDX, IDC_COMBO_CURRENCY, strCurrency);
	DDV_MaxChars(pDX, strCurrency, 3);
	DDX_Control(pDX, IDC_EDIT_PASSWORDREG, ctrPass);
	DDX_Control(pDX, IDC_CHECK_PASS, ctrCheckBox);
}


BEGIN_MESSAGE_MAP(Register, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_SIGNUP, &Register::OnBnClickedButtonSignup)

	ON_BN_CLICKED(IDC_CHECK_PASS, &Register::OnBnClickedCheckPass)
END_MESSAGE_MAP()


// Register message handlers

void Register::createLoginAccount()
{
	//Save Query
	CString strSqlInsertQuery = _T("");
	strSqlInsertQuery.Format(
		L" INSERT INTO LoginAccounts(first_name, second_name, last_name, age, email, phone, city, street_address, pass, last_login) VALUES ('%s', '%s', '%s', %i, '%s', '%s', '%s', '%s', '%s', GETDATE())",
		strFirstName,
		strSecondName,
		strLastName,
		_wtoi(strAge),
		strEmailReg,
		strPhone,
		strCity,
		strAddress,
		strPasswordReg
	);
	///Execute the query
	database.Execute(strSqlInsertQuery);
}

void Register::createBankAccount()
{
	//Get account id form Login table
	CString selectQuery = _T("SELECT id FROM LoginAccounts");
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmailReg);
	CString fieldName = _T("id");

	int log_acc_id = database.selectInt(selectQuery, fieldName, whereStatement);

	//Generate a unique bank account number starting including the currency and 6 random numbers
	CString bankAccNumber;
	bankAccNumber.Append(strCurrency); //add the currency
	bankAccNumber.Append(genRandomBankNumber()); //generate the numbers

	Database database;
	//Create a bank account
	CString sqlInsertQuery;
	sqlInsertQuery.Format(
		L"INSERT INTO BankAccounts(log_acc_id, bank_acc_number, currency, balance, date_of_creation) VALUES ('%i', '%s', '%s', '%d', GETDATE())",
		log_acc_id,
		bankAccNumber,
		strCurrency,
		0.0
	);

	database.Execute(sqlInsertQuery);
}

void Register::OnBnClickedButtonSignup()
{

		UpdateData(TRUE);

		if (validateAge() && validateName() && validatePassword() && validateEmail() && validateNumber() && validateCity() && isValidCurrency(strCurrency))
		{

			createLoginAccount();
			createBankAccount();

			CDialogEx::OnOK();
		}


}








void Register::OnBnClickedCheckPass()
{
	// TODO: Add your control notification handler code here
	if (ctrCheckBox.GetCheck())
	{
		ctrPass.SetPasswordChar('*');
		ctrPass.Invalidate();
	}
	else
	{
		ctrPass.SetPasswordChar(0);
		ctrPass.Invalidate();
	}
}

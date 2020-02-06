#pragma once
#include "Database.h"
// Register dialog

class Register : public CDialogEx
{
	DECLARE_DYNAMIC(Register)

public:
	Register(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Register();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:

	// First Name from register form
	CString strFirstName;
	// Second name from the register form
	CString strSecondName;
	// Last Name from the register form
	CString strLastName;
	// Email from the register form
	CString strEmailReg;
	// Phone from the form register
	CString strPhone;
	// City from the register form
	CString strCity;
	// Address from the reigster form
	CString strAddress;
	// Password from the register form
	CString strPasswordReg;
	afx_msg void OnBnClickedButtonSignup();
	// Age from the register form
	CString strAge;
	// Default currency for the first account
	CString strCurrency;
	// Control the state of the password editbox (show/hide)
	CEdit ctrPass;
	afx_msg void OnBnClickedCheckPass();
	// Control the state of the password CEdit box
	CButton ctrCheckBox;

	Database database;

	//Validate input data
	BOOL validateAge();
	BOOL validateName();
	BOOL validateNumber();
	BOOL validateEmail();
	BOOL validatePassword();
	BOOL validateCity();

	void createLoginAccount();
	void createBankAccount();
};

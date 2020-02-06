
// BankManagementSystemMFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BankManagementSystemMFC.h"
#include "BankManagementSystemMFCDlg.h"
#include "afxdialogex.h"
#include "RegisterDlg.h"
#include "BankingDlg.h"
#include "BankAccountNumberGenerator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBankManagementSystemMFCDlg dialog



CBankManagementSystemMFCDlg::CBankManagementSystemMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BANKMANAGEMENTSYSTEMMFC_DIALOG, pParent)
	, strEmailLogin(_T("Email"))
	, strPasswordLogin(_T("Password"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//If not exist table for the Login Accounts - create it
	CString strCreateLoginTable = _T("");
	strCreateLoginTable.Format(L"IF NOT EXISTS (select * from sysobjects where name='LoginAccounts')CREATE TABLE LoginAccounts(id int IDENTITY(1, 1) NOT NULL, first_name varchar(255) NOT NULL, second_name varchar(255) NOT NULL, last_name varchar(255) NOT NULL, age int NOT NULL, email varchar(255) NOT NULL, phone varchar(255) NOT NULL, city varchar(255) NOT NULL, street_address varchar(255) NOT NULL, pass varchar(255) NOT NULL, last_login DATETIME NOT NULL)");
	database.Execute(strCreateLoginTable);

	//If not exist table for the Bank Accounts - create it
	CString strCreateBankAccTable = _T("");
	strCreateBankAccTable.Format(L"IF NOT EXISTS (select * from sysobjects where name='BankAccounts') CREATE TABLE BankAccounts(id int IDENTITY(1, 1) NOT NULL,log_acc_id int NOT NULL, bank_acc_number varchar(11) NOT NULL, currency varchar(3) NOT NULL, balance numeric(18,2) NOT NULL, date_of_creation DATETIME NOT NULL)");
	database.Execute(strCreateBankAccTable);

	//If not exist table for the Transactions - create it
	CString strCreateTransTable = _T("");
	strCreateTransTable.Format(L"IF NOT EXISTS(select * from sysobjects where name ='Transactions') CREATE TABLE Transactions (id int IDENTITY(1, 1) NOT NULL, bank_acc_id int NOT NULL, amount numeric(18,2) NOT NULL, bank_number_sender varchar(11) NOT NULL, bank_number_recipient varchar(11) NOT NULL, transaction_time DATETIME NOT NULL)");
	database.Execute(strCreateTransTable);
}

void CBankManagementSystemMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EMAIL, strEmailLogin);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, strPasswordLogin);
	DDX_Control(pDX, IDC_CHECK_PASS, ctrCheckBox);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, ctrPass);
}

BEGIN_MESSAGE_MAP(CBankManagementSystemMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CBankManagementSystemMFCDlg::OnBnClickedButtonRegister)

	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CBankManagementSystemMFCDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_CHECK_PASS, &CBankManagementSystemMFCDlg::OnBnClickedCheckPass)
END_MESSAGE_MAP()


// CBankManagementSystemMFCDlg message handlers

BOOL CBankManagementSystemMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBankManagementSystemMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBankManagementSystemMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBankManagementSystemMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBankManagementSystemMFCDlg::OnBnClickedButtonRegister()
{
	// TODO: Add your control notification handler code here
	Register regDlg;
	INT_PTR result = regDlg.DoModal();

	
}

BOOL CBankManagementSystemMFCDlg::checkPassword(const CString& strPass)
{
	//Select password Where email = input email
	CString selectQuery = L"SELECT pass FROM LoginAccounts";
	CString whereStatement;
	whereStatement.Format(L"email = '%s'", strEmailLogin);
	CString currentPassword = database.selectString(selectQuery, L"pass", whereStatement);

	if (strPass == currentPassword)
	{
		return TRUE;
	}

	return FALSE;
}

void CBankManagementSystemMFCDlg::OnBnClickedButtonLogin()
{

	UpdateData(TRUE);

	Banking bankDlg;
	bankDlg.strEmailAcc = strEmailLogin;

	if (checkPassword(strPasswordLogin))
	{ 


		//set the email for the BankDlg
		INT_PTR result = bankDlg.DoModal();

	}
	 else
	{
		AfxMessageBox(L"Wrong email or password!");
	}



}


void CBankManagementSystemMFCDlg::OnBnClickedCheckPass()
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

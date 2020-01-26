// Register.cpp : implementation file
//

#include "pch.h"
#include "BankManagementSystemMFC.h"
#include "Register.h"
#include "afxdialogex.h"


// Register dialog

IMPLEMENT_DYNAMIC(Register, CDialogEx)

Register::Register(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REGISTER, pParent)
{

}

Register::~Register()
{
}

void Register::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Register, CDialogEx)
END_MESSAGE_MAP()


// Register message handlers

#include "pch.h"
#include "Database.h"
#include "BankManagementSystemMFCDlg.h"
#include "afxdb.h"
#include <vector>
#include <string>

Database::Database() :
	database()
{
	CString sDriver = L"ODBC Driver 17 for SQL Server";						//set the Driver
	CString path = L"C:\\Program Files\\Microsoft SQL Server\\MSSQL14.MYSQLSERVER\\MSSQL\\DATA\\BankManagementSytem.mdf"; //set path to the DB
	CString serverName = L"DESKTOP-UCGTPN5";				//set Server Name
	CString nameDB = L"BankManagementSytem";											//set DATABASE Name
	CString user = L"Mladen2";											//set user Name for access DataBase
	CString pass = L"mladen";											//set password for access DataBase
	
	sDsn.Format(L"ODBC;DRIVER={%s};Server=%s;DSN='';DBQ=%s;DATABASE=%s; UID=%s; PWD=%s", sDriver, serverName, path, nameDB, user, pass); // DSN String
	
																																		 //Open the database
	TRY
	{
		database.Open(NULL, false, false, sDsn);
	} CATCH(CDBException, error)
	{
		AfxMessageBox(L"Database error: " + error->m_strError);
	} END_CATCH;
}

Database::~Database() {
	//Close database
	TRY
	{
		database.Close();
	} CATCH(CDBException, error)
	{
		AfxMessageBox(L"Database error: " + error->m_strError);
	} END_CATCH;
}


BOOL Database::Execute(CString CmdStr)
{

	TRY{

		database.ExecuteSQL(CmdStr);


	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
		return FALSE;
	}
	END_CATCH;
	return TRUE;
}

int Database::selectCount(const CString& selectQuery)
{
	TRY{

		database.ExecuteSQL(selectQuery);


	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
		return FALSE;
	}
	END_CATCH;
	return TRUE;
}

int Database::selectInt(const CString& selectQuery, const CString& fieldName, const CString& whereStatement)
{
	CString strId;

	CRecordset recset(&database);
	CRecordset* ptr = &recset;
	ptr->m_strFilter = whereStatement;


	TRY{

		ptr->Open(CRecordset::snapshot, selectQuery, CRecordset::readOnly);
		ptr->GetFieldValue(fieldName, strId);

		return _wtoi(strId);
	}CATCH(CDBException, error) {
		// If a database exception occured, show error msg
		//AfxMessageBox(L"Database error: " + error->m_strError);
		return -1;
	}
	END_CATCH;
}


double Database::selectDouble(const CString& selectQuery, const CString& fieldName, const CString& whereStatement)
{
	CString strId;

	CRecordset recset(&database);
	CRecordset* ptr = &recset;
	ptr->m_strFilter = whereStatement;


	TRY{

		ptr->Open(CRecordset::snapshot, selectQuery, CRecordset::readOnly);
		ptr->GetFieldValue(fieldName, strId);

		return _wtof(strId);
	}CATCH(CDBException, error) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + error->m_strError);
		return -1.0;
	}
	END_CATCH;
}

std::vector<double> Database::selectMultipleDoubles(const CString& selectQuery, const CString& fieldName, const CString& whereStatement)
{
	CString str;
	std::vector<double> result;

	CRecordset recset(&database);
	CRecordset* ptr = &recset;
	ptr->m_strFilter = whereStatement;

	TRY{

		ptr->Open(CRecordset::forwardOnly, selectQuery);

		while (!ptr->IsEOF())
		{
			ptr->GetFieldValue(fieldName, str);
			result.push_back(_wtof(str));
			ptr->MoveNext();
		}


		return result;
	}CATCH(CDBException, error) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + error->m_strError);
		//result.push_back(_T("No Data"));
		return result;
	}
	END_CATCH;
}

CString Database::selectString(const CString& selectQuery, const CString& fieldName, const CString& whereStatement)
{
	CString str;

	CRecordset recset(&database);
	CRecordset* ptr = &recset;
	ptr->m_strFilter = whereStatement;

	TRY{

		ptr->Open(CRecordset::snapshot, selectQuery, CRecordset::readOnly);
		ptr->GetFieldValue(fieldName, str);

		return str;
	}CATCH(CDBException, error) {
		// If a database exception occured, show error msg
		//AfxMessageBox(L"Database error: " + error->m_strError);
		return _T("No Data");
	}
	END_CATCH;
}

std::vector<CString> Database::selectMultipleStrings(const CString& selectQuery, const CString& fieldName, const CString& whereStatement)
{


	CString str;
	std::vector<CString> result;

	CRecordset recset(&database);
	CRecordset* ptr = &recset;
	ptr->m_strFilter = whereStatement;

	TRY{

		ptr->Open(CRecordset::snapshot, selectQuery);

		while (!ptr->IsEOF())
		{
			ptr->GetFieldValue(fieldName, str);
			result.push_back(str);

			ptr->MoveNext();
		}


		return result;
	}CATCH(CDBException, error) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + error->m_strError);
		result.push_back(_T("No Data"));
		return result;
	}
	END_CATCH;
}

#pragma once
#include "odbcinst.h"
#include "afxdb.h"
#include <vector>


class Database
{
public:

	Database();
	~Database();
	BOOL Execute(CString CmdStr);
	//BOOL Database::select(const CString& selectQuery);
	int selectInt(const CString& selectQuery, const CString& fieldName, const CString& whereStatement);
	int selectCount(const CString& selectQuery);
	double selectDouble(const CString& selectQuery, const CString& fieldName, const CString& whereStatement);
	CString selectString(const CString& selectQuery, const CString& fieldName, const CString& whereStatement);
	std::vector<CString> selectMultipleStrings(const CString& selectQuery, const CString& fieldName, const CString& whereStatement);
	std::vector<double> selectMultipleDoubles(const CString& selectQuery, const CString& fieldName, const CString& whereStatement);
private:
	CDatabase database;
	CString sDsn;
};
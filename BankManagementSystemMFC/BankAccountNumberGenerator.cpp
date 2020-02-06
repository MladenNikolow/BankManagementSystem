#include "BankAccountNumberGenerator.h"
#include "pch.h"

CString genRandomBankNumber()
{
    CString result = L"-";
    char alphanum[] = "0123456789";

    int stringLength = sizeof(alphanum) - 1;
    srand(time(0));

    for (size_t i = 0; i < 6; ++i)
    {
        result += alphanum[rand() % stringLength];

    }

    return result;
}
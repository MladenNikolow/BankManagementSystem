#include "Currency.h"
#include "pch.h"



double exchangeRateCalculator(const double& amount, const CString& fromCurr, const CString& toCurr)
{
	if (fromCurr == "BGN")
	{
		if (toCurr == "USD")
		{
			return amount * 0.57;
		}
		else if (toCurr == "GBP")
		{
			return amount * 0.43;
		}
		else if (toCurr == "EUR")
		{
			return amount * 0.51;
		}
	}
	else if (fromCurr == "USD")
	{
		if (toCurr == "BGN")
		{
			return amount * 1.77;
		}
		else if (toCurr == "GBP")
		{
			return amount * 0.77;
		}
		else if (toCurr == "EUR")
		{
			return amount * 0.90;
		}
	}
	else if (fromCurr == "EUR")
	{
		if (toCurr == "BGN")
		{
			return amount * 1.95;
		}
		else if (toCurr == "GBP")
		{
			return amount * 0.85;
		}
		else if (toCurr == "USD")
		{
			return amount * 1.11;
		}
	}
	else if (fromCurr == "GBP")
	{
		if (toCurr == "BGN")
		{
			return amount * 2.31;
		}
		else if (toCurr == "USD")
		{
			return amount * 1.31;
		}
		else if (toCurr == "EUR")
		{
			return amount * 1.18;
		}
	}
}

BOOL isValidCurrency(const CString& curr)
{
	if (curr == L"BGN" || curr == L"USD" || curr == L"GBP" || curr == L"EUR")
	{
		return TRUE;
	}

	AfxMessageBox(L"Enter a valid currency! You can choose between: BGN, USD, EUR or GBP!");
	return FALSE;

}

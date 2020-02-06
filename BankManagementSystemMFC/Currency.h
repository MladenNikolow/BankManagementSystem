#pragma once


	enum Currency
	{
		BGN,
		USD,
		EUR,
		GBP
	};


	double exchangeRateCalculator(const double& amount, const CString& fromCurr, const CString& toCurr);

	BOOL isValidCurrency(const CString& curr);

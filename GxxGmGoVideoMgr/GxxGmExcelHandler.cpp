#include "stdafx.h"
#include "GxxGmExcelHandler.h"

#include "Poco/Path.h"

GxxGmExcelHandler::GxxGmExcelHandler()
: spXlApp(NULL)
{
	// ��ʼ��COM����
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
}

GxxGmExcelHandler::~GxxGmExcelHandler()
{
	// Uninitialize COM for this thread.
	CoUninitialize();
}

int GxxGmExcelHandler::InitializeWrite(const char * xls_path)
{
	file_path = xls_path;
	return 0;
}

int GxxGmExcelHandler::WriteHeader(const char *sheet_name, std::vector<std::string> &headers)
{
	int errCode = 0;

	HRESULT hr = spXlApp.CreateInstance(__uuidof(Excel::Application));
	if (FAILED(hr))
	{
		// ����EXCELʵ��ʧ�ܣ�
		return hr;
	}

	// ����Excelʵ���ķ���״̬
	spXlApp->Visible[0] = VARIANT_FALSE;

	// ����һ���µĹ�����
	spXlBooks = spXlApp->Workbooks;
	spXlBook = spXlBooks->Add();


	int element_count = headers.size();;

	// Get the active Worksheet and set its name.
	// ��ȡ��Ĺ�����������������
	USES_CONVERSION;
	Excel::_WorksheetPtr spXlSheet = spXlBook->ActiveSheet;
	spXlSheet->Name = _bstr_t(A2T(sheet_name));

	// ����һ��1��x�ľ���
	VARIANT saNames;
	saNames.vt = VT_ARRAY | VT_VARIANT;

	SAFEARRAYBOUND sab[2];
	sab[0].lLbound = 1; sab[0].cElements = 1;
	sab[1].lLbound = 1; sab[1].cElements = element_count;
	saNames.parray = SafeArrayCreate(VT_VARIANT, element_count, sab);

	SafeArrayPutName(saNames.parray, 1, headers);

	wchar_t range_str[4096] = {0};
	wchar_t letter_A = L'A';
	wchar_t letter_last = letter_A + headers.size() - 1;
	swprintf(range_str, L"A1:%c1", letter_last);

	VARIANT param;
	param.vt = VT_BSTR;
	param.bstrVal = SysAllocString(range_str);
	Excel::RangePtr spXlRange = spXlSheet->Range[param];

	spXlRange->Value2 = saNames;

	return errCode;
}

int GxxGmExcelHandler::WriteLine(std::vector<std::string> item)
{
	int errCode = 0;

	return errCode;
}

void GxxGmExcelHandler::FinishWrite()
{
	// �õ���ǰĿ¼
	Poco::Path xls_path(Poco::Path::current());
	xls_path.append(file_path);

	// Convert the NULL-terminated string to BSTR
	variant_t vtFileName(xls_path.toString().c_str());

	spXlBook->SaveAs(vtFileName, Excel::xlOpenXMLWorkbook, vtMissing, 
		vtMissing, vtMissing, vtMissing, Excel::xlNoChange);

	spXlBook->Close();

	// Quit the Excel application. (i.e. Application.Quit)
	spXlApp->Quit();
}

int GxxGmExcelHandler::InitializeExcel()
{
	int errCode = 0;

	
	return errCode;
}

void GxxGmExcelHandler::CleanupExcel()
{
	// �����ͷŵ�EXCEL
	spXlApp->Quit();
}

// HRESULT GxxGmExcelHandler::AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs, ...)
// {
// 	// Begin variable-argument list
// 	va_list marker;
// 	va_start(marker, cArgs);
// 
// 	if (!pDisp) 
// 	{
// 		_putws(L"NULL IDispatch passed to AutoWrap()");
// 		//_exit(0);
// 		return E_INVALIDARG;
// 	}
// 
// 	// Variables used
// 	DISPPARAMS dp = { NULL, NULL, 0, 0 };
// 	DISPID dispidNamed = DISPID_PROPERTYPUT;
// 	DISPID dispID;
// 	HRESULT hr;
// 
// 	// Get DISPID for name passed
// 	hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
// 	if (FAILED(hr))
// 	{
// 		wprintf(L"IDispatch::GetIDsOfNames(\"%s\") failed w/err 0x%08lx\n", 
// 			ptName, hr);
// 		//_exit(0);
// 		return hr;
// 	}
// 
// 	// Allocate memory for arguments
// 	VARIANT *pArgs = new VARIANT[cArgs + 1];
// 	// Extract arguments...
// 	for(int i=0; i < cArgs; i++) 
// 	{
// 		pArgs[i] = va_arg(marker, VARIANT);
// 	}
// 
// 	// Build DISPPARAMS
// 	dp.cArgs = cArgs;
// 	dp.rgvarg = pArgs;
// 
// 	// Handle special-case for property-puts
// 	if (autoType & DISPATCH_PROPERTYPUT)
// 	{
// 		dp.cNamedArgs = 1;
// 		dp.rgdispidNamedArgs = &dispidNamed;
// 	}
// 
// 	// Make the call
// 	hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT,
// 		autoType, &dp, pvResult, NULL, NULL);
// 	if (FAILED(hr)) 
// 	{
// 		wprintf(L"IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx\n", 
// 			ptName, dispID, hr);
// 		//_exit(0);
// 		return hr;
// 	}
// 
// 	// End variable-argument section
// 	va_end(marker);
// 
// 	delete[] pArgs;
// 
// 	return hr;
// }
// 
// int GxxGmExcelHandler::SafeArrayPutName(SAFEARRAY* psa, long index, int args, ...)
// {
// 	// Begin variable-argument list
// 	va_list marker;
// 	va_start(marker, args);
// 
// 	int errCode = 0;
// 
// 	// Allocate memory for arguments
// 	VARIANT *pArgs = new VARIANT[args + 1];
// 	// Extract arguments...
// 	for(int i=0; i < args; i++) 
// 	{
// 		pArgs[i] = va_arg(marker, VARIANT);
// 
// 		int element_item_index = i + 1;
// 		long indices1[] = { index, element_item_index };
// 
// 		// Copies the VARIANT into the SafeArray
// 		HRESULT hr = SafeArrayPutElement(psa, indices1, (void*)&pArgs[i]);
// 		if (FAILED(hr))
// 		{
// 			errCode = hr;
// 			break;
// 		}
// 	}
// 
// 	// End variable-argument section
// 	va_end(marker);
// 
// 	delete[] pArgs;
// 
// 	return errCode;
// }

HRESULT GxxGmExcelHandler::SafeArrayPutName(SAFEARRAY* psa, long index, std::vector<std::string> args)
{
	int errCode = 0;

	// Extract arguments...
	int i = 0;
	std::vector<std::string>::iterator iter;
	for(iter = args.begin(); iter != args.end(); ++iter) 
	{
		int element_item_index = i + 1;
		long indices1[] = { index, element_item_index };

		VARIANT element;
		element.vt = VT_LPSTR;
		element.cVal = *iter->c_str();

		// Copies the VARIANT into the SafeArray
		HRESULT hr = SafeArrayPutElement(psa, indices1, (void*)&element);
		if (FAILED(hr))
		{
			errCode = hr;
			break;
		}

		++i;
	}

	return errCode;
}
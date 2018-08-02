#include "GxxGmLocalVideoCapture.h"

GxxGmLocalVideoCapture::GxxGmLocalVideoCapture()
{

}

GxxGmLocalVideoCapture::~GxxGmLocalVideoCapture()
{

}

int GxxGmLocalVideoCapture::EnumVideoDevices()
{
	// ����ö�����б�������ͷ
	int errCode = 0;

	TDeviceName name;   
	HRESULT hr;  

	// ��ʼ��  
	video_devices.clear();  

	// ��ʼ��COM  
	hr = CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );  
	if (FAILED(hr))  
	{  
		return hr;  
	}  

	// ����ϵͳ�豸ö����ʵ��  
	ICreateDevEnum *pSysDevEnum = NULL;  
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pSysDevEnum);  
	if (FAILED(hr))  
	{  
		CoUninitialize();  
		return hr;  
	}  

	// ��ȡ�豸��ö����  
	int index = 0;
	IEnumMoniker *pEnumCat = NULL;  
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);  
	if (hr == S_OK)   
	{  
		// ö���豸����  
		IMoniker *pMoniker = NULL;  
		ULONG cFetched;  
		while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)  
		{  
			IPropertyBag *pPropBag;  
			hr = pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void **)&pPropBag);  
			if (SUCCEEDED(hr))  
			{  
				// ��ȡ�豸�Ѻ���  
				VARIANT varName;  
				VariantInit(&varName);  

				hr = pPropBag->Read(L"FriendlyName", &varName, NULL);  
				if (SUCCEEDED(hr))  
				{  
					StringCchCopy(name.FriendlyName, MAX_FRIENDLY_NAME_LENGTH, varName.bstrVal);  

					// ��ȡ�豸Moniker��  
					LPOLESTR pOleDisplayName = reinterpret_cast<LPOLESTR>(CoTaskMemAlloc(MAX_MONIKER_NAME_LENGTH * 2));  
					if (pOleDisplayName != NULL)  
					{  
						hr = pMoniker->GetDisplayName(NULL, NULL, &pOleDisplayName);  
						if (SUCCEEDED(hr))  
						{  
							StringCchCopy( name.MonikerName, MAX_MONIKER_NAME_LENGTH, pOleDisplayName );  
							video_devices.insert(std::pair<int, TDeviceName>(index, name));
							//vectorDevices.push_back( name );  
							++index;
						}  

						CoTaskMemFree( pOleDisplayName );  
					}  
				}  

				VariantClear( &varName );  
				pPropBag->Release();                       
			}  

			pMoniker->Release();  
		} // End for While  

		pEnumCat->Release();  
	}  

	pSysDevEnum->Release();  
	CoUninitialize();

	return errCode;
}

int GxxGmLocalVideoCapture::StartCapture(int video_index)
{
	// ���￪ʼ������Ƶ�ɼ�������Ӧ�û��յ�����ͷ���صı����
	// ��Ҫһ���ص��ӿڽ����������ȥ
	int errCode = 0;

	return errCode;
}

int GxxGmLocalVideoCapture::StopCapture()
{
	return 0;
}
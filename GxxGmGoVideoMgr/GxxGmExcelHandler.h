#ifndef _GxxGmExcelHandler_H_
#define _GxxGmExcelHandler_H_

// �����������COM��������д���

class GxxGmExcelHandler
{
public:
	GxxGmExcelHandler();
	~GxxGmExcelHandler();

public:
	int Initialize();

private:
	HRESULT AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp,	LPOLESTR ptName, int cArgs...);
};

#endif//_GxxGmExcelHandler_H_

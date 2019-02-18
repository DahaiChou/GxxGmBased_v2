#pragma once
#include "afxwin.h"

#include "..\GxxGmGoVideo\GxxGmGoVideo.h"

#include "Poco/Util/IniFileConfiguration.h"


// CGxxGmGoVideoLoginDlg �Ի���

class CGxxGmGoVideoLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CGxxGmGoVideoLoginDlg)

public:
	CGxxGmGoVideoLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGxxGmGoVideoLoginDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	GxxGmGoVideo *govideo_;

public:
	std::string config_path_;
	Poco::AutoPtr<Poco::Util::IniFileConfiguration> ini_cfg_;

public:
	CEdit m_cGoVideoIp;
	CEdit m_cGoVideoPort;
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnCancel();
};

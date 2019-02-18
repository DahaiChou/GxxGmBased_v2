
// GxxGmGoVideoMgrDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "GxxGmGoVideoLoginDlg.h"
#include "GxxGmDevMgrDlg.h"
#include "GxxGmServiceStateDlg.h"

#include "..\GxxGmGoVideo\GxxGmGoVideo.h"

#include "Poco/Util/IniFileConfiguration.h"


// CGxxGmGoVideoMgrDlg �Ի���
class CGxxGmGoVideoMgrDlg : public CDialog
{
// ����
public:
	CGxxGmGoVideoMgrDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GXXGMGOVIDEOMGR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_cTabMain;

public:
	GxxGmGoVideo govideo_;

public:
	Poco::AutoPtr<Poco::Util::IniFileConfiguration> ini_cfg_;

public:
	CGxxGmGoVideoLoginDlg login_dlg_;
	CGxxGmDevMgrDlg dev_mgr_dlg_;
	CGxxGmServiceStateDlg service_state_dlg_;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
};

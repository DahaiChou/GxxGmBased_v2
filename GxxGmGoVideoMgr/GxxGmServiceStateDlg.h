#pragma once

#include "..\GxxGmGoVideo\GxxGmGoVideo.h"
#include "afxcmn.h"

// CGxxGmServiceStateDlg �Ի���

class CGxxGmServiceStateDlg : public CDialog
{
	DECLARE_DYNAMIC(CGxxGmServiceStateDlg)

public:
	CGxxGmServiceStateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGxxGmServiceStateDlg();

// �Ի�������
	enum { IDD = IDD_CHILDDLG_SERVICESTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	GxxGmGoVideo *govideo_;

public:
	CListCtrl m_cRunningServiceList;
	afx_msg void OnBnClickedBtnRefreshOnlineservice();
	afx_msg void OnLvnKeydownListServices(NMHDR *pNMHDR, LRESULT *pResult);
};

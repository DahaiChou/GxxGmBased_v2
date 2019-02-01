#pragma once

#include "..\GxxGmGoVideo\GxxGmGoVideo.h"
#include "afxcmn.h"
#include "afxwin.h"

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
	afx_msg void OnBnClickedBtnRegisterService();
	CComboBox m_cServType;
	CEdit m_cServName;
	CEdit m_cServVersion;
	CEdit m_cServDetail;
	CEdit m_cServIP;
	CEdit m_cServPort;
	CEdit m_cUsername;
	CEdit m_cPassword;
	CEdit m_cServGBCode;
	CEdit m_cServLicense;
	CComboBox m_cIsDNS;
	afx_msg void OnNMClickListServices(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_cServID;
	afx_msg void OnBnClickedBtnModifyService();
};

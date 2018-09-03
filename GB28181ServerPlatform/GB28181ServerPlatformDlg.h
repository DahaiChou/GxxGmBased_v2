
// GB28181ServerPlatformDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

class GB28181ServerSignalObject;


// CGB28181ServerPlatformDlg �Ի���
class CGB28181ServerPlatformDlg : public CDialog
{
// ����
public:
	CGB28181ServerPlatformDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GB28181SERVERPLATFORM_DIALOG };

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
	GB28181ServerSignalObject *object_;

public:
	CEdit m_cServerIp;
	CEdit m_cServerPort;
	CEdit m_ccServerGbcode;
	CEdit m_cServerUsername;
	CEdit m_cServerPassword;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
};


// GxxGmCDBurnDemoExDlg.h : ͷ�ļ�
//

#pragma once
#include "cdrecordocxctrl1.h"
#include "afxwin.h"


// CGxxGmCDBurnDemoExDlg �Ի���
class CGxxGmCDBurnDemoExDlg : public CDialog
{
// ����
public:
	CGxxGmCDBurnDemoExDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GXXGMCDBURNDEMOEX_DIALOG };

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
	CCdrecordocxctrl1 m_cPlugin;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnBnClickedBtnInstallCallback();
	afx_msg void OnBnClickedBtnFindDevice();
	afx_msg void OnBnClickedBtnBurn();
	CListBox m_cState;
	afx_msg void OnBnClickedBtnCheckCd();
};

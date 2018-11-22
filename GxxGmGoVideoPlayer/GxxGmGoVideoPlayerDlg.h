
// GxxGmGoVideoPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "GxxGmGoVideo.h"


// CGxxGmGoVideoPlayerDlg �Ի���
class CGxxGmGoVideoPlayerDlg : public CDialog
{
// ����
public:
	CGxxGmGoVideoPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GXXGMGOVIDEOPLAYER_DIALOG };

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
	CEdit m_cCmsIp;
	CEdit m_cCmsPort;
	CListCtrl m_cOnlineDevicesList;
	CListBox m_cState;
	CEdit m_cVideoURL;

public:
	GxxGmGoVideo govideo;

public:
	afx_msg void OnBnClickedBtnLoginCms();
	afx_msg void OnBnClickedBtnUpdateOnlineDevices();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnNMDblclkListOnlineDevices(NMHDR *pNMHDR, LRESULT *pResult);
	
};


// GxxGmPlayerDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "GxxGmGoVideo.h"
#include "GSPlaySDK.h"
#include "afxcmn.h"


// CGxxGmPlayerDemoDlg �Ի���
class CGxxGmPlayerDemoDlg : public CDialog
{
// ����
public:
	CGxxGmPlayerDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GXXGMPLAYERDEMO_DIALOG };

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
	CEdit m_cGoVideoIP;
	CEdit m_cGoVideoPort;
	CEdit m_cVideoURL;
	//CListBox m_cOnlineDevices;
	CListBox m_cState;
	CListCtrl m_cOnlineDevices;

public:
	GxxGmGoVideo govideo;

public:
	GSPlayHandle play_handle_;
	static void CALLBACK _PFuncRawDataCB(StruGSPlayFrameData* pstMediaFrameData, void* pUser);
	static void CALLBACK _PFuncDisplayCB(StruGSPlayFrameData* pstMediaFrameData, void* pUser);
	static void CALLBACK _PFuncDrawCB(PDC pdc, void* pUser);
	static void CALLBACK _PFuncMediaSatusCB(EnumMediaStatus eStatus, void* pStatusData, void* pUser);

public:
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnGetOnlineDevice();
	afx_msg void OnNMDblclkListOnlineDev(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnOpenUrl();
	afx_msg void OnBnClickedBtnCloseUrl();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnOpenSound();
	afx_msg void OnBnClickedBtnCloseSound();
	afx_msg void OnTRBNThumbPosChangingSliderVolume(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	CSliderCtrl m_cVolume;
};

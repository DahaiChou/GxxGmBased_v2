
// GxxGmGoVideoPlayerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "GxxGmGoVideo.h"
#include "vlc/vlc.h"


// CGxxGmGoVideoPlayerDlg 对话框
class CGxxGmGoVideoPlayerDlg : public CDialog
{
// 构造
public:
	CGxxGmGoVideoPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GXXGMGOVIDEOPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	libvlc_instance_t		*m_vlcInst;
	libvlc_media_player_t	*m_vlcMplay;
	libvlc_media_t			*m_vlcMedia;

public:
	afx_msg void OnBnClickedBtnLoginCms();
	afx_msg void OnBnClickedBtnUpdateOnlineDevices();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnNMDblclkListOnlineDevices(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedBtnStreamAnalyze();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnGxxProtocolStack();
};

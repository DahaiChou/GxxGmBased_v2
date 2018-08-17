
// GB28181DeviceSimulaterDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "GxxGmInputDevice.h"

#include "GB28181AgentSignalObject.h"
#include "RtpServerObject.h"


// CGB28181DeviceSimulaterDlg �Ի���
class CGB28181DeviceSimulaterDlg : public CDialog
{
// ����
public:
	CGB28181DeviceSimulaterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GB28181DEVICESIMULATER_DIALOG };

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
	GxxGmInputDevice input_device_;
	
public:
	GB28181AgentSignalObject agent_object_;
	RtpServerObject rtp_server_obj_;

public:
	void SetStatus(const TCHAR *status);

public:
	CEdit m_cLocalIp;
	CEdit m_cLocalPort;
	CEdit m_cLocalGbcode;
	CEdit m_cGatewayIp;
	CEdit m_cGatewayPort;
	CEdit m_cGatewayGbcode;
	CEdit m_cGatewayUsername;
	CEdit m_cGatewayPassword;
	CButton m_cIsSendHeartbeat;
	CEdit m_cHeartbeatTime;
	CEdit m_cLongtitude;
	CEdit m_cLatitude;
	CComboBox m_cVideoInputDevices;
	CEdit m_cVideoBitrate;
	CEdit m_cVideoFramerate;
	CComboBox m_cAudioInputDevices;
	CEdit m_cAudioBitrate;
	CEdit m_cAudioSamplerate;
	CEdit m_cAudioChannels;
	CEdit m_cRecordPath;
	CEdit m_cStatus;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnRegister();
	afx_msg void OnBnClickedBtnUnregister();
	CButton m_cIsManualRtpPort;
	CEdit m_cRtpPortBegin;
	CEdit m_cRtpPortFinish;
};

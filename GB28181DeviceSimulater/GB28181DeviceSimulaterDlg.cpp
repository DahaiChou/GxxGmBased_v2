
// GB28181DeviceSimulaterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GB28181DeviceSimulater.h"
#include "GB28181DeviceSimulaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGB28181DeviceSimulaterDlg �Ի���




CGB28181DeviceSimulaterDlg::CGB28181DeviceSimulaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGB28181DeviceSimulaterDlg::IDD, pParent)
	//, handle_(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGB28181DeviceSimulaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOCAL_IP, m_cLocalIp);
	DDX_Control(pDX, IDC_EDIT_LOCAL_PORT, m_cLocalPort);
	DDX_Control(pDX, IDC_EDIT_LOCAL_GBCODE, m_cLocalGbcode);
	DDX_Control(pDX, IDC_EDIT_GATEWAY_IP, m_cGatewayIp);
	DDX_Control(pDX, IDC_EDIT_, m_cGatewayPort);
	DDX_Control(pDX, IDC_EDIT6, m_cGatewayGbcode);
	DDX_Control(pDX, IDC_EDIT7, m_cGatewayUsername);
	DDX_Control(pDX, IDC_EDIT8, m_cGatewayPassword);
	DDX_Control(pDX, IDC_CHECK_IS_SEND_HEARTBEAT, m_cIsSendHeartbeat);
	DDX_Control(pDX, IDC_EDIT_HEARTBEAT_TIME, m_cHeartbeatTime);
	DDX_Control(pDX, IDC_EDIT_LONGTITUDE, m_cLongtitude);
	DDX_Control(pDX, IDC_EDIT_LATITUDE, m_cLatitude);
	DDX_Control(pDX, IDC_COMBO_VIDEO_DEVICES, m_cVideoInputDevices);
	DDX_Control(pDX, IDC_EDIT_VIDEO_BITRATE, m_cVideoBitrate);
	DDX_Control(pDX, IDC_EDIT_VIDEO_FRAMERATE, m_cVideoFramerate);
	DDX_Control(pDX, IDC_COMBO_AUDIO_DEVICES, m_cAudioInputDevices);
	DDX_Control(pDX, IDC_EDIT_AUDIO_BITRATE, m_cAudioBitrate);
	DDX_Control(pDX, IDC_EDIT_AUDIO_SAMPLERATE, m_cAudioSamplerate);
	DDX_Control(pDX, IDC_EDIT_AUDIO_CHANNELS, m_cAudioChannels);
	DDX_Control(pDX, IDC_EDIT2, m_cRecordPath);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_cStatus);
	DDX_Control(pDX, IDC_CHECK_MANUAL_PORT, m_cIsManualRtpPort);
	DDX_Control(pDX, IDC_EDIT_RTP_PORT_BEGIN, m_cRtpPortBegin);
	DDX_Control(pDX, IDC_EDIT_RTP_PORT_FINISH, m_cRtpPortFinish);
}

BEGIN_MESSAGE_MAP(CGB28181DeviceSimulaterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CGB28181DeviceSimulaterDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CGB28181DeviceSimulaterDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CGB28181DeviceSimulaterDlg::OnBnClickedBtnRegister)
	ON_BN_CLICKED(IDC_BTN_UNREGISTER, &CGB28181DeviceSimulaterDlg::OnBnClickedBtnUnregister)
END_MESSAGE_MAP()


// CGB28181DeviceSimulaterDlg ��Ϣ�������

BOOL CGB28181DeviceSimulaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//////////////////////////////////////////////////////////////////////////
	//
	//
	TCHAR local_ip[4096] = {0};
	TCHAR local_port[4096] = {0};
	TCHAR local_gbcode[4096] = {0};

	GetPrivateProfileString(_T("CONNECTION"), _T("LOCAL_IP"), _T("127.0.0.1"), local_ip, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("CONNECTION"), _T("LOCAL_PORT"), _T("5090"), local_port, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("CONNECTION"), _T("LOCAL_GBCODE"), _T("00000000000000000000"), local_gbcode, 4096, _T("config.ini"));

	TCHAR gateway_ip[4096] = {0};
	TCHAR gateway_port[4096] = {0};
	TCHAR gateway_gbcode[4096] = {0};
	TCHAR gateway_username[4096] = {0};
	TCHAR gateway_password[4096] = {0};

	GetPrivateProfileString(_T("CONNECTION"), _T("GATEWAY_IP"), _T("127.0.0.1"), gateway_ip, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("CONNECTION"), _T("GATEWAY_PORT"), _T("5060"), gateway_port, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("CONNECTION"), _T("GATEWAY_GBCODE"), _T("00000000000000000000"), gateway_gbcode, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("CONNECTION"), _T("GATEWAY_USERNAME"), _T("admin"), gateway_username, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("CONNECTION"), _T("GATEWAY_PASSWORD"), _T("12345678"), gateway_password, 4096, _T("config.ini"));

	m_cLocalIp.SetWindowText(local_ip);
	m_cLocalPort.SetWindowText(local_port);
	m_cLocalGbcode.SetWindowText(local_gbcode);

	m_cGatewayIp.SetWindowText(gateway_ip);
	m_cGatewayPort.SetWindowText(gateway_port);
	m_cGatewayGbcode.SetWindowText(gateway_gbcode);
	m_cGatewayUsername.SetWindowText(gateway_username);
	m_cGatewayPassword.SetWindowText(gateway_password);

	int is_send_heartbeat = 0;
	TCHAR heartbeat_time[4096] = {0};
	TCHAR latitude[4096] = {0};
	TCHAR longtitude[4096] = {0};

	// 23.1370351056,113.27692949358
	is_send_heartbeat = GetPrivateProfileInt(_T("DEVICE_INFO"), _T("HEARTBEAT"), 1, _T("config"));
	GetPrivateProfileString(_T("DEVICE_INFO"), _T("HEARTBEAT_TIME"), _T("30000"), heartbeat_time, 4096, _T("config"));
	GetPrivateProfileString(_T("DEVICE_INFO"), _T("LONGTITUDE"), _T("23.1370351056"), latitude, 4096, _T("config"));
	GetPrivateProfileString(_T("DEVICE_INFO"), _T("LATITUDE"), _T("113.27692949358"), longtitude, 4096, _T("config"));

	m_cIsSendHeartbeat.SetCheck(is_send_heartbeat);
	m_cHeartbeatTime.SetWindowText(heartbeat_time);
	m_cLatitude.SetWindowText(latitude);
	m_cLongtitude.SetWindowText(longtitude);
	//
	//
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//
	// ö�ٵ�ǰϵͳ������Դ
	input_device_.EnumDevices();

	std::map<int, TDeviceName>::iterator iter;
	for (iter = input_device_.video_devices.begin(); iter != input_device_.video_devices.end(); ++iter)
	{
		m_cVideoInputDevices.AddString(iter->second.FriendlyName);
	}
	m_cVideoInputDevices.SetCurSel(0);

	for (iter = input_device_.audio_devices.begin(); iter != input_device_.audio_devices.end(); ++iter)
	{
		m_cAudioInputDevices.AddString(iter->second.FriendlyName);
	}
	m_cAudioInputDevices.SetCurSel(0);
	//
	//////////////////////////////////////////////////////////////////////////

	TCHAR rtp_port_mode[4096] = {0};
	TCHAR rtp_port_begin[4096] = {0};
	TCHAR rtp_port_finish[4096] = {0};

	GetPrivateProfileString(_T("RTP"), _T("RTP_PORT_MODE"), _T("1"), rtp_port_mode, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("RTP"), _T("RTP_PORT_BEGIN"), _T("15000"), rtp_port_begin, 4096, _T("config.ini"));
	GetPrivateProfileString(_T("RTP"), _T("RTP_PORT_FINISH"), _T("15100"), rtp_port_finish, 4096, _T("config.ini"));

	if (_tcscmp(rtp_port_mode, _T("1")) == 0)
		m_cIsManualRtpPort.SetCheck(1);
	else
		m_cIsManualRtpPort.SetCheck(0);

	m_cRtpPortBegin.SetWindowText(rtp_port_begin);
	m_cRtpPortFinish.SetWindowText(rtp_port_finish);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGB28181DeviceSimulaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGB28181DeviceSimulaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGB28181DeviceSimulaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGB28181DeviceSimulaterDlg::SetStatus(const TCHAR *status)
{
	CString history;
	m_cStatus.GetWindowText(history);

	if (!history.IsEmpty())
		history += _T("\r\n");
	
	history += status;
	m_cStatus.SetWindowText(history);
}

void CGB28181DeviceSimulaterDlg::OnBnClickedBtnStart()
{
	TCHAR status[4096] = {0};

	CString local_ip;
	CString local_port;
	CString local_gbcode;

	m_cLocalIp.GetWindowText(local_ip);
	m_cLocalPort.GetWindowText(local_port);
	m_cLocalGbcode.GetWindowText(local_gbcode);

	USES_CONVERSION;
	agent_object_.simulate_device_->SetDeviceGbcode(T2A(local_gbcode.GetBuffer(0)));
	agent_object_.simulate_device_->SetVideoGbcode(T2A(local_gbcode.GetBuffer(0)));

	int errCode = agent_object_.Start(T2A(local_ip.GetBuffer(0)), _ttoi(local_port.GetBuffer(0)), T2A(local_gbcode.GetBuffer(0)));
	if (errCode != 0)
	{
		_stprintf_s(status, 4096, _T("����28181Э��ջʧ�ܣ������룺%d"), errCode);
		SetStatus(status);
	}
	SetStatus(_T("����28181Э��ջ�ɹ���"));

	int is_manual_rtp_port = m_cIsManualRtpPort.GetCheck();

	CString str_rtp_port_begin;
	CString str_rtp_port_finish;

	m_cRtpPortBegin.GetWindowText(str_rtp_port_begin);
	m_cRtpPortFinish.GetWindowText(str_rtp_port_finish);

	int rtp_port_begin = _ttoi(str_rtp_port_begin.GetBuffer(0));
	int rtp_port_finish = _ttoi(str_rtp_port_finish.GetBuffer(0));
	errCode = rtp_server_obj_.Start(is_manual_rtp_port, rtp_port_begin, rtp_port_finish, T2A(local_ip.GetBuffer(0)));
	if (errCode != 0)
	{
		_stprintf_s(status, 4096, _T("��ʼ��RTP������ʧ�ܣ������룺%d"), errCode);
		SetStatus(status);
	}
	SetStatus(_T("��ʼ��RTP�������ɹ���"));

	// ��ʼ�����ӵ�����ͷ
	CString video_device_name;
	CString audio_device_name;
	m_cVideoInputDevices.GetWindowText(video_device_name);
	m_cAudioInputDevices.GetWindowText(audio_device_name);

	if (!video_device_name.IsEmpty())
	{
		agent_object_.simulate_device_->SetVideoDeviceName(T2A(video_device_name.GetBuffer(0)));
	}
}

void CGB28181DeviceSimulaterDlg::OnBnClickedBtnStop()
{
	agent_object_.Stop();
	SetStatus(_T("��ֹͣ28181Э��ջ��"));
}

void CGB28181DeviceSimulaterDlg::OnBnClickedBtnRegister()
{
	CString local_ip;
	CString local_port;
	CString local_gbcode;

	m_cLocalIp.GetWindowText(local_ip);
	m_cLocalPort.GetWindowText(local_port);
	m_cLocalGbcode.GetWindowText(local_gbcode);

	CString gateway_ip;
	CString gateway_port;
	CString gateway_gbcode;
	CString gateway_username;
	CString gateway_password;

	m_cGatewayIp.GetWindowText(gateway_ip);
	m_cGatewayPort.GetWindowText(gateway_port);
	m_cGatewayGbcode.GetWindowText(gateway_gbcode);
	m_cGatewayUsername.GetWindowText(gateway_username);
	m_cGatewayPassword.GetWindowText(gateway_password);

	USES_CONVERSION;
	TCHAR status[4096] = {0};
	int errCode = agent_object_.Register(T2A(gateway_ip.GetBuffer(0)), _ttoi(gateway_port.GetBuffer(0)), T2A(gateway_gbcode.GetBuffer(0)), T2A(gateway_username.GetBuffer(0)), T2A(gateway_password.GetBuffer(0)));
	if (errCode != 0)
	{
		_stprintf_s(status, 4096, _T("ע�ᵽ��������ʧ�ܣ������룺%d"), errCode);
		SetStatus(status);
		return ;
	}
	else
	{
		SetStatus(_T("ע�ᵽ�������سɹ���"));

		// ���������ļ�
		int errCode = 0;
		WritePrivateProfileString(_T("CONNECTION"), _T("LOCAL_IP"), local_ip.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();
		WritePrivateProfileString(_T("CONNECTION"), _T("LOCAL_PORT"), local_port.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();
		WritePrivateProfileString(_T("CONNECTION"), _T("LOCAL_GBCODE"), local_gbcode.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();

		WritePrivateProfileString(_T("CONNECTION"), _T("GATEWAY_IP"), gateway_ip.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();
		WritePrivateProfileString(_T("CONNECTION"), _T("GATEWAY_PORT"), gateway_port.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();
		WritePrivateProfileString(_T("CONNECTION"), _T("GATEWAY_GBCODE"), gateway_gbcode.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();
		WritePrivateProfileString(_T("CONNECTION"), _T("GATEWAY_USERNAME"), gateway_username.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();
		WritePrivateProfileString(_T("CONNECTION"), _T("GATEWAY_PASSWORD"), gateway_password.GetBuffer(0), _T("config.ini"));
		errCode = GetLastError();
	}
}

void CGB28181DeviceSimulaterDlg::OnBnClickedBtnUnregister()
{
	agent_object_.UnRegister();
	SetStatus(_T("ע���ɹ���"));
}

//////////////////////////////////////////////////////////////////////////
//

//SIP_REPSOND_CODE CGB28181DeviceSimulaterDlg::_StreamRequestCB(STREAM_HANDLE hStream, const char * czSrvGBCode, EnumStreamRequest eRequest,
//										 StruMediaInfo * pInMedia, const StruStreamDescription * pDescri, void * pUserData)
//{
//	CGB28181DeviceSimulaterDlg *dlg = (CGB28181DeviceSimulaterDlg *)pUserData;
//
//	// �ȷ�����������Ϣ
//	StruMediaInfo local_media;
//	if (pInMedia)
//	{
//		// ����Ĵ���Ӧ���ر�ע�⣬Ӧ������Ϊ���ز������ظ��ϼ�ƽ̨��
//		// ������һЩ����Ӧ�ý���ת��
//		EnumStreamRequest eTargetRequest = eRequest;
//		ZeroMemory(&local_media, sizeof(local_media));
//
//		// ����Ŀ���豸ID
//		strncpy(local_media.czDeviceID, pInMedia->czDeviceID, STR_GBCODE_LEN);
//		// ��������RTP�������IP��ַ
//		strncpy(local_media.czIP, pInMedia->czIP, STR_IPADDRESS_LEN);
//
//		// �����ֵʱ��Э������������������Play/Playback/download/Talk��
//		strncpy(local_media.czMediaName, pInMedia->czMediaName, STR_NAME_LEN);
//
//		// �Ƿ�֧��RTCP
//		local_media.bEnableRTCP = pInMedia->bEnableRTCP;
//
//		// ���䷽ʽ������Ҫת���ɱ��صģ�����������Ҫ��Rtp���ͷ�ת
//		if (pInMedia->eRtpType == eRTP_OVER_UDP)
//		{
//			// RTP over UDP����
//			local_media.eRtpType = eRTP_OVER_UDP;
//		}
//		else if (pInMedia->eRtpType == eRTP_OVER_TCP_ACTIVE)
//		{
//			// Զ����RTP over TCP��������
//			// ���������Ӧ���Ǳ�������
//			local_media.eRtpType = eRTP_OVER_TCP_PASSIVE;
//		}
//		else if (pInMedia->eRtpType = eRTP_OVER_TCP_PASSIVE)
//		{
//			// RTP over TCP��������
//			local_media.eRtpType = eRTP_OVER_TCP_ACTIVE;
//		}
//
//		//��������  1:������  2:������
//		local_media.iStreamType = pInMedia->iStreamType;
//
//		// ��ʼUNIXʱ���
//		local_media.iStart = pInMedia->iStart;
//		// ����UNIXʱ���
//		local_media.iEnd = pInMedia->iEnd;
//
//		//�����������ʱ������ɲ���
//		//Ϊ��֤SSRC�������ԣ�����ͳһ�о���Ҫôÿ�β��Ҫôÿ�ζ���
//		local_media.iSSRC = pInMedia->iSSRC;
//
//		local_media.stuDescri.iDescriNum = pInMedia->stuDescri.iDescriNum;
//		for (int index = 0; index < local_media.stuDescri.iDescriNum; ++index)
//		{
//			local_media.stuDescri.mapDescri[index].eMediaType = pInMedia->stuDescri.mapDescri[index].eMediaType;
//			local_media.stuDescri.mapDescri[index].iRtpmapNum = pInMedia->stuDescri.mapDescri[index].iRtpmapNum;
//			for (int index2 = 0; index2 < local_media.stuDescri.mapDescri[index].iRtpmapNum; ++index2)
//			{
//				strncpy(local_media.stuDescri.mapDescri[index].mapRtp[index2].czMimeType, pInMedia->stuDescri.mapDescri[index].mapRtp[index2].czMimeType, STR_RTPTYPENAME_LEN);
//				local_media.stuDescri.mapDescri[index].mapRtp[index2].iMediaFormat = pInMedia->stuDescri.mapDescri[index].mapRtp[index2].iMediaFormat;
//				local_media.stuDescri.mapDescri[index].mapRtp[index2].iSampleRate = pInMedia->stuDescri.mapDescri[index].mapRtp[index2].iSampleRate;
//			}
//		}
//	}
//
//	if (eRequest == eSTREAM_REALPLAY)
//	{
//		// ʵʱ��Ƶ
//		dlg->real_stream_mgr_.AddStream(pInMedia->czDeviceID, hStream, *pInMedia, local_media);
//	}
//	else if (eRequest == eSTREAM_BYE)
//	{
//		// �ر���
//	}
//	else if (eRequest == eSTREAM_AUDIO)
//	{
//		// ������Ƶ
//	}
//	else if (eRequest == eSTREAM_DOWNLOAD)
//	{
//		// ��ʷ��Ƶ����
//	}
//	else if (eRequest == eSTREAM_PLAYBACK)
//	{
//		// ��ʷ��Ƶ�ط�
//	}
//
//	return SIP_RESPONSE_CODE_SUCCESS;
//}
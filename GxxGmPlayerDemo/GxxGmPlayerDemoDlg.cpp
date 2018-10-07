
// GxxGmPlayerDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GxxGmPlayerDemo.h"
#include "GxxGmPlayerDemoDlg.h"

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


// CGxxGmPlayerDemoDlg �Ի���




CGxxGmPlayerDemoDlg::CGxxGmPlayerDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxxGmPlayerDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxxGmPlayerDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GOVIDEO_IP, m_cGoVideoIP);
	DDX_Control(pDX, IDC_EDIT_GOVIDEO_PORT, m_cGoVideoPort);
	DDX_Control(pDX, IDC_EDIT_URL, m_cVideoURL);
	//DDX_Control(pDX, IDC_LIST_ONLINE_DEV, m_cOnlineDevices);
	DDX_Control(pDX, IDC_LIST_STATE, m_cState);
	DDX_Control(pDX, IDC_LIST_ONLINE_DEV, m_cOnlineDevices);
}

BEGIN_MESSAGE_MAP(CGxxGmPlayerDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CGxxGmPlayerDemoDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_GET_ONLINE_DEVICE, &CGxxGmPlayerDemoDlg::OnBnClickedBtnGetOnlineDevice)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ONLINE_DEV, &CGxxGmPlayerDemoDlg::OnNMDblclkListOnlineDev)
END_MESSAGE_MAP()


// CGxxGmPlayerDemoDlg ��Ϣ�������

BOOL CGxxGmPlayerDemoDlg::OnInitDialog()
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
	m_cOnlineDevices.SetExtendedStyle(m_cOnlineDevices.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cOnlineDevices.InsertColumn(0, _T("�豸�ڲ�����"), LVCFMT_LEFT, 100);
	m_cOnlineDevices.InsertColumn(1, _T("�豸�������"), LVCFMT_LEFT, 150);

	m_cGoVideoIP.SetWindowText(_T("10.10.9.176"));
	m_cGoVideoPort.SetWindowText(_T("99"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGxxGmPlayerDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxxGmPlayerDemoDlg::OnPaint()
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
HCURSOR CGxxGmPlayerDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGxxGmPlayerDemoDlg::OnBnClickedBtnLogin()
{
	TCHAR msg[4096] = {0};
	CString str_govideo_ip;
	CString str_govideo_port;

	m_cGoVideoIP.GetWindowText(str_govideo_ip);
	m_cGoVideoPort.GetWindowText(str_govideo_port);

	USES_CONVERSION;
	int errCode = govideo.Login(T2A(str_govideo_ip.GetBuffer(0)), _ttoi(str_govideo_port.GetBuffer(0)));
	if (errCode != 0)
	{
		_stprintf_s(msg, _T("��¼��GoVideo[%s:%s]ʧ�ܣ������룺%d"), str_govideo_ip.GetBuffer(0), str_govideo_port.GetBuffer(0), errCode);
		m_cState.AddString(msg);
	}
	else
	{
		_stprintf_s(msg, _T("��¼��GoVideo[%s:%s]�ɹ��������룺%d"), str_govideo_ip.GetBuffer(0), str_govideo_port.GetBuffer(0), errCode);
		m_cState.AddString(msg);
	}
}

void CGxxGmPlayerDemoDlg::OnBnClickedBtnGetOnlineDevice()
{
	TCHAR msg[4096] = {0};

	// ��ȡ�����豸��Ϣ
	int errCode = govideo.GetAllDevices();
	if (errCode != 0)
	{
		_stprintf_s(msg, _T("��ȡ�����豸��Ϣʧ�ܣ������룺%d"), errCode);
		m_cState.AddString(msg);
		return ;
	}

	// ��ȡ�����豸״̬��Ϣ
	errCode = govideo.GetAllDeviceStatus();
	if (errCode != 0)
	{
		_stprintf_s(msg, _T("��ȡ�����豸״̬��Ϣʧ�ܣ������룺%d"), errCode);
		m_cState.AddString(msg);
		return ;
	}

	// ��ɾ�����������豸��Ϣ
	m_cOnlineDevices.DeleteAllItems();

	// ���������豸����״̬��Ϣ
	std::vector<GOVIDEO_DEVICE_STATUS>::iterator iter;
	for (iter = govideo.devices_status_.begin(); iter != govideo.devices_status_.end(); ++iter)
	{
		GOVIDEO_DEVICE_STATUS *dev_status = &(*iter);
		if (dev_status->status_ == 0)
		{
			// �豸���ߣ����豸ID��ӡ����������ʹ��CViewCtrl��ȽϺ�һ��
			// ��Ϊ��ʱ�����ʵ��ʹ�õĲ��ǹ����룬��GoVideo���ڲ�����
			int index = m_cOnlineDevices.GetItemCount();

			USES_CONVERSION;
			TCHAR internal_id[4096] = {0};
			_stprintf_s(internal_id, 4096, _T("%d"), dev_status->device_id_);
			m_cOnlineDevices.InsertItem(index, internal_id);
			m_cOnlineDevices.SetItemText(index, 1, A2T(dev_status->gb28181_code_.c_str()));
		}
	}
}

void CGxxGmPlayerDemoDlg::OnNMDblclkListOnlineDev(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	TCHAR msg[4096] = {0};
	
	// �ҵ���ǰѡ�е�
	POSITION pos = m_cOnlineDevices.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return ;

	TCHAR internal_index[4096] = {0};
	TCHAR device_gbcode[4096] = {0};
	int select_index = m_cOnlineDevices.GetNextSelectedItem(pos);
	m_cOnlineDevices.GetItemText(select_index, 0, internal_index, 4096);
	m_cOnlineDevices.GetItemText(select_index, 1, device_gbcode, 4096);

	int internal_id = _ttoi(internal_index);

	// ������������ȡURL
	std::string rtsp_url;
	int errCode = govideo.GetRealStream(internal_id, rtsp_url);
	if (errCode == 0)
	{
		_stprintf_s(msg, _T("[%s]����ʧ�ܣ��������룺%d"), device_gbcode, errCode);
		m_cState.AddString(msg);
		return ;
	}
	
	USES_CONVERSION;
	m_cVideoURL.SetWindowText(A2T(rtsp_url.c_str()));

	*pResult = 0;
}

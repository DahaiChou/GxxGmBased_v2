
// GxxGmGoVideoPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GxxGmGoVideoPlayer.h"
#include "GxxGmGoVideoPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CGxxGmGoVideoPlayerDlg 对话框




CGxxGmGoVideoPlayerDlg::CGxxGmGoVideoPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxxGmGoVideoPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxxGmGoVideoPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CMS_IP, m_cCmsIp);
	DDX_Control(pDX, IDC_EDIT_CMS_PORT, m_cCmsPort);
	DDX_Control(pDX, IDC_LIST_ONLINE_DEVICES, m_cOnlineDevicesList);
	DDX_Control(pDX, IDC_LIST_STATE, m_cState);
	DDX_Control(pDX, IDC_EDIT_STREAM_URL, m_cVideoURL);
}

BEGIN_MESSAGE_MAP(CGxxGmGoVideoPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_LOGIN_CMS, &CGxxGmGoVideoPlayerDlg::OnBnClickedBtnLoginCms)
	ON_BN_CLICKED(IDC_BTN_UPDATE_ONLINE_DEVICES, &CGxxGmGoVideoPlayerDlg::OnBnClickedBtnUpdateOnlineDevices)
	ON_BN_CLICKED(IDC_BTN_STOP, &CGxxGmGoVideoPlayerDlg::OnBnClickedBtnStop)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ONLINE_DEVICES, &CGxxGmGoVideoPlayerDlg::OnNMDblclkListOnlineDevices)
END_MESSAGE_MAP()


// CGxxGmGoVideoPlayerDlg 消息处理程序

BOOL CGxxGmGoVideoPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->SetWindowText(_T("高新兴国迈 GoVideo播放器 For RTMP流 V1.0"));

	m_cOnlineDevicesList.SetExtendedStyle(m_cOnlineDevicesList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cOnlineDevicesList.InsertColumn(0, _T("设备内部索引"), LVCFMT_LEFT, 100);
	m_cOnlineDevicesList.InsertColumn(1, _T("设备国标编码"), LVCFMT_LEFT, 180);

	m_cCmsIp.SetWindowText(_T("10.10.9.18"));
	m_cCmsPort.SetWindowText(_T("99"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGxxGmGoVideoPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGxxGmGoVideoPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGxxGmGoVideoPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGxxGmGoVideoPlayerDlg::OnBnClickedBtnLoginCms()
{
	// 登录到GoVideo
	TCHAR msg[4096] = {0};
	CString str_cms_ip;
	CString str_cms_port;

	m_cCmsIp.GetWindowText(str_cms_ip);
	m_cCmsPort.GetWindowText(str_cms_port);

	USES_CONVERSION;
	int errCode = govideo.Login(T2A(str_cms_ip.GetBuffer(0)), _ttoi(str_cms_port.GetBuffer(0)));
	if (errCode != 0)
	{
		_stprintf_s(msg, _T("登录到GoVideo-Cms[%s:%s]失败！错误码：%d"), str_cms_ip.GetBuffer(0), str_cms_port.GetBuffer(0), errCode);
		m_cState.AddString(msg);
	}
	else
	{
		_stprintf_s(msg, _T("登录到GoVideo-Cms[%s:%s]成功！错误码：%d"), str_cms_ip.GetBuffer(0), str_cms_port.GetBuffer(0), errCode);
		m_cState.AddString(msg);
	}
}

void CGxxGmGoVideoPlayerDlg::OnBnClickedBtnUpdateOnlineDevices()
{
	// 更新在线设备列表
	TCHAR msg[4096] = {0};

	// 获取所有设备信息
	int errCode = govideo.GetAllDevices();
	if (errCode != 0)
	{
		_stprintf_s(msg, _T("获取所有设备信息失败！错误码：%d"), errCode);
		m_cState.AddString(msg);
		return ;
	}

	// 获取所有设备状态信息
	errCode = govideo.GetAllDeviceStatus();
	if (errCode != 0)
	{
		_stprintf_s(msg, _T("获取所有设备状态信息失败！错误码：%d"), errCode);
		m_cState.AddString(msg);
		return ;
	}

	// 先删除所有在线设备信息
	m_cOnlineDevicesList.DeleteAllItems();

	// 遍历所有设备在线状态信息
	std::vector<GOVIDEO_DEVICE_STATUS>::iterator iter;
	for (iter = govideo.devices_status_.begin(); iter != govideo.devices_status_.end(); ++iter)
	{
		GOVIDEO_DEVICE_STATUS *dev_status = &(*iter);
		if (dev_status->status_ == 0)
		{
			// 设备在线，将设备ID打印出来，可能使用CViewCtrl会比较好一点
			// 因为到时候点流实际使用的不是国标码，是GoVideo的内部编码
			int index = m_cOnlineDevicesList.GetItemCount();

			USES_CONVERSION;
			TCHAR internal_id[4096] = {0};
			_stprintf_s(internal_id, 4096, _T("%d"), dev_status->device_id_);
			m_cOnlineDevicesList.InsertItem(index, internal_id);
			m_cOnlineDevicesList.SetItemText(index, 1, A2T(dev_status->gb28181_code_.c_str()));
		}
	}
}

void CGxxGmGoVideoPlayerDlg::OnBnClickedBtnStop()
{
	// 停止播放
}

void CGxxGmGoVideoPlayerDlg::OnNMDblclkListOnlineDevices(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	TCHAR msg[4096] = {0};

	// 找到当前选中的
	POSITION pos = m_cOnlineDevicesList.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return ;

	TCHAR internal_index[4096] = {0};
	TCHAR device_gbcode[4096] = {0};
	int select_index = m_cOnlineDevicesList.GetNextSelectedItem(pos);
	m_cOnlineDevicesList.GetItemText(select_index, 0, internal_index, 4096);
	m_cOnlineDevicesList.GetItemText(select_index, 1, device_gbcode, 4096);

	int internal_id = _ttoi(internal_index);

	// 发起点流命令，获取URL
	std::string rtsp_url;
	int errCode = govideo.GetRealStream(internal_id, rtsp_url, enStreamAgentType::StreamAgentRtmp);
	if (errCode != 0)
	{
		_stprintf_s(msg, _T("[%s]点流失败！！错误码：%d"), device_gbcode, errCode);
		m_cState.AddString(msg);
		return ;
	}

	USES_CONVERSION;
	m_cVideoURL.SetWindowText(A2T(rtsp_url.c_str()));

	*pResult = 0;
}

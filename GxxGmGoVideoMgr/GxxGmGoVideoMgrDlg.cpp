
// GxxGmGoVideoMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GxxGmGoVideoMgr.h"
#include "GxxGmGoVideoMgrDlg.h"

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


// CGxxGmGoVideoMgrDlg �Ի���




CGxxGmGoVideoMgrDlg::CGxxGmGoVideoMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxxGmGoVideoMgrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxxGmGoVideoMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_cTabMain);
}

BEGIN_MESSAGE_MAP(CGxxGmGoVideoMgrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CGxxGmGoVideoMgrDlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// CGxxGmGoVideoMgrDlg ��Ϣ�������

BOOL CGxxGmGoVideoMgrDlg::OnInitDialog()
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
	SetWindowText(_T("GoVideo5.0 �豸������ V1.1"));

	login_dlg_.govideo_ = &govideo_;
	service_state_dlg_.govideo_ = &govideo_;
	dev_mgr_dlg_.govideo_ = &govideo_;

	char current_program_path[4096] = {0};
	GetModuleFileNameA(NULL, current_program_path, 4096);
	std::string stl_current_program_path = current_program_path;
	int pos = stl_current_program_path.find_last_of("\\");
	std::string stl_current_dir = stl_current_program_path.substr(0, pos);
	std::string stl_ini_cfg_path = stl_current_dir + "\\GxxGmGoVideoMgrConfig.ini";

	// �������ù�����
	//ini_cfg_ = new Poco::Util::IniFileConfiguration();
	//ini_cfg_->load(stl_ini_cfg_path.c_str());

	login_dlg_.config_path_ = stl_ini_cfg_path;
	//login_dlg_.ini_cfg_ = ini_cfg_;
	INT_PTR ret = login_dlg_.DoModal();
	if (ret == IDCANCEL)
		exit(4);

	// ΪTab Control��������ҳ��
	m_cTabMain.InsertItem(0, _T("����״̬"));
	m_cTabMain.InsertItem(1, _T("�豸����"));

	// ���������Ի���
	service_state_dlg_.Create(IDD_CHILDDLG_SERVICESTATE, &m_cTabMain);
	dev_mgr_dlg_.Create(IDD_CHILDDLG_DEVMGR, &m_cTabMain);

	// �趨��Tab����ʾ�ķ�Χ
	CRect rc;
	m_cTabMain.GetClientRect(rc);
	rc.top += 22;
	rc.bottom -= 2;
	rc.left += 2;
	rc.right -= 2;
	service_state_dlg_.MoveWindow(&rc);
	dev_mgr_dlg_.MoveWindow(&rc);

	// ��ʾ��ʼҳ��
	service_state_dlg_.ShowWindow(SW_SHOW);
	dev_mgr_dlg_.ShowWindow(SW_HIDE);

	// ���浱ǰѡ��
	m_cTabMain.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGxxGmGoVideoMgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxxGmGoVideoMgrDlg::OnPaint()
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
HCURSOR CGxxGmGoVideoMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGxxGmGoVideoMgrDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �õ���ǰ��ǩ����
	int current_tab_index = m_cTabMain.GetCurSel();
	switch (current_tab_index)
	{
	case 0:
		service_state_dlg_.ShowWindow(SW_SHOW);
		dev_mgr_dlg_.ShowWindow(SW_HIDE);
		break;
	case 1:
		service_state_dlg_.ShowWindow(SW_HIDE);
		dev_mgr_dlg_.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}

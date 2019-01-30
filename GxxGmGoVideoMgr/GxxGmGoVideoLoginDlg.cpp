// GxxGmGoVideoLoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GxxGmGoVideoMgr.h"
#include "GxxGmGoVideoLoginDlg.h"


// CGxxGmGoVideoLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CGxxGmGoVideoLoginDlg, CDialog)

CGxxGmGoVideoLoginDlg::CGxxGmGoVideoLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxxGmGoVideoLoginDlg::IDD, pParent)
	, govideo_(NULL)
{

}

CGxxGmGoVideoLoginDlg::~CGxxGmGoVideoLoginDlg()
{
}

void CGxxGmGoVideoLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IP, m_cGoVideoIp);
	DDX_Control(pDX, IDC_EDIT_PWD, m_cGoVideoPort);
}


BEGIN_MESSAGE_MAP(CGxxGmGoVideoLoginDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CGxxGmGoVideoLoginDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CGxxGmGoVideoLoginDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CGxxGmGoVideoLoginDlg ��Ϣ�������
BOOL CGxxGmGoVideoLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_cGoVideoIp.LimitText(16);
	m_cGoVideoPort.LimitText(6);

	m_cGoVideoIp.SetWindowText(_T("192.168.124.128"));
	m_cGoVideoPort.SetWindowText(_T("99"));

	return TRUE;
}

void CGxxGmGoVideoLoginDlg::OnBnClickedBtnLogin()
{
	CString ipaddr;
	m_cGoVideoIp.GetWindowText(ipaddr);

	CString ipport;
	m_cGoVideoPort.GetWindowText(ipport);

	// ���Ե�¼
	USES_CONVERSION;
	int errCode = govideo_->Login(T2A(ipaddr.GetBuffer(0)), _ttoi(ipport.GetString()));
	if (errCode != 0)
		MessageBox(_T("��¼����"), _T("����"), MB_OK|MB_ICONERROR);
	else
		CDialog::OnOK();
}

void CGxxGmGoVideoLoginDlg::OnBnClickedBtnCancel()
{
	CDialog::OnCancel();
}

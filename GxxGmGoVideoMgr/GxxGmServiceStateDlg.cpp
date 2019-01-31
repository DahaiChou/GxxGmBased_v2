// GxxGmServiceStateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GxxGmGoVideoMgr.h"
#include "GxxGmServiceStateDlg.h"


// CGxxGmServiceStateDlg �Ի���

IMPLEMENT_DYNAMIC(CGxxGmServiceStateDlg, CDialog)

CGxxGmServiceStateDlg::CGxxGmServiceStateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxxGmServiceStateDlg::IDD, pParent)
{

}

CGxxGmServiceStateDlg::~CGxxGmServiceStateDlg()
{
}

void CGxxGmServiceStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICES, m_cRunningServiceList);
}


BEGIN_MESSAGE_MAP(CGxxGmServiceStateDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_REFRESH_ONLINESERVICE, &CGxxGmServiceStateDlg::OnBnClickedBtnRefreshOnlineservice)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_SERVICES, &CGxxGmServiceStateDlg::OnLvnKeydownListServices)
END_MESSAGE_MAP()


// CGxxGmServiceStateDlg ��Ϣ�������
BOOL CGxxGmServiceStateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cRunningServiceList.SetExtendedStyle(m_cRunningServiceList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cRunningServiceList.InsertColumn(0, _T("����ID"), LVCFMT_LEFT, 80);
	m_cRunningServiceList.InsertColumn(1, _T("��������"), LVCFMT_LEFT, 60);
	m_cRunningServiceList.InsertColumn(2, _T("��������"), LVCFMT_LEFT, 60);
	m_cRunningServiceList.InsertColumn(3, _T("����˵��"), LVCFMT_LEFT, 80);
	m_cRunningServiceList.InsertColumn(4, _T("IP��ַ"), LVCFMT_LEFT, 100);
	m_cRunningServiceList.InsertColumn(5, _T("�˿ں�"), LVCFMT_LEFT, 60);
	m_cRunningServiceList.InsertColumn(6, _T("�û���"), LVCFMT_LEFT, 60);
	m_cRunningServiceList.InsertColumn(7, _T("����"), LVCFMT_LEFT, 60);
	m_cRunningServiceList.InsertColumn(8, _T("��������"), LVCFMT_LEFT, 60);
	m_cRunningServiceList.InsertColumn(9, _T("�������"), LVCFMT_LEFT, 100);
	m_cRunningServiceList.InsertColumn(10, _T("�汾��"), LVCFMT_LEFT, 80);
	m_cRunningServiceList.InsertColumn(11, _T("�����Ϣ"), LVCFMT_LEFT, 80);

	OnBnClickedBtnRefreshOnlineservice();

	return TRUE;
}

void CGxxGmServiceStateDlg::OnBnClickedBtnRefreshOnlineservice()
{
	USES_CONVERSION;
	int errCode = govideo_->GetDeviceGatewayList();

	m_cRunningServiceList.DeleteAllItems();

	std::vector<GOVIDEO_SERVICE_INFO>::iterator iter;
	for (iter = govideo_->govideo_services_.begin(); iter != govideo_->govideo_services_.end(); ++iter)
	{
		int count = m_cRunningServiceList.GetItemCount();
		m_cRunningServiceList.InsertItem(count, A2T(iter->service_id_.c_str()));
		m_cRunningServiceList.SetItemText(count, 1, A2T(iter->service_name_.c_str()));
		m_cRunningServiceList.SetItemText(count, 2, A2T(iter->service_type_id_.c_str()));
		m_cRunningServiceList.SetItemText(count, 3, A2T(iter->service_descript_.c_str()));
		m_cRunningServiceList.SetItemText(count, 4, A2T(iter->service_ip_.c_str()));
		m_cRunningServiceList.SetItemText(count, 5, A2T(iter->service_port_.c_str()));
		m_cRunningServiceList.SetItemText(count, 6, A2T(iter->username_.c_str()));
		m_cRunningServiceList.SetItemText(count, 7, A2T(iter->password_.c_str()));
		m_cRunningServiceList.SetItemText(count, 8, A2T(iter->is_dns_.c_str()));
		m_cRunningServiceList.SetItemText(count, 9, A2T(iter->gbcode_.c_str()));
		m_cRunningServiceList.SetItemText(count, 10, A2T(iter->version_.c_str()));
		m_cRunningServiceList.SetItemText(count, 11, A2T(iter->license_info_.c_str()));
	}
}

void CGxxGmServiceStateDlg::OnLvnKeydownListServices(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	// �ҵ�ɾ����
	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		INT_PTR ret = MessageBox(_T("ȷ��Ҫɾ��ѡ�еķ�����"), _T("����"), MB_OKCANCEL|MB_ICONMASK);
		if (ret != IDOK)
			return ;

		// �ҵ���ǰѡ�е���
		POSITION pos = m_cRunningServiceList.GetFirstSelectedItemPosition();
		if (pos == NULL)
			return ;

		do 
		{
			int index = m_cRunningServiceList.GetNextSelectedItem(pos);
			if (index != -1)
			{
				// �ȵ���GoVideo��ɾ������ӿڣ��ɹ����ٵ��ý����Ƴ���¼		
				m_cRunningServiceList.DeleteItem(index);
			}
			else
				break;

		} while (true);
	}

	*pResult = 0;
}

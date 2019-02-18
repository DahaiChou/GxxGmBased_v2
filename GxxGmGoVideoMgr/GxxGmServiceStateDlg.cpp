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
	DDX_Control(pDX, IDC_COMBO_SERVICE_TYPE, m_cServType);
	DDX_Control(pDX, IDC_EDIT_SERVICE_NAME, m_cServName);
	DDX_Control(pDX, IDC_EDIT_VERSION, m_cServVersion);
	DDX_Control(pDX, IDC_EDIT_SERVICE_DETAIL, m_cServDetail);
	DDX_Control(pDX, IDC_EDIT_IPADDRESS, m_cServIP);
	DDX_Control(pDX, IDC_EDIT_PORT, m_cServPort);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_cUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_cPassword);
	DDX_Control(pDX, IDC_EDIT_GBCODE, m_cServGBCode);
	DDX_Control(pDX, IDC_EDIT_LICENSE, m_cServLicense);
	DDX_Control(pDX, IDC_COMBO_IS_DNS, m_cIsDNS);
	DDX_Control(pDX, IDC_EDIT_SERV_ID, m_cServID);
}


BEGIN_MESSAGE_MAP(CGxxGmServiceStateDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_REFRESH_ONLINESERVICE, &CGxxGmServiceStateDlg::OnBnClickedBtnRefreshOnlineservice)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_SERVICES, &CGxxGmServiceStateDlg::OnLvnKeydownListServices)
	ON_BN_CLICKED(IDC_BTN_REGISTER_SERVICE, &CGxxGmServiceStateDlg::OnBnClickedBtnRegisterService)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SERVICES, &CGxxGmServiceStateDlg::OnNMClickListServices)
	ON_BN_CLICKED(IDC_BTN_MODIFY_SERVICE, &CGxxGmServiceStateDlg::OnBnClickedBtnModifyService)
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

	m_cIsDNS.AddString(_T("��"));
	m_cIsDNS.AddString(_T("��"));
	m_cIsDNS.SetCurSel(0);
	
	m_cServType.AddString(_T(""));
	m_cServType.AddString(_T("�豸����"));
	m_cServType.AddString(_T("�澯�������"));
	m_cServType.AddString(_T("¼��洢����"));
	m_cServType.AddString(_T("����ǽ�������"));
	m_cServType.AddString(_T("���Ĺ������"));
	m_cServType.AddString(_T("ҵ��������"));
	m_cServType.AddString(_T("��ý�����"));
	m_cServType.AddString(_T("¼��㲥����"));
	m_cServType.AddString(_T("�����Խ�����"));
	m_cServType.AddString(_T("28181�������ط���"));

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
	USES_CONVERSION;
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
				CString serv_id = m_cRunningServiceList.GetItemText(index, 0);
				int errCode = govideo_->UnregisterService(T2A(serv_id.GetBuffer(0)));
				if (errCode != 0)
				{
					MessageBox(_T("�Ƴ�����ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
					return ;
				}

				govideo_->DataUpdate();
				m_cRunningServiceList.DeleteItem(index);
			}
			else
				break;

		} while (true);
	}

	*pResult = 0;
}

void CGxxGmServiceStateDlg::OnBnClickedBtnRegisterService()
{
	CString serv_type;
	m_cServType.GetWindowText(serv_type);
	if (serv_type.IsEmpty())
	{
		MessageBox(_T("δѡ���豸���ͣ�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION);
		return ;
	}

	std::string serv_type_id = "102";
	if (serv_type.CompareNoCase(_T("�豸����")) == 0)
		serv_type_id = "102";
	else if (serv_type.CompareNoCase(_T("�澯�������")) == 0)
		serv_type_id = "105";
	else if (serv_type.CompareNoCase(_T("¼��洢����")) == 0)
		serv_type_id = "106";
	else if (serv_type.CompareNoCase(_T("����ǽ�������")) == 0)
		serv_type_id = "110";
	else if (serv_type.CompareNoCase(_T("���Ĺ������")) == 0)
		serv_type_id = "120";
	else if (serv_type.CompareNoCase(_T("ҵ��������")) == 0)
		serv_type_id = "130";
	else if (serv_type.CompareNoCase(_T("��ý�����")) == 0)
		serv_type_id = "150";
	else if (serv_type.CompareNoCase(_T("¼��㲥����")) == 0)
		serv_type_id = "160";
	else if (serv_type.CompareNoCase(_T("�����Խ�����")) == 0)
		serv_type_id = "170";
	else if (serv_type.CompareNoCase(_T("28181�������ط���")) == 0)
		serv_type_id = "180";

	CString serv_name;
	m_cServName.GetWindowText(serv_name);

	CString serv_version;
	m_cServVersion.GetWindowText(serv_version);

	CString serv_detail;
	m_cServDetail.GetWindowText(serv_detail);

	CString serv_ip;
	m_cServIP.GetWindowText(serv_ip);

	CString serv_port;
	m_cServPort.GetWindowText(serv_port);

	CString serv_username;
	m_cUsername.GetWindowText(serv_username);

	CString serv_password;
	m_cPassword.GetWindowText(serv_password);

	CString serv_gbcode;
	m_cServGBCode.GetWindowText(serv_gbcode);

	CString serv_license;
	m_cServLicense.GetWindowText(serv_license);

	CString serv_isdns;
	m_cIsDNS.GetWindowText(serv_isdns);

	std::string is_dns = "0";
	if (serv_isdns.CompareNoCase(_T("��")) == 0)
		is_dns = "1";

	USES_CONVERSION;
	GOVIDEO_SERVICE_INFO service_info;
	service_info.service_id_ = "0";
	service_info.service_name_ = T2A(serv_name.GetBuffer(0));
	service_info.service_type_id_ = serv_type_id;
	service_info.service_descript_ = T2A(serv_detail.GetBuffer(0));
	service_info.service_ip_ = T2A(serv_ip.GetBuffer(0));
	service_info.service_port_ = T2A(serv_port.GetBuffer(0));
	service_info.username_ = T2A(serv_username.GetBuffer(0));
	service_info.password_ = T2A(serv_password.GetBuffer(0));
	service_info.gbcode_ = T2A(serv_gbcode.GetBuffer(0));
	service_info.license_info_ = T2A(serv_license.GetBuffer(0));
	service_info.is_dns_ = is_dns;

	int errCode = govideo_->RegisterService(service_info);
	if (errCode != 0)
		MessageBox(_T("ע�����ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
	else
	{
		MessageBox(_T("ע�����ɹ���"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION);

		govideo_->DataUpdate();
		
		// ˢ���б�
		OnBnClickedBtnRefreshOnlineservice();
	}
}

void CGxxGmServiceStateDlg::OnNMClickListServices(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int selected_item_index = pNMItemActivate->iItem;
	if (selected_item_index >= 0)
	{
		CString serv_id = m_cRunningServiceList.GetItemText(selected_item_index, 0);

		USES_CONVERSION;
		std::vector<GOVIDEO_SERVICE_INFO>::iterator iter;
		for (iter = govideo_->govideo_services_.begin(); iter != govideo_->govideo_services_.end(); ++iter)
		{
			GOVIDEO_SERVICE_INFO service_info = *iter;

			if (serv_id.CompareNoCase(A2T(service_info.service_id_.c_str())) == 0)
			{
				m_cServID.SetWindowText(serv_id);

				int pos = 0;
				if (service_info.service_type_id_.compare("102") == 0)
					pos = m_cServType.FindString(0, _T("�豸����"));
				else if (service_info.service_type_id_.compare("105") == 0)
					pos = m_cServType.FindString(0, _T("�澯�������"));
				else if (service_info.service_type_id_.compare("106") == 0)
					pos = m_cServType.FindString(0, _T("¼��洢����"));
				else if (service_info.service_type_id_.compare("110") == 0)
					pos = m_cServType.FindString(0, _T("����ǽ�������"));
				else if (service_info.service_type_id_.compare("120") == 0)
					pos = m_cServType.FindString(0, _T("���Ĺ������"));
				else if (service_info.service_type_id_.compare("130") == 0)
					pos = m_cServType.FindString(0, _T("ҵ��������"));
				else if (service_info.service_type_id_.compare("150") == 0)
					pos = m_cServType.FindString(0, _T("��ý�����"));
				else if (service_info.service_type_id_.compare("160") == 0)
					pos = m_cServType.FindString(0, _T("¼��㲥����"));
				else if (service_info.service_type_id_.compare("170") == 0)
					pos = m_cServType.FindString(0, _T("�����Խ�����"));
				else if (service_info.service_type_id_.compare("180") == 0)
					pos = m_cServType.FindString(0, _T("28181�������ط���"));

				if (pos >= 0)
					m_cServType.SetCurSel(pos);
				else
					m_cServType.SetCurSel(0);

				m_cServName.SetWindowText(A2T(service_info.service_name_.c_str()));
				m_cServVersion.SetWindowText(A2T(service_info.version_.c_str()));
				m_cServDetail.SetWindowText(A2T(service_info.service_descript_.c_str()));
				m_cServIP.SetWindowText(A2T(service_info.service_ip_.c_str()));
				m_cServPort.SetWindowText(A2T(service_info.service_port_.c_str()));
				m_cUsername.SetWindowText(A2T(service_info.username_.c_str()));
				m_cPassword.SetWindowText(A2T(service_info.password_.c_str()));
				m_cServGBCode.SetWindowText(A2T(service_info.gbcode_.c_str()));
				m_cServLicense.SetWindowText(A2T(service_info.license_info_.c_str()));
				m_cIsDNS.SetCurSel(0);
			}
		}
	}
	

	*pResult = 0;
}

void CGxxGmServiceStateDlg::OnBnClickedBtnModifyService()
{
	CString serv_type;
	m_cServType.GetWindowText(serv_type);
	if (serv_type.IsEmpty())
	{
		MessageBox(_T("δѡ���豸���ͣ�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION);
		return ;
	}

	std::string serv_type_id = "102";
	if (serv_type.CompareNoCase(_T("�豸����")) == 0)
		serv_type_id = "102";
	else if (serv_type.CompareNoCase(_T("�澯�������")) == 0)
		serv_type_id = "105";
	else if (serv_type.CompareNoCase(_T("¼��洢����")) == 0)
		serv_type_id = "106";
	else if (serv_type.CompareNoCase(_T("����ǽ�������")) == 0)
		serv_type_id = "110";
	else if (serv_type.CompareNoCase(_T("���Ĺ������")) == 0)
		serv_type_id = "120";
	else if (serv_type.CompareNoCase(_T("ҵ��������")) == 0)
		serv_type_id = "130";
	else if (serv_type.CompareNoCase(_T("��ý�����")) == 0)
		serv_type_id = "150";
	else if (serv_type.CompareNoCase(_T("¼��㲥����")) == 0)
		serv_type_id = "160";
	else if (serv_type.CompareNoCase(_T("�����Խ�����")) == 0)
		serv_type_id = "170";
	else if (serv_type.CompareNoCase(_T("28181�������ط���")) == 0)
		serv_type_id = "180";

	CString serv_name;
	m_cServName.GetWindowText(serv_name);

	CString serv_version;
	m_cServVersion.GetWindowText(serv_version);

	CString serv_detail;
	m_cServDetail.GetWindowText(serv_detail);

	CString serv_ip;
	m_cServIP.GetWindowText(serv_ip);

	CString serv_port;
	m_cServPort.GetWindowText(serv_port);

	CString serv_username;
	m_cUsername.GetWindowText(serv_username);

	CString serv_password;
	m_cPassword.GetWindowText(serv_password);

	CString serv_gbcode;
	m_cServGBCode.GetWindowText(serv_gbcode);

	CString serv_license;
	m_cServLicense.GetWindowText(serv_license);

	CString serv_isdns;
	m_cIsDNS.GetWindowText(serv_isdns);

	std::string is_dns = "0";
	if (serv_isdns.CompareNoCase(_T("��")) == 0)
		is_dns = "1";

	CString serv_id;
	m_cServID.GetWindowText(serv_id);

	USES_CONVERSION;
	GOVIDEO_SERVICE_INFO service_info;
	service_info.service_id_ = T2A(serv_id.GetBuffer(0));
	service_info.service_name_ = T2A(serv_name.GetBuffer(0));
	service_info.service_type_id_ = serv_type_id;
	service_info.service_descript_ = T2A(serv_detail.GetBuffer(0));
	service_info.service_ip_ = T2A(serv_ip.GetBuffer(0));
	service_info.service_port_ = T2A(serv_port.GetBuffer(0));
	service_info.username_ = T2A(serv_username.GetBuffer(0));
	service_info.password_ = T2A(serv_password.GetBuffer(0));
	service_info.gbcode_ = T2A(serv_gbcode.GetBuffer(0));
	service_info.license_info_ = T2A(serv_license.GetBuffer(0));
	service_info.is_dns_ = is_dns;

	int errCode = govideo_->ModifyService(service_info);
	if (errCode != 0)
		MessageBox(_T("�޸ķ���ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
	else
	{
		MessageBox(_T("�޸ķ���ɹ���"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION);

		govideo_->DataUpdate();

		// ˢ���б�
		OnBnClickedBtnRefreshOnlineservice();
	}
}

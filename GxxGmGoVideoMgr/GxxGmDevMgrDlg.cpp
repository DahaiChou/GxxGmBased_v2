// GxxGmDevMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GxxGmGoVideoMgr.h"
#include "GxxGmDevMgrDlg.h"


// CGxxGmDevMgrDlg �Ի���

IMPLEMENT_DYNAMIC(CGxxGmDevMgrDlg, CDialog)

CGxxGmDevMgrDlg::CGxxGmDevMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxxGmDevMgrDlg::IDD, pParent)
{

}

CGxxGmDevMgrDlg::~CGxxGmDevMgrDlg()
{
}

void CGxxGmDevMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DGWS, m_cDGWList);
	DDX_Control(pDX, IDC_LIST_DEVS, m_cDevList);
	DDX_Control(pDX, IDC_EDIT_DEVID, m_cDevID);
	DDX_Control(pDX, IDC_EDIT_DEVNAME, m_cDevName);
	DDX_Control(pDX, IDC_COMBO_MODELID, m_cModelID);
	DDX_Control(pDX, IDC_COMBO_CATEGORYID, m_cCategoryID);
	DDX_Control(pDX, IDC_EDIT_CONNECTION_INFO, m_cConnInfo);
	DDX_Control(pDX, IDC_EDIT_CFG_VERSION, m_cCfgVersion);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_cUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_cPassword);
	DDX_Control(pDX, IDC_EDIT_DEVICE_CODE, m_cDevCode);
	DDX_Control(pDX, IDC_EDIT_EXT_INFO, m_cExtInfo);
	DDX_Control(pDX, IDC_EDIT_DEV_GBCODE, m_cDevGBCdeo);
	DDX_Control(pDX, IDC_EDIT_DEV_NAME_ABBR, m_cNameAbbr);
	DDX_Control(pDX, IDC_EDIT_DEV_VERSION, m_cDevVersion);
	DDX_Control(pDX, IDC_COMBO_DGW_ID, m_cDGWs);
}


BEGIN_MESSAGE_MAP(CGxxGmDevMgrDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_REFRESH_SERVICES, &CGxxGmDevMgrDlg::OnBnClickedBtnRefreshServices)
	ON_BN_CLICKED(IDC_BTN_REFRESH_DEVICES, &CGxxGmDevMgrDlg::OnBnClickedBtnRefreshDevices)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DGWS, &CGxxGmDevMgrDlg::OnNMClickListDgws)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CGxxGmDevMgrDlg::OnBnClickedBtnRegister)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CGxxGmDevMgrDlg::OnBnClickedBtnModify)
END_MESSAGE_MAP()


// CGxxGmDevMgrDlg ��Ϣ�������
BOOL CGxxGmDevMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cDGWList.SetExtendedStyle(m_cDGWList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cDGWList.InsertColumn(0, _T("����ID"), LVCFMT_LEFT, 60);
	m_cDGWList.InsertColumn(1, _T("��������"), LVCFMT_LEFT, 60);
	m_cDGWList.InsertColumn(2, _T("�����ַ"), LVCFMT_LEFT, 140);

	m_cDevList.SetExtendedStyle(m_cDevList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cDevList.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60);
	m_cDevList.InsertColumn(1, _T("�豸�ڲ�ID"), LVCFMT_LEFT, 60);
	m_cDevList.InsertColumn(2, _T("�豸����ID"), LVCFMT_LEFT, 100);
	m_cDevList.InsertColumn(3, _T("�豸����"), LVCFMT_LEFT, 80);
	//m_cDevList.InsertColumn(4, _T("�豸�汾"), LVCFMT_LEFT, 80);

	OnBnClickedBtnRefreshServices();

	return TRUE;
}

void CGxxGmDevMgrDlg::OnBnClickedBtnRefreshServices()
{
	USES_CONVERSION;
	int errCode = govideo_->GetDeviceGatewayList();

	m_cDGWList.DeleteAllItems();

	// ͬʱ�����豸�б�
	errCode = govideo_->GetAllDevices();

	std::vector<GOVIDEO_SERVICE_INFO>::iterator iter;
	for (iter = govideo_->govideo_services_.begin(); iter != govideo_->govideo_services_.end(); ++iter)
	{
		if (iter->service_type_id_.compare("102") == 0)
		{
			int count = m_cDGWList.GetItemCount();
			m_cDGWList.InsertItem(count, A2T(iter->service_id_.c_str()));
			m_cDGWList.SetItemText(count, 1, A2T(iter->service_name_.c_str()));

			char ipaddress[4096] = {0};
			sprintf_s(ipaddress, 4096, "%s:%s", iter->service_ip_.c_str(), iter->service_port_.c_str());
			m_cDGWList.SetItemText(count, 2, A2T(ipaddress));

			m_cDGWs.AddString(A2T(iter->service_id_.c_str()));
		}
		
	}
}

void CGxxGmDevMgrDlg::OnBnClickedBtnRefreshDevices()
{
	// �������ʾ����ע����豸
	// �Ȼ�ȡ�����豸�б�
	int errCode = govideo_->GetAllDevices();
	if (errCode != 0)
	{
		return ;
	}

	//// �ٻ�ȡ����״̬
	//errCode = govideo_->GetAllDeviceStatus();
	//if (errCode != 0)
	//{
	//	return ;
	//}

	m_cDevList.DeleteAllItems();
	
	USES_CONVERSION;
	std::vector<GOVIDEO_DEVICE_INFO *>::iterator iter;
	for (iter = govideo_->devices_.begin(); iter != govideo_->devices_.end(); ++iter)
	{
		int count = m_cDevList.GetItemCount();

		TCHAR str_val[64] = {0};
		_stprintf_s(str_val, 64, _T("%d"), count);
		m_cDevList.InsertItem(count, str_val);

		_stprintf_s(str_val, 64, _T("%d"), (*iter)->device_id_);
		m_cDevList.SetItemText(count, 1, str_val);

		m_cDevList.SetItemText(count, 2, A2T((*iter)->gb28181_code_.c_str()));

		_stprintf_s(str_val, 64, _T("%d"), (*iter)->model_id_);
		m_cDevList.SetItemText(count, 3, str_val);
	}
}

void CGxxGmDevMgrDlg::OnNMClickListDgws(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// ��鵱ǰѡ�е�����һ��
	CString service_id_str = m_cDGWList.GetItemText(pNMItemActivate->iItem, 0);
	if (service_id_str.IsEmpty())
		return ;

	int dgw_id = _ttoi(service_id_str.GetBuffer(0));

	m_cDevList.DeleteAllItems();

	// ����DGW-ID�����������豸���г���
	USES_CONVERSION;
	std::vector<GOVIDEO_DEVICE_INFO *>::iterator iter;
	for (iter = govideo_->devices_.begin(); iter != govideo_->devices_.end(); ++iter)
	{
		GOVIDEO_DEVICE_INFO *device_info = *iter;

		if (device_info->dgw_server_id_ == dgw_id)
		{
			int count = m_cDevList.GetItemCount();

			TCHAR str_val[64] = {0};
			_stprintf_s(str_val, 64, _T("%d"), count);
			m_cDevList.InsertItem(count, str_val);

			_stprintf_s(str_val, 64, _T("%d"), (*iter)->device_id_);
			m_cDevList.SetItemText(count, 1, str_val);

			m_cDevList.SetItemText(count, 2, A2T((*iter)->gb28181_code_.c_str()));

			_stprintf_s(str_val, 64, _T("%d"), (*iter)->model_id_);
			m_cDevList.SetItemText(count, 3, str_val);
		}
	}

	*pResult = 0;
}

void CGxxGmDevMgrDlg::OnBnClickedBtnRegister()
{
	// ע���豸,�ӿؼ����ó�����
	CString device_name;
	m_cDevName.GetWindowText(device_name);

	CString model_id;
	m_cModelID.GetWindowText(model_id);

	CString category_id;
	m_cCategoryID.GetWindowText(category_id);

	CString conn_info;
	m_cConnInfo.GetWindowText(conn_info);

	CString cfg_version;
	m_cCfgVersion.GetWindowText(cfg_version);

	CString username;
	m_cUsername.GetWindowText(username);

	CString password;
	m_cPassword.GetWindowText(password);

	CString dev_code;
	m_cDevCode.GetWindowText(dev_code);

	CString dev_ext_info;
	m_cExtInfo.GetWindowText(dev_ext_info);

	CString gbcode;
	m_cDevGBCdeo.GetWindowText(gbcode);

	CString name_abbr;
	m_cNameAbbr.GetWindowText(name_abbr);

	CString dev_version;
	m_cDevVersion.GetWindowText(dev_version);

	CString dgw_id;
	m_cDGWs.GetWindowText(dgw_id);

	// ��������豸�ӿ�
	//int errCode = govideo_;
}

void CGxxGmDevMgrDlg::OnBnClickedBtnModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

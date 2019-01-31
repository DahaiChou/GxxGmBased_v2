// GxxGmDevMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GxxGmGoVideoMgr.h"
#include "GxxGmDevMgrDlg.h"


// CGxxGmDevMgrDlg 对话框

IMPLEMENT_DYNAMIC(CGxxGmDevMgrDlg, CDialog)

CGxxGmDevMgrDlg::CGxxGmDevMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxxGmDevMgrDlg::IDD, pParent)
	, current_select_dgw_id_(-1)
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
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_DEVS, &CGxxGmDevMgrDlg::OnLvnKeydownListDevs)
END_MESSAGE_MAP()


// CGxxGmDevMgrDlg 消息处理程序
BOOL CGxxGmDevMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cDGWList.SetExtendedStyle(m_cDGWList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cDGWList.InsertColumn(0, _T("服务ID"), LVCFMT_LEFT, 60);
	m_cDGWList.InsertColumn(1, _T("服务名称"), LVCFMT_LEFT, 60);
	m_cDGWList.InsertColumn(2, _T("服务地址"), LVCFMT_LEFT, 140);

	m_cDevList.SetExtendedStyle(m_cDevList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cDevList.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
	m_cDevList.InsertColumn(1, _T("设备内部ID"), LVCFMT_LEFT, 60);
	m_cDevList.InsertColumn(2, _T("设备国标ID"), LVCFMT_LEFT, 100);
	m_cDevList.InsertColumn(3, _T("设备厂商"), LVCFMT_LEFT, 80);
	//m_cDevList.InsertColumn(4, _T("设备版本"), LVCFMT_LEFT, 80);

	OnBnClickedBtnRefreshServices();

	return TRUE;
}

void CGxxGmDevMgrDlg::OnBnClickedBtnRefreshServices()
{
	USES_CONVERSION;
	int errCode = govideo_->GetDeviceGatewayList();

	m_cDGWList.DeleteAllItems();

	// 同时更新设备列表
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
	// 这里会显示所有注册的设备
	// 先获取所有设备列表
	int errCode = govideo_->GetAllDevices();
	if (errCode != 0)
	{
		return ;
	}

	//// 再获取在线状态
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
		GOVIDEO_DEVICE_INFO *device_info = *iter;
		if ((current_select_dgw_id_ == -1)) || (current_select_dgw_id_ == device_info->dgw_server_id_))
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
}

void CGxxGmDevMgrDlg::OnNMClickListDgws(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// 检查当前选中的是哪一项
	CString service_id_str = m_cDGWList.GetItemText(pNMItemActivate->iItem, 0);
	if (service_id_str.IsEmpty())
		return ;

	current_select_dgw_id_ = _ttoi(service_id_str.GetBuffer(0));

	m_cDevList.DeleteAllItems();

	// 根据DGW-ID，将所属的设备都列出来
	USES_CONVERSION;
	std::vector<GOVIDEO_DEVICE_INFO *>::iterator iter;
	for (iter = govideo_->devices_.begin(); iter != govideo_->devices_.end(); ++iter)
	{
		GOVIDEO_DEVICE_INFO *device_info = *iter;

		if (device_info->dgw_server_id_ == current_select_dgw_id_)
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
	// 注册设备,从控件中拿出数据
	CString device_id;
	m_cDevID.GetWindowText(device_id);

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

	GOVIDEO_DEVICE_INFO device_info;
	device_info.device_id_ == 0;
	device_info.device_name_ = T2A(device_name.GetBuffer(0));
	device_info.model_id_ = _ttoi(model_id.GetBuffer(0));
	device_info.category_id_ = _ttoi(category_id.GetBuffer(0));
	device_info.device_connection_info_ = T2A(conn_info.GetBuffer(0));
	device_info.version_ = _ttoi(cfg_version.GetBuffer(0));
	device_info.device_username_ = T2A(username.GetBuffer(0));
	device_info.device_password_ = T2A(password.GetBuffer(0));
	device_info.device_code_ = T2A(dev_code.GetBuffer(0));
	device_info.device_extra_info_ = T2A(dev_ext_info.GetBuffer(0));
	device_info.gb28181_code_ = T2A(gbcode.GetBuffer(0));
	device_info.device_name_abbr_ = T2A(name_abbr.GetBuffer(0));
	device_info.device_version_ = T2A(dev_version.GetBuffer(0));
	device_info.dgw_server_id_ = _ttoi(dgw_id.GetBuffer(0));

	// 调用添加设备接口
	int errCode = govideo_->RegisterDevice(device_info);
	if (errCode != 0)
		MessageBox(_T("注册设备失败！"), _T("错误"), MB_OK|MB_ICONERROR);
	else
	{
		MessageBox(_T("注册设备成功！"), _T("提示"), MB_OK|MB_ICONINFORMATION);

		OnBnClickedBtnRefreshDevices();
	}
}

void CGxxGmDevMgrDlg::OnBnClickedBtnModify()
{
	// 
	CString device_id;
	m_cDevID.GetWindowText(device_id);

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

	GOVIDEO_DEVICE_INFO device_info;
	device_info.device_id_ == _ttoi(device_id.GetBuffer(0));
	device_info.device_name_ = T2A(device_name.GetBuffer(0));
	device_info.model_id_ = _ttoi(model_id.GetBuffer(0));
	device_info.category_id_ = _ttoi(category_id.GetBuffer(0));
	device_info.device_connection_info_ = T2A(conn_info.GetBuffer(0));
	device_info.version_ = _ttoi(cfg_version.GetBuffer(0));
	device_info.device_username_ = T2A(username.GetBuffer(0));
	device_info.device_password_ = T2A(password.GetBuffer(0));
	device_info.device_code_ = T2A(dev_code.GetBuffer(0));
	device_info.device_extra_info_ = T2A(dev_ext_info.GetBuffer(0));
	device_info.gb28181_code_ = T2A(gbcode.GetBuffer(0));
	device_info.device_name_abbr_ = T2A(name_abbr.GetBuffer(0));
	device_info.device_version_ = T2A(dev_version.GetBuffer(0));
	device_info.dgw_server_id_ = _ttoi(dgw_id.GetBuffer(0));

	// 调用添加设备接口
	int errCode = govideo_->RegisterDevice(device_info);
	if (errCode != 0)
		MessageBox(_T("修改设备失败！"), _T("错误"), MB_OK|MB_ICONERROR);
	else
	{
		MessageBox(_T("修改设备成功！"), _T("提示"), MB_OK|MB_ICONINFORMATION);

		OnBnClickedBtnRefreshDevices();
	}
}

void CGxxGmDevMgrDlg::OnLvnKeydownListDevs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	// 找到删除键
	if (pLVKeyDow->wVKey == VK_DELETE)
	{
		INT_PTR ret = MessageBox(_T("确定要移除选中的设备吗？"), _T("警告"), MB_OKCANCEL|MB_ICONMASK);
		if (ret != IDOK)
			return ;

		// 找到当前选中的行
		POSITION pos = m_cDevList.GetFirstSelectedItemPosition();
		if (pos == NULL)
			return ;

		do 
		{
			int index = m_cDevList.GetNextSelectedItem(pos);
			if (index != -1)
			{
				// 拿到设备内部id
				CString dev_id = m_cDevList.GetItemText(index, 1);

				// 先调用GoVideo的删除服务接口，成功后再调用界面移除记录
				int errCode = govideo_->UnregisterDevice(T2A(dev_id.GetBuffer(0)));
				if (errCode == 0)
					m_cDevList.DeleteItem(index);
				else
				{
					MessageBox(_T("移除设备失败！"), _T("错误"), MB_OK|MB_ICONERROR);
					return ;
				}
			}
			else
				break;

		} while (true);
	}

	*pResult = 0;
}

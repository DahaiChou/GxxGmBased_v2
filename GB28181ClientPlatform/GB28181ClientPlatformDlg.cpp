
// GB28181ClientPlatformDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GB28181ClientPlatform.h"
#include "GB28181ClientPlatformDlg.h"

#include "DBBusiness.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGB28181ClientPlatformDlg �Ի���




CGB28181ClientPlatformDlg::CGB28181ClientPlatformDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGB28181ClientPlatformDlg::IDD, pParent)
	, db_business_(new DBBusiness())
	, org_and_devs(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGB28181ClientPlatformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CURRENT_IP, m_cCurrentIp);
	DDX_Control(pDX, IDC_EDIT_CURRENT_PORT, m_cCurrentPort);
	DDX_Control(pDX, IDC_EDIT_CURRENT_GBCODE, m_cCurrentGbcode);
	DDX_Control(pDX, IDC_EDIT_REMOTE_IP, m_cRemoteIp);
	DDX_Control(pDX, IDC_EDIT_REMOTE_PORT, m_cRemotePort);
	DDX_Control(pDX, IDC_EDIT__REMOTE_GBCODE, m_cRemoteGbcode);
	DDX_Control(pDX, IDC_EDIT_AUTHEN_USERNAME, m_cAuthenUsername);
	DDX_Control(pDX, IDC_EDIT_AUTHEN_PASSWORD, m_cAuthenPassword);
	DDX_Control(pDX, IDC_TREE_GROUP_AND_DEVICES, m_cOrgAndDevAndChannel);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_cStatus);
}

BEGIN_MESSAGE_MAP(CGB28181ClientPlatformDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CGB28181ClientPlatformDlg::OnBnClickedBtnRegister)
	ON_BN_CLICKED(IDC_BTN_START, &CGB28181ClientPlatformDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CGB28181ClientPlatformDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CGB28181ClientPlatformDlg ��Ϣ�������

BOOL CGB28181ClientPlatformDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	// ��ʼ��GB28181Э��ջ
	gb28181_handle_ = GB28181Agent_Init(2, 16, eVERSION_2016, 30000);

	// ��װ�ص�
	GB28181Agent_SetLogCB(gb28181_handle_, _AgentLogCallBack, this);
	GB28181Agent_SetDevInfoQueryCB(gb28181_handle_, _DevInfoQueryCB, this);
	GB28181Agent_SetDevControlCB(gb28181_handle_, _DevControlCB, this);
	GB28181Agent_SetPlayControlCB(gb28181_handle_, _PlayControlCB, this);
	GB28181Agent_SetStreamRequestCB(gb28181_handle_, _StreamRequestCB, this);
	GB28181Agent_SetNotifyInfoCB(gb28181_handle_, _NotifyInfo_CallBackFunc, this);
	GB28181Agent_SetExtendRequestCB(gb28181_handle_, _ExtendRqeustCallBack, this);

	// ����ʱ�ȶ�ȡһ���������ݿ⣬�������ô���
	std::string current_ip = db_business_->GetCurrentIp();
	std::string current_port = db_business_->GetCurrentPort();
	std::string current_gbcode = db_business_->GetCurrentGbcode();

	std::string remote_ip = db_business_->GetRemoteIp();
	std::string remote_port = db_business_->GetRemotePort();
	std::string remote_gbcode = db_business_->GetRemoteGbcode();

	std::string authen_username = db_business_->GetAuthenUsername();
	std::string authen_password = db_business_->GetAuthenPassword();

	USES_CONVERSION;
	m_cCurrentIp.SetWindowText(A2T(current_ip.c_str()));
	m_cCurrentPort.SetWindowText(A2T(current_port.c_str()));
	m_cCurrentGbcode.SetWindowText(A2T(current_gbcode.c_str()));

	m_cRemoteIp.SetWindowText(A2T(remote_ip.c_str()));
	m_cRemotePort.SetWindowText(A2T(remote_port.c_str()));
	m_cRemoteGbcode.SetWindowText(A2T(remote_gbcode.c_str()));

	m_cAuthenUsername.SetWindowText(A2T(authen_username.c_str()));
	m_cAuthenPassword.SetWindowText(A2T(authen_password.c_str()));

	// �жϱ���ƽ̨���ͻ����豸����
	// �����ƽ̨���;�չʾ��֯�ܹ����豸��
	// ������豸���;�չʾ�豸�����豸��ֻ����������Ŀ¼���豸����Ŀ¼��ͨ��
	// �����ݿ������֯�ܹ����������ӵ���֯�ܹ����ؼ���
	db_business_->GetOrgInfos(org_infos);

	// ���ﹹ��������
	BuildOrgTreeCtrl(org_infos, h_rootNode);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGB28181ClientPlatformDlg::OnPaint()
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
HCURSOR CGB28181ClientPlatformDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CGB28181ClientPlatformDlg::GetOrgCounts(struct OrgInfo org_infos, unsigned int &count)
{
	++count;

	if (org_infos.sub_devs.size() > 0)
		count += org_infos.sub_devs.size();

	if (org_infos.sub_orgs.size() > 0)
	{
		std::vector<struct OrgInfo>::iterator iter;
		for (iter = org_infos.sub_orgs.begin(); iter != org_infos.sub_orgs.end(); ++iter)
		{
			struct OrgInfo sub_info = *iter;
			GetOrgCounts(sub_info, count);
		}
	}

	return count;
}

StruCatalogInfo* CGB28181ClientPlatformDlg::BuildGB28181Orgs(struct OrgInfo org_infos, int count, int &index)
{
	// ����֯���е���֯�ܹ�
	// �����ж��ǲ��Ǹ�Ŀ¼
	if (org_infos.gbcode.compare(org_infos.father_gbcode) == 0)
	{
		// �Ǹ�Ŀ¼�������µ���֯�ܹ����豸�б�����
		if (org_and_devs != NULL)
			delete [] org_and_devs;

		org_and_devs = new StruCatalogInfo[count];
		memset(org_and_devs, 0, sizeof(StruCatalogInfo) * count);
	}

	// �жϵ�ǰ�ڵ���û���ӽڵ�
	int isParent = 0;
	if (org_infos.sub_devs.size() > 0 || org_infos.sub_orgs.size() > 0)
		isParent = 1;

	strncpy(org_and_devs[index].czGBCode, org_infos.gbcode.c_str(), STR_GBCODE_LEN);				// �������
	strncpy(org_and_devs[index].czDeviceName, org_infos.name.c_str(), STR_NAME_LEN);				// �豸����
	strncpy(org_and_devs[index].czManufacturer, "GOSUNCN", STR_MANUFACTURER_LEN);					// ��Ϊ�豸�ǣ���ʾ�豸������
	strncpy(org_and_devs[index].czModel, "V3.3", STR_MANUFACTURER_LEN);								// ��Ϊ�豸�ǣ���ʾ�豸�ͺ�
	strncpy(org_and_devs[index].czOwner, org_infos.father_gbcode.c_str(), STR_OWNER_LEN);			// ��Ϊ�豸�ǣ���ʾ�豸�ͺ�
	strncpy(org_and_devs[index].czCivilCode, org_infos.father_gbcode.c_str(), STR_CIVILCODE_LEN);	// ��������
	strncpy(org_and_devs[index].czBlock, org_infos.father_gbcode.c_str(), STR_BLOCK_LEN);			// ����
	strncpy(org_and_devs[index].czAddress, org_infos.father_gbcode.c_str(), STR_ADDRESS_LEN);		// ��Ϊ�豸ʱ����ʾ��װ��ַ
	org_and_devs[index].iParental = isParent;														// ��Ϊ�豸ʱ����ʾ�Ƿ������豸
	strncpy(org_and_devs[index].czParentID, org_infos.father_gbcode.c_str(), STR_PARENTID_LEN);		// ��Ϊ�豸ʱ����ʾ�Ƿ������豸
	org_and_devs[index].iSafetyWay = 0;																// ���ȫģʽ����������ǿ��Ϊ0
	org_and_devs[index].iRegisterWay = 1;															// ע�᷽ʽ��Ĭ��Ϊ1������sip3261��׼����֤ע��ģʽ
	strncpy(org_and_devs[index].czCertNum, "CertNum 0", STR_CERTNUM_LEN);							// ֤�����к�
	org_and_devs[index].iCertifiable = 0;
	org_and_devs[index].iErrCode = 400;
	strncpy(org_and_devs[index].czEndTime, "2010-11-11T19:46:17", STR_DATETIME_LEN);				// ֤����Ч��
	org_and_devs[index].iSecrecy = 0;
	strncpy(org_and_devs[index].czIPAddress, "192.168.1.1", STR_IPADDRESS_LEN);
	org_and_devs[index].iPort = 5060;
	strncpy(org_and_devs[index].czPassword, "Password", STR_PASSWORD_LEN);
	org_and_devs[index].iStatus = 1;																// ����״̬
	org_and_devs[index].dLongitude = 113.283198;
	org_and_devs[index].dLatitude = 23.143136;
	//org_and_devs[0].iPtzType = 

	++index;

	// Ȼ����֯���е��豸��Ϣ
	std::vector<struct OrgInfo>::iterator iter_org;
	for (iter_org = org_infos.sub_orgs.begin(); iter_org != org_infos.sub_orgs.end(); ++iter_org)
	{
		struct OrgInfo orginfo = *iter_org;
		BuildGB28181Orgs(orginfo, count, index);
	}

	//// �����֯���е�ͨ����Ϣ
	//std::vector<struct DevInfo>::iterator iter_dev;
	//for (iter_dev = org_infos.sub_devs.begin(); iter_dev != org_infos.sub_devs.end(); ++iter_dev)
	//{
	//	struct DevInfo orginfo = *iter_dev;
	//	BuildGB28181Orgs(orginfo, count, index);
	//}
	return org_and_devs;
}

void CGB28181ClientPlatformDlg::SetStatus(const char *status)
{
	CString history;
	m_cStatus.GetWindowText(history);

	if (!history.IsEmpty())
		history += _T("\r\n");
	
	USES_CONVERSION;
	history += A2T(status);
	m_cStatus.SetWindowText(history);
}

void CGB28181ClientPlatformDlg::BuildOrgTreeCtrl(struct OrgInfo root_org, HTREEITEM root_handle)
{
	HTREEITEM h_childNode = NULL;

	USES_CONVERSION;
	if (root_org.gbcode.compare(root_org.father_gbcode) == 0)
	{
		// �Ǹ��ڵ�
		h_rootNode = m_cOrgAndDevAndChannel.InsertItem(A2T(root_org.name.c_str()));
		TRACE("�����Ӹ��ڵ㣺%s, ���ڵ���Ϊ��%X", root_org.name.c_str(), h_rootNode);

		// �����Լ����ӽڵ㣬ȫ�������Լ�����ڵ���
		std::vector<OrgInfo>::iterator iter;
		for (iter = root_org.sub_orgs.begin(); iter != root_org.sub_orgs.end(); ++iter)
		{
			struct OrgInfo sub_org = *iter;
			BuildOrgTreeCtrl(sub_org, h_rootNode);
		}
	}
	else
	{
		// ���Ǹ��ڵ㣬����ǰ�ڵ���ڸ��ڵ���
		h_childNode = m_cOrgAndDevAndChannel.InsertItem(A2T(root_org.name.c_str()), root_handle, NULL);
		TRACE("�������ӽڵ㣺%s, ���ڵ���Ϊ��%X\n", root_org.name.c_str(), root_handle);

		// �����Լ����ӽڵ㣬ȫ�������Լ�����ڵ���
		std::vector<OrgInfo>::iterator iter;
		for (iter = root_org.sub_orgs.begin(); iter != root_org.sub_orgs.end(); ++iter)
		{
			struct OrgInfo sub_org = *iter;
			BuildOrgTreeCtrl(sub_org, h_childNode);
		}
	}
}

void CGB28181ClientPlatformDlg::OnBnClickedBtnStart()
{
	CString str_ip, str_port, str_gbcode;
	m_cCurrentIp.GetWindowText(str_ip);
	m_cCurrentPort.GetWindowText(str_port);
	m_cCurrentGbcode.GetWindowText(str_gbcode);

	USES_CONVERSION;
	GS28181_ERR err = GB28181Agent_Start(gb28181_handle_, T2A(str_ip.GetBuffer(0)), _ttoi(str_port.GetBuffer(0)), T2A(str_gbcode.GetBuffer(0)), eTYPE_UDP);
	if (err != GS28181_ERR_SUCCESS)
	{
		SetStatus("����GB28181����ʧ�ܣ�");
		return ;
	}
	else
	{
		SetStatus("����GB28181����ɹ���");
		return ;
	}
}

void CGB28181ClientPlatformDlg::OnBnClickedBtnStop()
{
	GB28181Agent_Stop(gb28181_handle_);
	SetStatus("ֹͣGB28181����ɹ���");
}

void CGB28181ClientPlatformDlg::OnBnClickedBtnRegister()
{
	CString str_remote_ip, str_remote_port, str_remote_gbcode, str_authen_username, str_authen_password;
	m_cRemoteIp.GetWindowText(str_remote_ip);
	m_cRemotePort.GetWindowText(str_remote_port);
	m_cRemoteGbcode.GetWindowText(str_remote_gbcode);
	m_cAuthenUsername.GetWindowText(str_authen_username);
	m_cAuthenPassword.GetWindowText(str_authen_password);

	USES_CONVERSION;
	StruRegistMsg srvinfo;
	strcpy(srvinfo.szUserName, T2A(str_authen_username.GetBuffer(0)));
	strcpy(srvinfo.szPassword, T2A(str_authen_password.GetBuffer(0)));
	srvinfo.iExpires = 9999999;
	strcpy(srvinfo.stuCnnParam.szIP, T2A(str_remote_ip.GetBuffer(0)));
	strcpy(srvinfo.stuCnnParam.szGBCode, T2A(str_remote_gbcode.GetBuffer(0)));
	srvinfo.stuCnnParam.iPort = _ttoi(str_remote_port.GetBuffer(0));

	char datetime[4096] = {0};
	GS28181_ERR err = GB28181Agent_Register(gb28181_handle_, &srvinfo, datetime);
	if (err != GS28181_ERR_SUCCESS)
	{
		SetStatus("ע�ᵽ�ϼ�ƽ̨ʧ�ܣ�");
		return ;
	}
	else
	{
		// ע��ɹ�����������д�����ݿ�
		CString current_ip, current_port, current_gbcode;
		m_cCurrentIp.GetWindowText(current_ip);
		m_cCurrentPort.GetWindowText(current_port);
		m_cCurrentGbcode.GetWindowText(current_gbcode);

		db_business_->SetCurrentIp(T2A(current_ip.GetBuffer(0)));
		db_business_->SetCurrentPort(T2A(current_port.GetBuffer(0)));
		db_business_->SetCurrentGbcode(T2A(current_gbcode.GetBuffer(0)));

		db_business_->SetRemoteIp(T2A(str_remote_ip.GetBuffer(0)));
		db_business_->SetRemotePort(T2A(str_remote_port.GetBuffer(0)));
		db_business_->SetRemoteGbcode(T2A(str_remote_gbcode.GetBuffer(0)));
		db_business_->SetAuthenUsername(T2A(str_authen_username.GetBuffer(0)));
		db_business_->SetAuthenPassword(T2A(str_authen_password.GetBuffer(0)));

		SetStatus("ע�ᵽ�ϼ�ƽ̨�ɹ���");
		return ;
	}
}

void CGB28181ClientPlatformDlg::_AgentLogCallBack(EnumLogLevel eLevel, const char * szTemp, int iLen, void * pUserData)
{
	CGB28181ClientPlatformDlg *dlg = (CGB28181ClientPlatformDlg *)pUserData;
}

SIP_REPSOND_CODE CGB28181ClientPlatformDlg::_DevInfoQueryCB(SESSION_HANDLE hSession, const char * czSrvGBCode, StruQueryReqDescri * stuQuery, void * pUserData)
{
	CGB28181ClientPlatformDlg *dlg = (CGB28181ClientPlatformDlg *)pUserData;

	if (!stuQuery)
		return SIP_RESPONSE_CODE_FORBIDDEN;

	// ���� StruQueryReqDescri �ṹ����Ϣ�жϲ�ѯ��������
	switch (stuQuery->eType)
	{
	case eQUE_DEV_CATALOG:
		// �豸Ŀ¼��ѯ
		{
			StruDeviceCatalog stuCata;
			//memset(&stuCata, 0, sizeof(StruDeviceCatalog));
			stuCata.iSumNum = 0;
			strncpy(stuCata.czGBCode, stuQuery->czGBCode, STR_GBCODE_LEN);

			// �������нڵ������
			// Ȼ��������֯����
			int index = 0;
			dlg->GetOrgCounts(dlg->org_infos, stuCata.iSumNum);
			stuCata.ptrCatalog = dlg->BuildGB28181Orgs(dlg->org_infos, stuCata.iSumNum, index);

			GS28181_ERR errCode = GB28181Agent_RespondDevCatalog(hSession, &stuCata, 2, false);
			if( GS28181_ERR_SUCCESS == errCode )
				TRACE("[�豸Ŀ¼��ѯ] ָ����Ӧ�ɹ���\n");
			else
				TRACE("[�豸Ŀ¼��ѯ] ָ����Ӧʧ�ܣ�\n");

			// ����Ͳ�ɾ��֯�ܹ��б��ˣ��͵��ǻ�����
			//delete [] stuCata.ptrCatalog;
			//stuCata.ptrCatalog = NULL;
		}
		break;
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE CGB28181ClientPlatformDlg::_DevControlCB(const char * czSrvGBCode, const StruDevCtrlCmd * stuCmd, void * pUserData)
{
	CGB28181ClientPlatformDlg *dlg = (CGB28181ClientPlatformDlg *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE CGB28181ClientPlatformDlg::_PlayControlCB(STREAM_HANDLE hStream, const StruPlayCtrlCmd * stuCmd, void * pUserData)
{
	CGB28181ClientPlatformDlg *dlg = (CGB28181ClientPlatformDlg *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE CGB28181ClientPlatformDlg::_StreamRequestCB(STREAM_HANDLE hStream, const char * czSrvGBCode, EnumStreamRequest eRequest,	StruMediaInfo * pInMedia, const StruStreamDescription * pDescri, void * pUserData)
{
	CGB28181ClientPlatformDlg *dlg = (CGB28181ClientPlatformDlg *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE CGB28181ClientPlatformDlg::_NotifyInfo_CallBackFunc(EnumNotifyType eType, const char * czSrvGBCode, void * pMsg, void * pUserData)
{
	CGB28181ClientPlatformDlg *dlg = (CGB28181ClientPlatformDlg *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE CGB28181ClientPlatformDlg::_ExtendRqeustCallBack(SESSION_HANDLE hSession, EnumExtendType eType, const char * czTargetDevID, void * pMsg, void * pUserData)
{
	CGB28181ClientPlatformDlg *dlg = (CGB28181ClientPlatformDlg *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

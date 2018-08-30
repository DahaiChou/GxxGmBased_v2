// GxxGmFaceRecong.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "curl/curl.h"
#include <iostream>
#include <windows.h>
#include <string>

DWORD img_size = 0;
DWORD sended_img_size = 0;
unsigned char *img = NULL;

std::string response_data = "";

size_t RequestMatch(void *buf, size_t size, size_t nmemb, void *pUserData)
{
	size_t nSize = size * nmemb;

	if (sended_img_size == img_size)
	{
		//����ص��������Σ���ʱ����֪��ԭ�������ж��Ƿ���꣬����ͷ���
		return 0;
	}

	//������͵����ݱȽ�С��һ�ο������
	if (img_size<=nSize)
	{
		memcpy(buf, img, img_size);
		sended_img_size = img_size;
		nSize = img_size;
		return nSize;
	}

	//������ݱȽϴ󣬷�������
	if ((sended_img_size + nSize) <= img_size)
	{
		memcpy(buf, img+sended_img_size, nSize);
		sended_img_size += nSize;
	}
	else
	{
		nSize = img_size- sended_img_size;
		memcpy(buf, img+img_size, nSize);
		sended_img_size += nSize;
	}

	return nSize;
}

size_t ResponseMatch(void *buf, size_t size, size_t nmemb, void *pUserData)
{
	response_data.append((char *)buf, size * nmemb);
	//printf("%s", (char*)buf);

	return size * nmemb;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int errCode = 0;

	// ��ȡ����ͼƬ
	const char *jpg_file = "C:\\Users\\WANGY\\Desktop\\wy.jpg";

	HANDLE hFace = CreateFileA(jpg_file, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFace == INVALID_HANDLE_VALUE)
	{
		errCode = GetLastError();
		return errCode;
	}

	DWORD img_size_high = 0;
	img_size = GetFileSize(hFace, &img_size_high);

	img = new unsigned char[img_size];
	DWORD readed = 0;
	BOOL bret = ReadFile(hFace, img, img_size, &readed, NULL);

	CloseHandle(hFace);

	const char *url = "http://10.10.18.88:6081/compareface?logicdb=1&threshold=50&pictype=jpg";
	CURL *m_curl = curl_easy_init();
	if(!m_curl)
	{
		return -1;
	}

	//���ö�ȡ���ͻ������Ļص�����
	curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, RequestMatch);
	//curl_easy_setopt(m_curl, CURLOPT_READDATA, this);

	//���û�ȡ�������ݵĻص�����
	//curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, ResponseMatch);

	//����HTTP��POST������
	curl_easy_setopt(m_curl, CURLOPT_POST, 1L); 

	//�����ݴ���
	curl_easy_setopt(m_curl, CURLOPT_HTTP_TRANSFER_DECODING, 0L);
	curl_easy_setopt(m_curl, CURLOPT_HTTP_CONTENT_DECODING, 0L);

	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, (long)30);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, (long)30);

	//�������ݵ��ܳ���
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE,(curl_off_t)img_size);
	//����URL
	curl_easy_setopt(m_curl, CURLOPT_URL, url);

	CURLcode res = curl_easy_perform(m_curl);
	if(res == CURLE_OK)
	{
		long nCode = 200;
		curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &nCode);
		if(nCode == 200)
		{
			//���سɹ���д���ļ�
			const char *result_file = "C:\\Users\\WANGY\\Desktop\\result.html";

			HANDLE hResult = CreateFileA(result_file, GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hResult == INVALID_HANDLE_VALUE)
			{
				errCode = GetLastError();
				return errCode;
			}

			DWORD written = 0;
			BOOL bret = WriteFile(hResult, response_data.c_str(), response_data.size(), &written, NULL);

			CloseHandle(hFace);
		}
		else
		{
			//m_nRet = FMRT_HTTPOPR_FAIL;
			printf("curl_easy_perform badrequest,errorcode:%d\n", nCode);
		}
	}
	else
	{
		//m_nRet = FMRT_HTTPOPR_FAIL;
		//CTRACE("curl_easy_perform fail,errorcode:["<<res<<"]", LOGGER_LEVEL_ERROR, true);
	}

	//nRetCode = m_nRet;

	curl_easy_cleanup(m_curl);
	m_curl = NULL;

	system("pause");
	return 0;
}


#ifndef _GxxGmQueue_H_
#define _GxxGmQueue_H_

#include <queue>

#include "Poco/Mutex.h"
#include "Poco/Event.h"

template <class T>
class GxxGmQueue
{
public:
	GxxGmQueue() {};
	~GxxGmQueue() {};

public:
	// ��ӣ�����β
	int push(T element)
	{
		int errCode = 0;

		// ����
		rw_lock_.lock();
		elements_.push(element);
		rw_lock_.unlock();

		empty_event_.set();

		return errCode;
	}

	/**
	 * ���ʵ�һ��Ԫ��
	 * ������
	 *	@bolck �Ƿ���������
	 */
	int front(T **element, bool block = false)
	{
		while (elements_.empty())
		{
			// ������ȡ���������Ϊ�գ���һֱ����ȥ
			if (block)
			{
				try
				{
					empty_event_.wait(1000);
				}
				catch (Poco::Exception e)
				{
					char errmsg[4096] = {0};
					std::string estr = e.displayText();
					int errCode = e.code();
					sprintf_s(errmsg, 4096, "[��������] �����쳣���쳣��Ϣ��%s���쳣���룺%d\n", estr.c_str(), errCode);
					OutputDebugStringA(errmsg);
				}
				
			}
			else
				return -1;
		}

		rw_lock_.lock();
		*element = av_packet_clone(&elements_.front());
		rw_lock_.unlock();

		return 0;
	}

	// ��һ��Ԫ�س���
	int pop()
	{
		int errCode = 0;

		rw_lock_.lock();
		elements_.pop();
		rw_lock_.unlock();

		return errCode;
	}

public:
	std::queue<T> elements_;
	Poco::Mutex rw_lock_;
	Poco::Event empty_event_;
};
#endif

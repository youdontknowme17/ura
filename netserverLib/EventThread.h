#ifndef _EVENT_THREAD_H_
#define _EVENT_THREAD_H_

#pragma once

#include "s_CLock.h"

class EventThread : public CLock
{
public:
	EventThread(void);
	virtual ~EventThread(void);

protected:
	DWORD	m_dwThreadID;	//! ������ ID
	HANDLE	m_hThread;		//! ������ �ڵ�
	HANDLE	m_hEventQuit;	//! ������ ���� �̺�Ʈ
	HANDLE	m_hEventWork;	//! ������ work �̺�Ʈ
	bool    m_bThreadRun;   //! �����尡 �������ΰ�?
public:
	//! -----------------------------------------------------------------------
	//! Virtual Function	
	virtual void process() = 0;
	virtual void endThread();

public:
	void threadFunc();

protected:
	bool startThread();
	void runThread();	

}; // class EventThread

#endif // _EVENT_THREAD_H_
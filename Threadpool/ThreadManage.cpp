#include "ThreadManage.h"
#include "MThreadPool.h"
ThreadManage::ThreadManage():max_num(30),max_idlenum(5),init_num(10),min_idlenum(3),m_threadpool(nullptr){
	m_threadpool=new MThreadPool(max_num,max_idlenum,init_num,min_idlenum);
}
ThreadManage::ThreadManage(int maxnum,int maxidle,int initnum,int minidle):max_num(maxnum),max_idlenum(maxidle),init_num(initnum),min_idlenum(minidle),m_threadpool(nullptr){
	m_threadpool=new MThreadPool(max_num,max_idlenum,init_num,min_idlenum);
}

ThreadManage::~ThreadManage(){
	if(m_threadpool!=nullptr)
		delete m_threadpool;
}

void ThreadManage::Run(Task *mtask,void *mtaskdata){
	if(m_threadpool!=nullptr)
		m_threadpool->Run(mtask,mtaskdata);
}

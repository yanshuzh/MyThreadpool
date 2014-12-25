#include "MThread.h"
#include "Task.h"
#include "MThreadPool.h"
#include <iostream>
MThread::MThread():m_task(NULL),m_thread(0),m_stop(false),m_pthreadMutex(PTHREAD_MUTEX_INITIALIZER),
m_pthreadCond(PTHREAD_COND_INITIALIZER),m_taskdata(NULL),m_threadpool(NULL){
	 if(pthread_create(&m_thread,NULL,worker,this)!=0){
	 	std::cout<<"create this thread error!!"<<std::endl;
	 }
}
MThread::~MThread(){
	//std::cout<<"Mthread end!!"<<std::endl;
	end();
}

void *MThread::worker(void *arg){
	MThread *mthread=(MThread *)arg;
	mthread->Run();
	//return mthread;
	return mthread;
}

void MThread::Run(){
	while(!m_stop){
		pthread_mutex_lock(&m_pthreadMutex);
		if(NULL==m_task)
		{
			pthread_cond_wait(&m_pthreadCond,&m_pthreadMutex);
		}
		m_task->Run(m_taskdata);
		//m_task->SetMThread(NULL);
		//std::cout<<"finsh task"<<std::endl;
		m_task = NULL;
		m_threadpool->MoveToIdle(this);//fish the task move to idle list
		pthread_mutex_unlock(&m_pthreadMutex);
	}
}
Task *MThread::GetTask() const{
	return m_task;
}

void MThread::SetTask(Task *mtask,void *mtaskdata){
	m_task=mtask;
	m_taskdata=mtaskdata;
	//m_task->SetMThread(this);
	//std::cout<<"2222"<<"11"<<std::endl;
	pthread_cond_signal(&m_pthreadCond);

}

void MThread::SetThreadpool(MThreadPool *mthreadpool){
	m_threadpool=mthreadpool;
}

void MThread::end(){
	//std::cout<<"into mthread end"<<std::endl;
	m_stop=true;
	/*
	NO while will lead to memory problem.
	*/
	while(!pthread_cancel(m_thread));
	pthread_join(m_thread,NULL);
}
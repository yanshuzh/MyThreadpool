#include "MThreadPool.h"
#include "MThread.h"
#include "Task.h"
#include <algorithm>
#include <iostream>
MThreadPool::MThreadPool():max_num(30),max_idlenum(5),init_num(15),min_idlenum(3),cur_num(0){
 	m_BusyMutex=PTHREAD_MUTEX_INITIALIZER;
 	m_IdleMutex=PTHREAD_MUTEX_INITIALIZER;
 	m_IdleCond=PTHREAD_COND_INITIALIZER;
 	m_BusyCond=PTHREAD_COND_INITIALIZER;
 	CreateThread(init_num);

 }
 MThreadPool::MThreadPool(int maxnum,int maxidle,int initnum,int minidle):max_num(maxnum),max_idlenum(maxidle),init_num(initnum),min_idlenum(minidle),cur_num(0){
 	m_BusyMutex=PTHREAD_MUTEX_INITIALIZER;
 	m_IdleMutex=PTHREAD_MUTEX_INITIALIZER;
 	m_IdleCond=PTHREAD_COND_INITIALIZER;
 	m_BusyCond=PTHREAD_COND_INITIALIZER;
 	CreateThread(init_num);

 }

MThreadPool::~MThreadPool(){
	Destory();
}

void MThreadPool::CreateThread(int num){
	//pthread_mutex_lock(&m_IdleMutex);
	std::cout<<"Create thread"<<std::endl;
	MThread *mthread=NULL;
 	for(int i=0;i<num;i++){
 		mthread=new MThread();
 		mthread->SetThreadpool(this);
 		m_idlelist.push_back(mthread);
 		cur_num++;			
 	}
 	mthread=NULL;
 	//pthread_mutex_unlock(&m_IdleMutex);
}

void MThreadPool::DeleteThread(int num){
 	//pthread_mutex_lock(&m_IdleMutex);
 	//std::cout<<"Delete idle thread"<<std::endl;
 	//std::cout<<"idle:"<<m_idlelist.size()<<std::endl;
 	//std::cout<<"busy:"<<m_busylist.size()<<std::endl;
 	//std::cout<<"current:"<<cur_num<<std::endl;
 	MThread *temp;
 	for(int i=0;i<num;i++){
 		std::list<MThread*>::iterator it=m_idlelist.begin();
 		temp=*(it);
 		m_idlelist.erase(it);
 		cur_num--;
 		delete temp;
 		temp=NULL;
 	}
 	//pthread_mutex_unlock(&m_IdleMutex);
}

void MThreadPool::MoveToIdle(MThread *mthread){
	pthread_mutex_lock(&m_BusyMutex);
	//std::cout<<"move to idle"<<std::endl;
	std::list<MThread*>::iterator it;
	//std::cout<<"m_busylist size:"<<m_busylist.size()<<std::endl;
	it=find(m_busylist.begin(),m_busylist.end(),mthread);
	if(it!=m_busylist.end()){
		//std::cout<<"m_busylist erase"<<std::endl;
		m_busylist.erase(it);
	}
	pthread_mutex_unlock(&m_BusyMutex);
	pthread_mutex_lock(&m_IdleMutex);
	//std::cout<<"m_idlelist size:"<<m_idlelist.size()<<std::endl;
	m_idlelist.push_back(mthread);
	//std::cout<<"m_idlelist size:"<<m_idlelist.size()<<std::endl;
	if(m_idlelist.size()>max_idlenum){
		//pthread_mutex_unlock(&m_IdleMutex);
		//std::cout<<"delete idle thread"<<std::endl;
		DeleteThread(m_idlelist.size()-max_idlenum);
		pthread_mutex_unlock(&m_IdleMutex);
	}
	else{
		pthread_mutex_unlock(&m_IdleMutex);
	}
	pthread_cond_signal(&m_IdleCond);
}

void MThreadPool::MoveToBusy(MThread *mthread){
	pthread_mutex_lock(&m_BusyMutex);
	m_busylist.push_back(mthread);
	pthread_mutex_unlock(&m_BusyMutex);
	pthread_mutex_lock(&m_IdleMutex);
	std::list<MThread*>::iterator it;
	it=find(m_idlelist.begin(),m_idlelist.end(),mthread);
	if(it!=m_idlelist.end()){
		m_idlelist.erase(it);
	}
	pthread_mutex_unlock(&m_IdleMutex);
}
/*
Get an idle thread fore idlelist.
JUdge whether the idlelist is null,if it's null then create new thread.
If the number of all threads is the max,then wait until a task finish.
*/
MThread *MThreadPool::GetIdleThread(){
	pthread_mutex_lock(&m_IdleMutex);	
	if(m_idlelist.size()==0&&max_num!=cur_num){
		if(cur_num+min_idlenum<=max_num)
			CreateThread(min_idlenum);
		else
			CreateThread(max_num-cur_num);
	}
	while(m_idlelist.size()==0&&cur_num==max_num){
			pthread_cond_wait(&m_IdleCond,&m_IdleMutex);
		}
	MThread *mthread=m_idlelist.front();
	pthread_mutex_unlock(&m_IdleMutex);
	return mthread;
}
/*
Bind the task with the selected thread,and move the thread to busy list.
*/
void MThreadPool::Run(Task *mtask,void *mtaskdata){

	MThread *mthread=GetIdleThread();
	//std::cout<<"idlesize:"<<m_idlelist.size()<<std::endl;
	//std::cout<<"busysize:"<<m_busylist.size()<<std::endl;
	if(mthread!=NULL){
		MoveToBusy(mthread);
		mthread->SetThreadpool(this);
		//mtask->SetMThread(mthread);
		mthread->SetTask(mtask,mtaskdata);
	}
}
/*
kill all threads and free the 
*/
void MThreadPool::Destory(){
	std::list<MThread*>::iterator it;
	for(it=m_busylist.begin();it!=m_busylist.end();it++){
		(*it)->end();
		
	}
	for(it=m_idlelist.begin();it!=m_idlelist.end();it++){
		(*it)->end();
		delete *it;
	}
}

#ifndef MTHREADPOOL_H
#define MTHREADPOOL_H
#include <list>
#include <pthread.h>

class MThread;
class Task;
class MThreadPool{
public:
	MThreadPool();
	MThreadPool(int maxnum,int maxidle,int initnum,int minidle);
	~MThreadPool();
	void MoveToIdle(MThread *mthread);
	void MoveToBusy(MThread *mthread);
	void Run(Task *mtask,void *mtaskdata);
	void CreateThread(int num);
	void DeleteThread(int num);
	MThread *GetIdleThread();
	void Destory();


private:
	std::list<MThread*> m_busylist;// busy thread list
	std::list<MThread*> m_idlelist;// idle thread list
	unsigned int max_num;
	unsigned int max_idlenum;
	unsigned int init_num;
	unsigned int min_idlenum;
	unsigned int cur_num;
	
	pthread_mutex_t m_BusyMutex;
	pthread_mutex_t m_IdleMutex;
	pthread_cond_t m_IdleCond;
	pthread_cond_t m_BusyCond;
};

#endif
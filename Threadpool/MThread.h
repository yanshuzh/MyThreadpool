#ifndef MTHREAD_H
#define MTHREAD_H
#include <pthread.h>
class Task;
class MThreadPool;
class MThread{
public:
	MThread();
	~MThread();
	void Run();
	void SetTask(Task *mtask,void *mtaskdata);
	Task *GetTask() const;
	void SetThreadpool(MThreadPool *mthreadpool);
	void end();
private:
	static void *worker(void *arg);
private:
	Task *m_task;
	pthread_t m_thread;
	bool m_stop;
	pthread_mutex_t m_pthreadMutex;
	pthread_cond_t m_pthreadCond;
	void *m_taskdata;
	MThreadPool *m_threadpool;

};
#endif
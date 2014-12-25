#ifndef TASK_H
#define TASK_H
class MThread;
class Task{

public:
	Task();
	virtual ~Task();
	virtual void Run(void *arg)=0;
	//MThread *GetMThtead(void);
	//void SetMThread(MThread *mthread);
private:
	//MThread *m_thread;
};
#endif
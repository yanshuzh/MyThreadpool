#ifndef THREADMANAGE_H
#define THREADMANAGE_H
class MThreadPool;
class Task;
class ThreadManage{
public:
	ThreadManage();
	ThreadManage(int maxnum,int maxidle,int initnum,int minidle);
	virtual ~ThreadManage();
	void Run(Task *mtask,void *mtaskdata);
private:
	unsigned int max_num;
	unsigned int max_idlenum;
	unsigned int init_num;
	unsigned int min_idlenum;
	MThreadPool *m_threadpool;
};
#endif
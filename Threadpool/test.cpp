#include <iostream>
#include <unistd.h>
#include "ThreadManage.h"
#include "Task.h"
#include "MThread.h"
#include "MThreadPool.h"
using namespace std;
class MTask1:public Task{
public:
	MTask1(){};
	~MTask1(){};
	void Run(void *arg){
		//while(1);
		cout<<"task test test test test"<<endl;
	};
};
class MTask2:public Task{
public:
	MTask2(){};
	~MTask2(){};
	void Run(void *arg){
		cout<<"task test 2"<<endl;
	};
};
int main(){
/*	ThreadManage *thmanage=new ThreadManage(10);

	*/
	//while(1){
		ThreadManage *thmanage=new ThreadManage(30,5,10,3);
		Task *task1=new MTask1;
		Task *task2=new MTask2;
	while(1){
		for(int i=0;i<40;i++){
			thmanage->Run(task1,NULL);
		}
		
		for(int i=0;i<60;i++){
			thmanage->Run(task2,NULL);	
		}
		
	}
		//delete thmanage;
		//sleep(2);
	
	//}
		//while(1);

}
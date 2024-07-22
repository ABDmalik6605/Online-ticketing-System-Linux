#include <iostream>
using namespace std;
#include <semaphore.h>
#include <sys/shm.h>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    int id=134;
    int shm = shmget(id,21,0666);
    if(shm == -1)
    {
        perror("shm: ");
        return 1;
    }
    int sema1_id = shmget(id+1,1024,0666);
    if(sema1_id == -1)
    {
        shmctl(shm,IPC_RMID,NULL);
        perror("sema1_id: ");
        return 1;
    }
    int sema2_id = shmget(id+2,1024,0666);
    if(sema2_id == -1)
    {
        shmctl(shm,IPC_RMID,NULL);
        shmctl(sema1_id,IPC_RMID,NULL);
        perror("sema2_id: ");
        return 1;
    }
    sem_t * sema1;
    sema1 = (sem_t*) shmat(sema1_id,NULL,0);

    sem_t * sema2;
    sema2 = (sem_t*) shmat(sema2_id,NULL,0);

    char *shared;
    if(shm == -1)
    {
        perror("shm: ");
        return 1;
    }
    shared = (char*)shmat(shm,NULL,0);
    int total;char tot[5];
    sem_wait(sema2);
    for (int i=0;shared[i]!='\0';i++)
    {
	tot[i]=shared[i];tot[i+1]='\0';
    }
    total=atoi(tot);sem_post(sema1);sem_wait(sema2);
    cout<<shared<<endl;
    sem_post(sema1);
    for (int i=0;i<total;i++)
    {
        sem_wait(sema2);
	cout<<"["<<i+1<<"]";
	for (int j=0;shared[j]!='\0';j++)
            cout<<shared[j];
	cout<<endl;
        sem_post(sema1);
    }
    bool checking=0;
    while (checking==0)
    {
	string s="";cout<<"Enter movie"<<endl;cin>>s;
	for (int i=0;s[i]!='\0';i++){
		shared[i]=s[i];shared[i+1]='\0';
	}
	sem_post(sema1);sem_wait(sema2);
	if (shared[0]!='0')
	{
		cout<<"Enter seats"<<endl;cin>>s;
		for (int i=0;s[i]!='\0';i++){
			shared[i]=s[i];shared[i+1]='\0';
		}
		sem_post(sema1);sem_wait(sema2);
		if (shared[0]!='1')
		{
			cout<<shared<<endl;checking=1;
		}
	}
    }
    bool loop=0;char choice;
    while (loop==0){
    	cin>>choice;
	if (choice=='Y' || choice=='N' || choice=='y' || choice=='n')
		loop=1;
	}
    shared[0]=choice;sem_post(sema1);
    if (choice=='Y' || choice=='y')
    {
	sem_wait(sema2);cout<<"Enter Cnic (One-by-One)"<<endl;
	char cnic[4];
	cin>>cnic[0];cin>>cnic[1];cin>>cnic[2];cin>>cnic[3];
	for (int i=0;i<4;i++)
	shared[i]=cnic[i];
	shared[4]='\0';
	sem_post(sema1);
    }
    sem_wait(sema2);
    cout<<shared<<endl;
    if(shmdt(shared) == -1)
    {
        perror("dt shared: ");
    }
    if(shmdt(sema1) == -1)
    {
        perror("sema1 shared: ");
    }
    if(shmdt(sema2) == -1)
    {
        perror("sema2 shared: ");
    }

    return 0;
}

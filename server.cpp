#include <iostream>
using namespace std;
#include <semaphore.h>
#include <sys/shm.h>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int args,char* argv[])
{
    if(args == 1)
    {
        cout<<"format for running ./exe_file file.txt"<<endl;
        return 0;
    }
    char buffer[BUFSIZ];int id=134;
    int fd = open(argv[1],O_RDONLY,0666);
    if (fd==-1)
    {
	cout<<"error opening the file"<<endl;return 0;
    }
    int r=read(fd,buffer,BUFSIZ);close(fd);
    if (r==-1)
    {
	cout<<"error reading the file"<<endl;return 0;
    }
	buffer[r]='\0';
    char temp[5];
    for (int i=0;buffer[i]!='\n';i++){
	temp[i]=buffer[i];
	temp[i+1]=0;
	}
   int total=atoi(temp);char movies[total][100];int j=0;int k=0;
   int *seats=new int [total];
   while (buffer[j]!='\0')
    {
	if (buffer[j]=='/')
	{
		int i=j+1;
		for (int l=0;buffer[i]!='/';l++,i++)
		{
			movies[k][l]=buffer[i];
			movies[k][l+1]='\n';
			movies[k][l+2]='\0';
		}
		char temp2[5];i++;
		for (int l=0;buffer[i]!='\n';l++,i++)
		{
			temp2[l]=buffer[i];
			temp2[l+1]='\0';
		}
		seats[k]=atoi(temp2);
		j=i+1;k++;
	}
	j++;
    }
    int shm = shmget(id,21,0666|IPC_CREAT|IPC_EXCL);
    if(shm == -1)
    {
        perror("shm: ");
        return 1;
    }
    int sema1_id = shmget(id+1,1024,0666|IPC_CREAT|IPC_EXCL);
    if(sema1_id == -1)
    {
        shmctl(shm,IPC_RMID,NULL);
        perror("sema1_id: ");
        return 1;
    }
    int sema2_id = shmget(id+2,1024,0666|IPC_CREAT|IPC_EXCL);
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

    sem_init(sema1,1,0);
    sem_init(sema2,1,0);
    for (int i=0;temp[i]!='\0';i++)
    {
	shared[i]=temp[i];
	shared[i+1]='\0';
    } 
    sem_post(sema2);
    sem_wait(sema1);
    string welcome="--------------WELCOME TO MOVIE WORLD------------------";
    for (int i=0;welcome[i]!='\0';i++)
    {
	shared[i]=welcome[i];
	shared[i+1]='\0';
    }
    sem_post(sema2);
    for (int i=0;i<total;i++)
    {
        sem_wait(sema1);
	for (int j=0;movies[i][j]!='\n';j++)
	{
		shared[j]=movies[i][j];
		shared[j+1]='\0';
	}
	 sem_post(sema2);
    }
    sem_wait(sema1);sem_wait(sema1);
    string s;bool f=0;int m;int s1;
    while (f==0){
	cout<<s;m=0;
	for (int i=0;shared[i]!='\0';i++)
	{
		s[i]=shared[i];s[i+1]='\0';
	}
	m=stoi(s);m--;f=1;
	if (m>=total){
		cout<<"Invalid movie number "<<m<<endl;
		shared[0]='0';f=0;shared[1]='\0';
		sem_post(sema2);sem_wait(sema1);
	}
	else
	{
		sem_post(sema2);sem_wait(sema1);
		for (int i=0;shared[i]!='\0';i++)
		{
			s[i]=shared[i];s[i+1]='\0';
		}
		s1=stoi(s);
		cout<<"Client has chosen "<<s1<<" tickets for "<<movies[m]<<endl;
		if (s1>seats[m])
		{
			cout<<"Not enough seats available"<<endl;f=0;shared[0]='1';
			shared[1]='\0';
			sem_post(sema2);sem_wait(sema1);
		}
        }
    }
	string bill="Your total bill for the show ";int l=0;
	for (;bill[l]!='\0';l++)
	{
		shared[l]=bill[l];
	}
	for (int j=0;movies[m][j]!='\n';j++,l++)
	{
		shared[l]=movies[m][j];shared[l+1]=' ';
	}
	l++;shared[l]='i';l++;shared[l]='s';l++;shared[l]=' ';l++;
	bill=to_string(s1*500);
	for (int i=0;bill[i]!='\0';i++,l++)
	{
		shared[l]=bill[i];shared[l+1]='\n';
	}
	string check="Press Y to proceed or press N to cancel";l++;
	for (int i=0;check[i]!='\0';i++,l++)
	{
		shared[l]=check[i];shared[l+1]='\0';
	}
	sem_post(sema2);
	sem_wait(sema1);
	if (shared[0]=='Y' || shared[0]=='y')
	{
	seats[m]=seats[m]-s1;int tot=total;
	if (seats[m]==0)
		tot--;
	pid_t pid=fork();
	if (pid==-1)
	{
		cout<<"error creating child process"<<endl;
		return 1;
	}
	else if (pid==0){
		fd = open(argv[1],O_WRONLY|O_TRUNC|O_APPEND,0777);
   		dup2(fd,1);
		cout<<tot<<endl;int j=0;
		for (int i=0;i<total;i++)
		{
			if (seats[i]>0)
			{
				cout<<j+1<<"/";
				for (int j=0;movies[i][j]!='\n';j++)
					cout<<movies[i][j];
				cout<<"/"<<seats[i]<<endl;j++;
			}
		}
		exit(0);
	}
	else{
		wait(0);
		sem_post(sema2);
		sem_wait(sema1);bool flag=1;
		pid_t pid2=fork();
		if (pid2==-1)
		{
			cout<<"error creating child process"<<endl;flag=0;
			return 1;
		}
		else if (pid2==0)
		{
			int fd2 = open("Bookinginfo.txt",O_RDONLY|O_CREAT,0777);
			if (fd2==-1) 
				flag=0;
			char s[100];
			int r=read(fd2,s,100);close(fd2);
			pid_t pid3=fork();
			if (pid3==-1)
			{
				cout<<"error creating child process"<<endl;
				flag=0;return 1;
			}
			else if (pid3==0){
			int fd3 = open("Bookinginfo.txt",O_WRONLY|O_APPEND|O_TRUNC,0777);
			if (fd3==-1) 
				flag=0;
			if (dup2(fd3,1)==-1)
			{
				cout<<"Error redirecting the file"<<endl;
				flag=0;return 1;
			}
			if (r>0){
				char t[5];int i=0;
				for (;s[i]!='\n';i++)
				{	t[i]=s[i];t[i+1]='\0';
				}
				int num=atoi(t);
				cout<<num+1;
				for (;s[i]!='\0';i++)
					cout<<s[i];
			}
			else 
				cout<<"1"<<endl;
			cout<<shared<<"/"<<m+1<<"/"<<s1<<endl;close(fd3);
			pid_t pid4=fork();
			if (pid4==-1)
			{
				cout<<"error creating child process"<<endl;
				return 1;flag=0;
			}
			else if (pid4==0)
			{
				string result;
				if (flag==1)
					result="Booking completed";
				else 
					result="Booking failed";
				for (int i=0;result[i]!='\0';i++)
				{
					shared[i]=result[i];shared[i+1]='\0';
				}
				sem_post(sema2);
				_exit(0);
			}
			else 
				wait(0);
			_exit(0);
			}
			else
				wait(0);
		_exit(0);
		}
	}
    }
    sem_post(sema2);
  //  sem_wait(sema1);
//	cout<<shared[0];
    sem_destroy(sema1);
    sem_destroy(sema2);

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
    //not returning becase we still need to deallocate

    if(shmctl(shm,IPC_RMID,NULL) == -1)
    {
        perror("shm ctl shared: ");
    }
    if(shmctl(sema1_id,IPC_RMID,NULL) == -1)
        perror("sema1 ctl shared: ");
    if(shmctl(sema2_id,IPC_RMID,NULL) == -1)
        perror("sema2 ctl shared: ");
    return 0;
}

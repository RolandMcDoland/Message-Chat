
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>//ipc
#include <sys/ipc.h>//ipc
#include <sys/msg.h>//ipc
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct msgbuf
{
    long type;
    int id;
    char username[64];
    char password[64];
    char group_name[64];
    char message[1024];
}msg1,msg2,msg5,msg7,msg9,msg11,msg15,msg17,msg19,msg20,msg22,msg24,msg26,msg27,msg28,msg30,msg31;

struct user
{
    int id;
    char username[64];
    char password[64];
    int logged;
    int id_q;
}user;

struct group
{
    char group_name[64];
    int id_list[9];
}group;

int main(void)
{
    int id,id1,fd,id2,index;
    struct user user_list[9];
    struct group group_list[3];
    FILE* file=fopen("setup.txt","r");
	char buf[64];
	id2=234567;
	for (int i=0;i<9;i++)
	{
		fgets(buf,64,file);
		buf[strlen(buf)-1]='\0';
		strcpy(user_list[i].username,buf);
		fgets(buf,64,file);
		buf[strlen(buf)-1]='\0';
		strcpy(user_list[i].password,buf);
		user_list[i].id=id2;
		user_list[i].logged=0;
		id2=id2+10;
	}
	for (int i=0;i<3;i++)
	{
		fgets(buf,64,file);
		buf[strlen(buf)-1]='\0';
		strcpy(group_list[i].group_name, buf);
		for (int j=0;j<9;j++)
		{
			group_list[i].id_list[j]=-1;
		}
		for (int j=0;j<2;j++)
		{
			fgets(buf,64,file);
            buf[strlen(buf)-1]='\0';
            index=atoi(buf);
            group_list[i].id_list[index]=user_list[index].id;
		}
	}
	fclose(file);
    id=msgget(12345678,IPC_CREAT|0644);
    for(int i=0;i<9;i++)
    {
        id1=msgget(user_list[i].id,IPC_CREAT|0644);
        user_list[i].id_q=id1;
    }
    while(1)
    {
        for(int j=0;j<9;j++)
        {
        id1=user_list[j].id_q;
        int nowait=msgrcv(id, &msg1, (sizeof(msg1)-sizeof(long)),1, IPC_NOWAIT);
        if(nowait!=-1)
        {
            int n=0;
            for(int i=0;i<9;i++)
            {
                printf("Porownuje %s z %s\n",msg1.username,user_list[i].username);
                printf("%d\n",strcmp(msg1.username,user_list[i].username));
                if(strncmp(msg1.username,user_list[i].username,strlen(msg1.username))==0)
                {
                    printf("Sukces\n");
                    printf("Porownuje %s z %s\n",msg1.password,user_list[i].password);
                    if(strncmp(msg1.password,user_list[i].password,strlen(msg1.password))==0)
                    {
                        printf("Sukces\n");
                        user_list[i].logged=1;
                        msg2.type=10;
                        msg2.id=user_list[i].id;
                        strcpy(msg2.message,"Zalogowano");
                        msgsnd(id,&msg2,(sizeof(msg2)-sizeof(long)),0);
                    }
                }
                else n++;
            }
            if(n==9)
            {
                msg2.type=10;
                strcpy(msg2.message,"Zla nazwa uzytkownika lub haslo");
                msgsnd(id,&msg2,(sizeof(msg2)-sizeof(long)),0);
            }
        }
        nowait=msgrcv(id1, &msg5, (sizeof(msg5)-sizeof(long)),3, IPC_NOWAIT);
        if(nowait!=-1)
        {
            for(int i=0;i<3;i++)
            {
                printf("Porownuje %s z %s\n",msg5.group_name,group_list[i].group_name);
                if(strncmp(msg5.group_name,group_list[i].group_name,strlen(msg5.group_name))==0)
                {
                    printf("Sukces\n");
                    for(int j=0;j<9;j++)
                    {
                        printf("Porownuje %d z -1\n",group_list[i].id_list[j]);
                        if(group_list[i].id_list[j]!=-1)
                        {
                            strcat(msg24.message,user_list[j].username);
                            strcat(msg24.message,"\t");
                        }
                    }
                }
            }
            msg24.type=10;
            msgsnd(id1,&msg24,(sizeof(msg24)-sizeof(long)),0);
        }
        nowait=msgrcv(id1, &msg7, (sizeof(msg7)-sizeof(long)),4, IPC_NOWAIT);
        if(nowait!=-1)
        {
            int n=0;
            for(int i=0;i<9;i++)
            {
                if(msg7.id==user_list[i].id)
                {
                    for(int j=0;j<3;j++)
                    {
                        if(strcmp(msg7.group_name,group_list[j].group_name)==0)
                        {
                            if(group_list[j].id_list[i]!=-1)
                            {
                                group_list[j].id_list[i]=-1;
                                msg28.type=10;
                                strcpy(msg28.message,"Wypisano do grupy");
                                msgsnd(id1,&msg28,(sizeof(msg28)-sizeof(long)),0);
                                n=1;
                            }
                            else
                            {
                                group_list[j].id_list[i]=msg7.id;
                                msg28.type=10;
                                strcpy(msg28.message,"Zapisano z grupy");
                                msgsnd(id1,&msg28,(sizeof(msg28)-sizeof(long)),0);
                                n=1;
                            }
                        }
                    }
                }
            }
            if(n!=1)
            {
                msg28.type=10;
                strcpy(msg28.message,"Brak grupy o podanej nazwie");
                msgsnd(id1,&msg28,(sizeof(msg28)-sizeof(long)),0);
            }
        }
        nowait=msgrcv(id1, &msg9, (sizeof(msg9)-sizeof(long)),6, IPC_NOWAIT);
        if(nowait!=-1)
        {
            int n=0;
            char username[25];
            for(int i=0;i<9;i++)
            {
                if(msg9.id==user_list[i].id)
                {
                    strcpy(username,user_list[i].username);
                }
            }
            for(int i=0;i<9;i++)
            {
                if(strcmp(msg9.username,user_list[i].username)==0)
                {
                    msg26.type=11;
                    strcpy(msg26.message,msg9.message);
                    strcpy(msg26.username,username);
                    msgsnd(user_list[i].id_q,&msg26,(sizeof(msg26)-sizeof(long)),0);
                    msg30.type=10;
                    msg30.id=user_list[i].id;
                    strcpy(msg30.message,"Wyslano");
                    msgsnd(id1,&msg30,(sizeof(msg30)-sizeof(long)),0);
                    n=1;
                }
            }
            if(n!=1)
            {
                msg30.type=10;
                strcpy(msg30.message,"Brak uzytkownika o podanej nazwie");
                msgsnd(id1,&msg30,(sizeof(msg30)-sizeof(long)),0);
            }
        }
        nowait=msgrcv(id1, &msg11, (sizeof(msg11)-sizeof(long)),7, IPC_NOWAIT);
        if(nowait!=-1)
        {
            int n=0;
            char username[25];
            for(int i=0;i<9;i++)
            {
                if(msg11.id==user_list[i].id)
                {
                    strcpy(username,user_list[i].username);
                }
            }
            printf("%s\n",username);
            for(int i=0;i<3;i++)
            {
                if(strcmp(msg11.group_name,group_list[i].group_name)==0)
                {
                    n=1;
                    msg31.type=10;
                    msg31.id=user_list[i].id;
                    strcpy(msg31.message,"Wyslano");
                    msgsnd(id1,&msg31,(sizeof(msg31)-sizeof(long)),0);
                    msg27.type=11;
                    strcpy(msg27.message,msg11.message);
                    strcpy(msg27.username,username);
                    for(int j=0;j<9;j++)
                    {
                        if(group_list[i].id_list[j]!=-1)
                        {
                            msgsnd(user_list[j].id_q,&msg27,(sizeof(msg27)-sizeof(long)),0);
                        }
                    }
                }
            }
            if(n!=1)
            {
                msg30.type=10;
                strcpy(msg30.message,"Brak grupy o podanej nazwie");
                msgsnd(id1,&msg30,(sizeof(msg30)-sizeof(long)),0);
            }
        }
        nowait=msgrcv(id1, &msg15, (sizeof(msg15)-sizeof(long)),2, IPC_NOWAIT);
        if(nowait!=-1)
        {
            for(int i=0;i<9;i++)
            {
                if(user_list[i].logged==1)
                {
                    strcat(msg20.message,user_list[i].username);
                    strcat(msg20.message,"\t");
                }
            }
            msg20.type=10;
            msgsnd(id1,&msg20,(sizeof(msg20)-sizeof(long)),0);
        }
        nowait=msgrcv(id1, &msg17, (sizeof(msg17)-sizeof(long)),5, IPC_NOWAIT);
        if(nowait!=-1)
        {
            /*int n=0;
            char buf[1024];*/
            for(int i=0;i<3;i++)
            {
                /*for(int j=0;j<strlen(group_list[i].group_name);j++)
                {
                    buf[n]=group_list[i].group_name[j];
                    printf("%d %c\n",n,buf[n]);
                    n++;
                }
                buf[n]='\t';
                n++;*/
                strcat(msg22.message,group_list[i].group_name);
                strcat(msg22.message,"\t");
                printf("%s\n",group_list[i].group_name);
                printf("%d\n",strlen(group_list[i].group_name));
                printf("%s\n",buf);
            }
            msg22.type=10;
            msgsnd(id1,&msg22,(sizeof(msg22)-sizeof(long)),0);
        }
        nowait=msgrcv(id1, &msg19, (sizeof(msg19)-sizeof(long)),9, IPC_NOWAIT);
        if(nowait!=-1)
        {
            for(int i=0;i<9;i++)
            {
                if(msg1.id==user_list[i].id)
                {
                    user_list[i].logged=0;
                }
            }
        }
        }
    }
    return 0;
}

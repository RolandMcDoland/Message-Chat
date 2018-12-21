#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>//ipc
#include <sys/ipc.h>//ipc
#include <sys/msg.h>//ipc
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

struct msgbuf
{
    long type;
    int id;
    char username[64];
    char password[64];
    char group_name[64];
    char message[1024];
}msg,msg3,msg4,msg6,msg8,msg10,msg13,msg14,msg16,msg18,msg21,msg23,msg25,msg29,msg32,msg33;

int main(void)
{
    int choice,nowait;
    int id,id1,id2;
    id1 = msgget(12345678,IPC_CREAT|0644);
    printf("%d\n",id1);
    while(1)
    {
    printf("Menu: \n1-logowanie\n2-podglad listy zalogowanych\n3-podglad listy zapisanych do grupy\n4-zapisanie/wypisanie z grupy\n5-podglad listy grup\n6-wyslij wiadomosc do uzytkownika\n7-wyslij wiadomosc do grupy\n8-odbierz wiadomosci\n9-wyloguj\n10-zamknij\n");
    scanf("%d",&choice);
    switch(choice)
    {
    case 1:
        printf("Nazwa uzytkownika: ");
        scanf("%s",&msg.username);
        printf("Haslo: ");
        scanf("%s",&msg.password);
        msg.type=1;
        msgsnd(id1,&msg,(sizeof(msg)-sizeof(long)),0);
        msgrcv(id1, &msg3, (sizeof(msg3)-sizeof(long)),10, 0);
        printf("%s\n",msg3.message);
        id=msg3.id;
        printf("%d\n",id);
        id2 = msgget(id,IPC_CREAT|0644);
        break;
    case 2:
        msg14.id=id;
        msg14.type=2;
        msgsnd(id2,&msg14,(sizeof(msg14)-sizeof(long)),0);
        msgrcv(id2, &msg21, (sizeof(msg21)-sizeof(long)),10, 0);
        printf("%s\n",msg21.message);
        break;
    case 3:
        printf("Podaj nazwe grupy, ktorej uzytkownikow chcesz zobaczyc: ");
        scanf("%s",&msg4.group_name);
        msg4.id=id;
        msg4.type=3;
        msgsnd(id2,&msg4,(sizeof(msg4)-sizeof(long)),0);
        msgrcv(id2, &msg25, (sizeof(msg25)-sizeof(long)),10, 0);
        printf("%s\n",msg25.message);
        break;
    case 4:
        printf("Podaj nazwe grupy, do ktorej chcesz sie zapisac/ z ktorej chcesz sie wypisac: ");
        scanf("%s",&msg6.group_name);
        msg6.id=id;
        msg6.type=4;
        msgsnd(id2,&msg6,(sizeof(msg6)-sizeof(long)),0);
        msgrcv(id2, &msg29, (sizeof(msg29)-sizeof(long)),10, 0);
        printf("%s\n",msg29.message);
        break;
    case 5:
        msg16.id=id;
        msg16.type=5;
        msgsnd(id2,&msg16,(sizeof(msg16)-sizeof(long)),0);
        msgrcv(id2, &msg23, (sizeof(msg23)-sizeof(long)),10, 0);
        printf("%s\n",msg23.message);
        break;
    case 6:
        printf("Podaj nazwe uzytkownika, do ktorego chcesz wyslac wiadomosc: ");
        scanf("%s",&msg8.username);
        printf("Podaj wiadomosc:\n");
        scanf(" %[^\t\n]s",&msg8.message);
        msg8.id=id;
        msg8.type=6;
        msgsnd(id2,&msg8,(sizeof(msg8)-sizeof(long)),0);
        msgrcv(id2,&msg32, (sizeof(msg32)-sizeof(long)),10, 0);
        printf("%s\n",msg32.message);
        break;
    case 7:
        printf("Podaj nazwe grupy, do ktorej chcesz wyslac wiadomosc: ");
        scanf("%s",&msg10.group_name);
        printf("Podaj wiadomosc:\n");
        scanf(" %[^\t\n]s",&msg10.message);
        msg10.id=id;
        msg10.type=7;
        msgsnd(id2,&msg10,(sizeof(msg10)-sizeof(long)),0);
        msgrcv(id2, &msg33, (sizeof(msg33)-sizeof(long)),10, 0);
        printf("%s\n",msg33.message);
        break;
    case 8:
        while(msgrcv(id2, &msg13, (sizeof(msg13)-sizeof(long)),11, IPC_NOWAIT)>0)
        {
            printf("%s says:\n%s\n",msg13.username,msg13.message);
        }
        break;
    case 9:
        msg18.id=id;
        msg18.type=9;
        msgsnd(id2,&msg18,(sizeof(msg18)-sizeof(long)),0);
        break;
    case 10:
        exit(0);
        break;
    }
    }
    return 0;
}

/*
// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
// Tue Jun 30 13:55:20 WIB 2020
 */

#include     "share.h"
char*        progs[]={P01, P02, P03, P04, P05, P06};
char         tmpStr[256]={};
extern char* akunGitHub;
extern int   delay;
extern int   boss;
myshare*     mymap;

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
void init(int isboss, int argc, char* argv[]) {
    if (isboss == BOSS) {
        int ssize=sizeof(myshare);
        if (access(SHAREMEM, F_OK ) == -1) {
            int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
            fchmod   (fd, CHMOD);
            ftruncate(fd, ssize);
            mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
            close(fd);
            mymap->entry = 0;
            mymap->state = OPEN;
            mymap->mutexctr = 0;
            sem_init (&(mymap->mutex), 1, MUTEX);
        }
        else {
            int fd   =open(SHAREMEM, O_RDWR, CHMOD);
            mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
            close(fd);
        }
        if (argc > 1) {
            mymap->state = OPEN;
            printf("ShareMemory is OPEN, BYE BYE ==== ====\n");
            exit(0);
        }
    } else {
        sleep(delay);
        if( access(SHAREMEM, F_OK ) == -1 ) {
            printf("No \"%s\" file.\n", SHAREMEM);
            exit(-1);
        }
        int fd=open(SHAREMEM, O_RDWR, CHMOD);
        int ssize=sizeof(myshare);
        mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
        close(fd);
    }
}

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
void myPrint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    fflush(NULL);
}

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
// Tue Jun 30 00:50:29 WIB 2020
int getEntry(char* akunGitHub) {
    int entry=-1;
    sem_wait (&(mymap->mutex));
    mymap->mutexctr++;
    int cntEntry;
    cntEntry = mymap->entry;
    usrinfo listOfUser[MAXPROGS+1];
    memcpy(listOfUser, mymap->progs, sizeof(listOfUser));
    for (int j = 0; j < cntEntry; j++) {
        if (listOfUser[j].akun[0] != '\0') {
            if (strcmp(listOfUser[j].akun, akunGitHub) == 0) {
                mymap->progs[j].stamp++;
                entry = j;
                break;
        	}
        }
    }
    if (entry == -1) {
        entry = mymap->entry++;
        mymap->progs[entry].stamp++;
    }
    sem_post (&(mymap->mutex));
    return entry;
}

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
void display(int entry) {
    sem_wait (&(mymap->mutex));
    int cntMutex;
    cntMutex = mymap->mutexctr++;
    int userStamp;
    userStamp = mymap->progs[entry].stamp++;
    printf("%s[progs[%02d] TIME[%02d] MUTEX[%02d] MMAP[OPEN] ", akunGitHub, entry, cntMutex+1, userStamp+1);
    int cntEntry;
    cntEntry = mymap->entry;
    for (int j = 0; j < cntEntry; j++) {
        printf("[%s]", mymap->progs[j].akun);
    }
    printf("]\n");
    sem_post (&(mymap->mutex));
}

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
void putInfo(char* akun, int entry) {
    sem_wait (&(mymap->mutex));
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    strcpy(mymap->progs[entry].akun, akun);
    sem_post (&(mymap->mutex));
}

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
// Mon Jun 29 23:08:43 WIB 2020
void checkOpen(void) {
    int isClosed;
    isClosed = mymap->state;
    if (isClosed == CLOSED) {
        printf("ShareMemory is NOT OPEN, BYE BYE ==== ====\n");
        exit(0);
    }
}

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
// Tue Jun 30 01:38:31 WIB 2020
// Tue Jun 30 14:24:31 WIB 2020
void myWait(int boss, int entry) {
    if (boss == BOSS) {
        for (int k = 0; k < 6; k++) {
            wait(NULL);
        }
        mymap->state = CLOSED;
    }
    else {
        mymap->progs[entry].stamp++;
    }
}

// Contributor: GibranBrahmanta, ryaasabsar, alkwrzm, aditmuhmd, alviadibby, luthfiidrus, naufalhilmi12345
// Harap UPDATE tanggal revisi!
// Tue Jun  9 17:46:47 WIB 2020
// Mon Jun 29 22:53:36 WIB 2020
// Mon Jun 29 23:58:40 WIB 2020
// Tue Jun 30 01:09:12 WIB 2020
// Tue Jun 30 02:42:56 WIB 2020
int main(int argc, char* argv[]) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myPrint(akunGitHub, tmpStr);
    init(boss, argc, argv);
    checkOpen();
    if (boss == BOSS) {
        for (int k = 0; k < 6; k++) {
            if (!fork()) {
                execlp(progs[k], NULL);
            }
        }
    }
    sleep  (delay);
    int entry = getEntry(akunGitHub);
    sleep  (delay);
    display(entry);
    sleep  (delay);
    putInfo(akunGitHub, entry);
    sleep  (delay);
    display(entry);
    myWait (boss, entry);
    myPrint(akunGitHub, "BYEBYE =====  ===== =====");
}


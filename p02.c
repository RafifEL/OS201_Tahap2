/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define  DELAY 6
#define  akunGitHub "egiffari"
#include "p00.h"

char*    progs[]={P01, P02, P03, P04, P05, P06, P07};
myshare* mymap;

int init(void) {
    if (access( SHAREMEM, F_OK ) == -1)
    {
        printf("No \"SharedMemoryFile.bin\" file.\n");
        exit(1);
    }
    sleep(DELAY);
    int ssize=sizeof(myshare);
    int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
    fchmod(fd, CHMOD);
    ftruncate(fd, ssize);
    mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
    return NOTBOSS;
}

char tmpStr[256]={};  // a temporary string.
void myprint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    fflush(NULL);
}

int getEntry(void) {
    sem_wait(&mymap->mutex);
    // get an entry number
    int entry = mymap->entry++;
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    sem_post(&mymap->mutex);
    return entry;
}

void display(int entry) {
    // display an entry of MMAP.
    // eg. akunGH2[progs[03] TIME[18] MUTEX[05] MMAP[OPEN] [akunGH1][akunGH3][akunGH0][akunGH2]]
    sem_wait(&mymap->mutex);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    int mutcounter = mymap->mutexctr;
    int stamp = mymap->progs[entry].stamp;
    char* state;
    switch (mymap->state)
    {
    case OPEN:
        state = "OPEN";
        break;
    default:
        state = "CLOSED";
        break;
    }
    printf("%s[progs[%2.2d] TIME[%2.2d] MUTEX[%2.2d] MMAP[%s]] ", akunGitHub, entry, mutcounter, stamp, state);
    for(int i = 0; i < mymap->entry; i++) {
        printf("[%s]", mymap->progs[i].akun);
    } 
    printf("]\n");
    fflush(NULL);
    sem_post(&mymap->mutex);
}

void putInfo(char* akun, int entry) {
    // put "akunGitHub" into akun[] array (MMAP)
    sem_wait(&mymap->mutex);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    char* account = mymap->progs[entry].akun;
    strcpy(account, akun);
    sem_post(&mymap->mutex);
}

void checkOpen(void) {
    // exit if MMAP is closed.
    if (mymap->state == CLOSED) {
        printf("CLOSED, BYE BYE ==== ====\n");
        exit(1);
    }
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    checkOpen();
    int entry = getEntry();
    display(entry);
    sleep (DELAY);    
    putInfo(akunGitHub, entry);
    display(entry);
    sleep(DELAY);
    display(entry);
    sleep(DELAY);
    wait(NULL);
    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}


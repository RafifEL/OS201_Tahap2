/*
// Sun Jun 21 14:27:37 WIB 2020
// Tue Jun  9 17:46:47 WIB 2020
 */

// adjust DELAY and akunGitHub
#define  DELAY 8
#define  akunGitHub "RafifEL"
#include "p00.h"

char*    progs[]={P01, P02, P03, P04, P05, P06, P07};
myshare* mymap;

int init(void) {
    int ssize=sizeof(myshare);
    int fd   =open(SHAREMEM, MYFLAGS, CHMOD);
    fchmod   (fd, CHMOD);
    ftruncate(fd, ssize);
    mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
    close(fd);
    //  more INIT STUFFs...
    //  more INIT STUFFs...
    //  more INIT STUFFs...
    mymap->state=OPEN;
    mymap->entry=1;
    sem_init(&(mymap->mutex), 1, 1);
    return BOSS;
}

char tmpStr[256]={};  // a temporary string.
void myprint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    // blah blah blah
    // blah blah blah
    // blah blah blah
}

int getEntry(void) {
    sem_wait(&(mymap->mutex));
    int entry;
    // get an entry number
    entry = mymap->entry;
    sem_post(&(mymap->mutex));
    return entry;
}

void display(int entry) {
    // display an entry of MMAP. Eg.
    // akunGH2[progs[03] TIME[18] MUTEX[05] MMAP[OPEN] [akunGH1][akunGH3][akunGH0][akunGH2]]
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
}

void putInfo(char* akun, int entry) {
    // put "akunGitHub" into akun[] array (MMAP) 
    usrinfo account;
    strcpy(account.akun, akun);
    account.stamp = 0;
    mymap->progs[entry] = account;
}

void checkOpen(void) {
    // exit if MMAP is closed.
    if(mymap->state == CLOSED){
        printf("CLOSED, BYEBYE =====  ===== =====");
    }
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    checkOpen();
    sleep (DELAY);
    // blah... blah... blah...
    // blah... blah... blah...
    // blah... blah... blah...
    for (int ii = 0; ii < 7; ii++){
        fork();
        execlp(progs[ii], progs[ii], NULL);
    }
    mymap->state=CLOSED;
    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}


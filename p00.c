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
    mymap->state=OPEN;
    mymap->mutexctr = 0;
    mymap->entry = 0;
    sem_init(&(mymap->mutex), 1, 1);
    for(int ii = 0; ii <= MAXPROGS; ii++){
        strcpy(mymap->progs[ii].akun, "");
        mymap->progs[ii].stamp = 0;
    }
    return BOSS;
}

char tmpStr[256]={};  // a temporary string.
void myprint(char* str1, char* str2) {
    printf("%s[%s]\n", str1, str2);
    fflush(NULL);
}

int getEntry(void) {
    sem_wait(&(mymap->mutex));
    int entry = mymap->entry++;
    mymap->progs[entry].stamp++;
    mymap->mutexctr++;
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
    sprintf(tmpStr,"progs[%02d] TIME[%02d] MUTEX[%02d] MMAP[%s] ", entry, mymap->mutexctr, mymap->progs[entry].stamp, state);
    // fflush(NULL);

    for(int ii = 0; ii < mymap->entry; ii++) {
        char tmpStr2[32]={};
        sprintf(tmpStr2,"[%s]", mymap->progs[ii].akun);
        strcat(tmpStr, tmpStr2);
	    // fflush(NULL);
    }
    // printf("]\n");
    myprint(akunGitHub, tmpStr);
    fflush(NULL);
    sem_post(&(mymap->mutex));
}

void putInfo(char* akun, int entry) {
    // put "akunGitHub" into akun[] array (MMAP)
    sem_wait(&(mymap->mutex)); 
    mymap->progs[entry].stamp++;
    strcpy(mymap->progs[entry].akun, akun);
    mymap->mutexctr++;
    mymap->progs[entry].stamp++;
    sem_post(&(mymap->mutex));
}

void checkOpen(void) {
    // exit if MMAP is closed.
    if(mymap->state == CLOSED){
        printf("CLOSED, BYEBYE =====  ===== =====");
        exit(0);
    }
}

int main(void) {
    sprintf(tmpStr, "START PID[%d] PPID[%d]", getpid(), getppid());
    myprint(akunGitHub, tmpStr);
    int boss=init();
    checkOpen();
    for (int ii = 0; ii < 7; ii++){
        if(!fork()){
            sprintf(tmpStr, "p%02d", ii+1);
            execlp(progs[ii], tmpStr, (char *)NULL);
        }  
    }
    sleep (DELAY);
    int entry = getEntry();

    display(entry);
    sleep (DELAY);
    putInfo(akunGitHub, entry);

    for(int i = 0; i < 2; i++) {
        display(entry);
        sleep (DELAY);
    }
    for (int ii = 0; ii < 7; ii++){
        wait(NULL);
    }

    mymap->state=CLOSED;
    myprint(akunGitHub, "BYEBYE =====  ===== =====");
}


#ifndef OKCLIENT_H
#define OKCLIENT_H

typedef struct filenode{
  char *fname;
  struct filenode *next;
}FNODE;

typedef struct filelist{
  FNODE *head;
}FLIST;

extern int okclient(char *);
void createflist(FLIST *fl);
int newokclient(FLIST *flist, char ip[4]);

#endif

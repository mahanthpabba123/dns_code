#include <sys/types.h>
#include <sys/stat.h>
#include "str.h"
#include "ip4.h"
#include "okclient.h"
#include <dirent.h>
#include "log.h"
#include <stdlib.h>
#include <string.h>


static char fn[3 + IP4_FMT];

static char fname1[IP4_FMT];

int string_len(const char *str){
  int count=0;
  while(str[count]!='\0'){
    count++;
  }
  return count;
}

void string_copy(char *to, const char *from, int len){

  int i;
  for(i=0;i<len;i++){
    to[i] = from[i];
  }
  to[i]='\0';
}

/*adding to list of filenames (acceptable client IP's)*/
void addtolist(FLIST *list, char *str){

  FNODE *newnode;
  int len = string_len(str);
  newnode = (FNODE *)malloc(sizeof(FNODE));
  newnode->fname = malloc(string_len(str)+1);
  string_copy(newnode->fname, str, len);
  newnode->next = NULL;
  FNODE *head = list->head;
  if(head==NULL){
    list->head = newnode;
    return;
  }
  while(head->next!=NULL){
    head = head->next;
  }
  head->next = newnode;
}

/*
void printfilelist(FLIST *list){                                          FNODE *head = list->head;                                              while(head!=NULL){                                                          log_string(head->fname);                                               log_string("\n");                                                      head = head->next;                                                }                                                                   }
*/

/* get all file names in ip/ folder and add to list */
void createflist(FLIST *flist){

  DIR *dir;
  struct dirent *ent;
  if((dir = opendir("ip/")) != NULL){
    while((ent=readdir(dir))!=NULL){
      addtolist(flist, ent->d_name);
    }
    closedir(dir);
  }
//  printfilelist(flist);
}

int okclient(char ip[4])
{
  struct stat st;
  int i;

  fn[0] = 'i';
  fn[1] = 'p';
  fn[2] = '/';
  fn[3 + ip4_fmt(fn + 3,ip)] = 0;

  for (;;) {
    if (stat(fn,&st) == 0) return 1;
    /* treat temporary error as rejection */
    i = str_rchr(fn,'.');
    if (!fn[i]) return 0;
    fn[i] = 0;
  }
}

int newokclient(FLIST *flist, char ip[4])
{
  fname1[ip4_fmt(fname1,ip)] = '\0';
  FNODE *node = flist->head;
  int len = string_len(fname1);
  while(node!=NULL){
    if(strncmp(node->fname,fname1,len)==0){
      return 1;
    }
    node = node->next;
  }
  return 0;
}

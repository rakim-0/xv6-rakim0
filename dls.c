#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
dls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  int numFiles = 0;
  int numDir = 0;
  if((fd = open(path, 0)) < 0){
    printf(2, "dls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "dls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    printf(1, "dls: Please enter a directory name.\n");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "dls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "dls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_FILE) {
        numFiles++;
      }
      else if (st.type == T_DIR) {
        numDir++;
      }
    }
    break;
  }
  close(fd);
  printf(1, "Number of Directories: %d\nNumber of Files: %d\n",numDir, numFiles);
}

void 
usage() {
  printf(1, "Usage:\n");
  printf(1, "dls    : Print number of directories and number of files.\n");
  printf(1, "dls -d : Print depth of all the directories.\n");
  printf(1, "dls -t : Print count of types of each file.\n");
}

int max(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}
/*
    usage: depth(path, 0);
*/
int 
getDepth(char *path, int depth) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  int depthret = depth;

  if((fd = open(path, 0)) < 0){
    printf(2, "dls: cannot open %s\n", path);
    return -1;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "dls: cannot stat %s\n", path);
    close(fd);
    return -1;
  }

  switch(st.type){
  case T_FILE:
    // printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    printf(1, "dls: Please enter a directory name.\n");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "dls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "dls: cannot stat %s\n", buf);
        continue;
      }
      
      if (st.type == T_DIR && strcmp(p, ".") && strcmp(p, "..")) {
        int newDepth = getDepth(buf, depth+1);
        depthret = max(depthret, newDepth);
      }
    }
    break;
  }
  close(fd);
  return depthret;
}


void
type(char* path) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "dls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "dls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    printf(1, "dls: Please enter a directory name.\n");
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "dls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "dls: cannot stat %s\n", buf);
        continue;
      }
    }
    break;
  }
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    dls(".");
    exit();
  }
  if (argc > 3) {
    usage();
  }

  if (strcmp(argv[1], "-d") == 0) {
    if (argc == 2)
      printf(1, "Depth of Directory: %d\n", getDepth(".", 0));
    else
      printf(1, "Depth of Directory: %d\n", getDepth(argv[2], 0));
  }
  else if (strcmp(argv[1], "-t") == 0) {
    printf(1, "-t\n");
  }
  else {
    usage();
  }
  exit();
}

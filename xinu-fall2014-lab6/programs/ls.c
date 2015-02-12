#include <xinu.h>
// ls implementation
shellcmd main(int nargs, char *args[]){
	char *p1 = ".";
	char *p2 = "..";
  if(nargs>2){
    kprintf("%s: too many arguments\n", args[0]);
    return 1;
  }
  
  if(nargs==2){
    //test whether if its a file a not
    int32 w = 0;
    int32 dirfd1 = open(RFILESYS,".","ro");  
    if (dirfd1 == SYSERR){
      kprintf("Open Error when ls current directory\n");
      close(dirfd1);
      return 0;
    }
    struct rfdirent rfdentry;
    while(1){
      int32 rc1 = read(dirfd1, (char *)&rfdentry, sizeof(struct rfdirent));
      if(rc1 == SYSERR) { /* error accured while reading */
	kprintf("Error when reading file in directory\n");
	close(dirfd1);
	return 0;
      } 
      else if(rc1 == 0) { /* Reached end of list */
	
	break;
      }
      else { /* Handle next entry in the directory */
	if((int32)(rfdentry.d_type)==RF_DIRENT_FILE){
	  if(myStrCmp(args[1],(char *)&rfdentry.d_name)==0)
	    w=1; //this is a file!
	}
	
	else if((int32)(rfdentry.d_type)==RF_DIRENT_DIR){
	  if(myStrCmp(args[1],(char *)&rfdentry.d_name)==0)
	    w=2; //this is dir
	}
      }
    }
    close(dirfd1);
    if(w==0){
      kprintf("No such directory or file\n");
      return 0;
    }
    //if it's a file but not dir
    else if(w==1){
      kprintf("%s is a file, cannot be read\n",args[1]);
      return 0;
    }
    //if it's a dir but not a file
    else{

    int32 dirfd = open(RFILESYS,(char *)args[1],"ro");
    if (dirfd == SYSERR){
      kprintf("No Such Directory or File\n");
      close(dirfd);
      return 0;
    }
    struct rfdirent rfdentry;
    while(1){
      
      int32 rc = read(dirfd, (char *)&rfdentry, sizeof(struct rfdirent));
      if(rc == SYSERR) { /* error accured while reading */
	kprintf("Error when reading file in directory\n");
	close(dirfd);
	return 0;
      } 
      else if(rc == 0) { /* Reached end of list */
	break;
      }
      else { /* Handle next entry in the directory */
	if((int32)(rfdentry.d_type)==RF_DIRENT_FILE&&  myStrCmp(p2,(char *)&rfdentry.d_name)!=0&&myStrCmp(p1,(char *)&rfdentry.d_name)!=0){
	  kprintf("%s\n",&rfdentry.d_name);
	 
	  }

        if((int32)(rfdentry.d_type)==RF_DIRENT_DIR&&  myStrCmp(p2,(char *)&rfdentry.d_name)!=0&&myStrCmp(p1,(char *)&rfdentry.d_name)!=0)
	  kprintf("%s/\n",&rfdentry.d_name);
      }
    }
    close(dirfd);
    return 1;
    }
  }
  int32 dirfd = open(RFILESYS,".","ro");
  if (dirfd == SYSERR){
      kprintf("Open Error when ls current directory\n");
      close(dirfd);
      return 0;
    }
  struct rfdirent rfdentry;
    while(1){
      int32 rc = read(dirfd, (char *)&rfdentry, sizeof(struct rfdirent));
      if(rc == SYSERR) { /* error accured while reading */
	kprintf("Error when reading file in directory\n");
	close(dirfd);
	return 0;
      } 
      else if(rc == 0) { /* Reached end of list */
	break;
      }
      else { /* Handle next entry in the directory */
	
	if((int32)(rfdentry.d_type)==RF_DIRENT_FILE&&  myStrCmp(p1,(char *)&rfdentry.d_name)!=0&&myStrCmp(p2,(char *)&rfdentry.d_name)!=0){
	  kprintf("%s\n",&rfdentry.d_name);
	}
	
	else if((int32)(rfdentry.d_type)==RF_DIRENT_DIR &&  myStrCmp(p2,(char *)&rfdentry.d_name)!=0&&myStrCmp(p1,(char *)&rfdentry.d_name)!=0)
	  kprintf("%s/\n",&rfdentry.d_name);
      }
    }
    close(dirfd);
    return 1;
}

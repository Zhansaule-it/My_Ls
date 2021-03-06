#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

int mystrcmp (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

void mycpy(const  char *dest, const  char *src,int n) 
{ 
   char *csrc = (char *)src; 
   char *cdest = (char *)dest; 
   for (int i = 0; i<n; i++) 
       cdest[i] = csrc[i]; 
}

int my_strlen(const char* ch)
{
  int count = 0;
     if (ch[0] == '\0')
       {
         return 0;
       }

     while (*ch) 
      { 
       count++; 
       ch++;
      }
    return count;
}

void swap(const char *p1, const char *p2,int ctr){
    const  char *s1 = malloc(ctr*sizeof(p1));
    mycpy(s1,p1,my_strlen(p1)+1);
    mycpy(p1,p2,my_strlen(p2)+1);
    mycpy(p2,s1,my_strlen(s1)+1);
}

void sort(struct dirent** p1, int ctr){
      for (int i = 0;i < ctr;i++)  
    {
        for (int j = i + 1;j < ctr; j++)
        { 
           if ((mystrcmp(p1[i]->d_name, p1[j]->d_name)) > 0)
            {
               swap (p1[i]->d_name, p1[j]->d_name,ctr);
            }
        }
    }
}
void t_sort(struct dirent **p1,unsigned long* sec,unsigned long* nsec, int ctr){
    unsigned long sam;
      for (int i = 0;i < ctr;i++)  
    {
        for (int j = i + 1;j < ctr; j++)
        { 
           if (sec[i] < sec[j])
            {
                 sam  = sec[i];
               sec[i] = sec[j];
               sec[j] = sam; 
               swap (p1[i]->d_name, p1[j]->d_name,ctr);
            }
            if (sec[i] == sec[j]){
                if (nsec[i] < nsec[j])
            {
                 sam   = nsec[i];
               nsec[i] = nsec[j];
               nsec[j] = sam; 
               swap (p1[i]->d_name, p1[j]->d_name,ctr);
            }
           
            }
        }
    }
}

int all(char* fd,char* x,char* y){

    DIR *folder;
    struct dirent *entry;
    int count;
    struct dirent **list;
    struct dirent **t_list;
    int d=0;
    folder = opendir(fd);
    if(folder == NULL)
    {
        printf("Unable to read directory");
        return(1);
    }
    count = 0;
    while ((entry = readdir(folder)) != NULL)
        ++count;
    
    list = malloc(count* sizeof(entry));
    if (list == NULL)
    {
        closedir(folder);
        printf("memory exhausted.\n");
    }
   
    rewinddir(folder); 
    count = 0;
    while ((entry = readdir(folder)) != NULL){
       list[count++] = entry;
    }
        t_list = malloc(count * sizeof(entry));
     for (int index = 0 ; index < count ; ++index)
     {
         if(mystrcmp(list[index]->d_name,".")!=0&&mystrcmp(list[index]->d_name,"..")!=0&&mystrcmp(list[index]->d_name,".git")!=0)
         {
           t_list[d]=list[index]; d++;
         }
       
    }
    if(x == NULL)
     {
      sort(t_list,d); 
      for (int index = 0 ; index < d ; ++index)
      printf("%s  ", t_list[index]->d_name);
     }
    struct stat stat_result;
    unsigned long* sec = malloc(count * sizeof(struct stat)); 
    unsigned long* nsec = malloc(count * sizeof(struct stat)); 
    if(x!=NULL)
    {
         if (mystrcmp(x,"-at") == 0||mystrcmp(x,"-ta") == 0)
         {
         sort(list,count); 
             for (int index = 0 ; index < count ; ++index)
             {
                if(!lstat(list[index]->d_name, &stat_result)) 
                {
                 sec[index] = (unsigned long) stat_result.st_mtim.tv_sec;
                 nsec[index] =(unsigned long)  stat_result.st_mtim.tv_nsec;
                } 
                else 
                {
                 printf("error\n");
                 return 1;
                 }
             }
                t_sort(list,sec,nsec,count);
    
                for (int index = 0 ; index < count ; ++index)
                {
                fprintf(stderr, "%s  ", list[index]->d_name);
                }
         }
        if(y != NULL)


{
          if ( (mystrcmp(x,"-t")==0&&mystrcmp(y,"-a")== 0)|| (mystrcmp(x,"-a") == 0&&mystrcmp(y,"-t") == 0))
          {
           sort(list,count); 
           for (int index = 0 ; index < count ; ++index)
          {
            if(!lstat(list[index]->d_name, &stat_result)) 
            {
              sec[index] = stat_result.st_mtim.tv_sec;
              nsec[index] = stat_result.st_mtim.tv_nsec;
              }
               else 
               {
                 printf("error\n");
                 return 1;
                 }
        }
            t_sort(list,sec,nsec,count);
            
            for (int index = 0 ; index < count ; ++index)
            {
               printf("%s  ", list[index]->d_name);
            }
          }
     }
 if (mystrcmp(x,"-a") == 0&& y == NULL)
 {
    sort(list,count); 
    
    for (int index = 0 ; index < count ; ++index)
    printf( "%s  ", list[index]->d_name);
 }
if (mystrcmp(x,"-t") == 0&& y == NULL)
 {    
     for (int index = 0 ; index < d; ++index)
     {
         if(!lstat(t_list[index]->d_name, &stat_result)) 
          {
            sec[index] = stat_result.st_mtim.tv_sec;
            nsec[index] = stat_result.st_mtim.tv_nsec;
          } 
         else 
          {
            printf("error\n");
            return 1;
          }
     }
         t_sort(t_list,sec,nsec,d);

       for (int index = 0 ; index < d ; ++index)
        printf("%s  ", t_list[index]->d_name);
 }
}
closedir(folder);
return(0);
}
int main(int ac, char **av){
 char** e = malloc(3);
 char* x = malloc(3) ;
 char* y = malloc(3) ;
 char** fd = malloc(100);
 int dcount=0;
    for (int i=1;i<ac;i++)
    {
         if(av[i][0]!='-') 
         {
           fd[dcount] = av[i];
           //printf("%s",fd[dcount]);
           dcount++;
        }
         if(av[i][0]=='-')
         {
            e[i-1] = av[i];
         }
    }
       x = e[0];
       y = e[1];
   
  
  if(x!=NULL&&y==NULL)
    {
        if(mystrcmp(x,"-at")!=0&&mystrcmp(x,"-ta")!=0&&mystrcmp(x,"-t")!=0&&mystrcmp(x,"-a")!=0)   //&&x!="-ta"&&x!="-t"&&x!="-a"
         {
          printf("my_ls: invalid option '%s' \n",x);
          printf("Try 'ls --help' for more information. \n");
         return 1;
         }
    }

    if(x!=NULL&&y!=NULL)
    {
        if((mystrcmp(x,"-t")!=0&&mystrcmp(x,"-a")!=0)||(mystrcmp(y,"-t")!=0&&mystrcmp(y,"-a")!=0))   
         {
          printf("my_ls: invalid option '%s' '%s' \n",x,y);
          printf("Try 'ls --help' for more information. \n");
         return 1;
         }
    }



   if(fd[0] == NULL)
   {
      fd[0] =".";
      all(fd[0],x,y);
   }
   else
   {
     for (int i = 0;i < dcount;i++)
     {
       if(dcount>1)
        {
          printf("folder %s: \n",fd[i]);
        }
      all(fd[i],x,y);
      printf("\n");
    }
 }
}

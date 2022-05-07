#include <stdio.h>
#include <stdlib.h>
unsigned char* disk;
unsigned char* i_bmp;
unsigned char* d_bmp;
//inode 256byte
struct inode{
 unsigned int fsize; //4
 unsigned int blocks; //4
 unsigned int pointer[12]; //48 56
 unsigned int unused[50];//200Byte
};
struct inode*inodes;
#define inode_start 12288
#define i_bitmap 4096
#define d_bitmap 9192
#define datablock_start 32768
void init_disk()
{
  disk=(unsigned char*)malloc(sizeof(int)*1024*64);
  for(int i=0;i<4*1024*64;i++)
  {
   disk[i]=0b00000000;
  }
  
  for(int i=0;i<80;i++)
  {
   int val=inode_start+256*i;
   inodes=(struct inode*)(disk+val);
   inodes->blocks=0;
  }
  struct inode*alpa=(struct inode*)(disk+inode_start+512);
  alpa->blocks=1;
  alpa->fsize=320;
  alpa->pointer[0]=0;
  
  unsigned char*beta= (unsigned char *)(disk+i_bitmap);
  *beta=0b11100000;
  unsigned char*gamma=(unsigned char*)(disk+d_bitmap);
  *gamma=0b10000000;
  //aldksgo bbb

}

int d_map_search()
{
int i=0;
int num;
  while(1)
  {
     if(d_bitmap+i==inode_start)
     {
       return -1; //fail;
     }
    num=8*i;
     unsigned char*beta= (unsigned char *)(disk+d_bitmap+i);
     char a=0b10000000;
 char s;
 for(int r=0;r<8;r++)
 {
   s=a&*beta;
   if(s!=a)
   {
    //find
    *beta=*beta|a; 
    return num+r;
   }
   a=a>>1;
  }
    i++;

  }
return -1;


}

int search_root_directroy()
{
unsigned char* inode_st=(unsigned char*)(disk+datablock_start);
for(int i=0; i<4096;i+=4)
{
  if(*(inode_st+i)==0)
  {
      return i;
  }
}

return -1;
}


int real_write(unsigned char*alpabets,int sizes,int num)
{
 
   //data
   //inode write
   //ino research
   //dta bitamp research
int bbbb=sizes%4096;
int block_num; //pointer_num
if(bbbb>0)
{
  block_num=1+sizes/4096;
}
else if(bbbb==0){
block_num=sizes/4096;
}
int number=search_root_directroy();
   unsigned char* inode_st=(unsigned char*)(disk+datablock_start+number);
   *inode_st=num;//3 
   //rihgt fix
   *(inode_st+1)=alpabets[0];
   *(inode_st+2)=alpabets[1];
   
  int data_write;
  for(int v=0;v<block_num;v++)
  {
        if(v==0)
        data_write=d_map_search();
        else{
          d_map_search();
        }

  }

  unsigned char* dd=(unsigned char*)(disk+(datablock_start+data_write*4096));
   for(int i=0;i<sizes;i++)
   {
   *(dd+i)=alpabets[0];
  }
   //inode
    struct inode*alpa=(struct inode*)(disk+inode_start+256*(num));
    alpa->fsize=4*sizes;
    
    alpa->blocks=block_num;
    
    for(int s=0;s<block_num;s++)
    {
       alpa->pointer[s]=s+data_write;
    }



}

int exists(unsigned char* alpabet)
{
  //root
  unsigned char* inode_st;
  for(int i=0;i<4096;i+=4)
{
   
inode_st=(unsigned char*)(disk+datablock_start+i);
 if(*(inode_st+1)==alpabet[0]&&*(inode_st+2)==alpabet[1]&&*(inode_st)!=0)
{
 return 1;
}
}
return 0;
}

int write(unsigned char * alpabet,int size_)
{
//search
int i=0;
int bbbb=size_%4096;
int block_num; //pointer_num
if(bbbb>0)
{
  block_num=1+size_/4096;
}
else if(bbbb==0){
block_num=size_/4096;
}

if(exists(alpabet))
  {
    printf("Already exists\n");
    return -1;
  }

while(1)
{
  
  if(i+i_bitmap==d_bitmap)
  {
    printf("No space\n");
    return -1; //fail
  }
  
 unsigned char*beta= (unsigned char *)(disk+i_bitmap+i);
 int num=i*8;
 char a=0b10000000;
 char s;
 for(int r=0;r<8;r++)
 {
   
   s=a&*beta;
   if(s!=a)
   {
    //find
    *beta=*beta|a; 
  
    real_write(alpabet,size_,num+r);
    return 1;
   }
   a=a>>1;
 }
 
i++;
}


}
void printff()
{

 unsigned char *block =disk;
   for(int i=0;i<4096*64;i++)
    {
    printf("%.2x",*((unsigned char*)block+i));
    }
}


int search_inum(unsigned char* alpabet)
{
  //root
  unsigned char* inode_st;
  for(int i=0;i<4096;i+=4)
{
inode_st=(unsigned char*)(disk+datablock_start+i);
 if(*(inode_st+1)==alpabet[0]&&*(inode_st+2)==alpabet[1]&& *(inode_st)!=0)
{

 return *(inode_st);

}
}
return -1;
}



void Read(unsigned char* search_alpabet,int size)
{
  //root -> inode num-> data block -> read;
  if(!exists(search_alpabet))
  {
   printf("No such file\n");
    return;
  }  
  int inum=search_inum(search_alpabet);  
 struct inode*alpa=(struct inode*)(disk+inode_start+256*(inum));
 //datablock search
 int data_num=alpa->pointer[0];
   unsigned char* dd=(unsigned char*)(disk+(datablock_start+data_num*4096));
   for(int i=0;i<size;i++)
   {
    printf("%c",*(dd+i));

   }
 printf("\n");

}

void Delete_name(unsigned char* alpabet)
{
  if(!exists(alpabet))
   {
    printf("No such file\n");
     return;
   }  
// root directroy inum -> 0 -> inode -> 0 datablock -> 0;
unsigned char* inode_st;
int inum=search_inum(alpabet);
//root directory inode->0
 for(int z=0;z<4096;z+=4)
 {
   inode_st=(unsigned char*)(disk+datablock_start+z);
    if(inum==*(inode_st))
    {
      *(inode_st)=0;
          break;
    }
 }

unsigned char m;
struct inode*alpa=(struct inode*)(disk+inode_start+256*(inum));
int inum_page=inum/8;
int inum_number=inum%8;
unsigned char*beta=(unsigned char*)(disk+i_bitmap+inum_page);
  m=0b10000000;
      for(int v=0;v<inum_number;v++)
      {
        m=m>>1;
      } 
      *beta=(*beta)^m;
      


int count=0;
int data_block_num;
int page;
int aam;

for(int i=0;i<12;i++)
{
  if(alpa->pointer[i]!=0)
    {
      data_block_num=alpa->pointer[i];
      //data_bitmap erase
      page=data_block_num/8;
      aam=data_block_num%8;
      unsigned char*gamma=(unsigned char*)(disk+d_bitmap+page);
      m=0b10000000;
      //2
      for(int r=0;r<aam;r++)
      {
        m=m>>1;
      } 
      *gamma=(*gamma)^m;

      alpa->pointer[i]=0;
    } 
}

}



int main(int argc, char *argv[])
{
	FILE *fd=NULL;
	char fpid, pid=0, va, pa;
	unsigned int pmem_size, swap_size;
	void *ku_cr3, *pmem=NULL;


	fd = fopen(argv[1], "r");
	if(!fd){
		printf("ku_cpu: Fail to open the input file\n");
		return 1;
	}
    if(argc != 2){
		printf("ku_cpu: Wrong number of arguments\n");
		return 1;
	}
   init_disk();
  int size_;
  char command;
 
  char file_name[2];
  while(fscanf(fd, "%s %c %d",file_name ,&command, &size_) != EOF){
		if(pid != fpid){
			if(command=='w')
            {
          
                  write(file_name,size_);
            }
            else if(command=='r')
            {
             
              Read(file_name,size_);
            }
            else if(command=='d')
            {
                   Delete_name(file_name);
            }
            else{
                printf("no anwser");
            }

		}

    //256KB allocate
  }
  
printff();
  
  

}
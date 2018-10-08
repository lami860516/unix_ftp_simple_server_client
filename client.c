#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#define MAX_PATH_LEN	500
 



int main(){
	int sockfd, numbytes, i,j,fd,size,size2,get,put,ls,lls,exit,new_fd,success,progress,mkd,putA,show;
	char buf[101],command[10],filename[100],temp[2],filesize[10],filesize2[10];
	char storedFile[10][100];
	char isDown='Y';
	int storedSize,isPutA=0;
	struct sockaddr_in address;
	struct stat fileStat;
	struct passwd *pwd_ent;
	struct group *grp_ent;

	
	char cwd[MAX_PATH_LEN];
	DIR *dp;
	struct dirent *dir_ent_info;
	char ent_path_name[MAX_PATH_LEN];
	struct stat inode_info;
	int is_file;
	int is_dir;
	int year=0,mon=0,day=0,hour=0,min=0;
	struct tm *object;

 
	//TCP socket
	if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) == -1 ) perror("socket");
 
	//Initial, connect to port 5555
	address.sin_family = AF_INET;
	address.sin_port = htons(4646);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Connect to server
	if ( connect(sockfd, (struct sockaddr*)&address, sizeof(struct sockaddr)) == -1) perror("connect");
 
	while(1){
	//command 
	for(i=0;i<100;i++){
		filename[i]='\0';
		if(i<10)command[i]='\0';
		}
	get=0;
	put=0;
	ls=0;
	lls=0;
	exit=0;
	mkd=0;
	putA=0;
	show=0;
		//isPutA=0;
	if(isPutA==0){
		printf("ftp:D ");
		scanf("%s",command);
		if(command[0]=='e' && command[1]=='x' && command[2]=='i' && command[3]=='t' && command[4]=='\0')exit=1;
		if(command[0]=='g' && command[1]=='e' && command[2]=='t' && command[3]=='\0')get=1;
		if(command[0]=='p' && command[1]=='u' && command[2]=='t' && command[3]=='\0')put=1;
		if(command[0]=='l' && command[1]=='l' && command[2]=='s' && command[3]=='\0')lls=1;
		if(command[0]=='l' && command[1]=='s' && command[2]=='\0')ls=1;
		if(command[0]=='m' && command[1]=='k' && command[2]=='d' && command[3]=='\0')mkd=1;
		if(command[0]=='p' && command[1]=='u' && command[2]=='t' && command[3]=='A' && command[4]=='\0')putA=1;
		if(command[0]=='s' && command[1]=='h' && command[2]=='o' && command[3]=='w' && command[4]=='\0')show=1;
		if(get==1||put==1||mkd==1||show==1)	
			scanf(" %s",filename);
	}else{	
		//printf("%d %d\n",storedSize,isPutA);
		if(!(isPutA>storedSize)){
			for(i=0;storedFile[isPutA-1][i]!='\0';i++){
				filename[i]=storedFile[isPutA-1][i];
			}
			printf("%d %s:\n",isPutA,filename);
			scanf(" %c",&isDown);
			isPutA++;
			if(isDown=='Y'){
				command[0]='p';command[1]='u';command[2]='t';
				put=1;	
			}else{
				continue;			
			}
			
				
		}else{
			isPutA=0;
			continue;		
		}
		

	}
	

	//send command
	if ( (numbytes = write(sockfd, command, strlen(command)) )== -1 ) perror("send");
	if ( (numbytes = read(sockfd, buf, sizeof(buf)-3 ) ) == -1 ) perror("recv");
	buf[numbytes] = '\0';

	if(exit==0&&lls==0&&ls==0&&get==0&&put==0&&mkd==0&&putA==0&&show==0){
		printf("command not found!\n");
		continue;
		}
	if(mkd==1){
//send filename
		if ( (numbytes = write(sockfd, filename, strlen(filename)) )== -1 ) perror("send");
		if ( (numbytes = read(sockfd, buf, sizeof(buf)-3 ) ) == -1 ) perror("recv");
		buf[numbytes] = '\0';
	}
	if(exit==1){
		if ( (numbytes = write(sockfd, command, strlen(command)) )== -1 ) perror("send");
		if ( (numbytes = read(sockfd, buf, sizeof(buf)-3 ) ) == -1 ) perror("recv");
		printf("bye~\n");
		close(sockfd);
		return 0;
		}
	
	if(lls==1){
		getcwd (cwd, MAX_PATH_LEN);
		dp = opendir (cwd);
		while ((dir_ent_info=readdir(dp))!=NULL) {
			//get inode info of an directory entry
			sprintf (ent_path_name, "%s/%s", cwd, dir_ent_info->d_name);
			stat (ent_path_name, &inode_info);

			//check type of a directory entry
			is_file = S_ISREG (inode_info.st_mode);
			is_dir = S_ISDIR (inode_info.st_mode);

			pwd_ent = getpwuid (inode_info.st_uid);		
			grp_ent = getgrgid (inode_info.st_gid);

			//print-out info
			if (is_file){
				printf("\033[31m-");
		        	printf( (inode_info.st_mode & S_IRUSR) ? "r" : "-");
		       	printf( (inode_info.st_mode & S_IWUSR) ? "w" : "-");
		        	printf( (inode_info.st_mode & S_IXUSR) ? "x" : "-");
		        	printf( (inode_info.st_mode & S_IRGRP) ? "r" : "-");
		        	printf( (inode_info.st_mode & S_IWGRP) ? "w" : "-");
		        	printf( (inode_info.st_mode & S_IXGRP) ? "x" : "-");
		        	printf( (inode_info.st_mode & S_IROTH) ? "r" : "-");
		        	printf( (inode_info.st_mode & S_IWOTH) ? "w" : "-");
		        	printf( (inode_info.st_mode & S_IXOTH) ? "x" : "-");
		        	object=localtime(&inode_info.st_mtime);
		        	year=object->tm_year+1900;//least change time
		        	mon=object->tm_mon+1;
		        	day=object->tm_mday;
		        	hour=object->tm_hour;
		        	min=object->tm_min;  
				printf (" %d %s %s %6d %d-%d-%d %d:%d %s\033[33m\n",
					inode_info.st_nlink,
					pwd_ent->pw_name,
					grp_ent->gr_name,
					inode_info.st_size,
					year,
					mon,
					day,
					hour,
					min,
					dir_ent_info->d_name
					);
				}
			if (is_dir && dir_ent_info->d_name[0]!='.'){
				
				printf("\033[31md");
		        	printf( (inode_info.st_mode & S_IRUSR) ? "r" : "-");
		       	printf( (inode_info.st_mode & S_IWUSR) ? "w" : "-");
		        	printf( (inode_info.st_mode & S_IXUSR) ? "x" : "-");
		        	printf( (inode_info.st_mode & S_IRGRP) ? "r" : "-");
		        	printf( (inode_info.st_mode & S_IWGRP) ? "w" : "-");
		        	printf( (inode_info.st_mode & S_IXGRP) ? "x" : "-");
		        	printf( (inode_info.st_mode & S_IROTH) ? "r" : "-");
		        	printf( (inode_info.st_mode & S_IWOTH) ? "w" : "-");
		        	printf( (inode_info.st_mode & S_IXOTH) ? "x" : "-");
		        	object=localtime(&inode_info.st_mtime);
		        	year=object->tm_year+1900;//least change time
		        	mon=object->tm_mon+1;
		        	day=object->tm_mday;
		        	hour=object->tm_hour;
		        	min=object->tm_min; 
				printf (" %d %s %s   4096 %d-%d-%d %d:%d %s\033[33m\n",
					inode_info.st_nlink,
					pwd_ent->pw_name,
					grp_ent->gr_name,
					year,
					mon,
					day,
					hour,
					min,
					dir_ent_info->d_name
					);
				}
			}
			printf("\n");
		}//end lls
		
	if(ls==1){

		}//end ls
	if(putA==1){
		for(i=0;i<10;i++)
			for(j=0;j<100;j++)
				storedFile[i][j]='\0';
		storedSize=0;
		getcwd (cwd, MAX_PATH_LEN);
		dp = opendir (cwd);
		while ((dir_ent_info=readdir(dp))!=NULL) {
			//get inode info of an directory entry
			sprintf (ent_path_name, "%s/%s", cwd, dir_ent_info->d_name);
			stat (ent_path_name, &inode_info);

			//check type of a directory entry
			is_file = S_ISREG (inode_info.st_mode);
			is_dir = S_ISDIR (inode_info.st_mode);

			pwd_ent = getpwuid (inode_info.st_uid);		
			grp_ent = getgrgid (inode_info.st_gid);

			//print-out info
			if (is_file){
				//if(dir_ent_info->d_name[sizeof(dir_ent_info->d_name)-1]=='c'||dir_ent_info->d_name[sizeof(dir_ent_info->d_name)-1]=='t'){
				//printf("%s is File\n",dir_ent_info->d_name);
				
		        	object=localtime(&inode_info.st_mtime);
		        	year=object->tm_year+1900;//least change time
		        	mon=object->tm_mon+1;
		        	day=object->tm_mday;
		        	hour=object->tm_hour;
		        	min=object->tm_min;  
				/*printf (" %d %s %s %6d %d-%d-%d %d:%d %s\033[33m\n",
					inode_info.st_nlink,
					pwd_ent->pw_name,
					grp_ent->gr_name,
					inode_info.st_size,
					year,
					mon,
					day,
					hour,
					min,
					dir_ent_info->d_name
					);*/
		
				for(i=0;i<sizeof(dir_ent_info->d_name);i++){
					storedFile[storedSize][i]=dir_ent_info->d_name[i];
				}
				storedFile[storedSize][sizeof(dir_ent_info->d_name)]='\0';
				//printf("%d : %s\n",storedSize,storedFile[storedSize]);
				storedSize++;//}
			}
			if (is_dir && dir_ent_info->d_name[0]!='.'){
					//printf("%s is dir",dir_ent_info->d_name);
			}
		}
		isPutA=1;
			//printf("\n");
		}//end putA
	if(show==1){
		printf("\n");
		stat(filename,&fileStat);
		fd=open(filename,O_RDONLY);
		size=(int)fileStat.st_size;
		while(size>0){
			for(i=0;i<101;i++)buf[i]='\0';
			if(size>100){
				read(fd,buf,100);
				size-=100;
				buf[100]='\0';
			}
			else{
				read(fd,buf,size);
				buf[size]='\0';
				size=0;
			}
		printf("%s",buf);
		}
		printf("\n");
	}
	if(put==1){//put
		//send filename
		if ( (numbytes = write(sockfd, filename, strlen(filename)) )== -1 ) perror("send");
		if ( (numbytes = read(sockfd, buf, sizeof(buf)-3 ) ) == -1 ) perror("recv");
		buf[numbytes] = '\0';

		//Send filesize
		stat(filename,&fileStat);	//get file properties,such as file size	
		size=(int)fileStat.st_size;
		size2=(int)fileStat.st_size;
		for(i=9;i>=0;i--){
			filesize[i]=(size%10)+'0';
			size/=10;
			}
		if ( (numbytes = write(sockfd, filesize, 10) )== -1 ) perror("send");
		size=(int)fileStat.st_size;
		if ( (numbytes = read(sockfd, buf, sizeof(buf)-3 ) ) == -1 ) perror("recv");
		buf[numbytes] = '\0';

		//send the file contents
		fd=open(filename,O_RDONLY);
		while(size>0){
			progress=((size2-size)*10/size2);
			if(progress==0)printf("\r\033[36m[          ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==1)printf("\r[>         ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==2)printf("\r[=>        ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==3)printf("\r[==>       ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==4)printf("\r[===>      ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==5)printf("\r[====>     ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==6)printf("\r[=====>    ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==7)printf("\r[======>   ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==8)printf("\r[=======>  ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==9)printf("\r[========> ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==10)printf("\r[=========>]  %3.1f%%\033[33m",(double)(size2-size)*100/size2);

			for(i=0;i<101;i++)buf[i]='\0';
			if(size>100){
				read(fd,buf,100);
				size-=100;
				if ( (numbytes = write(sockfd, buf, 100) )== -1 ) perror("send");
				}
			else{
				read(fd,buf,size);
				if ( (numbytes = write(sockfd, buf, size) )== -1 ) perror("send");
				size=0;
				}
			//Receive from server
			if ( (numbytes = read(sockfd, buf, sizeof(buf) ) ) == -1 ) perror("recv");
			buf[numbytes] = '\0';
			//printf("1 %s\n",buf);
			}//end while
		//check size
		if ( (numbytes = read(sockfd, buf, sizeof(buf)-3 ) ) == -1 ) perror("recv");
		buf[numbytes] = '\0';
		//printf("2 %s\n",buf);
		printf("\r\033[36m[=========>]  100.0%%\033[33m\n");
		printf("\033[36m------%s------\033[33m\n",buf);
		close(fd);
		
		}//end put
	
	if(get==1){
		//send filename
		if ( (numbytes = write(sockfd, filename, strlen(filename)) )== -1 ) perror("send");
		if ( (numbytes = read(sockfd, buf, sizeof(buf)-3 ) ) == -1 ) perror("recv");
		buf[numbytes] = '\0';

		//receive filesize
		if ( (numbytes = read(sockfd, filesize, 10 ) ) == -1 ) perror("recv");
		size=0;
		for(i=0;i<10;i++){
			size*=10;		
			size+=filesize[i]-'0';
			}
		size2=size;
		if ( (numbytes = write(sockfd, "receive!!!", sizeof("receive!!!") ) ) == -1 ) perror("recv");	//send back	

		fd=open(filename,O_WRONLY|O_CREAT);
		fchmod(fd,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);//chmod 777
		while(size>0){
			progress=((size2-size)*10/size2);
			if(progress==0) printf("\r\033[36m[          ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==1) printf("\r[>         ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==2) printf("\r[=>        ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==3) printf("\r[==>       ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==4) printf("\r[===>      ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==5) printf("\r[====>     ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==6) printf("\r[=====>    ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==7) printf("\r[======>   ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==8) printf("\r[=======>  ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==9) printf("\r[========> ]  %3.1f%%",(double)(size2-size)*100/size2);
			if(progress==10)printf("\r[=========>]  %3.1f%%\033[33m",(double)(size2-size)*100/size2);

			for(i=0;i<100;i++)buf[i]='\0';
			if(size>100){
				if ( (numbytes = read(sockfd, buf, 100 ) ) == -1 ) perror("recv");
				buf[numbytes] = '\0';
				write(fd,buf,100);
				size-=100;
				}
			else{
				if ( (numbytes = read(sockfd, buf, sizeof(buf) ) ) == -1 ) perror("recv");
				buf[numbytes] = '\0';
				for(i=0;buf[i]!='\0';i++){
					temp[0]=buf[i];
					write(fd,temp,1);
					}
				size=0;
				}
			//printf("  %s  \n",buf);
			}
		//check size
		success=1;
		stat(filename,&fileStat);	//get file properties,such as file size	
		size=(int)fileStat.st_size;
		for(i=9;i>=0;i--){
			filesize2[i]=(size%10)+'0';
			size/=10;
			}
		for(i=0;i<10;i++)if(filesize[i]!=filesize2[i])success=0;
		if(success==1) printf("\033[36m------success!!------\033[33m\n");
		else printf("\033[36m------failure!!------\033[33m\n");
		printf("\r\033[36m[=========>]  100.0%%\033[33m\n");
		close(fd);
		}
	}
	close(sockfd);
	return 0;
}

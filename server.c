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
	int addr_size,sockfd, new_fd, numbytes,i,fd,size,success,put,get,ls,lls,exit,mkd,putA;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	struct stat fileStat;
	struct passwd *pwd_ent;
	struct group *grp_ent;
	char buf[101],filename[100],filesize[11],filesize2[10],temp[10],command[10];
 	
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
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		perror("socket");
		}
 
	//Initail, bind to port 5555
	bzero (&my_addr, sizeof(my_addr)); 
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(4646);
	my_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");
 
	//binding
	if ( bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1 ){
		perror("bind");
		}
 
	//Start listening
	if ( listen(sockfd, 10) == -1 ){
		perror("listen");
		}
 
	//Wait for connect!
	perror("server running");
	addr_size = sizeof(their_addr);
	if ( (new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size)) == -1 ) perror("accept");
	
	while(1){
	//Receive command
	for(i=0;i<10;i++)command[i]='\0';
	if ( (numbytes = read(new_fd, command, sizeof(command))) == -1 ) perror("recv");
	printf("%s\n",command);
	command[numbytes]='\0';
	get=0;
	put=0;
	lls=0;
	ls=0;
	exit=0;
	mkd=0;
	putA=0;
	if(command[0]=='e' && command[1]=='x' && command[2]=='i' && command[3]=='t')exit=1;
	if(command[0]=='p' && command[1]=='u' && command[2]=='t')put=1;
	if(command[0]=='g' && command[1]=='e' && command[2]=='t')get=1;
	if(command[0]=='l' && command[1]=='l' && command[2]=='s')lls=1;
	if(command[0]=='l' && command[1]=='s')ls=1;
	if(command[0]=='m' && command[1]=='k' && command[2]=='d' && command[3]=='\0')mkd=1;
	if(command[0]=='p' && command[1]=='u' && command[2]=='t' && command[3]=='A' && command[4]=='\0'){putA=1;printf("hi\n");}
	//Send back
	if ( (numbytes = write(new_fd, "Receive!!", sizeof("Receive!!") )) == -1) perror("send");


	if(exit==1){
		close(new_fd);
		close(sockfd);
		return 0;	
		}
	if(putA==1){
		printf("putAAAAAAAAAAAAA\n");
		continue;
	}//end putA
	if(mkd==1){
		//Receive filename
		for(i=0;i<100;i++)filename[i]='\0';
		if ( (numbytes = read(new_fd, filename, sizeof(filename))) == -1 ) perror("recv");	

		//Send back
		if ( (numbytes = write(new_fd, "Receive!!", sizeof("Receive!!") )) == -1) perror("send");
		
		//fd=open(filename,O_WRONLY|O_CREAT);	
		int status;
		status = mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
	}
	if(lls==1){	
		}

	if(ls==1){
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
		}
	
	if(put==1){
		//Receive filename
		for(i=0;i<100;i++)filename[i]='\0';
		if ( (numbytes = read(new_fd, filename, sizeof(filename))) == -1 ) perror("recv");	

		//Send back
		if ( (numbytes = write(new_fd, "Receive!!", sizeof("Receive!!") )) == -1) perror("send");
		fd=open(filename,O_WRONLY|O_CREAT);	

		//Receive filesize
		if ( (numbytes = read(new_fd, filesize, 10)) == -1 ) perror("recv");
		size=0;
		for(i=0;i<10;i++){
			size*=10;		
			size+=filesize[i]-'0';
			}

		//Send back
		if ( (numbytes = write(new_fd, "Receive!!", sizeof("Receive!!") )) == -1) perror("send");

		while(size>0){	
			//Receive file contents
			for(i=0;i<101;i++)buf[i]='\0';
			if ( (numbytes = read(new_fd, buf, sizeof(buf))) == -1 ) perror("recv");
			if(size>100){
				write(fd,buf,100);
				size-=sizeof(buf);
				}
			else
				for(i=0;buf[i]!='\0';i++){
					temp[0]=buf[i];
					write(fd,temp,1);
					size--;
					}
			//Send back
			if ( (numbytes = write(new_fd, "Receive!!", sizeof("Receive!!") )) == -1) perror("send");
			};
		//check size
		stat(filename,&fileStat);	//get file properties,such as file size	
		size=(int)fileStat.st_size;
		for(i=9;i>=0;i--){
			filesize2[i]=(size%10)+'0';
			size/=10;
			}
		success=1;
		for(i=0;i<10;i++)if(filesize[i]!=filesize2[i])success=0;
		if(success==1){
			if ( (numbytes = write(new_fd, "success!!", sizeof("success!!") )) == -1) perror("send");
			}
		else{
			if ( (numbytes = write(new_fd, "failure!!", sizeof("failure!!") )) == -1) perror("send");
			}

		fchmod(fd,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);//chmod 777
		close(fd);
		}//end put

	if(get==1){
		//Receive filename
		for(i=0;i<100;i++)filename[i]='\0';
		if ( (numbytes = read(new_fd, filename, sizeof(filename))) == -1 ) perror("recv");	

		//Send back
		if ( (numbytes = write(new_fd, "Receive!!", sizeof("Receive!!") )) == -1) perror("send");

		//send filesize
		for(i=0;i<10;i++)filesize[i]='\0';
		stat(filename,&fileStat);	//get file properties,such as file size	
		size=(int)fileStat.st_size;
		for(i=9;i>=0;i--){
			filesize[i] = (size%10) + '0' ;
			size/=10;
			}
		if ( (numbytes = write(new_fd, filesize, 10 )) == -1) perror("send");
		if ( (numbytes = read(new_fd, buf, sizeof(buf))) == -1 ) perror("recv");
		fd=open(filename,O_RDONLY);
		size=(int)fileStat.st_size;
		while(size>100){
			printf("\033[36msize========>%d\033[33m\n",size);
			for(i=0;i<101;i++)buf[i]='\0';
			if(size>100){
				read(fd,buf,100);
				if ( (numbytes = write(new_fd, buf, 100 )) == -1) perror("send");
				size-=100;
				}
			}//end while
		if(size>0){
			read(fd,buf,size);
			if ( (numbytes = write(new_fd, buf, size )) == -1) perror("send");
			size=0;
			}	
		
		close(fd);
		}//end get

	
	}//end while
	close(new_fd);

	close(sockfd);
	return 0;
}

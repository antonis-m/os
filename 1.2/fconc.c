#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<string.h>
#include <sys/stat.h>


#define BUF_SIZE 8

void doWrite(int fd, const char *buff, int len) {

	if (write(fd, buff, len) != len) {
		perror("couldn't write whole buffer");
		exit(1);
	}

}

int doRead(int fd,char * buf,int len) {
	int r = read(fd,buf,len);
	if (r < 0) {
		perror("Problem with reading");
		exit(1);
	}
	return r;


}

void write_file(int fd, const char *infile) {
	int r;	
	char buf[BUF_SIZE];	
	int inf = open(infile, O_RDONLY); //file descriptor 1
	if (inf<0) {
		perror(infile);
		exit(1);	
	}
	while ((r = doRead(inf,buf,BUF_SIZE))>0) {
		doWrite(fd,buf,r);
	}

	close(inf);
		

}

void createNewOutputFile(char * file1,char *file2,char * file3) {
	char * filename;
	if (file3 == NULL) 
		filename = "fconc.out";
	else 
		filename = file3;
	int out = open(filename,O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);	
	if (out<0) {
		perror("Can't create output file");
		exit(1);	
	}		
		
	write_file(out,file1);
	write_file(out,file2);
		
	close(out);

}

void append (char * file1, char * file2) {
	int out = open(file1,O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);	
	if (out<0) {
		perror("Can't create output file");
		exit(1);	
	}		
	write_file(out,file2);

}

void prepend(char * file1,char * file2) {

	createNewOutputFile(file1,file2,".test.txt");
	char  s[] = "rm ";	
	system(strcat(s,file2));
	char t[] = "mv .test.txt ";	
	system(strcat(t,file2));
	
}
int main(int argc ,char **argv) {
	
	int fd1,fd2,out;
	int r1;
	char * filename;
	
	if (argc <3 )	{
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
	} else {
		
		if (argc ==3) createNewOutputFile(argv[1],argv[2],NULL); 
		else {
			if ((strcmp(argv[1],argv[3]) != 0) && (strcmp(argv[2],argv[3]) != 0))
				createNewOutputFile(argv[1],argv[2],argv[3]);
			else 
				if (strcmp(argv[1],argv[3]) == 0) append(argv[1],argv[2]);
				else prepend(argv[1],argv[2]);
		}	
	
	}
	return 0;
}

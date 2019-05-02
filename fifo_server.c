// proces 1: citeste de la intrarea standard si scrie:
// 1.intrare: toate liniile care incep cu o litera
// 2.intrare: toate liniile care incep cu o cifra
// 3.intrare: restul liniilor
// proces 2,3,4: afiseaza ceea ce primesc


#include "header.h"

int is_digit (char c){
	if ( c >= '0' && c <= '9')
		return 1;
	return 0;
}

int is_alpha (char c){
	if ( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

void create_fifo(char* file_name){
	if ((mkfifo(file_name, 0666)) && (errno!=EEXIST)){
		printf("error creating fifo\n");
		exit(0);
	}
	printf(CYN "fifo created\n");
}

void delete_fifo(char* file_name){
	if(unlink(file_name) < 0){
		printf("error deleting fifo\n");
		exit(0);
	}
	printf(CYN "fifo deleted\n");
}

void read_fifo(char* file_name){
	printf(GRN "%s\n", file_name);
	char str[31];
	int fs;
	fs = open(file_name, O_RDONLY);
	read(fs, str, 31);
	printf(CYN "%s   \n", str);
}

void read_line(int fl, int fn, int fs){
	// read string, write in fifo
	int ok = 0, *linii;
	char str[31];

	do{
		printf(MAG "Enter max 30 characters: ");
		scanf("%s", str);
		str[strlen(str)+1]='\0';
		printf("\n");
		// write in fifo
		if (is_alpha(str[0])){
			write(fl, str, strlen(str));
		}else if (is_digit(str[0])){
			write(fn, str, strlen(str));
		}else{
			write(fs, str, strlen(str));
		}
		printf(YEL "Read another line? 1/0 ");
		scanf("%d", &ok);
	}while(ok == 1);
}

int main(){

	int fl, fn, fs, ok = 0;

	printf("START-%d\n", getpid());

	// create fifo
	create_fifo(FLETTER);
	create_fifo(FNUMBER);
	create_fifo(FSOMETHING);
	
	// open fifo
	fl = open(FLETTER, O_RDWR);
	fn = open(FNUMBER, O_RDWR);
	fs = open(FSOMETHING, O_RDWR);

	// read string, write in fifo
	read_line(fl, fn, fs);

	// read from fifo, print string
	read_fifo(FLETTER);
	read_fifo(FNUMBER);
	read_fifo(FSOMETHING);

	close(fl);
	close(fn);
	close(fs);

	// delete fifo
	delete_fifo(FLETTER);
	delete_fifo(FNUMBER);
	delete_fifo(FSOMETHING);

	return 0;
}

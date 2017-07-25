#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define __DEBUG__

//Размер конечного файла с заголовком
#define FILE_LENGTH 480054
//Размер заголовка
#define FIRST_HEADER 55
//Размер файла с цифрами
#define NUMER_SIZE 60015
//Размер файла без заголовка
#define NUMER_PIXELS 60000

//имена файлов цифр и точек
#define ZERO_FILE	"0.pgm"
#define ONE_FILE	"1.pgm"
#define TWO_FILE	"2.pgm"
#define THREE_FILE	"3.pgm"
#define FOUR_FILE	"4.pgm"
#define FIVE_FILE	"5.pgm"
#define SIX_FILE	"6.pgm"
#define SEVEN_FILE	"7.pgm"
#define EIGHT_FILE	"8.pgm"
#define NINE_FILE	"9.pgm"
#define DOTS_FILE	"dots.pgm"

//заголовок конечного PGM-файла
char * header = "P5\x0A# CREATOR: GIMP PNM Filter Version 1.1\x0A\x36\x30\x30 800\x0A\x32\x35\x35\x0A";

char numer_files[10][6]={ZERO_FILE, ONE_FILE, TWO_FILE, THREE_FILE, FOUR_FILE, FIVE_FILE,\
						 SIX_FILE,SEVEN_FILE, EIGHT_FILE, NINE_FILE};

void* image;

static int device = 0;
int first_header = FIRST_HEADER;

void print_symbol (unsigned int position, char * filename) {
	void * numer_im;
	int numer;
	numer = open (filename, O_RDWR);
	numer_im = mmap (0, NUMER_SIZE, PROT_WRITE, MAP_SHARED, numer, 0);
	memcpy(&image[first_header + NUMER_PIXELS * position], &numer_im[16], NUMER_PIXELS);
	munmap (numer_im, NUMER_SIZE);
	close(numer);
}

int main (int argc, char* const argv[])
{
	int fd;
	int result;

	struct tm *tm_ptr;
	time_t the_time;
	int file_lenght = FILE_LENGTH;

	if (argc > 1) {
		device = 1;
		first_header = 0;
		file_lenght = 480000;
	}
	
	if (!device) {
		fd = open ("test.pgm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	
	} else {
		fd = open (argv[1], O_RDWR, S_IRUSR | S_IWUSR);
	}
	if (fd == -1) {
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
	}
	//result = lseek (fd, FILE_LENGTH+1, SEEK_SET);
	result = lseek (fd, file_lenght - 1, SEEK_SET);
	if (result == -1) {
		close(fd);
		perror("Error calling lseek() to 'stretch' the file");
		exit(EXIT_FAILURE);
	}
	result = write(fd, "", 1);
	if (result != 1) {
		close(fd);
		perror("Error writing last byte of the file");
		exit(EXIT_FAILURE);
	}

	image = mmap (0, file_lenght, PROT_WRITE, MAP_SHARED, fd, 0);
	if (image == MAP_FAILED) {
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    }
	if (!device) {
		memcpy(image, header,FIRST_HEADER);
	}

//Получаем ЛОКАЛЬНОЕ время
	(void)time(&the_time);
	tm_ptr = localtime(&the_time);

#ifdef __DEBUG__
	printf("time: %02d:%02d:%02d\n",\
		tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
#endif
//выводим часы
	print_symbol(0 ,numer_files[(tm_ptr->tm_hour - tm_ptr->tm_hour%10) / 10]);
	print_symbol(1 ,numer_files[tm_ptr->tm_hour%10]);
	print_symbol(2 ,DOTS_FILE);

//выводим минуты
	print_symbol(3 ,numer_files[(tm_ptr->tm_min - tm_ptr->tm_min%10) / 10]);
	print_symbol(4 ,numer_files[tm_ptr->tm_min%10]);
	print_symbol(5 ,DOTS_FILE);

//выводим секунды
	print_symbol(6 ,numer_files[(tm_ptr->tm_sec - tm_ptr->tm_sec%10) / 10]);
	print_symbol(7 ,numer_files[tm_ptr->tm_sec%10]);
	//msync(image, FILE_LENGTH, MS_ASYNC);
	msync(image, file_lenght, MS_ASYNC);
	sleep(1);

	if (munmap(image, file_lenght) == -1) {
		perror("Error un-mmapping the file");
		/* Decide here whether to close(fd) and exit() or not. Depends... */
	}
	close (fd);
	return 0;
}


#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

unsigned int str_length(char * str);
int find_str(char * find,char * string);
void str_add_str (char * out, char * in);


int find_str(char * find,char * string){
	 int counter = 0;
	int find_length = str_length(find);
	int string_length = str_length(string);
	unsigned int coincidence = 0;

	while(counter <= (string_length - find_length)){
		int y;
		for (y = 0;y <= find_length;y++ ){
			if (find[y] == string[counter]){
				coincidence++;
				if (coincidence == find_length) return 1;
		}else{
				coincidence = 0;
				y = find_length + 1;
			}
			counter++;

		}
	}
	return 0;
}

unsigned int str_length(char * str){
	unsigned int i = 0;
	while(str[i] != '\0'){
		i++;
	}
	return i;
}

void str_add_str ( char * in,char * out){
	unsigned int i = 0;
	int in_len = str_length(in);
//	in_len ? in_len++ : 0;
	int out_len = str_length(out);
	for (i = 0;i<out_len;i++){
		in[in_len+i] = out[i];
	}
}

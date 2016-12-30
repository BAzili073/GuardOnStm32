#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "my_string.h"

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

void str_add_str ( char * in,int size_in,char * out,unsigned int len){
	unsigned int i = 0;
	unsigned int in_len = str_length(in);
	unsigned int out_len = str_length(out);
	if ((out_len > len) && (len)) out_len = len;
//	in_len ? in_len++ : 0;
	for (i = 0;i<out_len;i++){
		if ((in_len+i) >= size_in) return;
		in[in_len+i] = out[i];
	}
}

void str_add_num(char * out,int num){
	int i = 5;
	int out_len = str_length(out);
	for (i = 5;i;i--){
		int a = num/(10^i);
		if (a || (i == 1)){
			num = num-(a*(10^i));
			out[out_len] = (48 + a);
			out_len++;
		}
	}
}

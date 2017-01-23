#ifdef STR_H
#else
	#define SRT_H
	void str_add_num(char * out,int num);
	unsigned int str_length(char * str);
	int find_str(char * find,char * string);
	void str_add_str ( char * in,int size_in,char * out,unsigned int len);
	uint8_t get_size_number(int32_t num);
	 unsigned int counter;
#endif

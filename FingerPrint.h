int FP_detect_time;
char FP_check_allow;
char FP_time_for_rec;
char FP_del_base;

int alarm_flag[3];
int FP_time_check;
int FP_time_reset;
int FP_time_reset;
int time_LED[3];

#define ALARM_FLAG_TEMPERATURE 0
#define ALARM_FLAG_ACC 1
#define ALARM_FLAG_FP_TRY 2

#define LED_RED_FOR_TIME 0
#define LED_GREEN_FOR_TIME 1
#define LED_BLUE_FOR_TIME 2


void give_access(int time);
void FP_check_function();

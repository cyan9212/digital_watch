#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<termio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

pthread_t thread1;
pthread_t thread2;
pthread_t thread3;
pthread_t thread4;

void *watch_mode(void *);
void *alarm_mode(void *);
void *option_a_underbar(void *);
void *option_c_underbar(void *);
void print_time();
int getch(void);
void gotoxy(int x,int y);
void option_a();
void option_c();
int Alarm = 0;
int is_option_a = 0;
int is_option_c = 0;
int a_count = 0;
int c_count = 0;
int alarming = 0;
char *day_name[] = {"MO","TU","WE","TH","FR","SA","SU"};
struct Time{
	int year;
	int month;
	int day;
	int dow;
	int hour;
	int min;
	int sec;
	int ampm;
};

struct Time currentTime = {2017,1,1,6,12,0,0,0};
struct Time alarmTime = {0,0,0,0,0,0,0,0};
int main()
{
	pthread_create(&thread1,NULL,&watch_mode,NULL);
	while(1)
	{
		char c;
		c = getch();
		if(c == 'a'&& alarming == 0)
		{
			is_option_a = 1;
			pthread_cancel(thread1);
			option_a();
			is_option_a = 0;
			pthread_create(&thread1,NULL,&watch_mode,NULL);
		}
		if(c == 'c' && alarming == 0)
		{
			is_option_c = 1;
			option_c();
			is_option_c = 0;
		}
		if(alarming == 1)
		{
			if(c == 'a'||c == 'b'||c == 'c'||c == 'd')
				alarming =0;
		}
	
	}
}

int getch(void)
{
	int ch;
	struct termios buf, save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0,TCSAFLUSH,&buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH,&save);
	return ch;
}
void *watch_mode(void *arg)
{
	while(1)
	{	
		if(is_option_c == 0 && alarming == 0)
			print_time();
		currentTime.sec++;
		if(currentTime.sec == 60)
		{
			currentTime.sec = 0;
			currentTime.min++;
			if(currentTime.min == 60)
			{
				currentTime.min = 0;
				currentTime.hour++;
			}
			if(currentTime.hour == 12)
			{
				currentTime.ampm++;
			}
			if(currentTime.hour == 13)
			{
				currentTime.hour = 1;
			}
			if(currentTime.ampm == 2)
			{
				currentTime.ampm = 0;
				currentTime.day++;
			}
			if(currentTime.month == 1||currentTime.month == 3||currentTime.month == 5||currentTime.month == 7)
			{
				if(currentTime.day == 32)
				{
					currentTime.day = 1;
					currentTime.month++;
					currentTime.dow++;
				}
			}
			if(currentTime.month == 8||currentTime.month == 10||currentTime.month == 12)
			{
				if(currentTime.day == 32)
				{
					currentTime.day = 1;
					currentTime.month++;
					currentTime.dow++;
				}
			}
			if(currentTime.month == 4||currentTime.month == 6||currentTime.month == 9||currentTime.month == 11)
			{
				if(currentTime.day == 31)
				{
					currentTime.day = 1;
					currentTime.month++;
					currentTime.dow++;
				}
			}
			if(currentTime.month == 2)
			{
				if(currentTime.year%4 == 0&&currentTime.year!=2100)
				{
					if(currentTime.day == 30)
					{
						currentTime.day = 1;
						currentTime.month++;
						currentTime.dow++;
					}
				}
				else
				{
					if(currentTime.day == 29)
					{
						currentTime.day = 1;
						currentTime.month++;
						currentTime.dow++;
					}
				}
			}
			if(currentTime.dow == 7)
				currentTime.dow = 0;
			if(currentTime.month = 13)
			{
				currentTime.month = 1;
				currentTime.year++;
			}
			if(currentTime.year == 2101)
			{
				currentTime.year = 2000; ///
			}
			if(currentTime.hour == alarmTime.hour && currentTime.min == alarmTime.min && currentTime.sec == 0 && Alarm == 1)
				pthread_create(&thread2,NULL,&alarm_mode,NULL);

		}

		sleep(1);
	}
}
void gotoxy(int x,int y)
{
	printf("\033[%d;%df",y,x);
	fflush(stdout);
}
void option_a()
{
	int count = 1;
	pthread_create(&thread4,NULL,&option_a_underbar,NULL);
	while(1)
	{
		a_count = count;
		print_time();
		char opt;
		opt = getch();
		if(opt == 'a')
		{
			pthread_cancel(thread4);
			break;
		}
		if(opt == 'b')
		{
			if(count == 1)
			{
				if(currentTime.year+1<=2100)
				{
					currentTime.year++;
					if(currentTime.year%4 == 0 && currentTime.year!=2100 &&currentTime.month>2)
					{
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
					else if((currentTime.year-1)%4 == 0 && currentTime.month == 1)
					{
						
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
					else if((currentTime.year-1)%4 == 0 && currentTime.month == 2 && currentTime.day == 29)
					{
						currentTime.dow++;
						if(currentTime.dow == 7);
							currentTime.dow = 0;
					}
					else if((currentTime.year-1)%4 == 0 && currentTime.month == 2)
					{	
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
					else
					{
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
							
				}
				else if(currentTime.year+1 == 2101)
					currentTime.year = 2000;
			}
			if(count == 2)
			{
				if(currentTime.month == 1||currentTime.month == 3||currentTime.month == 5|| currentTime.month == 7)
				{
					int i;
					for(i=0;i<3;i++)
					{
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
				}
				else if(currentTime.month == 8||currentTime.month == 10||currentTime.month == 12)
				{
					int i;
					for(i=0;i<3;i++)
					{
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
				}
				else if(currentTime.month == 4||currentTime.month == 6||currentTime.month == 9||currentTime.month == 11)
				{
					int i;
					for(i=0;i<2;i++)
					{
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
				}
				else
				{
					if(currentTime.year%4 == 0 && currentTime.year!=2100)
					{
						currentTime.dow++;
						if(currentTime.dow == 7)
							currentTime.dow = 0;
					}
				}
				if(currentTime.month+1<=12)
					currentTime.month++;
				
				else if(currentTime.month+1 == 13)
					currentTime.month = 1;
			}
			if(count == 3)
			{
				if(currentTime.month == 1||currentTime.month == 3||currentTime.month == 5||currentTime.month == 7)
				{
					if(currentTime.day+1<=31)
					{
						currentTime.day++;
						currentTime.dow++;
					}
					else
					{
						currentTime.day = 1;
						currentTime.dow++;
					}
				}
				else if(currentTime.month == 8||currentTime.month == 10||currentTime.month == 12)
				{
					if(currentTime.day+1<=31)
					{
						currentTime.day++;
						currentTime.dow++;
					}
					else
					{
						currentTime.day = 1;
						currentTime.dow++;
					}
				}
				else if(currentTime.month == 4||currentTime.month == 6||currentTime.month == 9||currentTime.month == 11)
				{
					if(currentTime.day+1<=30)
					{	
						currentTime.day++;
						currentTime.dow++;
					}
					else 	
					{
						currentTime.day = 1;
						currentTime.dow++;
					}
				}
				else
				{
					if(currentTime.year%4 == 0 && currentTime.year!=2100)
					{
						if(currentTime.day+1<=29)
						{
							currentTime.day++;
							currentTime.dow++;
						}
						else
						{
							currentTime.day = 1;
							currentTime.dow++;
						}
					}
					else
					{
						if(currentTime.day+1<=28)
						{
							currentTime.day++;
							currentTime.dow++;
						}
						else
						{
							currentTime.day = 1;
							currentTime.dow++;
						}
					}
				}
				if(currentTime.dow == 7)
					currentTime.dow = 0;
			}
			if(count == 4)
			{
				if(currentTime.hour+1<=12)
					currentTime.hour++;
				else
				{
					currentTime.hour = 1;
				}
				if(currentTime.hour == 12)
					currentTime.ampm++;
				if(currentTime.ampm == 2)
					currentTime.ampm = 0;
			}
			if(count == 5)
			{
				if(currentTime.min+1<=59)
					currentTime.min++;
				else
					currentTime.min = 0;
			}
			if(count == 6)
			{
				if(currentTime.sec+1<=59)
					currentTime.sec++;
				else
					currentTime.sec = 0;
			}
		}
		if(opt == 'c')				
		{
			count++;
			if(count == 7)
				count = 1;
		}
	}
		
}
void option_c()
{
	int count = 1;
	pthread_create(&thread3,NULL,&option_c_underbar,NULL);
	while(1)
	{
		c_count = count;
		char opt;
		system("clear");
		gotoxy(0,1);
		printf("\nAL ");
		if(Alarm == 0)
			printf("OFF ");
		if(Alarm == 1)
			printf(" ON ");
		if(alarmTime.hour<10)
			printf("0%d ",alarmTime.hour);
		else
			printf("%d ",alarmTime.hour);
		if(alarmTime.min<10)
			printf("0%d ",alarmTime.min);
		else
			printf("%d ",alarmTime.min);
		if(alarmTime.ampm == 0)
			printf("AM\n");
		else
			printf("PM\n");
		opt = getch();
		if(opt == 'a')
		{
			pthread_cancel(thread3);
			break;
		}
		if(opt == 'b')
		{
			if(count == 1)
			{
				if(alarmTime.hour+1<=12)
					alarmTime.hour++;
				else
					alarmTime.hour = 1;
				if(alarmTime.hour == 12)
					alarmTime.ampm++;
				if(alarmTime.ampm == 2)
					alarmTime.ampm = 0;
			}
			if(count == 2)
			{
				if(alarmTime.min+1<=59)
					alarmTime.min++;
				else
					alarmTime.min = 0;
			}
		}
		if(opt == 'c')
		{
			count++;
			if(count == 3)
				count = 1;
		}
		if(opt == 'd')
		{
			pthread_cancel(thread3);
			if(Alarm == 0)
				Alarm = 1;
			else
				Alarm = 0;

			system("clear");
			printf("\nAL ");
			if(Alarm == 0)
				printf("OFF ");
			else
				printf(" ON ");
			if(alarmTime.hour<10)
				printf("0%d ",alarmTime.hour);
			else
				printf("%d ",alarmTime.hour);
			if(alarmTime.min<10)
				printf("0%d ",alarmTime.min);
			else
				printf("%d ",alarmTime.min);
			if(alarmTime.ampm == 0)
				printf("AM\n");
			else
				printf("PM\n");
			sleep(2);
			break;
		}
		
	}

}
void print_time()
{
	system("clear");
	gotoxy(0,0);
	printf("\n%d ",currentTime.year);
	if(currentTime.month<10)
	 	printf("0%d ",currentTime.month);
	else
	 	printf("%d ",currentTime.month);
	if(currentTime.day<10) 
	 	printf("0%d ",currentTime.day);
	else
		printf("%d ",currentTime.day);
        printf("%s\n\n",day_name[currentTime.dow]);
	if(currentTime.ampm == 0) 
		printf("  AM ");
        else
		printf("  PM ");
	if(currentTime.hour<10)
        	printf("0%d ",currentTime.hour);
        else
		printf("%d ",currentTime.hour);
	if(currentTime.min<10)
		printf("0%d ",currentTime.min);
	else
		printf("%d ",currentTime.min);
	if(currentTime.sec<10)
		printf("0%d\n\n",currentTime.sec);
        else
        	printf("%d\n\n",currentTime.sec);
	if(is_option_a == 0)
	{
		printf("  AL ");
		if(Alarm == 0)
			printf("OFF\n");
		else
			printf(" ON\n");
	}
}
void *alarm_mode(void *arg)
{
	alarming = 1;
	system("clear");
	gotoxy(0,0);
	printf("\nAL ");
	printf(" ON ");
	if(alarmTime.hour<10)
		printf("%0d ",alarmTime.hour);
	else
		printf("%d ",alarmTime.hour);
	if(alarmTime.min<10)
		printf("0%d",alarmTime.min);
	else
		printf("%d",alarmTime.min);
	if(alarmTime.ampm == 0)
		printf(" AM\n");
	else
		printf(" PM\n");
	while(alarming == 1)
	{	
		printf("\007\n");
		sleep(1);
	}
}
void *option_a_underbar(void *arg)
{
	while(1)
	{
		if(a_count == 1)
		{
			gotoxy(0,3);
			printf("----\n");
			sleep(1);
			gotoxy(0,3);
			printf("    \n");
			sleep(1);
		}
		if(a_count == 2)
		{
			gotoxy(6,3);
			printf("--\n");
			sleep(1);
			gotoxy(6,3);
			printf("  \n");
			sleep(1);
		}
		if(a_count == 3)
		{
			gotoxy(9,3);
			printf("--\n");
			sleep(1);
			gotoxy(9,3);
			printf("  \n");
			sleep(1);
		}
		if(a_count == 4)
		{
			gotoxy(6,5);
			printf("--\n");
			sleep(1);
			gotoxy(6,5);
			printf("  \n");
			sleep(1);
		}
		if(a_count == 5)
		{
			gotoxy(9,5);
			printf("--\n");
			sleep(1);
			gotoxy(9,5);
			printf("  \n");
			sleep(1);
		}
		if(a_count == 6)
		{
			gotoxy(12,5);
			printf("--\n");
			sleep(1);
			gotoxy(12,5);
			printf("  \n");
			sleep(1);
		}
	}

}
void *option_c_underbar(void *arg)
{
	while(1)
	{
		if(c_count == 1)
		{
			gotoxy(8,3);
			printf("--\n");
			sleep(1);
			gotoxy(8,3);
			printf("  \n");
			sleep(1);
		}
		if(c_count == 2)
		{
			gotoxy(11,3);
			printf("--\n");
			sleep(1);
			gotoxy(11,3);
			printf("  \n");
			sleep(1);
		}
	}
}










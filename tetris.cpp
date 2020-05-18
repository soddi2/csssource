#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

char form[7][4][8] =
{
	{{1,0,1,1,1,2,1,3}, {0,1,1,1,2,1,3,1}, {1,0,1,1,1,2,1,3}, {0,1,1,1,2,1,3,1}},
	{{1,1,2,1,1,2,2,2}, {1,1,2,1,1,2,2,2}, {1,1,2,1,1,2,2,2}, {1,1,2,1,1,2,2,2}},
	{{1,1,2,1,0,2,1,2}, {1,0,1,1,2,1,2,2}, {2,1,0,2,1,2,1,1}, {1,0,1,1,2,1,2,2}},
	{{0,1,1,1,1,2,2,2}, {2,0,2,1,1,1,1,2}, {0,1,1,1,1,2,2,2}, {2,0,2,1,1,1,1,2}},
	{{1,0,1,1,1,2,2,2}, {0,1,1,1,2,1,0,2}, {1,0,2,0,2,1,2,2}, {2,1,2,2,1,2,0,2}},
	{{2,0,2,1,2,2,1,2}, {0,1,0,2,1,2,2,2}, {1,0,2,0,1,1,1,2}, {0,1,1,1,2,1,2,2}},
	{{0,1,1,1,2,1,1,2}, {2,0,2,1,2,2,1,1}, {1,1,1,2,0,2,2,2}, {1,0,1,1,1,2,2,1}}
};

int tetris[21][12];
int form_kind;
int form_kind_next;
int rotate_kind;
int screen_x, screen_y;
int score=0; // 점수
int level; // 레벨
DWORD delay; //레벨에 따른 속도 조절

void CurrentXY(int x, int y)
{
	COORD Location;
	Location.X = x;
	Location.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Location);
}

void CursorOff(void)
{
	CONSOLE_CURSOR_INFO Information;

	Information.dwSize = 1;
	Information.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Information);
}

void CursorOn(void)
{
	CONSOLE_CURSOR_INFO Information;

	Information.dwSize = 20;
	Information.bVisible = TRUE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Information);
}

void Print_form()
{
	int i;

	for(i=0; i<4; i++)
	{
		CurrentXY(30 +(screen_x+form[form_kind][rotate_kind][i*2])*2, 
			screen_y + form[form_kind][rotate_kind][i*2+1]);

		tetris[screen_y + form[form_kind][rotate_kind][i*2+1]]
			[screen_x + form[form_kind][rotate_kind][i*2]] = 1;

		printf("□");
	}
}
//===================================================================
void Next_Print_form()
{
	int i;

	for(i=0; i<4; i++)
	{
		CurrentXY(60 +(0+form[form_kind][rotate_kind][i*2])*2, 
			8 + form[form_kind][rotate_kind][i*2+1]);
		printf(" ");
	}

	for(i=0; i<4; i++)
	{
		CurrentXY(60 +(0+form[form_kind_next][rotate_kind][i*2])*2, 
			8 + form[form_kind_next][rotate_kind][i*2+1]);
		printf("□");
	}
}
//===================================================================

void Delete_form()
{
	int i;

	for(i=0; i<4; i++)
	{
		CurrentXY(30 +(screen_x+form[form_kind][rotate_kind][i*2])*2, 
			screen_y + form[form_kind][rotate_kind][i*2+1]);

		tetris[screen_y + form[form_kind][rotate_kind][i*2+1]]
			[screen_x + form[form_kind][rotate_kind][i*2]] = 0;

		printf(" ");
	}
}

void Initial()
{
	int i, j;

	for(i=0; i<21; i++)
	{
		for(j=0;j<12;j++)
		{
			if(i==20 || j==0 || j==11)
				tetris	[i][j]=3;
			else
				tetris[i][j] =0;
		}
	}

	for(i=0; i<21; i++)
	{
		CurrentXY(30, i);
		printf("│");
		CurrentXY(52, i);
		printf("│");
	}

	CurrentXY(30,20);
	printf("└");
	for(j=1; j<11; j++)
	{
		CurrentXY(30 +j*2, 20);
		printf("─");
	}
	CurrentXY(52, 20);
	printf("┘");
}

int Check_board(int location_x, int location_y)
{
	int i, temp = 0;

	for(i=0; i<4; i++)
	{
		temp = tetris[location_y + form[form_kind][rotate_kind][i*2+1]]
			[location_x + form[form_kind][rotate_kind][i*2]];

		if(temp>1) return TRUE;
	}
	return FALSE;
}

void Print_data()
{
	int i,j;

	for(i=0; i<20; i++)
		for(j=1; j<11; j++)
			if(tetris[i][j]==2)
			{
				CurrentXY(30 + j*2, i);
				printf("□");
			}
			else
			{
				CurrentXY(30 + j*2, i);
				printf(" ");
			}
}

void Move_data(int row)
{
	int i,j;

	for(i=row-1; i>=0; i--)
		for(j=1; j<11; j++)
			tetris[i+1][j] = tetris[i][j];
}

int Check_line2(int row)
{
	int chk=0;
	int j;

	for(j=1; j<11; j++)
		chk += tetris[row][j];
	if(chk == 20) return TRUE;
	else return FALSE;
}

void Check_line()
{
	int rst;
	int i;

	for(i=screen_y; i<screen_y+4; i++)
	{
		rst = Check_line2(i);

		if(rst == TRUE)
		{
			Move_data(i);
			Print_data();
			i--;
			score++;
		}

	}
}

void Write_board()
{
	int i;

	for(i=0; i<4; i++)
	{
		tetris[screen_y + form[form_kind][rotate_kind][i*2+1]]
			[screen_x + form[form_kind][rotate_kind][i*2]] =2;
	}
}

int Go_down()
{
	int chk2;

	chk2 = Check_board(screen_x, screen_y +1);

	if(chk2==TRUE)
	{
		Write_board();
		Check_line();
		return TRUE;
	}

	Delete_form();
	screen_y++;
	Print_form();
	return FALSE;
}

int Select(void)
{
	int chk1;
	int prev_rotate;
	int new_rotate;
	int rtn_value;

	if(GetAsyncKeyState(VK_LEFT)){
		chk1 = Check_board(screen_x-1, screen_y);

		if(chk1==FALSE)
		{
			Delete_form();
			screen_x--;
			Print_form();
			return TRUE;
		}
	}

	if(GetAsyncKeyState(VK_RIGHT))
	{
		chk1 = Check_board(screen_x+1, screen_y);

		if(chk1==FALSE)
		{
			Delete_form();
			screen_x++;
			Print_form();
			return TRUE;
		}
	}

	if(GetAsyncKeyState(VK_UP))
	{
		prev_rotate = rotate_kind;

		if(rotate_kind==3) rotate_kind=0;
		else rotate_kind++;

		new_rotate = rotate_kind;

		chk1 = Check_board(screen_x, screen_y);

		if(chk1==FALSE)
		{
			rotate_kind = prev_rotate;
			Delete_form();

			rotate_kind = new_rotate;
			Print_form();
			return TRUE;
		}
		else
		{
			rotate_kind = prev_rotate;
		}
	}

	if(GetAsyncKeyState(VK_SPACE))
	{
		while(!Go_down());
		return FALSE;
	}

	rtn_value = Go_down();

	if(rtn_value==TRUE) return FALSE;
	else return TRUE;
}
void Timing(void)
{
	DWORD thisTickCount;
	static DWORD lastTickCount = 0;

	thisTickCount = GetTickCount();

	while(1)
	{
		if(thisTickCount - lastTickCount > delay)
		{
			lastTickCount = thisTickCount;
			return;
		}
		thisTickCount = GetTickCount();
	}
}

void SetColor(int color){ // 글자 색상을 지정한다(0~15)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color); 
}


void main()
{
	int chk, chk2;
	
	CursorOff();

	SetColor(11);
	CurrentXY(20, 20);
	printf("첫화면 멋지게 꾸며 볼 것\n\n");
	
	/*
	SetColor(10);
	printf("게임 시작하려면 아무키나 누르시오.\n\n");
	getch();
	fflush(stdin);
	*/
	
	printf("게임 시작하려면 엔터키를 누르시오.\n\n");
	while(1)
	{
		if(GetAsyncKeyState(VK_RETURN))	break;
		else Sleep(100);
	}


	system("cls");

	SetColor(7);
	Initial();

	srand((unsigned)time(NULL));
	form_kind_next = rand()%7;

	do{
		screen_y = 0;
		screen_x = 4;
		
		form_kind = form_kind_next;
		form_kind_next = rand()%7;		

		rotate_kind = 0;		

		if (score>=30) {
			level = 5;
			delay=50;
		}else if(score >=20) {
			level = 4;
			delay=100;
		}else if(score >=10) {
			level = 3;
			delay=200;
		}else if(score >=5) {
			level = 2;
			delay=250;
		}else {
			level = 1;
			delay=300;
		}

		CurrentXY(56, 3);
		printf("%d level", level);

		CurrentXY(56, 4);
		printf("점수:%d", score);

		CurrentXY(56, 6);
		printf("NEXT");
		CurrentXY(60, 8);

		Print_form();
		Next_Print_form();

		chk = Check_board(screen_x, screen_y);
		chk2 = Check_board(screen_x, screen_y+1);

		if(chk==FALSE && chk2 ==TRUE)
		{
			Write_board();
			Check_line();
			break;
		}
		else
			if(chk==TRUE && chk2==TRUE)
				break;
			do{
				Timing();
			}while(Select());	
	}while(1);

	system("cls");

	printf("끝났습니다.\n당신의 점수는 %d이고, 레벨은 %d입니다.", score, level);

	Sleep(5000);
 	CursorOn();
} 

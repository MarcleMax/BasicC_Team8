#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#define LEFT 75 //�·� �̵�    //Ű���尪�� 
#define RIGHT 77 //��� �̵� 
#define UP 72 //ȸ�� 
#define DOWN 80 //soft drop
#define SPACE 32 //hard drop
#define p 112 //�Ͻ����� 
#define P 80 //�Ͻ�����
#define ESC 27 //�������� 

#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // �����ǹ迭�� ����� ����� ���µ� 
#define CEILLING -1     // ����� �̵��� �� �ִ� ������ 0 �Ǵ� ���� ������ ǥ�� 
#define EMPTY 0         // ����� �̵��� �� ���� ������ ����� ǥ�� 
#define WALL 1
#define INACTIVE_BLOCK 2 // �̵��� �Ϸ�� ��ϰ�
#define BOMB -3 //��ź
#define BLANK -4 //��ĭ
#define OBSTACLE_BLOCK 3 //���� ��

#define MAIN_X 11 //������ ����ũ�� 
#define MAIN_Y 23 //������ ����ũ�� 
#define MAIN_X_ADJ 3 //������ ��ġ���� 
#define MAIN_Y_ADJ 1 //������ ��ġ���� 

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //��������ǥ�� ��ġ���� 

int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ���� 
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����

int blocks[8][4][4][4] = {
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}},

}; //��ϸ�� ���� 4*4������ ����� ǥ�� blcoks[b_type][b_rotation][i][j]�� ��� 

int b_type; //��� ������ ���� 
int b_rotation; //��� ȸ���� ���� 
int b_type_next; //���� ��ϰ� ���� 

int main_org[MAIN_Y][MAIN_X]; //�������� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� main_cpy�� ����� 
int main_cpy[MAIN_Y][MAIN_X]; //�� maincpy�� �������� ����Ϳ� ǥ�õǱ� ���� ������ ������ ���� 
							  //main�� ��ü�� ��� ����Ϳ� ǥ������ �ʰ�(�̷��� �ϸ� ����Ͱ� �����Ÿ�) 
							  //main_cpy�� �迭�� ���ؼ� ���� �޶��� ���� ����Ϳ� ��ħ
int main_cpy_blank[MAIN_Y][MAIN_X]; //blank ��ġ ��Ͽ� �迭
									//�Ϲ� ���� �������鼭 ��ĭ �� ǥ�ð� ������� ����.
int main_cpy_drop[MAIN_Y][MAIN_X]; //�Ϲ� ���� drop�� ���� ���� ���� cpy
								   //blank �Լ��� �̻��ϰ� �۵��ؼ� ����
int bx, by; //�̵����� ����� �����ǻ��� x,y��ǥ�� ���� 

int key; //Ű����� �Է¹��� Ű���� ���� 

int speed; //��������ӵ� 
int level; //���� level 
int level_goal; //���������� �Ѿ�� ���� ��ǥ���� 
int cnt; //���� �������� ������ �� ���� ���� 
int score; //���� ���� 
int last_score = 0; //�������������� 
int best_score = 0; //�ְ�������� 

int new_block_on = 0; //���ο� ���� �ʿ����� �˸��� flag 
int crush_on = 0; //���� �̵����� ����� �浹�������� �˷��ִ� flag
int space_key_on = 0; //hard drop�������� �˷��ִ� flag 

int count_blank; //�����ִ� ��ĭ ���� ����
int num_blank; //������ ��ĭ �� ����

void title(void); //���ӽ���ȭ�� 
void reset(void); //������ �ʱ�ȭ 
void reset_main(void); //���� ������(main_org[][]�� �ʱ�ȭ)
void reset_main_cpy(void); //copy ������(main_cpy[][]�� �ʱ�ȭ)
void draw_map(void); //���� ��ü �������̽��� ǥ�� 
void draw_main(void); //�������� �׸� 
void draw_blank(void); //��ĭ �� �ٽ� �׸���
void make_blank(void);//��ĭ �� ����
void make_obstacle(void);//���� �� ����
void steal_block(void);//�� �������
void steal_block_start(void); //�� ������� �Լ� ����
void new_block(void); //���ο� ����� �ϳ� ���� 
void check_key(void); //Ű����� Ű�� �Է¹��� 
void check_key_opp(void); //Ű����� Ű�� �Է¹��� - Ű �ݴ� Ÿ��
void drop_block(void); //����� �Ʒ��� ����Ʈ�� 
int check_crush(int bx, int by, int rotation); //bx, by��ġ�� rotationȸ������ ���� ��� �浹 �Ǵ� 
void move_block(int dir); //dir�������� ����� ������
void move_bomb(int dir); //dir�������� ��ź�� ������
void check_line(void); //���� ����á������ �Ǵ��ϰ� ����
void check_line_obstacle(void); //���� ���� ���� �ÿ� ���� ����á������ �Ǵ��ϰ� ���� ����
void check_blank(void);
void check_level_up(void); //������ǥ�� �޼��Ǿ������� �Ǵ��ϰ� levelup��Ŵ
void check_level_up_tens(void); // ���� �ڸ� �� ���� �� �׸�
void check_level_up_units(void); //���� �ڸ� ���� ���� �� �׸�
void check_game_over(void); //���ӿ������� �Ǵ��ϰ� ���ӿ����� ���� 
void pause(void);//������ �Ͻ�������Ŵ
void clear_game(void); //���� Ŭ���� �� ���� ȭ��

void gotoxy(int x, int y) { //gotoxy�Լ� 
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ��� ���Ǵ� ������ 
void setcursortype(CURSOR_TYPE c) { //Ŀ������� �Լ� 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
//main ����
int main() {
	system("mode con cols=91 lines=26"); //�ܼ� â ũ�� ����

	int i;

	srand((unsigned)time(NULL)); //����ǥ���� 
	setcursortype(NOCURSOR); //Ŀ�� ���� 
	title(); //����Ÿ��Ʋ ȣ�� 
	reset(); //������ ���� 

	while (level < 101) {

		for (i = 0; i < 5; i++) { //����� ��ĭ�������µ��� 5�� Ű�Է¹��� �� ���� 
			if (level % 10 == 0 || level % 10 == 5) {
				check_key_opp(); //Ű�ݴ�Ÿ��
			}
			else {
				check_key(); //Ű�Է�Ȯ��
			}
			draw_main(); // ȭ���� �׸� 
			if (((level - 1) / 10) % 4 == 1) { //TODO �̷��� ��Ĩ�ô�!!!
				draw_blank(); //��ĭ �ٽ� �׸�
			}
			Sleep(speed); //���Ӽӵ����� 
			if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
			//����� �浹���ΰ�� �߰��� �̵��� ȸ���� �ð��� ���� 
			if (space_key_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
				space_key_on = 0;
				break;
			}
		}
		drop_block(); // ����� ��ĭ ����
		check_level_up(); // �������� üũ 
		check_game_over(); //���ӿ����� üũ 
		if (new_block_on == 1) new_block(); // �� �� flag�� �ִ� ��� ���ο� �� ���� 
	}

	clear_game();
}

void title(void) {
	int x = 5; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ 
	int y = 3; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ 
	int cnt; //Ÿ��Ʋ �������� ���� ����  

	gotoxy(x, y + 0); printf("����������������"); Sleep(100);
	gotoxy(x, y + 1); printf("�����  ����    ������"); Sleep(100);
	gotoxy(x, y + 2); printf("�����              ���  ��"); Sleep(100);
	gotoxy(x, y + 3); printf("������  ��  ��  ������"); Sleep(100);
	gotoxy(x, y + 4); printf("���  �������������"); Sleep(100);
	gotoxy(x, y + 5); printf("S.C:  blog.naver.com/azure0777"); Sleep(100);
	gotoxy(x + 5, y + 2); printf("T E T R I S"); Sleep(100);
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
	gotoxy(x, y + 9); printf("  ��   : Shift");
	gotoxy(x, y + 10); printf("��  �� : Left / Right");
	gotoxy(x, y + 11); printf("  ��   : Soft Drop");
	gotoxy(x, y + 12); printf(" SPACE : Hard Drop");
	gotoxy(x, y + 13); printf("   P   : Pause");
	gotoxy(x, y + 14); printf("  ESC  : Quit");
	gotoxy(x, y + 15); printf("  ��   : Bomb");
	gotoxy(x, y + 16); printf("  ��   : Blank");
	gotoxy(x, y + 17); printf("  ��   : Obstacle");
	gotoxy(x, y + 19); printf("BONUS FOR HARD DROPS / COMBOS");

	for (cnt = 0;; cnt++) { //cnt�� 1�� ������Ű�鼭 ��� �ݺ�    //�ϳ��� ���߿��� �� ��¦�̴� �ִϸ��̼�ȿ�� 
		if (_kbhit()) break; //Ű�Է��� ������ ���ѷ��� ���� 
		if (cnt % 200 == 0) { gotoxy(x + 4, y + 1); printf("��"); }       //cnt�� 200���� ������ �������� ���� ǥ�� 
		if ((cnt % 200 - 100) == 0) { gotoxy(x + 4, y + 1); printf("  "); } //�� ī��Ʈ���� 100ī��Ʈ �������� ���� ���� 
		if ((cnt % 350) == 0) { gotoxy(x + 13, y + 2); printf("��"); } //������ ������ �ð����� ���� 
		if ((cnt % 350 - 100) == 0) { gotoxy(x + 13, y + 2); printf("  "); }
		Sleep(10); // 00.1�� ������  
	}

	while (_kbhit()) _getch(); //���ۿ� ��ϵ� Ű���� ���� 

}

void reset(void) {

	FILE* file = fopen("score.dat", "rt"); // score.dat������ ���� 
	if (file == 0) { best_score = 0; } //������ ������ �� �ְ������� 0�� ���� 
	else {
		fscanf(file, "%d", &best_score); // ������ ������ �ְ������� �ҷ��� 
		fclose(file); //���� ���� 
	}

	level = 1; //�������� �ʱ�ȭ 
	score = 0;
	level_goal = 1000;
	key = 0;
	crush_on = 0;
	cnt = 0;
	speed = 100;

	system("cls"); //ȭ������ 
	reset_main(); // main_org�� �ʱ�ȭ 
	draw_map(); // ����ȭ���� �׸�
	draw_main(); // �������� �׸� 

	b_type_next = rand() % 7; //�������� ���� ��� ������ �����ϰ� ���� 
	new_block(); //���ο� ����� �ϳ� ����  

}

void reset_main(void) { //�������� �ʱ�ȭ  
	int i, j, add;

	if ((level > 40 && level < 51) || (level > 80 && level < 91)) {
		add = (level - 1) % 10 + 1;
	}
	else {
		add = 0;
	}

	for (i = 0; i < MAIN_Y; i++) { // �������� 0���� �ʱ�ȭ  
		for (j = 0; j < MAIN_X; j++) {
			main_org[i][j] = 0;
			main_cpy[i][j] = 100;
		}
	}
	for (j = 1; j < MAIN_X; j++) { //y���� 3�� ��ġ�� õ���� ���� 
		main_org[3 + add][j] = CEILLING;
	}
	for (i = 1; i < MAIN_Y - 1; i++) { //�¿� ���� ����  
		main_org[i][0] = WALL;
		main_org[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j < MAIN_X; j++) { //�ٴں��� ���� 
		main_org[MAIN_Y - 1][j] = WALL;
	}
}

void reset_main_cpy(void) { //main_cpy�� �ʱ�ȭ 
	int i, j;

	for (i = 0; i < MAIN_Y; i++) {         //�����ǿ� ���ӿ� ������ �ʴ� ���ڸ� ���� 
		for (j = 0; j < MAIN_X; j++) {  //�̴� main_org�� ���� ���ڰ� ���� �ϱ� ���� 
			main_cpy[i][j] = 100;
		}
	}
}

void make_blank(void) {
	int i, j, ri = 23, rj = 0, count = 0; //i_y, j_x

	if (level % 10 == 0) {//level�� �����ڸ� ������ 3�� ����
		num_blank = 10 + 3;
	}
	else {
		num_blank = (level % 10) + 3;
	}
	for (int n = 0; n < num_blank; n++) {
		ri = rand() % (MAIN_Y - 6);//�ٴڰ� �Ʒ����� ���� 18��° ĭ ���̿� ��ĭ �� ����
		rj = rand() % (MAIN_X - 2);

		if (main_org[MAIN_Y - 2 - ri][1 + rj] == BLANK) {
			n--;
		} // ������ ���� ��ĭ�� ��ġ�� ������ �ٽ�!

		i = ri;
		j = rj;

		main_org[MAIN_Y - 2 - i][1 + j] = BLANK;
	}

	for (i = 0; i < MAIN_Y; i++) { //blank ��ġ�� main_cpy_blank�� ����  
		for (j = 0; j < MAIN_X; j++) {
			main_cpy_blank[i][j] = main_org[i][j];
		}
	}
}

void make_obstacle(void) {
	int i, j, ri, rj; //i_y, j_x

	if ((level % 10) == 0) {
		for (int n = 0; n < 12; n++) { //���� �� ����
			ri = rand() % (MAIN_Y - 8);//�ٴڰ� �Ʒ����� ���� 18��° ĭ ���̿� ��ĭ �� ����
			rj = rand() % (MAIN_X - 2);

			if (main_org[MAIN_Y - 2 - ri][1 + rj] == OBSTACLE_BLOCK) {
				n--;
			} // ������ ���� ��ĭ�� ��ġ�� ������ �ٽ�!

			i = ri;
			j = rj;


			main_org[MAIN_Y - 2 - i][1 + j] = OBSTACLE_BLOCK;
		}
	}
	else {
		for (int n = 0; n < (level % 10) + 2; n++) { //���� �� ����
			ri = rand() % (MAIN_Y - 8);//�ٴڰ� �Ʒ����� ���� 18��° ĭ ���̿� ��ĭ �� ����
			rj = rand() % (MAIN_X - 2);

			if (main_org[MAIN_Y - 2 - ri][1 + rj] == OBSTACLE_BLOCK) {
				n--;
			} // ������ ���� ��ĭ�� ��ġ�� ������ �ٽ�!

			i = ri;
			j = rj;


			main_org[MAIN_Y - 2 - i][1 + j] = OBSTACLE_BLOCK;
		}
	}
}

void steal_block(void) {
	int i, j;
	i = rand() % (MAIN_Y - 4);
	j = rand() % (MAIN_X - 2);

	if (level > 50 && level < 61) {
		if (main_org[MAIN_Y - 2 - i][j + 1] == INACTIVE_BLOCK && main_cpy_blank[MAIN_Y - 2 - i][j + 1] != BLANK)
			main_org[MAIN_Y - 2 - i][j + 1] = EMPTY;
	}
	else {
		if (main_org[MAIN_Y - 2 - i][j + 1] == INACTIVE_BLOCK) {
			main_org[MAIN_Y - 2 - i][j + 1] = EMPTY;
		}
	}
}

void steal_block_start(void) {
	int num;

	num = rand() % 5;

	if (num == 0) {
		steal_block();
	}
}

void draw_map(void) { //���� ���� ǥ�ø� ��Ÿ���� �Լ�  
	int y = 3;             // level, goal, score�� �����߿� ���� �ٲ�� �� ���� �� y���� ���� �����ص� 
						 // �׷��� Ȥ�� ���� ���� ǥ�� ��ġ�� �ٲ� �� �Լ����� �ȹٲ㵵 �ǰ�.. 
	gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y); printf(" LEVEL : %5d", level);
	gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y + 1); printf(" GOAL  : %5d", 10 - cnt);
	gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
	gotoxy(STATUS_X_ADJ, y + 8); printf(" YOUR SCORE :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score);
	gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");
	gotoxy(STATUS_X_ADJ, y + 11); printf("        %6d", last_score);
	gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");
	gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);
	gotoxy(STATUS_X_ADJ, y + 15); printf("  ��   : Shift        SPACE : Hard Drop   �� : Bomb");
	gotoxy(STATUS_X_ADJ, y + 16); printf("��  �� : Left / Right   P   : Pause       �� : Blank");
	gotoxy(STATUS_X_ADJ, y + 17); printf("  ��   : Soft Drop     ESC  : Quit        �� : Obstacle");
	gotoxy(STATUS_X_ADJ, y + 20); printf("blog.naver.com/azure0777");
}

void draw_main(void) { //������ �׸��� �Լ� 
	int i, j, add;

	if ((level > 40 && level < 51) || (level > 80 && level < 91)) {
		add = (level - 1) % 10 + 1;
	}
	else {
		add = 0;
	}

	for (j = 1; j < MAIN_X - 1; j++) { //õ���� ��� ���ο���� �������� �������� ���� �׷��� 
		if (main_org[3 + add][j] == EMPTY) main_org[3 + add][j] = CEILLING;
	}
	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			if (main_cpy[i][j] != main_org[i][j]) { //cpy�� ���ؼ� ���� �޶��� �κи� ���� �׷���.
												//�̰� ������ ��������ü�� ��� �׷��� �������� ��¦�Ÿ�
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_org[i][j]) {
				case EMPTY: //��ĭ��� 
					printf("  ");
					break;
				case CEILLING: //õ���� 
					printf(". ");
					break;
				case WALL: //����� 
					printf("��");
					break;
				case INACTIVE_BLOCK: //���� �� ���  
					printf("��");
					break;
				case ACTIVE_BLOCK: //�����̰��ִ� �� ���  
					printf("��");
					break;
				case BOMB: //��ź ���
					printf("��");
					break;
				case OBSTACLE_BLOCK: //���� �� ���
					printf("��");
					break;
				case BLANK: //��ĭ �� ���
					printf("��");
					break;
				}
			}
		}
	}
	for (i = 0; i < MAIN_Y; i++) { //�������� �׸� �� main_cpy�� ����  
		for (j = 0; j < MAIN_X; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}

void draw_blank(void) {
	int i, j;

	if (!(level > 10 && level < 21) && !(level > 50 && level < 61)) {
		return;
	}

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			if (count_blank != num_blank && main_org[i][j] == EMPTY && main_cpy_blank[i][j] == BLANK) { //�Ϲ� ���� �������� ����� ��ĭ ǥ�� �ٽ� ����.
													//���� ���� �� �ڸ��� �״�� ���д�.
				main_org[i][j] = BLANK;
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				printf("��");
			}
		}
	}
	for (i = 0; i < MAIN_Y; i++) { //�������� �׸� �� main_cpy�� ����  
		for (j = 0; j < MAIN_X; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}

void new_block(void) { //���ο� ��� ����  
	int i, j;

	bx = (MAIN_X / 2) - 1; //��� ���� ��ġx��ǥ(�������� ���) 
	by = 0;  //��� ������ġ y��ǥ(���� ��) 
	b_type = b_type_next; //���������� ������ 
	b_type_next = rand() % 8; //���� ���� ���� 
	if ((level > 30 && level < 41) || (level > 70 && level < 81) || (level > 90 && level < 101)) { //ȸ�� �Ұ� �ܰ迡���� �����ϰ� ����
		b_rotation = rand() % 4;  //ȸ���� 0������ ������ 
	}
	else {
		b_rotation = 0;  //Ư���� ��찡 �ƴ϶�� ȸ���� 0������ ������ 
	}


	new_block_on = 0; //new_block flag�� ��  

	for (i = 0; i < 4; i++) { //������ bx, by��ġ�� ������  
		for (j = 0; j < 4; j++) {
			if (b_type == 7) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = BOMB;
			} //��ź�� ����
			else {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
			} //�Ϲ� ��	
		}
	}
	for (i = 1; i < 3; i++) { //���ӻ���ǥ�ÿ� ������ ���ú��� �׸� 
		for (j = 0; j < 4; j++) {
			if (b_type_next == 7 && blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("��");
			}
			else if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("��");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void check_key(void) {
	key = 0; //Ű�� �ʱ�ȭ  

	if (_kbhit()) { //Ű�Է��� �ִ� ���  
		key = _getch(); //Ű���� ����
		if (key == 224) { //����Ű�ΰ�� 
			do { key = _getch(); } while (key == 224);//����Ű���ð��� ���� 
			if (b_type == 7) { // ��ź�� ���
				switch (key) {
				case LEFT: //����Ű ��������  
					if (check_crush(bx - 1, by, b_rotation) == true) move_bomb(LEFT);
					break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
				case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
					if (check_crush(bx + 1, by, b_rotation) == true) move_bomb(RIGHT);
					break;
				case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
					if (check_crush(bx, by + 1, b_rotation) == true) move_bomb(DOWN);
					break;
				case UP: //���� ����Ű �������� 
					if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_bomb(UP);
					//ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
					else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_bomb(100);
				}                    //�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
			}
			else {
				switch (key) {
				case LEFT: //����Ű ��������  
					if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
					break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
				case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
					if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
					break;
				case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
					if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
					break;
				case UP: //���� ����Ű ��������
					if ((level > 30 && level < 41) || (level > 70 && level < 81) || (level > 90 && level < 101)) {
						break;
					}
					else {
						if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
						//ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
						else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
						//�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
						break;
					}
				}
			}

		}
		else { //����Ű�� �ƴѰ�� 
			switch (key) {
			case SPACE: //�����̽�Ű �������� 
				space_key_on = 1; //�����̽�Ű flag�� ��� 
				while (crush_on == 0) { //�ٴڿ� ���������� �̵���Ŵ 
					drop_block();
					if (b_type != 7) {
						score += level; // hard drop ���ʽ�
						gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); //���� ǥ��
					}
				}
				break;
			case P: //P(�빮��) �������� 
			case p: //p(�ҹ���) �������� 
				pause(); //�Ͻ����� 
				break;
			case ESC: //ESC�������� 
				system("cls"); //ȭ���� ����� 
				exit(0); //�������� 
			}
		}
	}
	while (_kbhit()) _getch(); //Ű���۸� ��� 
}

void check_key_opp(void) {
	key = 0; //Ű�� �ʱ�ȭ  

	if (_kbhit()) { //Ű�Է��� �ִ� ���  
		key = _getch(); //Ű���� ����
		if (key == 224) { //����Ű�ΰ�� 
			do { key = _getch(); } while (key == 224);//����Ű���ð��� ���� 
			if (b_type == 7) { // ��ź�� ���
				switch (key) {
				case LEFT: //����Ű ��������  
					if (check_crush(bx + 1, by, b_rotation) == true) move_bomb(RIGHT);
					break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
				case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
					if (check_crush(bx - 1, by, b_rotation) == true) move_bomb(LEFT);
					break;
				case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
					if (check_crush(bx, by + 1, b_rotation) == true) move_bomb(DOWN);
					break;
				case UP: //���� ����Ű �������� 
					if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_bomb(UP);
					//ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
					else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_bomb(100);
				}                    //�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
			}
			else { //�Ϲ� ���� ���
				switch (key) {
				case LEFT: //����Ű ��������  
					if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
					break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
				case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
					if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
					break;
				case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
					if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
					break;
				case UP: //���� ����Ű �������� 
					if ((level > 30 && level < 41) || (level > 70 && level < 81) || (level > 90 && level < 101)) {
						break;
					}
					else {
						if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
						//ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
						else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
						//�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
						break;
					}
				}
			}
		}
		else { //����Ű�� �ƴѰ�� 
			switch (key) {
			case SPACE: //�����̽�Ű �������� 
				space_key_on = 1; //�����̽�Ű flag�� ��� 
				while (crush_on == 0) { //�ٴڿ� ���������� �̵���Ŵ 
					drop_block();
					if (b_type != 7) {
						score += level; // hard drop ���ʽ�
						gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); //���� ǥ��
					}
				}
				break;
			case P: //P(�빮��) �������� 
			case p: //p(�ҹ���) �������� 
				pause(); //�Ͻ����� 
				break;
			case ESC: //ESC�������� 
				system("cls"); //ȭ���� ����� 
				exit(0); //�������� 
			}
		}
	}
	while (_kbhit()) _getch(); //Ű���۸� ��� 
}

void drop_block(void) {
	int i, j;

	if (level > 50) {
		steal_block_start();
	}

	if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0; //���� ��������� crush flag �� 
	if (crush_on && check_crush(bx, by + 1, b_rotation) == false) { //���� ��������ʰ� crush flag�� ���������� 
		if (b_type == 7) {
			for (i = 0; i < MAIN_Y; i++) { //��ź �� ���� 
				for (j = 0; j < MAIN_X; j++) {
					if (main_org[i][j] == BOMB) {
						for (int k = 0; k < 3; k++) {
							for (int m = 0; m < 3; m++) {
								if (main_org[i - 1 + k][j - 1 + m] == INACTIVE_BLOCK || main_org[i - 1 + k][j - 1 + m] == BOMB) {
									main_org[i - 1 + k][j - 1 + m] = EMPTY;
								}
							}
						}
					}
				}
			}
		}
		else {
			for (i = 0; i < MAIN_Y; i++) { //���� �������� ���� ���� 
				for (j = 0; j < MAIN_X; j++) {
					if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK;
				}
			}
		}
		crush_on = 0; //flag�� ��

		if ((level > 10 && level < 21) || (level > 50 && level < 61)) {
			check_blank(); //��ĭ �� üũ�� ��
		}
		else if ((level > 20 && level < 31) || (level > 60 && level < 71)) {
			check_line_obstacle(); //���� �� üũ�� ��
		}
		else {
			check_line(); //����üũ�� ��
		}

		new_block_on = 1; //���ο� ������ flag�� ��    
		return; //�Լ� ���� 
	}
	if (b_type == 7) {
		if (check_crush(bx, by + 1, b_rotation) == true) move_bomb(DOWN); //���� ��������� ������ ��ĭ �̵� 
	}
	else {
		if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); //���� ��������� ������ ��ĭ �̵� 
	}
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++; //������ �̵��� �ȵǸ�  crush flag�� ��
}

int check_crush(int bx, int by, int b_rotation) { //������ ��ǥ�� ȸ�������� �浹�� �ִ��� �˻� 
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) { //������ ��ġ�� �����ǰ� ������� ���ؼ� ��ġ�� false�� ���� 
			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j] > 0) return false;
		}
	}
	return true; //�ϳ��� �Ȱ�ġ�� true���� 
};

void move_block(int dir) { //����� �̵���Ŵ 
	int i, j;

	switch (dir) {
	case LEFT: //���ʹ��� 
		for (i = 0; i < 4; i++) { //������ǥ�� ���� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) { //�������� ��ĭ���� active block�� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--; //��ǥ�� �̵� 
		break;

	case RIGHT:    //������ ����. ���ʹ����̶� ���� ������ ���� 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;

	case DOWN:    //�Ʒ��� ����. ���ʹ����̶� ���� ������ ����
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;

	case UP: //Ű���� ���� �������� ȸ����Ŵ. 
		for (i = 0; i < 4; i++) { //������ǥ�� ���� ����  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4; //ȸ������ 1������Ŵ(3���� 4�� �Ǵ� ���� 0���� �ǵ���) 
		for (i = 0; i < 4; i++) { //ȸ���� ����� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 100: //����� �ٴ�, Ȥ�� �ٸ� ��ϰ� ���� ���¿��� ��ĭ���� �÷� ȸ���� ������ ��� 
			  //�̸� ���۽�Ű�� Ư������ 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;
	}
}

void move_bomb(int dir) { //����� �̵���Ŵ 
	int i, j;

	switch (dir) {
	case LEFT: //���ʹ��� 
		for (i = 0; i < 4; i++) { //������ǥ�� ���� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) { //�������� ��ĭ���� active block�� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = BOMB;
			}
		}
		bx--; //��ǥ�� �̵� 
		break;

	case RIGHT:    //������ ����. ���ʹ����̶� ���� ������ ���� 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = BOMB;
			}
		}
		bx++;
		break;

	case DOWN:    //�Ʒ��� ����. ���ʹ����̶� ���� ������ ����
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = BOMB;
			}
		}
		by++;
		break;

	case UP: //Ű���� ���� �������� ȸ����Ŵ. 
		for (i = 0; i < 4; i++) { //������ǥ�� ���� ����  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4; //ȸ������ 1������Ŵ(3���� 4�� �Ǵ� ���� 0���� �ǵ���) 
		for (i = 0; i < 4; i++) { //ȸ���� ����� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = BOMB;
			}
		}
		break;

	case 100: //����� �ٴ�, Ȥ�� �ٸ� ��ϰ� ���� ���¿��� ��ĭ���� �÷� ȸ���� ������ ��� 
			  //�̸� ���۽�Ű�� Ư������ 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = BOMB;
			}
		}
		by--;
		break;
	}
}

void check_line(void) {
	int i, j, k, l;

	int    block_amount; //������ ��ϰ����� �����ϴ� ���� 
	int combo = 0; //�޺����� �����ϴ� ���� ������ �ʱ�ȭ 

	for (i = MAIN_Y - 2; i > 3;) { //i=MAIN_Y-2 : ���ʺ��� ��ĭ����,  i>3 : õ��(3)�Ʒ����� �˻� 
		block_amount = 0; //��ϰ��� ���� ���� �ʱ�ȭ 
		for (j = 1; j < MAIN_X - 1; j++) { //���� �������� ��ϰ��縦 �� 
			if (main_org[i][j] > 0) block_amount++;
		}
		if (block_amount == MAIN_X - 2) { //����� ���� �� ��� 
			score += 100 * level; //�����߰� 
			cnt++; //���� �� ���� ī��Ʈ ���� 
			combo++; //�޺��� ����  

			for (k = i; k > 1; k--) { //������ ��ĭ�� ��� ����(������ õ���� �ƴ� ��쿡��) 
				for (l = 1; l < MAIN_X - 1; l++) {
					if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
					if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;
					//������ õ���� ��쿡�� õ���� ��ĭ ������ �ȵǴϱ� ��ĭ�� ���� 
				}
			}
		}
		else i--;
	}
	if (combo) { //�� ������ �ִ� ��� ������ ���� ��ǥ�� ���� ǥ����  
		if (combo > 1) { //2�޺��̻��� ��� ��� ���ʽ��� �޼����� �����ǿ� ����ٰ� ���� 
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
			Sleep(500);
			score += (combo * level * 100);
			reset_main_cpy(); //�ؽ�Ʈ�� ����� ���� main_cpy�� �ʱ�ȭ.

//(main_cpy�� main_org�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ������ ��ü�� ���� �׸��� ��) 
		}
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
	}
}

void check_line_obstacle(void) {
	int i, j, k, l;

	int block_amount; //������ ��ϰ����� �����ϴ� ���� 
	int combo = 0; //�޺����� �����ϴ� ���� ������ �ʱ�ȭ 

	for (i = MAIN_Y - 2; i > 3;) { //i=MAIN_Y-2 : ���ʺ��� ��ĭ����,  i>3 : õ��(3)�Ʒ����� �˻� 
		block_amount = 0; //��ϰ��� ���� ���� �ʱ�ȭ 
		for (j = 1; j < MAIN_X - 1; j++) { //���� �������� ��ϰ��縦 �� 
			if (main_org[i][j] > 0 && main_org[i][j] != 3) block_amount++;
		}
		if (block_amount == MAIN_X - 2) { //����� ���� �� ���
			score += 100 * level; //�����߰� 
			cnt++; //���� �� ���� ī��Ʈ ���� 
			combo++; //�޺��� ����  


			for (l = 1; l < MAIN_X - 1; l++) {
				for (k = i; k > 1; k--) {
					if (main_org[k - 1][l] == CEILLING) {
						main_org[k][l] = EMPTY;
					}
					else if (main_org[k - 1][l] != OBSTACLE_BLOCK) {
						main_org[k][l] = main_org[k - 1][l];
						main_org[k - 1][l] = EMPTY;
					}
					else {
						while (main_org[k - 1][l] == OBSTACLE_BLOCK || main_org[k - 1][l] == INACTIVE_BLOCK) {
							k--;
						}
					}
				}
			}
		}
		else i--;
	}
	if (combo) { //�� ������ �ִ� ��� ������ ���� ��ǥ�� ���� ǥ����  
		if (combo > 1) { //2�޺��̻��� ��� ��� ���ʽ��� �޼����� �����ǿ� ����ٰ� ���� 
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
			Sleep(500);
			score += (combo * level * 100);
			reset_main_cpy(); //�ؽ�Ʈ�� ����� ���� main_cpy�� �ʱ�ȭ.

//(main_cpy�� main_org�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ������ ��ü�� ���� �׸��� ��) 
		}
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
	}
}

void check_blank(void) {
	int i = 0, j = 0;

	count_blank = num_blank;

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			if (main_org[i][j] == BLANK) {
				count_blank--;
			}
		}
	}

	//printf("%d", count_blank); //�̻��� �� ��� ���� ���ڽ��� //�� count�� �� �Ǵ� ���� �̰� ���� �ƾ�

	if (count_blank == num_blank) check_line();

}

void check_level_up_tens(void) {
	int i, j;

	for (i = 0; i < 4; i++) {
		gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
		printf("             ");
		switch (level / 10) {
		case 1:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("                  ");
			break;
		case 2:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 6);
			printf("                      ");
			break;
		case 3:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("                ");
			break;
		case 4:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("                 ");
			break;
		case 5:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("                  ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf(" ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("                       ");
			break;
		case 6:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 6);
			printf("                       ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf(" ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("                       ");
			break;
		case 7:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("                ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf(" ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("                       ");
			break;
		case 8:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("                 ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf(" ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("                       ");
			break;
		case 9:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("                  ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf(" ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 8);
			printf("                 ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 9);
			printf(" ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 10);
			printf("                       ");
			break;
		}
		Sleep(400);

		gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
		printf("��LEVEL UP!��");

		switch (level / 10) {
		case 1:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("��FILL BLANKS��!��");
			break;
		case 2:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 6);
			printf("��OBSTACLE BLOCKS��!��");
			break;
		case 3:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("��NO ROTATION!��");
			break;
		case 4:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("��CEILING DOWN!��");
			break;
		case 5:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("��FILL BLANKS��!��");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf("+");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("��BLOCK DISAPPEARANCE��");
			break;
		case 6:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 6);
			printf("��OBSTACLE BLOCKS��!��");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf("+");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("��BLOCK DISAPPEARANCE��");
			break;
		case 7:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("��NO ROTATION!��");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf("+");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("��BLOCK DISAPPEARANCE��");
			break;
		case 8:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("��CEILING DOWN!��");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf("+");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("��BLOCK DISAPPEARANCE��");
			break;
		case 9:
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 6);
			printf("��FILL BLANKS��!��");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 7);
			printf("+");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 4, MAIN_Y_ADJ + 8);
			printf("��NO ROTATION!��");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2), MAIN_Y_ADJ + 9);
			printf("+");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 10);
			printf("��BLOCK DISAPPEARANCE��");
			break;
		}
		Sleep(400);
	}
}

void check_level_up_units(void) {
	int i, j;

	for (i = 0; i < 4; i++) {
		gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
		printf("             ");
		gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
		printf("             ");
		if (level % 5 == 0) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("                      ");
		}
		Sleep(200);

		gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
		printf("��LEVEL UP!��");
		gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
		printf("��SPEED UP!��");
		if (level % 5 == 0) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 5, MAIN_Y_ADJ + 8);
			printf("��OPPOSITE KEY TIME!��");
		}
		Sleep(200);
	}
}

void check_level_up(void) {
	if (cnt >= 10) { //�������� 10�پ� ���־���. 10���̻� ���� ��� 
		draw_main();

		level += 1; //������ 1 �ø� 
		cnt = 0; //���� �ټ� �ʱ�ȭ 

		if (level % 10 == 1) {
			check_level_up_tens();
		}
		else {
			check_level_up_units();
		}
		reset_main_cpy(); //�ؽ�Ʈ�� ����� ���� main_cpy�� �ʱ�ȭ.
//(main_cpy�� main_org�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ������ ��ü�� ���� �׸��� ��) 

		system("cls"); //ȭ������ 
		reset_main(); //�ؽ�Ʈ�� ����� ���� main_cpy�� �ʱ�ȭ.
		draw_map(); // ����ȭ���� �׸�
		draw_main(); // �������� �׸� 


		switch ((level - 1) / 10) { //��������� ����
		case 1:
			make_blank();
			break;
		case 2:
			make_obstacle();
			break;
		case 3:

			break;
		case 4:

			break;
		case 5:
			make_blank();
			break;
		case 6:
			make_obstacle();
			break;
		case 7:

			break;
		case 8:

			break;
		case 9:
		case 10:

			break;
		}


		switch (level % 10) { //�������� �ӵ��� ��������. 
		case 2:
			speed = 80;
			break;
		case 3:
			speed = 60;
			break;
		case 4:
			speed = 55;
			break;
		case 5:
			speed = 50;
			break;
		case 6:
			speed = 40;
			break;
		case 7:
			speed = 30;
			break;
		case 8:
			speed = 25;
			break;
		case 9:
			speed = 20;
			break;
		case 0:
			speed = 15;
			break;
		}

		gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //����ǥ�� 
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // ������ǥ ǥ�� 

	}
}

void check_game_over(void) {
	int i;

	int x = 5;
	int y = 5;

	for (i = 1; i < MAIN_X - 1; i++) {
		if (main_org[3][i] > 0) { //õ��(������ ����° ��)�� inactive�� �����Ǹ� ���� ���� 
			gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�"); //���ӿ��� �޼��� 
			gotoxy(x, y + 1); printf("��                              ��");
			gotoxy(x, y + 2); printf("��  +-----------------------+   ��");
			gotoxy(x, y + 3); printf("��  |   G A M E  O V E R..  |   ��");
			gotoxy(x, y + 4); printf("��  +-----------------------+   ��");
			gotoxy(x, y + 5); printf("��   YOUR SCORE: %6d         ��", score);
			gotoxy(x, y + 6); printf("��                              ��");
			gotoxy(x, y + 7); printf("��  Press any key to restart..  ��");
			gotoxy(x, y + 8); printf("��                              ��");
			gotoxy(x, y + 9); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
			last_score = score; //���������� �ű� 

			if (score > best_score) { //�ְ��� ���Ž� 
				FILE* file = fopen("score.dat", "wt"); //score.dat�� ���� ����                

				gotoxy(x, y + 6); printf("��  �ڡڡ� BEST SCORE! �ڡڡ�   ��  ");

				if (file == 0) { //���� �����޼���  
					gotoxy(0, 0);
					printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"SCORE.DAT\"");
				}
				else {
					fprintf(file, "%d", score);
					fclose(file);
				}
			}
			Sleep(1000);
			while (_kbhit()) _getch();
			key = _getch();
			reset();
		}
	}
}

void pause(void) { //���� �Ͻ����� �Լ� 
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++) { //���� �Ͻ����� �޼��� 
		gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		gotoxy(x, y + 1); printf("��                              ��");
		gotoxy(x, y + 2); printf("��  +-----------------------+   ��");
		gotoxy(x, y + 3); printf("��  |       P A U S E       |   ��");
		gotoxy(x, y + 4); printf("��  +-----------------------+   ��");
		gotoxy(x, y + 5); printf("��  Press any key to resume..   ��");
		gotoxy(x, y + 6); printf("��                              ��");
		gotoxy(x, y + 7); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
	}
	_getch(); //Ű�Է½ñ��� ��� 

	system("cls"); //ȭ�� ����� ���� �׸� 
	reset_main_cpy();
	draw_main(); // �������� �׸� 
	if (level > 50 && level < 61) {
		draw_blank(); //��ĭ �ٽ� �׸�
	}
	draw_map();

	for (i = 1; i < 3; i++) { // ������� �׸� 
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("��");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void clear_game(void) {
	system("cls");

	int x = 5; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ 
	int y = 3; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ 
	int cnt; //Ÿ��Ʋ �������� ���� ����  

	gotoxy(x, y + 0); printf("�����  �����  ��      ��  �����"); Sleep(100);
	gotoxy(x, y + 1); printf("��    ��  ��    ��  ���  ���  ��      "); Sleep(100);
	gotoxy(x, y + 2); printf("��        ��    ��  �� ��� ��  ����  "); Sleep(100);
	gotoxy(x, y + 3); printf("��  ���  �����  ��  ��  ��  ��      "); Sleep(100);
	gotoxy(x, y + 4); printf("��    ��  ��    ��  ��      ��  ��    ��"); Sleep(100);
	gotoxy(x, y + 5); printf("�����  ��    ��  ��      ��  �����"); Sleep(100);
	gotoxy(x + 10, y + 7);  printf("�����  ��        �����  �����  �����");
	gotoxy(x + 10, y + 8);  printf("��        ��        ��        ��    ��  ��    ��");
	gotoxy(x + 10, y + 9);  printf("��        ��        ����    �����  �����");
	gotoxy(x + 10, y + 10); printf("��        ��        ��        ��    ��  ��  ��  ");
	gotoxy(x + 10, y + 11); printf("��        ��        ��    ��  ��    ��  ��   �� ");
	gotoxy(x + 10, y + 12); printf("�����  �����  �����  ��    ��  ��    ��");

	while (_kbhit()) _getch();
	key = _getch();

}//��!
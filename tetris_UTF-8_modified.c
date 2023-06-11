#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#define RESET_COLOR "\x1B[0m"
#define RED_COLOR "\x1B[31m"
#define GREEN_COLOR "\x1B[32m"
#define YELLOW_COLOR "\x1B[33m"
#define BLUE_COLOR "\x1B[34m"
#define MAGENTA_COLOR "\x1B[35m"
#define CYAN_COLOR "\x1B[36m"

#define MAX_OPTIONS 3
#define MAX_SCORES 3
#define MAX_NAME_LENGTH 50

#define LEFT 75 //좌로 이동    //키보드값들 
#define RIGHT 77 //우로 이동 
#define UP 72 //회전 
#define DOWN 80 //soft drop
#define SPACE 32 //hard drop
#define p 112 //일시정지 
#define P 80 //일시정지
#define ESC 27 //게임종료


#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // 게임판배열에 저장될 블록의 상태들 
#define CEILLING -1     // 블록이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블록값 

#define MAIN_X 11 //게임판 가로크기 
#define MAIN_Y 23 //게임판 세로크기 
#define MAIN_X_ADJ 3 //게임판 위치조정 
#define MAIN_Y_ADJ 1 //게임판 위치조정 

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정 

//int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장 
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장


int blocks[7][4][4][4] = {
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
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}

}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 

int b_type; //블록 종류를 저장 
int b_rotation; //블록 회전값 저장 
int b_type_next; //다음 블록값 저장 

int main_org[MAIN_Y][MAIN_X]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨 
int main_cpy[MAIN_Y][MAIN_X]; //즉 maincpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음 
//main의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림) 
//main_cpy와 배열을 비교해서 값이 달라진 곳만 모니터에 고침 
int bx, by; //이동중인 블록의 게임판상의 x,y좌표를 저장 

int key; //키보드로 입력받은 키값을 저장 

int speed; //게임진행속도 
int level; //현재 level 
int level_goal; //다음레벨로 넘어가기 위한 목표점수 
int cnt; //현재 레벨에서 제거한 줄 수를 저장 
int point; //현재 점수 
int score;
int last_score = 0; //마지막게임점수 
int best_score = 0; //최고게임점수
int worst_score = 0;
int flag = 0;


int new_block_on = 0; //새로운 블럭이 필요함을 알리는 flag 
int crush_on = 0; //현재 이동중인 블록이 충돌상태인지 알려주는 flag 
int level_up_on = 0; //다음레벨로 진행(현재 레벨목표가 완료되었음을) 알리는 flag 
int space_key_on = 0; //hard drop상태임을 알려주는 flag 
char playername[50];
int options;


int tmp = 0;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} ScoreEntry;

void title(); //遊戲開始畫面->GAME START/ INSTRUCTION/ RANKING
//void title2(); //遊戲模式選擇畫面->GENERAL/TIME mode
void title3(); //Instruction Intorduction
void title4(); //General Introduction
void title5(); //Ranking
void game(); //PLAYING
void exchange(int); //check exchange status
void reset(void); //遊戲版初始化
void reset_main(void); //메인 게임판(main_org[][]를 초기화)
void reset_main_cpy(void); //copy 게임판(main_cpy[][]를 초기화)
void draw_map(void); //게임 전체 인터페이스를 표시 
void draw_main(void); //게임판을 그림 
void new_block(void); //새로운 블록을 하나 만듦 
void check_key(void); //키보드로 키를 입력받음 
void drop_block(void); //블록을 아래로 떨어트림 
int check_crush(int bx, int by, int rotation); //bx, by위치에 rotation회전값을 같는 경우 충돌 판단 
void move_block(int dir); //dir방향으로 블록을 움직임 
void check_line(void); //줄이 가득찼는지를 판단하고 지움 
void check_level_up(); //레벨목표가 달성되었는지를 판단하고 levelup시킴 
void check_game_over(void); //게임오버인지 판단하고 게임오버를 진행 
void pause(void);
void choose_block(options);//to exchange  you want block
void change_speed();//to lower speed
void change_next_block(point);//to change next block
void exchange(point);

ScoreEntry Score_3[MAX_SCORES];

void gotoxy(int x, int y) { //gotoxy함수 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
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

int main() {

    srand((unsigned)time(NULL)); //生成隨機數表
    setcursortype(NOCURSOR); //大而消除
    title(); //主標題調用            

}

void insertEntry(ScoreEntry array[], int index, const char* name, int score) {
    for (int i = MAX_SCORES - 1; i > index; i--) {
        array[i] = array[i - 1];
    }
    snprintf(array[index].name, MAX_NAME_LENGTH, "%s", name);
    array[index].score = score;
}

void title() {
    flag = 0;
    int x = 15; //타이틀화면이 표시되는 x좌표 
    int y = 12; //타이틀화면이 표시되는 y좌표 

    char options[MAX_OPTIONS][20] = {
        "GAME START",
        "INSTRUCTION",
        "RANKING"
    };

    int selectedOption = 0;
    int keyPressed;

    while (1) {
        system("cls"); // 清除畫面

        gotoxy(x + 12, y - 11); printf("T E T R I S");
        gotoxy(x + 10, y - 8); printf("%s", "M A I N    M E N U");
        gotoxy(x, y - 5); printf("%s", "Choose the option and press ENTER to continue...");
        gotoxy(x, y + 0); printf(options[0]);
        gotoxy(x, y + 4); printf(options[1]);
        gotoxy(x, y + 8); printf(options[2]);
        // 顯示選單
        for (int i = 0; i < MAX_OPTIONS; i++) {
            if (i == selectedOption) {
                if (i == 0) {
                    gotoxy(x - 3, y + 0); printf(">>");
                }
                else if (i == 1) {
                    gotoxy(x - 3, y + 4); printf(">>");
                }
                else if (i == 2) {
                    gotoxy(x - 3, y + 8); printf(">>");
                }
            }
        }
        // 讀取按鍵
        keyPressed = _getch();

        if (keyPressed == 224) { // 224是方向鍵的前綴值
            keyPressed = _getch(); // 讀取方向鍵的具體值

            if (keyPressed == 72) { // 上方向鍵
                selectedOption = (selectedOption - 1 + MAX_OPTIONS) % MAX_OPTIONS;
            }
            else if (keyPressed == 80) { // 下方向鍵
                selectedOption = (selectedOption + 1) % MAX_OPTIONS;
            }
        }
        else if (keyPressed == 13) { // Enter鍵
            break; // 選擇完成，跳出迴圈
        }
    }

    if (selectedOption == 0) { //GAME START
        title4();
    }
    else if (selectedOption == 1) { //INSTRUCTION
        title3();
    }
    else if (selectedOption == 2) { //RANKING
        title5();
    }

}

void title3() {
    flag = 0;
    int x = 15; //타이틀화면이 표시되는 x좌표 
    int y = 12; //타이틀화면이 표시되는 y좌표           

    int selectedOption = 0;
    int keyPressed;

    while (1) {
        system("cls"); // 清除畫面

        gotoxy(x + 9, y - 10); printf("%s", "I N S T R U C T I O N");
        gotoxy(x - 12, y - 8); printf("%s", "← BACK");

        // 顯示選單                        
        //gotoxy(x+6, y); printf("-FIRST PLAYER-");
        gotoxy(x + 7, y - 2); printf("  ↑   : Shift -> rotate the block");
        gotoxy(x + 7, y - 1); printf("←  → : Left / Right -> moving the block");
        gotoxy(x + 7, y); printf("  ↓   : Soft Drop");
        gotoxy(x + 7, y + 1); printf(" SPACE : Hard Drop");

        gotoxy(x + 7, y + 2); printf("   P   : Pause");
        gotoxy(x + 7, y + 3); printf("  ESC  : Quit");
        gotoxy(x + 5, y + 4); printf(" BackSpace : Previous page");

        //gotoxy(x+20, y); printf("-SECOND PLAYER-");
        //gotoxy(x+18, y + 2); printf("  w    : Shift");
        //gotoxy(x+18, y + 3); printf("a  d   : Left / Right");
        //gotoxy(x + 18, y + 4); printf("  s    : Soft Drop");
        //gotoxy(x + 18, y + 5); printf(" t  : Hard Drop");

        //gotoxy(x + 9, y + 8); printf("   P    : Pause");
        //gotoxy(x + 9, y + 9); printf("  ESC   : Quit");
        //gotoxy(x + 7, y + 10); printf("  BackSpace : Previous page");
        // 讀取按鍵
        keyPressed = _getch();

        if (keyPressed == 8) {
            selectedOption = -1;
            break;
        }
    }

    if (selectedOption == -1) { //mainmenu
        title();
    }

}

void title4() { //General Introduction
    flag = 0;
    int x = 17; //타이틀화면이 표시되는 x좌표 
    int y = 6; //타이틀화면이 표시되는 y좌표           

    int selectedOption = 0;
    int keyPressed;

    while (1) {
        system("cls"); // 清除畫面

        gotoxy(x + 4, y - 4); printf("%s", "G E N E R A L    M O D E    R U L E");
        gotoxy(x - 14, y - 2); printf("%s", "← BACK");

        // 顯示選單                        
        gotoxy(x, y); printf("BONUS:");
        gotoxy(x + 2, y + 1); printf("HARD DROP");
        gotoxy(x + 2, y + 2); printf("COMBO");
        gotoxy(x + 2, y + 3); printf("CHAIN ELIMINATION");
        gotoxy(x, y + 5); printf("MINUS:");
        gotoxy(x + 2, y + 6); printf("Every step(up, left, right) cost 1 point.");
        gotoxy(x, y + 8); printf("EXCHANGE: press the key in ( ) to do related action");
        gotoxy(x + 2, y + 9); printf("  (c) Pass the current block: 100 point.");
        gotoxy(x + 2, y + 10); printf("(num) Change a specific block: 500 point.");
        gotoxy(x + 2, y + 11); printf("  (z) Bomb a whole line: 750 point.");
        gotoxy(x + 2, y + 12); printf("  (x) Lower the speed to last level: 1000 point.");
        gotoxy(x, y + 16); printf("Input Playername:");
        keyPressed = _getch();

        if (keyPressed == 8) {
            title();
            break;
        }
        else if (keyPressed == 13) { // Enter鍵  
            selectedOption = 1;
            break; // 選擇完成，跳出迴圈
        }
        scanf("%s", playername);
        gotoxy(x + 14, y + 14); printf("□□□□□□□□□□□□"); Sleep(100);
        gotoxy(x + 14, y + 15); printf("□                    □"); Sleep(100);
        gotoxy(x + 14, y + 16); printf("□   ENTER TO START   □"); Sleep(100);
        gotoxy(x + 14, y + 17); printf("□                    □"); Sleep(100);
        gotoxy(x + 14, y + 18); printf("□□□□□□□□□□□□"); Sleep(100);

        // 讀取按鍵
        keyPressed = _getch();

        if (keyPressed == 8) {
            title();
            break;
        }
        else if (keyPressed == 13) { // Enter鍵  
            game();
            break; // 選擇完成，跳出迴圈
        }
    }
}

void title5() {
    flag = 0;
    reset();
    int x = 15; //타이틀화면이 표시되는 x좌표 
    int y = 12; //타이틀화면이 표시되는 y좌표           

    int selectedOption = 0;
    int keyPressed;

    while (1) {
        system("cls"); // 清除畫面

        gotoxy(x + 11, y - 10); printf("%s", "R A N K I N G");
        gotoxy(x - 12, y - 8); printf("%s", "← BACK");

        // 顯示選單                        
        //gotoxy(x+6, y); printf("-FIRST PLAYER-");
        for (int i = 0; i < MAX_SCORES; i++) {
            gotoxy(x + 2, y - 2 + i * 2); printf("%d: Player- %s", i + 1, Score_3[i].name, Score_3[i].score);
            gotoxy(x + 17, y - 2 + i * 2); printf("score: %d", Score_3[i].score);
        }


        keyPressed = _getch();

        if (keyPressed == 8) {
            selectedOption = -1;
            break;
        }
    }
    if (selectedOption == -1) { //mainmenu
        title();
    }

}

void exchange(point) {
    if ((point >= 100) & (point <= 120)) {
        gotoxy(STATUS_X_ADJ, 19); printf(YELLOW_COLOR "●" RESET_COLOR "   (c) Shift Next");
    }
    else if (point > 120) {
        gotoxy(STATUS_X_ADJ, 19); printf(GREEN_COLOR "●" RESET_COLOR "   (c) Shift Next");
    }
    else {
        gotoxy(STATUS_X_ADJ, 19); printf(RED_COLOR "●" RESET_COLOR "   (c) Shift Next");
    }

    if ((point >= 500) & (point <= 520)) {
        gotoxy(STATUS_X_ADJ, 20); printf(YELLOW_COLOR "●" RESET_COLOR " (num) Change Block");
    }
    else if (point > 520) {
        gotoxy(STATUS_X_ADJ, 20); printf(GREEN_COLOR "●" RESET_COLOR " (num) Change Block");
    }
    else {
        gotoxy(STATUS_X_ADJ, 20); printf(RED_COLOR "●" RESET_COLOR " (num) Change Block");
    }

    if ((point >= 750) & (point <= 770)) {
        gotoxy(STATUS_X_ADJ, 21); printf(YELLOW_COLOR "●" RESET_COLOR "   (z) Bomb");
    }
    else if (point > 770) {
        gotoxy(STATUS_X_ADJ, 21); printf(GREEN_COLOR "●" RESET_COLOR "   (z) Bomb");
    }
    else {
        gotoxy(STATUS_X_ADJ, 21); printf(RED_COLOR "●" RESET_COLOR "   (z) Bomb");
    }

    if ((point >= 1000) & (point <= 1020)) {
        gotoxy(STATUS_X_ADJ, 22); printf(YELLOW_COLOR "●" RESET_COLOR "   (x) Lower Speed");
    }
    else if (point >= 1000) {
        gotoxy(STATUS_X_ADJ, 22); printf(GREEN_COLOR "●" RESET_COLOR "   (x) Lower Speed");
    }
    else {
        gotoxy(STATUS_X_ADJ, 22); printf(RED_COLOR "●" RESET_COLOR "   (x) Lower Speed");
    }
}


void game(void) {
    flag = 1;
    reset(); //遊戲盤復位      
    while (1) {

        int i;

        for (i = 0; i < 5; i++) { //塊隔開一格時可輸入5次密鑰
            check_key();//確認按鍵輸入
            draw_main(); //畫面上
            Sleep(speed);
            //遊戲速度調整
            if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
            //塊發生碰撞時，需要額外移動或旋轉的時間
            if (space_key_on == 1) { //執行hard drop時，無法追加移動或旋轉
                space_key_on = 0;
                break;
            }
        }



        drop_block(); // 降低一個區塊
        check_level_up(); // 檢查升級
        check_game_over(); //遊戲結束

        if (new_block_on == 1) new_block(); // 新塊flag存在時生成新塊
    }

}

void reset(void) {

    FILE* file = fopen("score.dat", "rt");

    if (file == NULL) {
        printf("無法開啟檔案。\n");
        return 1;
    }

    // 讀取每行資料
    for (int i = 0; i < MAX_SCORES; i++) {
        fscanf(file, "%[^,],%d\n", Score_3[i].name, &Score_3[i].score);
    }

    fclose(file);

    best_score = Score_3[0].score;
    worst_score = Score_3[2].score;

    if (flag == 1) {
        level = 1; //각종변수 초기화 
        score = 0;
        point = 100;
        level_goal = 1000;
        key = 0;
        crush_on = 0;
        cnt = 0;
        speed = 100;

        system("cls"); //화면지움 
        reset_main(); // main_org를 초기화 
        draw_map(); // 게임화면을 그림
        draw_main(); // 게임판을 그림 

        b_type_next = rand() % 7; //다음번에 나올 블록 종류를 랜덤하게 생성 
        new_block(); //새로운 블록을 하나 만듦  
    }

}

void reset_main(void) { //게임판을 초기화  
    int i, j;

    for (i = 0; i < MAIN_Y; i++) { // 게임판을 0으로 초기화  
        for (j = 0; j < MAIN_X; j++) {
            main_org[i][j] = 0;
            main_cpy[i][j] = 100;
        }
    }
    for (j = 1; j < MAIN_X; j++) { //y값이 3인 위치에 천장을 만듦 
        main_org[3][j] = CEILLING;
    }
    for (i = 1; i < MAIN_Y - 1; i++) { //좌우 벽을 만듦  
        main_org[i][0] = WALL;
        main_org[i][MAIN_X - 1] = WALL;
    }
    for (j = 0; j < MAIN_X; j++) { //바닥벽을 만듦 
        main_org[MAIN_Y - 1][j] = WALL;
    }
}

void reset_main_cpy(void) { //main_cpy를 초기화 
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {         //게임판에 게임에 사용되지 않는 숫자를 넣음 
        for (j = 0; j < MAIN_X; j++) {  //이는 main_org와 같은 숫자가 없게 하기 위함 
            main_cpy[i][j] = 100;
        }
    }
}

void draw_map(void) { //게임 상태 표시를 나타내는 함수  
    int y = 3;             // level, goal, score만 게임중에 값이 바뀔수 도 있음 그 y값을 따로 저장해둠 
    // 그래서 혹시 게임 상태 표시 위치가 바뀌어도 그 함수에서 안바꿔도 되게.. 
    gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y); printf(" LEVEL : %5d", level);
    //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y + 1); printf(" GOAL  : %5d", 10 - cnt);
    gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
    gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
    gotoxy(STATUS_X_ADJ, y + 9); printf(" YOUR POINT :");
    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 10); printf("        %6d", point);
    //gotoxy(STATUS_X_ADJ, y + 11); printf(" LAST SCORE :");
    //gotoxy(STATUS_X_ADJ, y + 12); printf("        %6d", last_score);
    //gotoxy(STATUS_X_ADJ, y + 13); printf(" BEST SCORE :");
    //gotoxy(STATUS_X_ADJ, y + 14); printf("        %6d", best_score);        

    gotoxy(STATUS_X_ADJ, y + 16); printf(RED_COLOR "●" RESET_COLOR "   (c) Shift Next");
    gotoxy(STATUS_X_ADJ, y + 17); printf(RED_COLOR "●" RESET_COLOR " (num) Change Block");
    gotoxy(STATUS_X_ADJ, y + 18); printf(RED_COLOR "●" RESET_COLOR "   (z) Bomb");
    gotoxy(STATUS_X_ADJ, y + 19); printf(RED_COLOR "●" RESET_COLOR "   (x) Lower Speed");
    //gotoxy(STATUS_X_ADJ + 10, y + 9); printf(RED_COLOR "●" RESET_COLOR " (z) Shift Next");        
    //gotoxy(STATUS_X_ADJ + 10, y + 11); printf(RED_COLOR "●" RESET_COLOR " (x) Change Block");    
    //gotoxy(STATUS_X_ADJ + 10, y + 13); printf(RED_COLOR "●" RESET_COLOR " (c) Bomb");    
    //gotoxy(STATUS_X_ADJ + 10, y + 15); printf(RED_COLOR "●" RESET_COLOR " (v) Lower Speed"); 

    for (int t = 0; t < 7; t++) {
        for (int i = 1; i < 3; i++) { //在遊戲狀態顯示時繪製下一個塊
            for (int j = 0; j < 4; j++) {
                if (blocks[t][0][i][j] == 1) {
                    gotoxy(STATUS_X_ADJ + 15, 4 * (t + 1) - 2); printf("%d.", t + 1);
                    gotoxy(STATUS_X_ADJ + j + 17, i + 4 * t); printf("\u25A1");
                }
            }
        }
    }
}

void draw_main(void) { //게임판 그리는 함수 
    int i, j;

    for (j = 1; j < MAIN_X - 1; j++) { //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌 
        if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
    }
    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            if (main_cpy[i][j] != main_org[i][j]) { //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.
                //이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                switch (main_org[i][j]) {
                case EMPTY: //빈칸모양 
                    printf("  ");
                    break;
                case CEILLING: //천장모양 
                    printf(". ");
                    break;
                case WALL: //벽모양 
                    printf(CYAN_COLOR"\u25A1" RESET_COLOR);
                    break;
                case INACTIVE_BLOCK: //굳은 블럭 모양  
                    printf("□");
                    break;
                case ACTIVE_BLOCK: //움직이고있는 블럭 모양  
                    printf("■");
                    break;
                }
            }
        }
    }
    for (i = 0; i < MAIN_Y; i++) { //게임판을 그린 후 main_cpy에 복사  
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = main_org[i][j];
        }
    }
}

void new_block(void) { //新建塊
    int i, j;

    bx = (MAIN_X / 2) - 1; //塊生成位置x做邊(遊戲盤的中間)
    by = 0;  //塊生成位置y座標(最上)
    b_type = b_type_next; //下一個塊值
    b_type_next = rand() % 7; //創建下一個塊
    b_rotation = 0;  //旋轉導入

    new_block_on = 0; //new_block flag를 끔  

    for (i = 0; i < 4; i++) { //遊戲版bx,by在位置生成塊
        for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
        }
    }
    for (i = 1; i < 3; i++) { //在遊戲狀態顯示時繪製下一個塊
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("■");
            }
            else {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("  ");
            }
        }
    }
}

void choose_block(options) {
    int i, j;
    int change_type;

    if (options >= 49 && options <= 55) {
        int i, j;
        for (i = 0; i < 4; i++) { //遊戲版bx,by在位置生成塊
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1)

                    main_org[by + i][bx + j] = EMPTY;
            }
        }

        bx = (MAIN_X / 2) - 1; //塊生成位置x做邊(遊戲盤的中間)
        by = 0;  //塊生成位置y座標(最上)
        change_type = options - 49;
        b_type = change_type;
        //創建下一個塊
        b_rotation = 0;  //旋轉導入

        new_block_on = 0; //new_block flag를 끔 

        for (i = 0; i < 4; i++) { //遊戲版bx,by在位置生成塊
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1)

                    main_org[by + i][bx + j] = ACTIVE_BLOCK;
            }
        }

    }

    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", point);
}

void change_next_block(point) {

    if (point >= 100) {
        for (int i = 0; i < 4; i++) { //遊戲版bx,by在位置生成塊
            for (int j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1)

                    main_org[by + i][bx + j] = EMPTY;
            }
        }
        new_block();


        point -= 100;
        gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", point);

    }


}

void change_speed() {



    if (point >= 1000 && level >= 2) {

        tmp++;
        int speed_level = level - tmp;
        switch (speed_level) {
        case 1:
            speed = 100;
            break;//레벨별로 속도를 조절해줌. 
        case 2:
            speed = 50;
            break;
        case 3:
            speed = 25;
            break;
        case 4:
            speed = 10;
            break;
        case 5:
            speed = 5;
            break;
        case 6:
            speed = 4;
            break;
        case 7:
            speed = 3;
            break;
        case 8:
            speed = 2;
            break;
        case 9:
            speed = 1;
            break;
        case 10:
            speed = 0;
            break;
        }


        point -= 1000;



    }
    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", point);

}

void check_key(void) {
    key = 0; //키값 초기화  


    if (_kbhit()) { //키입력이 있는 경우  
        key = _getch(); //키값을 받음
        if (key == 224) { //방향키인경우 
            do { key = _getch(); } while (key == 224);//방향키지시값을 버림 
            switch (key) {
            case LEFT: //왼쪽키 눌렀을때  
                if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
                break;                            //왼쪽으로 갈 수 있는지 체크 후 가능하면 이동
            case RIGHT: //오른쪽 방향키 눌렀을때- 위와 동일하게 처리됨 
                if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
                break;
            case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨 
                if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
                break;
            case UP: //위쪽 방향키 눌렀을때 
                if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
                //회전할 수 있는지 체크 후 가능하면 회전
                else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
            }                    //바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능하면 그렇게 함(특수동작)
        }
        else { //방향키가 아닌경우 
            switch (key) {
            case 99://use c to change next block
                change_next_block(point);
                if (point >= 100) {
                    point -= 100;
                    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", point);
                    exchange(point);
                }
                break;


            case 120:  //use x to lower speed


                change_speed();
                exchange(point);
                break;


            case 49:
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:  //use 1-7 to change block
                options = key;
                if (point >= 500) {
                    point -= 500;
                    choose_block(options);
                    exchange(point);
                }



                break;

            case SPACE: //스페이스키 눌렀을때 
                space_key_on = 1; //스페이스키 flag를 띄움 
                while (crush_on == 0) { //바닥에 닿을때까지 이동시킴 
                    drop_block();
                    point += level; // hard drop 보너스
                    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", point); //점수 표시                     
                    exchange(point);
                }
                break;
            case P: //P(대문자) 눌렀을때 
            case p: //p(소문자) 눌렀을때 
                pause(); //일시정지 
                break;
            case ESC: //ESC눌렀을때 
                system("cls"); //화면을 지우고 
                exit(0); //게임종료 
            }
        }
    }
    while (_kbhit()) _getch(); //키버퍼를 비움 
}

void drop_block(void) {
    int i, j;

    if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0; //밑이 비어있으면 crush flag 끔 
    if (crush_on && check_crush(bx, by + 1, b_rotation) == false) { //밑이 비어있지않고 crush flag가 켜저있으면 
        for (i = 0; i < MAIN_Y; i++) { //현재 조작중인 블럭을 굳힘 
            for (j = 0; j < MAIN_X; j++) {
                if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK;
            }
        }
        crush_on = 0; //flag를 끔 
        check_line(); //라인체크를 함 
        new_block_on = 1; //새로운 블럭생성 flag를 켬    
        return; //함수 종료 
    }
    if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); //如果下面是空的，向下移動一格
    if (check_crush(bx, by + 1, b_rotation) == false) crush_on++; //無法向下移動時打開crush flag
}

int check_crush(int bx, int by, int b_rotation) { //檢查指定座標與旋轉值是否存在衝突
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) { //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
            if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j] > 0) return false;
        }
    }
    return true; //하나도 안겹치면 true리턴 
};

void move_block(int dir) { //블록을 이동시킴 
    int i, j;

    switch (dir) {
    case LEFT: //왼쪽방향 
        for (i = 0; i < 4; i++) { //删除当前坐标块 
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
            }
        }
        for (i = 0; i < 4; i++) { //왼쪽으로 한칸가서 active block을 찍음 
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
            }
        }
        bx--; //좌표값 이동 
        break;

    case RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작 
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

    case DOWN:    //아래쪽 방향. 왼쪽방향이랑 같은 원리로 동작
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

    case UP: //키보드 위쪽 눌렀을때 회전시킴. 
        for (i = 0; i < 4; i++) { //현재좌표의 블럭을 지움  
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
            }
        }
        b_rotation = (b_rotation + 1) % 4; //회전값을 1증가시킴(3에서 4가 되는 경우는 0으로 되돌림) 
        for (i = 0; i < 4; i++) { //회전된 블록을 찍음 
            for (j = 0; j < 4; j++) {
                if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
            }
        }
        break;

    case 100: //블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 
        //이를 동작시키는 특수동작 
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

void check_line(void) {
    int i, j, k, l;

    int    block_amount; //存储一行块数的变量
    int combo = 0; //콤보갯수 저장하는 변수 지정및 초기화 

    for (i = MAIN_Y - 2; i > 3;) { //i= main_y -2:从墙体的上格开始，i>3:天花板(3)下面检查
        block_amount = 0; //블록갯수 저장 변수 초기화 
        for (j = 1; j < MAIN_X - 1; j++) { //벽과 벽사이의 블록갯루를 셈 
            if (main_org[i][j] > 0) block_amount++;
        }
        if (block_amount == MAIN_X - 2) { //블록이 가득 찬 경우 
            if (level_up_on == 0) { //레벨업상태가 아닌 경우에(레벨업이 되면 자동 줄삭제가 있음) 
                point += 100 * level; //점수추가 
                cnt++; //지운 줄 갯수 카운트 증가 
                combo++; //콤보수 증가  
            }
            for (k = i; k > 1; k--) { //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 
                for (l = 1; l < MAIN_X - 1; l++) {
                    if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
                    if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;
                    //윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음 
                }
            }
        }
        else i--;
    }
    if (combo) { //줄 삭제가 있는 경우 점수와 레벨 목표를 새로 표시함  
        if (combo > 1) { //2콤보이상인 경우 경우 보너스및 메세지를 게임판에 띄웠다가 지움 
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
            Sleep(500);
            point += (combo * level * 100);
            reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.
            //(main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨) 
        }
        //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
        gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", point);
        exchange(point);
    }
}

void check_level_up() {
    int i, j;

    if (cnt >= 10) { //레벨별로 10줄씩 없애야함. 10줄이상 없앤 경우 
        draw_main();
        level_up_on = 1; //레벨업 flag를 띄움 
        level += 1; //레벨을 1 올림 
        cnt = 0; //지운 줄수 초기화   

        for (i = 0; i < 4; i++) {
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("             ");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("             ");
            Sleep(200);

            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("☆LEVEL UP!☆");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("☆SPEED UP!☆");
            Sleep(200);
        }
        reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.
        //(main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨) 

        for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) { //레벨업보상으로 각 레벨-1의 수만큼 아랫쪽 줄을 지워줌 
            for (j = 1; j < MAIN_X - 1; j++) {
                main_org[i][j] = INACTIVE_BLOCK; // 줄을 블록으로 모두 채우고 
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i); // 별을 찍어줌.. 이뻐보이게 
                printf("★");
                Sleep(20);
            }
        }
        Sleep(100); //별찍은거 보여주기 위해 delay 
        check_line(); //블록으로 모두 채운것 지우기
        //.check_line()함수 내부에서 level up flag가 켜져있는 경우 점수는 없음.   
        int  changed_level = level - tmp;
        switch (changed_level) {
        case 1:
            speed = 100;
            break;//레벨별로 속도를 조절해줌. 
        case 2:
            speed = 50;
            break;
        case 3:
            speed = 25;
            break;
        case 4:
            speed = 10;
            break;
        case 5:
            speed = 5;
            break;
        case 6:
            speed = 4;
            break;
        case 7:
            speed = 3;
            break;
        case 8:
            speed = 2;
            break;
        case 9:
            speed = 1;
            break;
        case 10:
            speed = 0;
            break;
        }

        level_up_on = 0; //레벨업 flag꺼줌

        gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //레벨표시 
        //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // 레벨목표 표시 

    }
}

void check_game_over(void) {
    int i;

    int x = 20;
    int y = 10;

    for (i = 1; i < MAIN_X - 2; i++) {
        if (main_org[3][i] > 0) { //천장(위에서 세번째 줄)에 inactive가 생성되면 게임 오버 
            score = level * 10000 + point;
            gotoxy(x, y + 0); printf(YELLOW_COLOR"#################################"RESET_COLOR); //게임오버 메세지 
            gotoxy(x, y + 1); printf(YELLOW_COLOR"##                             ##"RESET_COLOR);
            gotoxy(x, y + 2); printf(YELLOW_COLOR"##  +-----------------------+  ##"RESET_COLOR);
            gotoxy(x, y + 3); printf(YELLOW_COLOR"##  |   G A M E  O V E R..  |  ##"RESET_COLOR);
            gotoxy(x, y + 4); printf(YELLOW_COLOR"##  +-----------------------+  ##"RESET_COLOR);
            gotoxy(x, y + 5); printf(YELLOW_COLOR"##      YOUR SCORE: %6d     ##"RESET_COLOR, score);
            gotoxy(x, y + 6); printf(YELLOW_COLOR"##                             ##"RESET_COLOR);
            gotoxy(x, y + 7); printf(YELLOW_COLOR"##  Press any key to restart.. ##"RESET_COLOR);
            gotoxy(x, y + 8); printf(YELLOW_COLOR"##                             ##"RESET_COLOR);
            gotoxy(x, y + 9); printf(YELLOW_COLOR"#################################"RESET_COLOR);
            //last_score = score; //게임점수를 옮김             

            if (score > Score_3[2].score) {
                strcpy(Score_3[2].name, playername);
                Score_3[2].score = score;

                // 重新排序分數資料，由大到小
                for (int i = 0; i < MAX_SCORES - 1; i++) {
                    for (int j = 0; j < MAX_SCORES - i - 1; j++) {
                        if (Score_3[j].score < Score_3[j + 1].score) {
                            ScoreEntry temp = Score_3[j];
                            Score_3[j] = Score_3[j + 1];
                            Score_3[j + 1] = temp;
                        }
                    }
                }
            }

            FILE* file = fopen("score.dat", "wt");
            if (file == NULL) {
                printf("無法開啟檔案。\n");
                return 1;
            }

            for (int i = 0; i < MAX_SCORES; i++) {
                fprintf(file, "%s,%d\n", Score_3[i].name, Score_3[i].score);
            }

            fclose(file);

            Sleep(1000);
            while (_kbhit()) _getch();
            key = _getch();
            reset();
        }
    }
}

void pause(void) { //게임 일시정지 함수 
    int i, j;

    int x = 20;
    int y = 10;

    for (i = 1; i < MAIN_X - 2; i++) { //게임 일시정지 메세지 
        gotoxy(x, y + 0); printf(MAGENTA_COLOR"################################"RESET_COLOR);
        gotoxy(x, y + 1); printf(MAGENTA_COLOR"##                            ##"RESET_COLOR);
        gotoxy(x, y + 2); printf(MAGENTA_COLOR"## +-----------------------+  ##"RESET_COLOR);
        gotoxy(x, y + 3); printf(MAGENTA_COLOR"## |       P A U S E       |  ##"RESET_COLOR);
        gotoxy(x, y + 4); printf(MAGENTA_COLOR"## +-----------------------+  ##"RESET_COLOR);
        gotoxy(x, y + 5); printf(MAGENTA_COLOR"## Press any key to continue  ##"RESET_COLOR);
        gotoxy(x, y + 6); printf(MAGENTA_COLOR"##                            ##"RESET_COLOR);
        gotoxy(x, y + 7); printf(MAGENTA_COLOR"################################"RESET_COLOR);
    }
    _getch(); //키입력시까지 대기 

    system("cls"); //화면 지우고 새로 그림 
    reset_main_cpy();
    draw_main();
    draw_map();

    for (i = 1; i < 3; i++) { // 다음블록 그림 
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("■");
            }
            else {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("  ");
            }
        }
    }
}
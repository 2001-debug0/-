/*-----------------------------------------------------------------
2020.4.4:       ���ֲ�Ӱ�ǡ��Լ����Լ�����bug������ṹ���ں�̨map�ﴢ��켣����

2020.4.6��      ����45�Ƚ�б�׹��ܣ��Ż�ǽ������ײ�������˵�bug

2020.4.27:      �˵��㶨..Ĭ�ϵ�ͼ������,�Զ����ͼ��������

Ŀǰbug: 1.�˵�����δ֪����ʾ����
         2.����δ֪ԭ���м�����������ڵ����
------------------------------------------------------------------
*/
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#define PI 3.14
#define V 26// �׳��ٶ�  ����Լ���
const double g = 10;
//////////ȫ�ֱ���
int width = 40, length = 150;
char map[1000][1000];
char _map[1000][1000];
int i, j, old_x, old_y, flag, color, if_miss, step, max, menu_x, menu_y, goal = 0, life;
int left = 1, right = 21;
double x, y, Vx, Vy, s1, s2, X, Y;//Vx����Vy����
char wall = '#', air = ' ', brick = '&', food = '*';
char ch, ch1;
double C[8] = { 0, 10, 30,45,70, 90 };
int flag = 0, if_start = -1, food_num = 100;
double x1_t1;
double x2_t1;
double x3_t1;
double x4_t1;
double s1, s2;
double temp;
double t1, t2;

///////
struct Step //�켣�ṹ��
{
    int x;
    int y;
} s[1000];
char End[200][800] = {
    {"##   ##   ##            ## #   "},
    {" ##   #   ##### ######  ## ##  "},
    {"   ##### ###### ######  ##  #  "},
    {"   #######          ##  ###### "},
    {"##  ##  # #####     ########## "},
    {" ## ####  #####  ## ######     "},
    {"    ####    ##   ## ##  ##  #  "},
    {"    ## # ######   ###   ## ##  "},
    {"  # ## # ######    ##   ## ##  "},
    {"  # ## #   ##     ####   ###   "},
    {" ## ## #   ##    ##  #   ##   #"},
    {" #  ## #   ##   ##      #### ##"},
    {"## ### #  ###   #     ###  ####"},
    {"## ## ##  ##          ##    ###"},
    {"                               "},
    {"  ##     ##            ##      "},
    {"  ##  ######### ###############"},
    {" ##   ######### ###############"},
    {"##  ##   ##            ##      "},
    {"###### #######    ############ "},
    {"  ##   #######    ############ "},
    {" ##               ##   ##   ## "},
    {"###### #######    ############ "},
    {"###### #######       #####     "},
    {"       ##   ##      ## ####    "},
    {"   ### ##   ##     ##  ## ##   "},
    {"###### #######   ###   ##  ### "},
    {"###    #######   ##    ##   ## "},
    {"       ##   ##         ##      "},
    {"                               "},
    {"     �� �� �� �� �� �� ��      "}//�ڴ����� Created By J.Henry
};
void gotoxy(int x, int y) //���꺯��  ��windows.h��   ������ʱҪ������һ�Σ��̶���
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
void HideCursor() //�������
{
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 }; //��ߵ�0�����겻�ɼ�
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void angle_switch()
{
    printf("-----ѡ���ʼ�׳��Ƕ�-----\n");
    gotoxy(length / 2, width + 4);
    printf("1.        0               \n");
    gotoxy(length / 2, width + 5);
    printf("2.       10               \n");
    gotoxy(length / 2, width + 6);
    printf("3.       30               \n");
    gotoxy(length / 2, width + 7);
    printf("4.       45               \n");
    gotoxy(length / 2, width + 8);
    printf("5.       70(�Ƽ�)         \n");
    gotoxy(length / 2, width + 9);
    printf("6.       90               \n");
    gotoxy(length / 2, width + 10);
    printf("----��Q����,��p��ͣ-------\n");
}
void end()
{
    int i;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
    for (i = 0; i <= 30; i++)
    {
        gotoxy(60, 03 + i);
        printf("%s\n", End[i]);
        Sleep(100);
    }
    getchar();
    getchar();
    return;
}
void  menu()
{
    char temp;
    char menu[40][40] = {
    {"     <--------------->     "},
    {"     <  ��ש���     >     "},
    {"     <--------------->     "},
    {"                           "},
    {"     <1>��ʼ��Ϸ��   <1>     "},
    {"                             "},
    {"     <2>�༭��Ϸ��   <2>     "},
    {"                             "},
    {"     <3>�˳���Ϸ��   <3>     "},
    {"                             "},
    {"     <4>�����       <4>     "},
    };

    HideCursor();

    for (i = 0;i <= width;i++)
        for (j = 0;j <= length;j++)
        {
            if ((i == width / 4 && j >= 2 * length / 5 && j <= 3 * length / 5) || (i == 3 * width / 4 && j >= 2 * length / 5 && j <= 3 * length / 5) || (j == 2 * length / 5 && i > width / 4 && i < 3 * width / 4) || (j == 3 * length / 5 && i > width / 4 && i < 3 * width / 4))
            {
                gotoxy(j, i);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);
                printf("#");
            }
        }
    for (i = 14;i <= 26;i++)
        for (j = 61;j <= 89;j++)
        {
            gotoxy(j, i);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            map[i][j] = menu[i - 14][j - 61];

            printf("%c", map[i][j]);
        }
}
void init()
{
    int a, b;
    HideCursor();
    if (flag != 5)//edit_map & no edit go start game
    {
        gotoxy(0, 0);
        for (i = 0; i <= width; i++)
        { //���ɵ�ͼģ�岢���������map��
            for (j = 0; j <= length; j++)
            {
                if (i == 0 || i == width || j == 0 || j == length || i == 39 && j >= left && j <= right || (i == width / 3 || i == 2 * width / 3) && (j >= 63 && j <= 87))
                    map[i][j] = wall;

                else
                    map[i][j] = air;
            }
        }
        for (i = 0; i <= width; i++)
        { //����ש��brick
            for (j = 0; j <= length; j++)
            {

                if (((i >= 20 && i <= 22) && (j >= 1 && j <= 50)) || ((i >= 20 && i <= 22) && (j >= 100 && j <= 149)))
                    map[i][j] = brick;
                if ((i == 10 || i == 11) && ((j >= 1 && j <= 50) || (j >= 100 && j <= 149)))
                    map[i][j] = brick;
            }
        }


    }
    if (flag == 5) {
        for (i = 0; i <= width; i++)
        {
            for (j = 0; j <= length; j++)
            {
                map[i][j] = _map[i][j];
            }
        }

    }
    if (if_start == 1)
    {
        food_num = 100;
        while (food_num--)
        { //̰��������ʳ��
            a = rand() % width;
            b = rand() % length;
            if ((a >= width / 3 && a <= width * 2 / 3) && (b >= length * 2 / 5 && b <= length * 3 / 5) || a == 0 || b == 0 || a == width || b == length || map[a][b] == brick || map[a][b] == wall)
                continue;//���ﲻ��ʳ��
            else
                map[a][b] = food;
        }
    }
    gotoxy(0, 0);
    //��ӡ��ͼ

    for (i = 0; i <= width; i++)
    {
        for (j = 0; j <= length; j++)
        {
            gotoxy(j, i);
            if (map[i][j] == wall && i != width)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);//����wall's fore and background��ɫ
                printf("%c", wall);
            }
            if (map[i][j] == air)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);//air
                printf("%c", air);
            }
            if (map[i][j] == brick)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 80);
                printf("%c", brick);
            }
            if (i == width)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);
                printf("%c", '!');
            }
            if (map[i][j] == food)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
                printf("%c", food);
            }

        }
        printf("\n");//��������ֹ���룡����
    }

}
///////////������
void auto_change()
{

    //������ɫ
    switch (color)
    {
    case 0:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);
        break; //��ɫ
    case 1:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 70);
        break; //��ɫ
    case 2:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 40);
        break; //��ɫ
    case 3:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 30);
        break; //����ɫ
    case 4:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
        break; //��ɫ
    }
    HideCursor();
    gotoxy(s[step].y, s[step].x);
    printf(" ");
    map[s[step].x][s[step].y]++;//map��ʱ����̨���飬�ڵ�������룬��ֹ�Լ����Լ�
    step++;

    /*-------------------------------------------------------
    ���Ĺ�ʽ��   ����X = X0 + Vx*t + 1/2*g*  mnVX*t*t;
                ����Y = Y0 + Vy*t;

    ����˼�룺  ���ѡ��ʱ����Ϊ�ƶ��ο�����ÿ�����ֵ����̫Զ
                ���ﷴ������������Ϊǰ�ᣬ����ʵ�֣�

                �ֱ���� X ����һ����λ���� Ӧ�����ӵ� Y �ľ��� s1
                        Y ����һ����λ���� Ӧ�����ӵ� X �ľ��� s2
                        ����ѡ����̵���Ϊ��һ���ƶ��Ļ�׼��

                X Y �����Ӷ���1

                X,Y�����ͣ����������λ���ۼӵ��������ϣ���gotoxy()����ֻ����int ����ÿ����old_x,old_y���н�X,Y

                ��Ϊʵ���������������ģ�ÿһ��ϸ΢��λ����������������

                ���s1 �� s2 ������ֻ����0��1�Լ���������2������

                s1=0:�½��Ͽ�         s2=0:�����߶���     ����Ϊ0���м�

     ------------------------------------------------------------*/
     //X Y��double�������Ӿ�׼�ȣ����������Ҫǿ��ת��

     //��X����һ����λ����ʱ
     //5*t*t + Vx t - 1= 0;��һԪ���η��̣������ж�  ���Ӻ������Ȼ��1    ���ܻ���4�����:�����⣬�����������µ�+/-1��
        ////////////X����һ����λ

    x1_t1 = (-Vx + sqrt(Vx * Vx + 20)) / 10;//�½�
    x2_t1 = (-Vx - sqrt(Vx * Vx + 20)) / 10;//<0
    x3_t1 = (-Vx + sqrt(Vx * Vx - 20)) / 10;//����
    x4_t1 = (-Vx - sqrt(Vx * Vx - 20)) / 10;//>0
    if (Vx >= 0) {
        t1 = x1_t1;
        s1 = t1 * Vy;
    }
    else {
        if (x3_t1 > 0 && x4_t1 > 0)
            temp = x3_t1 < x4_t1 ? x3_t1 : x4_t1;
        else
            temp = 10;
        s1 = temp * Vy;
    }


    //��Y����һ����λ����ʱ
    t2 = 1.0 / fabs(Vy);
    s2 = Vx * t2 + 0.5 * g * t2 * t2;
    //�ж�����·��
    if ((int)s1 == 0)
        flag = 1;
    else if ((int)s2 == 0)
        flag = 2;
    else
        flag = s1 < s2 ? 1 : 2;//1,2����
    ////ѡ������·��
    if (Vx >= 0) {
        if (flag == 1)
        {
            X = X + Vx * t1 + 5 * t1 * t1;
            Y = Y + s1;
            Vx = Vx + g * t1;
        }
        if (flag == 2)
        {
            Y = Y + Vy * t2;
            X = X + s2;
            Vx = Vx + g * t2;
        }
    }
    else
    {
        if (flag == 1)
        {
            X = X + Vx * temp + 5 * temp * temp;
            Y = Y + s1;
            Vx = Vx + g * temp;
        }
        if (flag == 2)
        {
            Y = Y + Vy * t2;
            X = X + s2;
            Vx = Vx + g * t2;
        }
    }

    //////////////
    //���и��õ��㷨���������������//
   ///////////////
    old_x = (int)X;
    old_y = (int)Y;

    //������ж���ײ   ����ǽ�������ײ
    if (map[old_x + 1][old_y] == wall && Vx >= 0 && map[old_x][old_y + 1] != wall && map[old_x][old_y - 1] != wall)
    {
        if (old_x != 39)
        {
            color = (color + 1) % 5;
            Vx = -Vx;
        }
        else
        {
            color = (color + 1) % 5;
            Vx = -Vx;
            life--;
        }

    }

    else if (map[old_x - 1][old_y] == wall && Vx <= 0 && map[old_x][old_y + 1] != wall && map[old_x][old_y - 1] != wall)
    {
        color = (color + 1) % 5;
        Vx = -Vx;
    }
    else if (map[old_x][old_y - 1] == wall && Vy <= 0 && map[old_x - 1][old_y] != wall && map[old_x + 1][old_y] != wall)
    {
        color = (color + 1) % 5;
        Vy = -Vy;
    }
    else if (map[old_x][old_y + 1] == wall && Vy >= 0 && map[old_x - 1][old_y] != wall && map[old_x + 1][old_y] != wall)
    {
        color = (color + 1) % 5;
        Vy = -Vy;
    }
    else if (map[old_x + 1][old_y + 1] == wall && Vx > 0 && Vy > 0 || map[old_x + 1][old_y - 1] == wall && Vx > 0 && Vy < 0 || map[old_x - 1][old_y + 1] == wall && Vx < 0 && Vy>0 || map[old_x - 1][old_y - 1] == wall && Vx < 0 && Vy < 0)
    {
        color = (color + 1) % 5;
        Vy = -Vy;
        Vx = -Vx;
    }
    ////////////
    if (map[old_x + 1][old_y] == brick && Vx >= 0)
    {
        //color = (color + 1) % 5;
        Vx = -Vx;
        map[old_x + 1][old_y] = air;
        gotoxy(old_y, old_x + 1);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
        goal++;
        gotoxy(2, width + 2);
        printf("goal:%5d", goal);
    }
    else if (map[old_x - 1][old_y] == brick && Vx <= 0)
    {
        //color = (color + 1) % 5;
        Vx = -Vx;
        map[old_x - 1][old_y] = air;
        gotoxy(old_y, old_x - 1);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
        goal++;
        gotoxy(2, width + 2);
        printf("goal:%5d", goal);
    }
    else if (map[old_x][old_y + 1] == brick && Vy >= 0)
    {
        //color = (color + 1) % 5;
        Vy = -Vy;
        map[old_x][old_y + 1] = air;
        gotoxy(old_y + 1, old_x);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
        goal++;
        gotoxy(2, width + 2);
        printf("�÷�:%5d", goal);
    }
    else if (map[old_x][old_y - 1] == brick && Vy <= 0)
    {
        //color = (color + 1) % 5;
        Vy = -Vy;
        map[old_x][old_y - 1] = air;
        gotoxy(old_y - 1, old_x);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
        goal++;
        gotoxy(2, width + 2);
        printf("goal:%5d", goal);
    }
    else if (map[old_x][old_y] == food)
    {
        map[old_x][old_y] = air;
        max++;
    }
    if (step >= max)
    {
        if_miss = 1; //������ȴ�������Ⱦ�Ҫ��ʼ��ĩβ����β�ڵ���
    }
    step = step % max;

    ////���Ƴ���,�ߵĹ켣��ʵ�Ǵ���ɫ�ġ� �����ú�ɫ�ġ� ��Ҳ���켣��ӡ�����ɡ���ȥ��β��
    if (if_miss == 1)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        gotoxy(s[step].y, s[step].x);
        map[s[step].x][s[step].y]--;//�켣�����map�����ܷ�ֹ�Լ����Լ�
        if (map[s[step].x][s[step].y] == air && !(s[step].x == 0 && s[step].y == 0))
        {
            gotoxy(s[step].y, s[step].x);
            printf("%c", air);
        }

    }
    s[step].x = old_x;
    s[step].y = old_y;

    if (_kbhit())
    {
        ch = getch();
        if (ch == 'q') {
            gotoxy(75, 2);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            printf("����\n");
            Sleep(1000);
            return;
        }
        if (ch == 'p')
            getch();
        if (ch == 'a' && map[39][left - 1] != wall)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);
            gotoxy(left - 1, 39);
            printf("%c", wall);
            gotoxy(right, 39);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            printf("%c", air);
            map[39][left - 1] = wall;
            map[39][right] = air;
            left--;
            right--;
        }
        if (ch == 'd' && map[39][right + 1] != wall)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);
            gotoxy(right + 1, 39);
            printf("%c", wall);
            gotoxy(left, 39);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            printf("%c", air);
            map[39][right + 1] = wall;
            map[39][left] = air;
            right++;
            left++;
        }
    }
    gotoxy(2, width + 3);
    printf("����:%5d", life);
    if (life == 0)
    {
        return;
    }
    gotoxy(2, width + 4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
    printf("����:%5d", max);
    Sleep(90 - old_x);//���Ƶ�ʣ���߶ȷ����

}
void play_game()
{
    int angle = 0;
    if_miss = 0;
    step = 0;
    color = 0;
    max = 80;//������

    //��ʼλ��
    X = (double)width - 4;
    Y = 4;
    old_x = (int)x;//����ֻ������
    old_y = (int)y;

    s[step].x = old_x;
    s[step].y = old_y;
    system("mode con cols=180 lines=60");
    system("color 07");
    init();
    gotoxy(length / 2, width + 3);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
    angle_switch();

begin:ch = getch();
    if (ch >= '1' && ch <= '6' || ch == 'q') {
        if (ch >= '1' && ch <= '6')
        {
            angle = (int)(ch - '1');
            gotoxy(length / 2, width + 11);
            printf("%c,��ѡ�����%d��  ", ch, (int)C[angle]);
        }
        else {
            goto begin3;
        }
    }
    else
    {
        HideCursor();
        gotoxy(length / 2, width + 11);
        printf("                   ");
        Sleep(500);
        gotoxy(length / 2, width + 11);
        printf("�������,����������");
        goto begin;
    }
    // a=��*pi/180
    Vx = -V * sin(C[angle] * PI / 180.0); //�ٶ���������Ϊ������ 
    Vy = V * cos(C[angle] * PI / 180.0);
    Sleep(1300);
    while (1)
    {
        auto_change();
        if (ch == 'q')
            break;
        if (life == 0) {
            end();
            break;
        }
    }
begin3: return;
}


int main()
{
    system("mode con cols=180 lines=60");
    system("color 07");

    HideCursor();
    ////////////
    gotoxy(0, 2);
    printf("!!!!!��ע��!!!!!      \n");
    printf("Ϊ��ֹ����,����ȫ��!  \n");
    printf("˵��:                 \n");
    printf("��λ:           wasd  \n");
    printf("ȷ��:           �ո�  \n");
    printf("------�����λس���ʼ  \n");
    getchar();
    getchar();
    ///////menu_move////
begin1:menu_x = 18;
    menu_y = 67;
    if (if_start == -1)
    {
        for (i = 0;i < width + 12;i++)
            for (j = 0;j < length + 1;j++)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
                gotoxy(j, i);
                printf(" ");
            }
        while (1)
        {
            menu();
            HideCursor();
            if (menu_x >= 18 && menu_x <= 24)
            {
                gotoxy(menu_y, menu_x);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);
                printf(" ");
            }

            ch = (char)getch();
            if (ch == 'w')
                menu_x -= 2;
            if (ch == 's')
                menu_x += 2;

            /////////////
            int flag = map[menu_x][menu_y] - '0';
            if (ch == ' ' && flag == 1)
            {
                if_start = 1;break;

            }
            if (ch == ' ' && flag == 2)//�༭��ͼ
            {
                if_start = 0;break;

            }
            if (ch == ' ' && flag == 3)
            {
                gotoxy(0, 0);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
                printf("�����˳���Ϸ...\n");
                Sleep(1500);
                exit(1);
            }
            if (ch == ' ' && flag == 4)
            {
                if_start = 2;
                break;

            }

        }
    }
    ///////game_move////////
    if (if_start == 1)
    {
        life = 3;
        left = 1;
        right = 21;
        play_game();
        if (ch == 'q')
        {
            if_start = -1;
            for (i = 1;i < width / 4;i++) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
                gotoxy(1, i);
                printf("                                         ");
            }
            for (i = 3 * width / 4;i < width;i++)
            {
                gotoxy(1, i);
                printf("                                         ");
            }
            for (i = 1;i < width;i++)
                for (j = 1;j < 2 * length / 5;j++)
                {
                    gotoxy(j, i);
                    printf(" ");
                }
            for (i = 1;i < width;i++)
                for (j = 3 * length / 5 + 1;j < length;j++) {
                    gotoxy(j, i);
                    printf(" ");
                }
            goto begin1;
        }
        if (life == 0)
        {
            if_start = -1;
            goto begin1;
        }
    }
    ///////eidt_move/////////
             ///if_start=0
             //1:brick  2:wall  3:air 4:exit
    if (if_start == 0)
    {
        int tx = width / 2, ty = 9;
        int zx = tx, zy = ty;
        init();
        gotoxy(2, width + 2);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf("1.ש��  2.ǽ��  3.����  4.����");
        for (i = 0;i <= width;i++)
            for (j = 0;j <= length;j++)
                _map[i][j] = map[i][j];

        while (1)
        {
            zx = tx;
            zy = ty;
            if (_map[tx][ty] != brick || _map[tx][ty] != wall)
            {
                gotoxy(ty, tx);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 60);
                printf("%c", '$');
            }
            ch = getch();
            switch (ch)
            {
            case 'w':tx--;break;
            case 's':tx++;break;
            case 'a':ty--;break;
            case 'd':ty++;break;
            default:break;
            }
            if (ch == '1') {
                _map[tx][ty] = brick;
                flag = 5;
            }
            if (ch == '2') {
                _map[tx][ty] = wall;
                flag = 5;
            }
            if (ch == '3') {
                _map[tx][ty] = air;
                flag = 5;
            }
            if (ch == '4') {
                gotoxy(70, 18);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
                printf("�Ƿ񱣴�? 1.Yes 2.No ,����󰴻س�ȷ��\n");
            begin2:gotoxy(73, 19);
                ch1 = getchar();
                if (ch1 == '1') {
                    if_start = -1;
                    flag = 5;
                    gotoxy(75, 19);
                    printf("����ɹ�              ");
                    Sleep(500);
                    goto begin1;
                }
                else if (ch1 == '2') {
                    if_start = -1;
                    flag = 0;
                    goto begin1;
                }
                else {
                    gotoxy(75, 19);
                    printf("����ȷ����,����������");
                    goto begin2;
                }
            }
            if (_map[zx][zy] != brick || _map[zx][zy] != wall) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
                gotoxy(zy, zx);
                printf(" ");
            }
            //ˢ�� 
            for (i = 0;i <= width;i++)
                for (j = 0;j <= length;j++)
                {
                    gotoxy(j, i);
                    printf("%c", _map[i][j]);
                }
        }
        return;
    }
    ///////more information////
    if (if_start == 2)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        for (i = 0;i <= 40;i++)
            for (j = 0;j <= 150;j++)
            {
                gotoxy(j, i);
                printf(" ");
            }
        gotoxy(75, 2);
        printf("�����ڴ�");
        if_start = -1;
        Sleep(2000);
        goto begin1;
    }
}
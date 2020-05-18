/*-----------------------------------------------------------------
2020.4.6���£�      ����45�Ƚ�б�׹��ܣ��Ż�ǽ������ײ�������˵�bug
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
int i, j, old_x, old_y, flag, color, if_miss, step, max;
double x, y, Vx, Vy, s1, s2, X, Y;//Vx����Vy����
char wall = '#', air = ' ',brick='&';
double C[8] = { 0, 10, 30,45,70, 90 };
int flag = 1;
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
    printf("-------��Q�˳�------------\n");
}


void init()
{
    HideCursor();//���ع��ĺ���
    gotoxy(0, 0);
    for (i = 0; i <= width; i++)
    { //���ɵ�ͼģ�岢���������map��
        for (j = 0; j <= length; j++)
        {
            if (i == 0 || i == width || j == 0 || j == length  || (i == 2 * width / 3) && (j >= 63 && j <= 87))
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


    gotoxy(0, 0);
    //��ӡ��ͼ

    for (i = 0; i <= width; i++)
    {
        for (j = 0; j <= length; j++)
        {

            if (map[i][j] == wall)
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


        }
        printf("\n");//��������ֹ���룡����
    }
    gotoxy(length / 2, width + 3);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
    angle_switch();
}




///////////������
void sport()
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
    ���Ĺ�ʽ��   ����X = X0 + Vx*t + 1/2*VX*t*t;
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
            color = (color + 1) % 5;
            Vx = -Vx;
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
    if (map[old_x + 1][old_y] == brick && Vx >= 0)
    {
        //color = (color + 1) % 5;
        Vx = -Vx;
        map[old_x + 1][old_y] = air;
        gotoxy(old_y, old_x + 1);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
      
    }
    else if (map[old_x - 1][old_y] == brick && Vx <= 0)
    {
        //color = (color + 1) % 5;
        Vx = -Vx;
        map[old_x - 1][old_y] = air;
        gotoxy(old_y, old_x - 1);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
      
    }
    else if (map[old_x][old_y + 1] == brick && Vy >= 0)
    {
        //color = (color + 1) % 5;
        Vy = -Vy;
        map[old_x][old_y + 1] = air;
        gotoxy(old_y + 1, old_x);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
        
    }
    else if (map[old_x][old_y - 1] == brick && Vy <= 0)
    {
        //color = (color + 1) % 5;
        Vy = -Vy;
        map[old_x][old_y - 1] = air;
        gotoxy(old_y - 1, old_x);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        printf(" ");
       
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
        if (map[s[step].x][s[step].y] == air)
            printf("%c", air);
    }
    s[step].x = old_x;
    s[step].y = old_y;

    if (_kbhit())
    {
        if (getch() == 'q') {
            gotoxy(2, 2);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            printf("�ټ�");
            exit(1);
        }
        else
            return;
    }
    Sleep(80 - 2 * old_x);//���Ƶ�ʣ���߶ȷ����
}

int main()
{
    char ch;
    int angle = 0;
    if_miss = 0;
    step = 0;
    color = 0;
    max = 80;//������

    //��ʼλ��
    X = width - 4;
    Y = 4;
    old_x = (int)x;//����ֻ������
    old_y = (int)y;

    s[step].x = old_x;
    s[step].y = old_y;
    system("mode con cols=180 lines=60");
    system("color 07");
    init();

    ch = getch();
    if (ch <= '6') {
        angle = (int)(ch - '1');
        gotoxy(length / 2, width + 11);
        printf("%c,���������%d��", ch, (int)C[angle]);
    }
    else
    {
        HideCursor();
        gotoxy(length / 2, width + 11);
        printf("�������");
    }
    // a=��*pi/180
    Vx = -V * sin(C[angle] * PI / 180.0); //�ٶ���������Ϊ������ 
    Vy = V * cos(C[angle] * PI / 180.0);
    while (1)
    {
        sport();
    }
}
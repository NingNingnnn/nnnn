#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

// ���� Windows Multimedia API application programming interface
#pragma comment(lib,"Winmm.lib")/* #pragma ��һ��������ָ��,�Ǹ����߱�������Ҫ����һЩ����,Ȼ���ں����comment����ָ����ʲô���� 
                                          winmm.lib��������windows¼���豸�ɼ�¼������*/
#define High 1000 // ��Ϸ����ߴ�
#define Width 462

IMAGE img_bk; // ����ͼƬ
float position_x, position_y; // �ɻ�λ��
float bullet_x, bullet_y; // �ӵ�λ��
float enemy_x, enemy_y; // �л�λ��
IMAGE img_planeNormal1, img_planeNormal2; // �����ɻ�ͼƬ
IMAGE img_planeExplode1, img_planeExplode2; // ��ը�ɻ�ͼƬ
IMAGE img_bullet1, img_bullet2; // �ӵ�ͼƬ
IMAGE img_enemyPlane1, img_enemyPlane2; // �л�ͼƬ
int isExpolde = 0; // �ɻ��Ƿ�ը
int score = 0; // �÷�

void startup()
{
	mciSendString("open D:\\game_music.mp3 alias bkmusic", 0, 0, 0);//�򿪱�������
	mciSendString("play bkmusic repeat", 0, 0, 0);  // ѭ������
	initgraph(Width, High);
	loadimage(&img_bk, "D:\\background.jpg");
	loadimage(&img_planeNormal1, "D:\\planeNormal_1.jpg");
	loadimage(&img_planeNormal2, "D:\\planeNormal_2.jpg");
	loadimage(&img_bullet1, "D:\\bullet1.jpg");
	loadimage(&img_bullet2, "D:\\bullet2.jpg");
	loadimage(&img_enemyPlane1, "D:\\enemyPlane1.jpg");
	loadimage(&img_enemyPlane2, "D:\\enemyPlane2.jpg");
	loadimage(&img_planeExplode1, "D:\\planeExplode_1.jpg");
	loadimage(&img_planeExplode2, "D:\\planeExplode_2.jpg");
	position_x = Width * 0.5;
	position_y = High * 0.7;
	bullet_x = position_x;
	bullet_y = -85;
	enemy_x = Width * 0.5;
	enemy_y = 10;
	BeginBatchDraw();//��������Ҫ���������Ķ���ʱ��һֱˢ����Ļ���������.��һ����װ��˫����
	//����BeginBatchDraw�����еĻ�ͼ������ʾ����Ļ�ϣ������ڴ��н���,ֱ������EndBatchDraw��֮ǰ�������ڴ��л�ͼ�ĳ�Ʒһ��չʾ����Ļ
}

void show()
{
	putimage(0, 0, &img_bk);	// ��ʾ����	
	if (isExpolde == 0)
	{
		putimage(position_x - 50, position_y - 60, &img_planeNormal1, NOTSRCERASE); // ��ʾ�����ɻ�	
		putimage(position_x - 50, position_y - 60, &img_planeNormal2, SRCINVERT);

		putimage(bullet_x - 7, bullet_y, &img_bullet1, NOTSRCERASE); // ��ʾ�ӵ�	
		putimage(bullet_x - 7, bullet_y, &img_bullet2, SRCINVERT);
		putimage(enemy_x, enemy_y, &img_enemyPlane1, NOTSRCERASE); // ��ʾ�л�	
		putimage(enemy_x, enemy_y, &img_enemyPlane2, SRCINVERT);
	}
	else
	{
		putimage(position_x - 50, position_y - 100, &img_planeExplode1, NOTSRCERASE); // ��ʾ��ը�ɻ�	
		putimage(position_x - 50, position_y - 100, &img_planeExplode2, SRCINVERT);
	}
	outtextxy(Width * 0.48, High * 0.95, "�÷֣�");//�ú�������Ļ�ϵ�ָ����(x��y)����ʾ�ı����ַ���
	char s[5];
	sprintf_s(s, "%d", score);
	outtextxy(Width * 0.55, High * 0.95, s);
	FlushBatchDraw();
	Sleep(2);
}

void updateWithoutInput()
{
	if (isExpolde == 0)
	{
		if (bullet_y > -25)
			bullet_y = bullet_y - 1;

		if (enemy_y < High - 25)
			enemy_y = enemy_y + 0.5;
		else
			enemy_y = 10;
		if (fabs(bullet_x - enemy_x) + fabs(bullet_y - enemy_y) < 80)  // �ӵ����ел�
		{
			enemy_x = rand() % Width; //��0~Width-1֮����������
			enemy_y = -40;
			bullet_y = -85;
			mciSendString("close gemusic", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString("open D:\\gotEnemy.mp3 alias gemusic", NULL, 0, NULL); // ������
			mciSendString("play gemusic", NULL, 0, NULL); // ������һ��
			score++;
			if (score > 0 && score % 5 == 0 && score % 2 != 0)
			{
				mciSendString("close 5music", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
				mciSendString("open D:\\5.mp3 alias 5music", NULL, 0, NULL); // ������
				mciSendString("play 5music", NULL, 0, NULL); // ������һ��
			}
			if (score % 10 == 0)
			{
				mciSendString("close 10music", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
				mciSendString("open D:\\10.mp3 alias 10music", NULL, 0, NULL); // ������
				mciSendString("play 10music", NULL, 0, NULL); // ������һ��
			}
		}

		if (fabs(position_x - enemy_x) + fabs(position_y - enemy_y) < 150)  // �л���������
		{
			isExpolde = 1;
			mciSendString("close exmusic", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString("open D:\\explode.mp3 alias exmusic", NULL, 0, NULL); // ������
			mciSendString("play exmusic", NULL, 0, NULL); // ������һ��		
		}
	}
}

void updateWithInput()
{
	if (isExpolde == 0)
	{
		MOUSEMSG m;		// ���������Ϣ
		if (MouseHit())  //����������ڼ�⵱ǰ�Ƿ��������Ϣ
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_MOUSEMOVE)
			{
				// �ɻ���λ�õ���������ڵ�λ��
				position_x = m.x;
				position_y = m.y;
			}
			else if (m.uMsg == WM_LBUTTONDOWN)
			{
				// �����������������ӵ�
				bullet_x = position_x;
				bullet_y = position_y - 85;
				mciSendString("close fgmusic", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
				mciSendString("open D:\\f_gun.mp3 alias fgmusic", NULL, 0, NULL); // ������
				mciSendString("play fgmusic", NULL, 0, NULL); // ������һ��
			}
		}
	}
}

void gameover()
{
	EndBatchDraw();
	_getch();
	closegraph();
}

int main()
{
	startup();  // ���ݳ�ʼ��	
	while (1)  //  ��Ϸѭ��ִ��
	{
		show();  // ��ʾ����
		updateWithoutInput();  // ���û������޹صĸ���
		updateWithInput();     // ���û������йصĸ���
	}
	gameover();     // ��Ϸ��������������
	return 0;
}

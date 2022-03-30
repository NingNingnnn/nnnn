#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

// 引用 Windows Multimedia API application programming interface
#pragma comment(lib,"Winmm.lib")/* #pragma 是一条编译器指令,是给告诉编译器你要链接一些东西,然后在后面的comment里面指明是什么东西 
                                          winmm.lib库用来对windows录音设备采集录音数据*/
#define High 1000 // 游戏画面尺寸
#define Width 462

IMAGE img_bk; // 背景图片
float position_x, position_y; // 飞机位置
float bullet_x, bullet_y; // 子弹位置
float enemy_x, enemy_y; // 敌机位置
IMAGE img_planeNormal1, img_planeNormal2; // 正常飞机图片
IMAGE img_planeExplode1, img_planeExplode2; // 爆炸飞机图片
IMAGE img_bullet1, img_bullet2; // 子弹图片
IMAGE img_enemyPlane1, img_enemyPlane2; // 敌机图片
int isExpolde = 0; // 飞机是否爆炸
int score = 0; // 得分

void startup()
{
	mciSendString("open D:\\game_music.mp3 alias bkmusic", 0, 0, 0);//打开背景音乐
	mciSendString("play bkmusic repeat", 0, 0, 0);  // 循环播放
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
	BeginBatchDraw();//当我们需要绘制连续的动画时，一直刷新屏幕会产生闪屏.是一个封装的双缓存
	//运行BeginBatchDraw后，所有的绘图都不显示在屏幕上，而在内存中进行,直到碰到EndBatchDraw，之前所有在内存中绘图的成品一并展示于屏幕
}

void show()
{
	putimage(0, 0, &img_bk);	// 显示背景	
	if (isExpolde == 0)
	{
		putimage(position_x - 50, position_y - 60, &img_planeNormal1, NOTSRCERASE); // 显示正常飞机	
		putimage(position_x - 50, position_y - 60, &img_planeNormal2, SRCINVERT);

		putimage(bullet_x - 7, bullet_y, &img_bullet1, NOTSRCERASE); // 显示子弹	
		putimage(bullet_x - 7, bullet_y, &img_bullet2, SRCINVERT);
		putimage(enemy_x, enemy_y, &img_enemyPlane1, NOTSRCERASE); // 显示敌机	
		putimage(enemy_x, enemy_y, &img_enemyPlane2, SRCINVERT);
	}
	else
	{
		putimage(position_x - 50, position_y - 100, &img_planeExplode1, NOTSRCERASE); // 显示爆炸飞机	
		putimage(position_x - 50, position_y - 100, &img_planeExplode2, SRCINVERT);
	}
	outtextxy(Width * 0.48, High * 0.95, "得分：");//该函数在屏幕上的指定点(x，y)上显示文本或字符串
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
		if (fabs(bullet_x - enemy_x) + fabs(bullet_y - enemy_y) < 80)  // 子弹击中敌机
		{
			enemy_x = rand() % Width; //在0~Width-1之间产生随机数
			enemy_y = -40;
			bullet_y = -85;
			mciSendString("close gemusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString("open D:\\gotEnemy.mp3 alias gemusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play gemusic", NULL, 0, NULL); // 仅播放一次
			score++;
			if (score > 0 && score % 5 == 0 && score % 2 != 0)
			{
				mciSendString("close 5music", NULL, 0, NULL); // 先把前面一次的音乐关闭
				mciSendString("open D:\\5.mp3 alias 5music", NULL, 0, NULL); // 打开音乐
				mciSendString("play 5music", NULL, 0, NULL); // 仅播放一次
			}
			if (score % 10 == 0)
			{
				mciSendString("close 10music", NULL, 0, NULL); // 先把前面一次的音乐关闭
				mciSendString("open D:\\10.mp3 alias 10music", NULL, 0, NULL); // 打开音乐
				mciSendString("play 10music", NULL, 0, NULL); // 仅播放一次
			}
		}

		if (fabs(position_x - enemy_x) + fabs(position_y - enemy_y) < 150)  // 敌机击中我们
		{
			isExpolde = 1;
			mciSendString("close exmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString("open D:\\explode.mp3 alias exmusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play exmusic", NULL, 0, NULL); // 仅播放一次		
		}
	}
}

void updateWithInput()
{
	if (isExpolde == 0)
	{
		MOUSEMSG m;		// 定义鼠标消息
		if (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_MOUSEMOVE)
			{
				// 飞机的位置等于鼠标所在的位置
				position_x = m.x;
				position_y = m.y;
			}
			else if (m.uMsg == WM_LBUTTONDOWN)
			{
				// 按下鼠标左键，发射子弹
				bullet_x = position_x;
				bullet_y = position_y - 85;
				mciSendString("close fgmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
				mciSendString("open D:\\f_gun.mp3 alias fgmusic", NULL, 0, NULL); // 打开音乐
				mciSendString("play fgmusic", NULL, 0, NULL); // 仅播放一次
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
	startup();  // 数据初始化	
	while (1)  //  游戏循环执行
	{
		show();  // 显示画面
		updateWithoutInput();  // 与用户输入无关的更新
		updateWithInput();     // 与用户输入有关的更新
	}
	gameover();     // 游戏结束、后续处理
	return 0;
}

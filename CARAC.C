#include <graphics.h>
#include <alloc.h>
#include <stdlib.h>


int g_mode,g_driver;
char texto [] = {"ABCDEFGHIJKLMNOPQRSTUVXZ abcdefghijklmnopqrstuvxz 0123456789"};
char c;

void main ()
{

	g_mode = 1;
	g_driver = 3;

	if (registerbgidriver (EGAVGA_driver) < 0) exit (1);
	if (registerbgifont (small_font) < 0) exit (1);

	initgraph (&g_driver,&g_mode," ");

	setcolor (WHITE);

	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	setusercharsize (1,1,1,1);
	outtextxy (60,33,"Opcao atual:");
	outtextxy (60,43,texto);
	outtextxy (60,53,"Light - Servicos de Eletricidade S. A.");
	outtextxy (60,63,"LIGHT - SERVICOS DE ELETRICIDADE S. A.");
	outtextxy (60,73,"DISJUNTOR 5345 SECCIONADORA 1H2345");

	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	setusercharsize (7,8,7,8);
	outtextxy (60,95,"op‡ao 1:");
	outtextxy (60,105,texto);
	outtextxy (60,115,"Light - Servicos de Eletricidade S.A.");
	outtextxy (60,125,"LIGHT SERVICOS DE ELETRICIDADE S.A.");
	outtextxy (60,135,"DISJUNTOR 5345  SECCIONADORA 1H2345");


	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	setusercharsize (7,8,3,4);
	outtextxy (60,158,"op‡ao 2:");
	outtextxy (60,168,texto);
	outtextxy (60,178,"Light - Servicos de Eletricidade S. A.");
	outtextxy (60,188,"LIGHT - SERVICOS DE ELETRICIDADE S. A.");
	outtextxy (60,198,"DISJUNTOR 5345  SECCIONADORA 1H2345");

	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	setusercharsize (1,1,1,1);
	outtextxy (60,230,"Para sair tecle algo.");

	c = getch ();

	clearviewport();
	closegraph ();       /* retorna … condi‡ao inicial do video, antes do init.. */
	exit (0);

}


						/************************/
						/*		 MEDDEF.C       */
						/* MODULO DE DEFINICOES */
						/************************/

#include <graphics.h>
#include <alloc.h>
#include <dos.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 0xff

int extern pos_x,pos_y,stbut,cor_rasc,cor_fg,cor_bg,direcao;

int tipo_linha,letra;
int mult_x,mult_y,div_x,div_y,op;
int tp_small = 2;  /* tipo de caracter em small font */

int vetor_p [] = { 5, 35, 45, 35, 45, 54,  5, 54,  5, 35},
    vetor_i [] = {46, 35, 85, 35, 85, 54, 46, 54, 46, 35},
	vetor_b [] = { 0, 31, 90, 31, 90,213,  0,213,  0, 31},
	vetor_f [] = { 5,200, 85,200, 85,210,  5,210,  5,200},
	vetor_c [] = { 0, 31, 90, 31, 90, 80,  0, 80,  0, 31},
	vetor_q [] = { 5, 35, 85, 35, 85, 75,  5, 75,  5, 35};


char texto [] = {"A"};

char extern *menu [];

/* DECLARACOES PARA ROTINA DEFINE */

#define QTD_OPCOES_DEF 5
#define POS_MENU_X 120
#define POS_MENU_Y  30
char menu_def [QTD_OPCOES_DEF][15] = { {"COR"}, {"LINHA"}, {"TIPO TEXTO"}, {"DIR TEXTO"}, {"SAIR"} };

/* DECLARACOES PARA ROTINA DEF_LINHA */

#define QTD_OPCOES_DLIN 3
#define POS_DLMENU_X 120
#define POS_DLMENU_Y  30
char menu_dflinha [QTD_OPCOES_DLIN][15] = { {"NORMAL"},{"CHEIA"}, {"SAIR"} };

/* DECLARACOES PARA ROTINA DEF_DIRECAO */

#define QTD_OPCOES_DDIR 2
#define POS_DDIR_X 120
#define POS_DDIR_Y  30
char menu_ddir[QTD_OPCOES_DDIR][15] = { {"HORIZONTAL"}, {"VERTICAL"} };
/*===================================================================*/

void faz_janela_texto ()  /* opcoes de tipo de texto */
{
	void *b_temp;
	unsigned tam;
	int y;


	esconde_mouse ();
	tam = imagesize (0,31,90,80);
	b_temp = malloc (tam);
	getimage (0,31,90,80,b_temp);
	setcolor (BROWN);
	setfillstyle (INTERLEAVE_FILL,6);			/* faz fundo do menu */
	fillpoly (5,vetor_c);
	setcolor (BLACK);
	setfillstyle (SOLID_FILL,BLACK);
	fillpoly (5,vetor_q);
	setcolor (6);
	line (25,35,25,55);
	line (45,35,45,55);
	line (65,35,65,55);
	line (5,55,85,55);
	setcolor (WHITE);

	settextstyle (SMALL_FONT,HORIZ_DIR,0);                /* mostra todas as opcoes */
	setusercharsize (7,8,3,4);
	outtextxy (14,43,texto);

	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	setusercharsize (7,8,7,8);
	outtextxy (34,42,texto);

	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	setusercharsize (4,4,4,4);
	outtextxy (52,40,texto);

	settextstyle (SANS_SERIF_FONT,HORIZ_DIR,1);
	outtextxy (70,32,texto);

	if (letra == SMALL_FONT)
	{
	   op = 0;
	   switch ( tp_small )
	   {
		case 0:
			y = 60;
			break;

		case 1:
			y = 60;
			break;

		case 2:
			y = 60;                      /* linha onde escreve o texto */
			break;
	   }
	   settextstyle (letra,HORIZ_DIR,op);
	   setusercharsize (mult_x,div_x,mult_y,div_y);
	}
	else
	{
		op = 1;
		y = 55;
		settextstyle (letra,HORIZ_DIR,op);
	}
	settextstyle (letra,HORIZ_DIR,op);
	setcolor (BROWN);
	outtextxy (40,y,texto);
	aparece_mouse ();
	get_buttom_pressed ();
	while ((stbut & 2) != 2)    					/* espera selecao do mouse */
	{
		if ((pos_x > 5  ) &&
			(pos_x < 85 ) &&
			(pos_y > 35 ) &&
			(pos_y < 55 ) )
		{
			esconde_mouse ();
			apg_ret (6,56,84,74);
			if (pos_x <= 25)
			{
				letra = SMALL_FONT;
				tp_small = 0;
				mult_x = 7;
				mult_y = 3;
				div_x =  8;
				div_y =  4;
				settextstyle (letra,HORIZ_DIR,0);
				setusercharsize (mult_x,div_x,mult_y,div_y);
				outtextxy (40,60,texto);                          /* exibe tipo de fonte, de acordo com o escolhido */
			}
			else
			if (pos_x <= 45)         							/* seta parametros de texto */
			{
				letra = SMALL_FONT;
				tp_small = 1;
				mult_x = 7;
				mult_y = 7;
				div_x =  8;
				div_y =  7;
				settextstyle (letra,HORIZ_DIR,0);
				setusercharsize (mult_x,div_x,mult_y,div_y);
				outtextxy (40,60,texto);                          /* exibe tipo de fonte, de acordo com o escolhido */
			}
			else
			if (pos_x <= 65)         							/* seta parametros de texto */
			{
				letra = SMALL_FONT;
				tp_small = 2;
				mult_x = 4;
				mult_y = 4;
				div_x =  4;
				div_y =  4;
				settextstyle (letra,HORIZ_DIR,0);
				setusercharsize (mult_x,div_x,mult_y,div_y);
				outtextxy (40,60,texto);                          /* exibe tipo de fonte, de acordo com o escolhido */
			}
			else
			{
				letra = SANS_SERIF_FONT;
				settextstyle (letra,HORIZ_DIR,1);
				outtextxy (40,53,texto);
			}
			aparece_mouse ();
		}
		get_buttom_pressed ();
	}
	esconde_mouse ();
	putimage (0,31,b_temp,COPY_PUT);
	aparece_mouse ();
	free (b_temp);
	setcolor (cor_fg);
	settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
}

/*=================================================================*/

void def_direcao ()
{
	int opcao;

	get_mouse_release (0);
	for (opcao = 0; opcao < QTD_OPCOES_DDIR; opcao++)
		menu [opcao] = &menu_ddir [opcao][0];
	opcao = opcoes (POS_DDIR_X,POS_DDIR_Y,QTD_OPCOES_DDIR);
	switch (opcao)
	{
		case 0: direcao = HORIZ_DIR;
				break;

		case 1: direcao = VERT_DIR;
				break;

		default: break;
	}
}

/*=================================================================*/

void faz_janela_cor ()  /* janela para selecao de paletas de cores */
{
   int k,desloc;

	setcolor (15);
	setfillstyle (INTERLEAVE_FILL,15);
	fillpoly (5,vetor_b);
	for (k = 0; k < 16; k++)
	{
		setcolor (k);
		setfillstyle (SOLID_FILL,k);
		desloc = k >> 1;
		if ((k & 1) == 0)
		{
			vetor_p[1] = vetor_p[3] = vetor_p [9] = (desloc * 20) + 35;
			vetor_p[5] = vetor_p[7] = (desloc * 20) + 55;
			fillpoly (5,vetor_p);
		}
		else
		{
			vetor_i[1] = vetor_i[3] = vetor_i [9] = (desloc * 20) + 35;
			vetor_i[5] = vetor_i[7] = (desloc * 20) + 55;
			fillpoly (5,vetor_i);
		}
	}
	setcolor (cor_fg);
	setfillstyle (SOLID_FILL,cor_fg);
	fillpoly (5,vetor_f);
}

/*======================================================================*/

void def_cor ()  /* define cor da paleta */
{

	int opcao;
	unsigned size;
	void *b_janela;

	get_mouse_release (0);
	esconde_mouse ();
	size = imagesize (0,31,90,213);
	b_janela = malloc (size);
	getimage (0,31,90,213,b_janela);
	faz_janela_cor ();
	aparece_mouse ();
	get_buttom_pressed ();
	while ((stbut & 2) != 2)
	{
		if ( (pos_x > 5  ) &&
			 (pos_x < 85 ) &&
			 (pos_y > 35 ) &&
			 (pos_y < 195))
		{
			opcao = ((( pos_y - 35) / 20)) * 2;
			if (pos_x > 45)
				opcao++;
			cor_fg = opcao;
			setcolor (cor_fg);
			setfillstyle (SOLID_FILL,cor_fg);
			esconde_mouse ();
			fillpoly (5,vetor_f);
			aparece_mouse ();
			cor_rasc = (cor_fg == cor_rasc) ? YELLOW : WHITE;
		}
		get_buttom_pressed ();
	}
	esconde_mouse ();
	putimage (0,31,b_janela,COPY_PUT);
	aparece_mouse ();
	free (b_janela);
}

/*====================================================*/


void def_linha ()
{
	int opcao;

	get_mouse_release (0); 		                                   /* espera soltar botao */
	for (opcao = 0;opcao < QTD_OPCOES_DLIN;opcao++)                /* preenche texto para menu */
		menu [opcao] = &menu_dflinha[opcao][0];
	opcao = opcoes (POS_DLMENU_X,POS_DLMENU_Y,QTD_OPCOES_DLIN);    /* escreve menu e aguarda opcao */
	switch (opcao)
	{

		case 0: tipo_linha = NORM_WIDTH;
				break;

		case 1: tipo_linha = THICK_WIDTH;  /* tracejada */
				break;

		default: break;
	}
}

/*=========================================================*/

void define ()
{

	int opcao,fim_def;

	fim_def = FALSE;
	do
	{
		get_mouse_release (0);
		for (opcao = 0;opcao < QTD_OPCOES_DEF; opcao++)
			menu [opcao] = &menu_def[opcao][0];
		opcao = opcoes (POS_MENU_X,POS_MENU_Y,QTD_OPCOES_DEF);
		switch (opcao)
		{
			case 0: def_cor ();
					mostra_def ();
					break;

			case 1:	def_linha ();
					mostra_def ();
					break;

			case 2: faz_janela_texto ();
					mostra_def ();
					break;

			case 3: def_direcao ();
					get_mouse_release (0);
					break;

			case 4: fim_def = TRUE;
					get_mouse_release (0);
					break;

			default: break;
		}
	}
	while (!fim_def);
	mostra_def ();
}

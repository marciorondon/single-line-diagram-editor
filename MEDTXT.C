
					/********************/
					/*     MEDTXT.C     */
					/* MODULO EDITOR DE */
					/*      TEXTOS      */
					/********************/


#include <graphics.h>
#include <alloc.h>
#include <ctype.h>
#include <dos.h>

#include "vert_ic.h"
#include "jantxt.h"

#define POS_XT_RET  137
#define POS_YT_RET  310
#define POS_XB_RET  514
#define POS_YB_RET  337
#define TRUE 0xFF
#define FALSE 0

int extern letra,pos_x,pos_y,stbut,cor_fg,
		   mult_x,mult_y,div_x,div_y;
int direcao,teclou_esc;
void extern *area;

char msg_texto [41],barra_o[12],barra_d[12];

/*===========================================================*/

void del_texto (int *indice,char origem,char energia)
{
	int comp;

	if (*indice != 0)
	{
		(*indice)--;
		msg_texto [*indice] = 0;
		comp = textwidth (msg_texto);
		switch (origem)
		{
			case 'T': bar (POS_XT_RET+5+48+comp,POS_YT_RET+11,POS_XT_RET+5+48+comp+8,POS_YT_RET+19);  /* texto */
					  break;

			case 'V': bar (TOPX+56+60+comp,TOPY+36,TOPX+56+60+comp+8,TOPY+44);                /* tensao */
					  break;

			case 'E': switch (energia)                                                        /* energia */
					  {
						case 'O': bar (TOPX+30+115+comp,TOPY+23,TOPX+30+115+comp+8,TOPY+31);  /* barra origem */
								  break;

						case 'D': bar (TOPX+30+120+comp,TOPY+33,TOPX+30+120+comp+8,TOPY+41);  /* barra destino */
								  break;
					  }
					  break;

			case 'X': bar (XT_TXT_IC+5+160+comp,YT_TXT_IC+8,XT_TXT_IC+5+160+comp+8,YT_TXT_IC+11+8);  /* Dimensao horinzontal */
					  break;

			case 'Y': bar (XT_TXT_IC+5+144+comp,YT_TXT_IC+20,XT_TXT_IC+5+160+comp+8,YT_TXT_IC+20+8);  /* Dimensao vertical */
					  break;
		}
	}
	else
		putch (07);
}

/*============================================================*/

void le_texto (char origem)
{

	int tecla,indx,sai,chega;
	char c,energia;

	limpa_teclado ();
	indx = msg_texto [0] = sai = chega  = FALSE;
	energia = 'O';
	do
	{
		tecla = getch ();
		if (isprint (tecla))    /* tecla printavel - || (tecla == 0x5C) || (tecla == 0x3A)) */
		{
			if (!chega)  /* se nao acabou o limite */
			{
				c = tecla;
				msg_texto [indx] = c;
				msg_texto [indx+1] = 0;     /* guarda na string */
				indx++;
				switch (origem)
				{
					case 'T':	outtextxy (POS_XT_RET+5+48,POS_YT_RET+11,&msg_texto [0]);  /* texto */
								if (indx == 40)
									chega = TRUE;
								break;

					case 'V':   if (!isdigit (c))             /* tensao - se nao numerico volta e bell */
								{
									indx--;
									msg_texto [indx] = 0;
									putch (07);
								}
								outtextxy (TOPX+56+60,TOPY+36,&msg_texto[0]);
								if (indx == 4)
									chega = TRUE;
								break;

					case 'E':	switch (energia)                                          /* energia */
								{
									case 'O': if (!isdigit (c))             /* tensao - se nao numerico volta e bell */
											  {
												  indx--;
												  msg_texto [indx] = 0;
												  putch (07);
											  }
											  outtextxy (TOPX+30+115,TOPY+23,&msg_texto[0]);  /* barra origem */
											  if (indx == 4)
												  chega = TRUE;
											  break;

									case 'D': if (!isdigit (c))             /* tensao - se nao numerico volta e bell */
											  {
												  indx--;
												  msg_texto [indx] = 0;
												  putch (07);
											  }
											  outtextxy (TOPX+30+120,TOPY+33,&msg_texto[0]);  /* barra destino */
											  if (indx == 4)
												  chega = TRUE;
											  break;
								}
								break;

					case 'X': if (!isdigit (c))       /* Dimensao horizontal */
							  {
								indx--;
								msg_texto [indx] = 0;
								putch (07);
							  }
							  outtextxy (XT_TXT_IC+5+160,YT_TXT_IC+8,&msg_texto [0]);
							  if (indx ==3)
								chega = TRUE;
							  break;

					case 'Y': if (!isdigit (c))       /* Dimensao vertical */
							  {
								indx--;
								msg_texto [indx] = 0;
								putch (07);
							  }
							  outtextxy (XT_TXT_IC+5+144,YT_TXT_IC+20,&msg_texto [0]);
							  if (indx ==3)
								chega = TRUE;
							  break;
				}

			}
			else
				putch (07);
		}
		else  /* se tecla nao printavel */
		{
			switch (tecla)
			{
				case 0x0D : msg_texto [indx] = 0;      /* Enter */
							if ((origem == 'E') && (energia == 'O'))
							{
								energia = 'D';
								strcpy (barra_o,msg_texto);
								msg_texto [0] = 0;
								indx = 0;
							}
							else
							{
								sai = TRUE;
								strcpy (barra_d,msg_texto);
							}
							chega = teclou_esc = FALSE;
							break;

				case 0x1B : sai = TRUE;               /* ESC   */
							msg_texto [0] = 0;
							teclou_esc = TRUE;
							break;

				case 0x08 : chega = teclou_esc = FALSE;   /* backspace */
							del_texto (&indx,origem,energia);                   /* Back Space */
							break;

				case 0:     tecla = getch ();
							switch (tecla)
							{
								case 0x53:                /* Seta */
								case 0x4B: chega = FALSE;
										   del_texto (&indx,origem,energia);      /* Del  */
										   break;

								default :  putch (07);
										   break;
							}
							break;

				default:    putch (07);
							break;
			}
		}
	}
	while (!sai);
}

/*================================================*/

void faz_fundo()
{

	unsigned tamanho;

	tamanho = imagesize (POS_XT_RET,POS_YT_RET,POS_XB_RET,POS_YB_RET);
	area = malloc (tamanho);

	getimage (POS_XT_RET,POS_YT_RET,POS_XB_RET,POS_YB_RET,area);

	setcolor (BROWN);                                      /* faz retangulos */
	setfillstyle (SOLID_FILL,BROWN);
	disable ();
	bar (POS_XT_RET,POS_YT_RET,POS_XB_RET,POS_YB_RET);
	enable ();
	setfillstyle (SOLID_FILL,CYAN);
	disable ();
	bar (POS_XT_RET+4,POS_YT_RET+4,POS_XB_RET-4, POS_YB_RET-4);
	enable ();
	setcolor (BLACK);
	rectangle (POS_XT_RET+1,POS_YT_RET+1,POS_XB_RET-1, POS_YB_RET-1);
}
/*==============================================*/
void tira_fundo ()
{
	putimage (POS_XT_RET,POS_YT_RET,area,COPY_PUT);
	free (area);
}
/*==============================================*/

void janela_texto ()                                     /*  trata area onde aparecera o menu */
{
	faz_fundo ();
	outtextxy (POS_XT_RET+5,POS_YT_RET+11,"TEXTO:");
	le_texto ('T');
	tira_fundo ();
}

/*==================================================================*/

void insere_texto ()
{
	int x,y;
	void *b_texto,*b_antigo,*b_fundo;
	int  altura,compri,prim,antx,anty,xt,yt,xb,yb;
	unsigned size;

	get_mouse_release (0);
	esconde_mouse ();
	do
	{
		janela_texto ();
		if (msg_texto [0] != 0)
		{
			posiciona_mouse (0,50);
			if (letra == SMALL_FONT)
			{
			   settextstyle (letra,direcao,0);
			   setusercharsize (mult_x,div_x,mult_y,div_y);
			}
			else
			    settextstyle (letra,direcao,1);
			if (direcao == HORIZ_DIR)
			{
				altura = textheight (msg_texto);
//				altura += 4;
				compri = textwidth (msg_texto);
			}
			else
			{
				altura = textwidth (msg_texto) + 4;
				compri = textheight (msg_texto);
			}
			size = altura * compri;
			b_texto = malloc (size);
			b_fundo = malloc (size);
			xt = 0;
			yt = 50;
			xb = xt + compri;
			yb = yt + altura;
			setcolor (cor_fg);
			getimage (xt,yt,xb,yb,b_fundo);
			apg_ret (xt,yt,xb,yb);
			outtextxy (xt,yt,msg_texto);
			getimage (xt,yt,xb,yb,b_texto);
			putimage (xt,yt,b_fundo,COPY_PUT);
			free (b_fundo);
			b_antigo = malloc (size);
			antx = anty = stbut = prim = 0;
			do
			{
				get_mouse_position ();
				if ((antx != pos_x) || (anty != pos_y) || (prim == 0))
				{
					if (prim == 0)
						prim = 1;
					else
						putimage (antx,anty,b_antigo,COPY_PUT);                 /* restaura area antiga */
					getimage (pos_x,pos_y,pos_x+compri,pos_y+altura,b_antigo);  /* guarda area nova */
					putimage (pos_x,pos_y,b_texto,COPY_PUT);                    /* escreve area a ser copiada */
					antx = pos_x;                                               /* salva posicao anterior */
					anty = pos_y;
				}
			}
			while (((stbut & 1) != 1) && ((stbut & 2) != 2));
			if ((stbut & 2) == 2)
			{
				get_mouse_release (1);
				putimage (antx,anty,b_antigo,COPY_PUT);
			}
			else
				get_mouse_release (0);
			free (b_texto);
			free (b_antigo);
		}
		settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
	}
	while (msg_texto[0] != 0);
	setwritemode (COPY_PUT);
	aparece_mouse ();

}

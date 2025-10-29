				  /***************************/
				  /* MODULO EDITOR DE ICONES */
				  /***************************/


#include <graphics.h>
#include <dos.h>
#include <alloc.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <dos.h>
#include "vert_ic.h"



#define GRADE_X 8
#define GRADE_Y GRADE_X
#define VERT_X 100
#define VERT_Y 50
#define X_SIMB 500
#define Y_SIMB 60
#define INI_TEC_X 120
#define INI_TEC_Y 300
#define FIM_TEC_X INI_TEC_X+110
#define FIM_TEC_Y INI_TEC_Y+23


int dim_x,dim_y,
	tam_grade_x,tam_grade_y,
	vertice_dir_grade_x,vertice_dir_grade_y,
	passo_x,passo_y;

int extern tam_op_x,letra,stbut,pos_x,pos_y,cor_fg;

void extern *area;
char extern msg_texto [],nome_arq [];
char sub_dir [30] = {"icones\\"};

#define QTD_SUB_IC 4
char menu_ed_icone [QTD_SUB_IC][5] = { {"COR"}, {"LER"}, {"GRV"}, {"SAI"} };

/*******************************************************************/

void faz_fundo_ed_ic ()
{

	unsigned tamanho;

	tamanho = imagesize (XT_TXT_IC,YT_TXT_IC,XB_TXT_IC,YB_TXT_IC);
	area = malloc (tamanho);

	getimage (XT_TXT_IC,YT_TXT_IC,XB_TXT_IC,YB_TXT_IC,area);

	setcolor (BROWN);                                      /* faz retangulos */
	setfillstyle (SOLID_FILL,BROWN);
	disable ();
	bar (XT_TXT_IC,YT_TXT_IC,XB_TXT_IC,YB_TXT_IC);
	enable ();
	setfillstyle (SOLID_FILL,CYAN);
	disable ();
	bar (XT_TXT_IC+4,YT_TXT_IC+4,XB_TXT_IC-4, YB_TXT_IC-4);
	enable ();
	setcolor (BLACK);
	rectangle (XT_TXT_IC+1,YT_TXT_IC+1,XB_TXT_IC-1,YB_TXT_IC-1);
}
/*==============================================*/
void tira_fundo_ed_ic ()
{
	putimage (XT_TXT_IC,YT_TXT_IC,area,COPY_PUT);
	free (area);
}
/*==============================================*/

void janela_texto_ed_ic ()         /*  trata area onde aparecera o texto */
{
	char c;

	do
	{
		faz_fundo_ed_ic ();
		outtextxy (XT_TXT_IC+56,YT_TXT_IC+20,"E novo ? (S/N)");
		c = getch ();
		if ((c == 'S') || (c == 's'))
		{
			setfillstyle (SOLID_FILL,CYAN);
			disable ();
			bar (XT_TXT_IC+4,YT_TXT_IC+4,XB_TXT_IC-4,YB_TXT_IC-4);
			enable ();
			outtextxy (XT_TXT_IC+5,YT_TXT_IC+8,"Dimensao horizontal:");
			le_texto ('X');
			dim_x = atoi (&msg_texto [0]);
			outtextxy (XT_TXT_IC+5,YT_TXT_IC+20,"Dimensao vertical:");
			le_texto ('Y');
			dim_y = atoi (&msg_texto [0]);

			outtextxy (XT_TXT_IC+40,YT_TXT_IC+32,"Confirma ? (S/N)");
			c = getch ();
			if ((dim_x > 40) || (dim_y > 30))
			{
				setfillstyle (SOLID_FILL,CYAN);
				disable ();
				bar (XT_TXT_IC+4,YT_TXT_IC+4,XB_TXT_IC-4, YB_TXT_IC-4);
				enable ();
				outtextxy (XT_TXT_IC+35,YT_TXT_IC+8,"DIMENSOES INVALIDAS !");
				outtextxy (XT_TXT_IC+10,YT_TXT_IC+20,"Horiz. <= 40 e Vert. <=30");
				outtextxy (XT_TXT_IC+20,YT_TXT_IC+32,"Tecle algo para voltar.");
				putch (07);
				c = getch ();
				c = 'N';
			}
			else
			{
				if ((dim_x == 0) || (dim_y == 0))
				{
					setfillstyle (SOLID_FILL,CYAN);
					disable ();
					bar (XT_TXT_IC+4,YT_TXT_IC+4,XB_TXT_IC-4, YB_TXT_IC-4);
					enable ();
					outtextxy (XT_TXT_IC+35,YT_TXT_IC+8,"DIMENSOES INVALIDAS !");
					outtextxy (XT_TXT_IC+15,YT_TXT_IC+20,"Existe um valor zerado !");
					outtextxy (XT_TXT_IC+20,YT_TXT_IC+32,"Tecle algo para voltar.");
					putch (07);
					c = getch ();
					c = 'N';
				}
			}
		}
		else
		{
			c = 'S';
		}
		tira_fundo_ed_ic ();

	}
	while ((c != 'S') && (c != 's'));
}

/*******************************************************************/

void faz_grade (int x,int y)
{

	tam_grade_x = x * GRADE_X;   /* identifica dimensao */
	tam_grade_y = y * GRADE_Y;

	vertice_dir_grade_x = VERT_X + tam_grade_x;
	vertice_dir_grade_y = VERT_Y + tam_grade_y;

	setcolor (WHITE);

	rectangle (VERT_X,VERT_Y,vertice_dir_grade_x,vertice_dir_grade_y); /* faz retangulo */

	for (passo_x = 1;passo_x < (dim_x); passo_x++)
		line (VERT_X+(passo_x*GRADE_X),VERT_Y,VERT_X+(passo_x*GRADE_X),vertice_dir_grade_y);
	for (passo_y = 1;passo_y < (dim_y); passo_y++)
		line (VERT_X,VERT_Y+(passo_y*GRADE_Y),vertice_dir_grade_x,VERT_Y+(passo_y*GRADE_Y));

}

/*******************************************************************/

void le_arq_icn ()
{
	int starq,x,y,color;
	long tam_arq;
	char *buff;

	ff ("icones\\*.ICN");
	if (nome_arq [0] != 0)
	{
		esconde_mouse ();
		sub_dir [7] = 0;
		strcat (sub_dir,nome_arq);
		starq = open (sub_dir,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
		tam_arq = filelength (starq);
		buff = malloc (tam_arq);
		read (starq,buff,tam_arq);
		close(starq);
		setfillstyle (SOLID_FILL,BLACK);
		disable ();
		bar (X_SIMB-1,Y_SIMB-1,X_SIMB+41,Y_SIMB+31);
		enable ();

		putimage (X_SIMB,Y_SIMB,buff,COPY_PUT);
		dim_x = buff[0];
		dim_y = buff[2];

		setfillstyle (SOLID_FILL,BLACK);
		disable ();
		bar (VERT_X,VERT_Y,VERT_X+(40*GRADE_X),VERT_Y+(30*GRADE_Y));
		enable ();

		faz_grade (dim_x, dim_y);

		for (x = 0;x < dim_x;x++)
		{
			for (y = 0;y < dim_y;y++)
			{
				color = getpixel (X_SIMB+x,Y_SIMB+y);
				setfillstyle (SOLID_FILL,color);
				disable ();
				bar (VERT_X + (x*GRADE_X)+1,
					 VERT_Y + (y*GRADE_Y)+1,
					 VERT_X + (x*GRADE_X) + GRADE_X-1,
					 VERT_Y + (y*GRADE_Y) + GRADE_Y-1);
				enable ();

			}
		}
		aparece_mouse ();
		free (area);
	}
}

/*******************************************************************/

void grava_icn ()
{
	void *icone;
	unsigned qtdbytes;
	int starq;
	struct ffblk ffblk;



	qtdbytes = imagesize (X_SIMB,Y_SIMB,X_SIMB+dim_x,Y_SIMB+dim_y);
	icone = malloc (qtdbytes);
	getimage (X_SIMB,Y_SIMB,X_SIMB+dim_x,Y_SIMB+dim_y,icone);

	faz_janela_nome ();
	if (nome_arq [0] != 0)
	{
		if (strlen(nome_arq) > 8)                               /* monta o nome com extensao EDT */
			strcpy (&nome_arq[8],".ICN");
		else
			strcat (nome_arq,".ICN");
		sub_dir [7] = 0;
		strcat (sub_dir,nome_arq);
		starq = findfirst (sub_dir,&ffblk,0);
		if (starq == -1)  /* arquivo de tela novo */
		{
			starq = open (sub_dir,O_RDWR|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
			write (starq,icone,qtdbytes);
			close (starq);
		}
		else               /* arquivo ja' existe */
		{
			if (sobrepoe())
			{
				starq = open (sub_dir,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
				write (starq,icone,qtdbytes);
//				chsize (starq,qtdbytes);  /* muda tamanho do arquivo pois houve alteracao */
				close (starq);
			}
		}
	}
	aparece_mouse ();
}

/*****************************************************************************/

void editar_icone ()
{

	int i,x,cl,ln,caso;

	esconde_mouse ();

	setfillstyle (SOLID_FILL,BLACK);
	disable ();
	bar (0,20,639,349);
	enable ();

	janela_texto_ed_ic ();

	if ((dim_x !=0) && (dim_y != 0))
		faz_grade (dim_x,dim_y);

	setfillstyle (SOLID_FILL,CYAN);
	disable ();                      /* desabilita e habilita qdo. for usar "bar" */
	bar (INI_TEC_X,INI_TEC_Y,FIM_TEC_X,FIM_TEC_Y);
	enable ();

	aparece_mouse ();

	settextstyle (SMALL_FONT,HORIZ_DIR,0);

	for (i = 0; i < 3 ; i++)
	{
		x = ((tam_op_x+7) * i)+INI_TEC_X+5;
		faz_tecla (x,INI_TEC_Y+3,x+tam_op_x,FIM_TEC_Y-3);  /* vertices das teclas */
		setcolor (BLACK);
		outtextxy (x+5,INI_TEC_Y+3+2,&menu_ed_icone [i][0]);
	}
	aparece_mouse ();
	settextstyle (DEFAULT_FONT,HORIZ_DIR,0);
	setcolor (cor_fg);
	get_buttom_pressed ();
	while ((stbut & 2) != 2)
	{
		if ( (pos_x > VERT_X) &&
			 (pos_x < vertice_dir_grade_x) &&
			 (pos_y > VERT_Y) &&
			 (pos_y < vertice_dir_grade_y) )
		{
			cl = (pos_x - VERT_X)/GRADE_X; /* identifica coluna */
			ln = (pos_y - VERT_Y)/GRADE_Y; /* identifica linha */

			setfillstyle (SOLID_FILL,cor_fg);           /* seta preenchimento */
			esconde_mouse ();
			disable ();
			bar (VERT_X + (cl*GRADE_X)+1,
				 VERT_Y + (ln*GRADE_Y)+1,
				 VERT_X + (cl*GRADE_X) + GRADE_X-1,
				 VERT_Y + (ln*GRADE_Y) + GRADE_Y-1);
			enable ();
			putpixel (X_SIMB+cl,Y_SIMB+ln,cor_fg);
			aparece_mouse ();
		}
		else
		{
			if ( (pos_x > INI_TEC_X) &&
				 (pos_x < FIM_TEC_X) &&
				 (pos_y > INI_TEC_Y) &&
				 (pos_y < FIM_TEC_Y) )
			{
				caso = (pos_x - INI_TEC_X)/(tam_op_x+7);
				switch (caso)
				{
					case 0: def_cor ();
							mostra_def ();
							break;

					case 1: le_arq_icn ();
							break;

					case 2: grava_icn ();
							break;
				}
			}
		}
		get_buttom_pressed ();
	}
	setfillstyle (SOLID_FILL,BLACK);
	disable ();
	bar (0,20,639,349);
	enable ();
	return;
}
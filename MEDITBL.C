

			   /****************************/
			   /*        MEDITBL.C         */
			   /* MODULO DE MANIPULACAO DE */
			   /*         BLOCOS           */
			   /****************************/


#include <graphics.h>
#include <alloc.h>

#define FALSE 0
#define TRUE 0xff


#define POS_XT_RET 137
#define POS_YT_RET 310
#define POS_XB_RET 514
#define POS_YB_RET 337


int extern cor_rasc,cor_bg,cor_fg,xt,xb,yt,yb,pos_x,pos_y,stbut;
char extern *menu[];
unsigned extern tamanho;

#define QTD_OPCOES_BL 3
#define POS_MENU_X  200
#define POS_MENU_Y   30

char menu_bloco [QTD_OPCOES_BL][15] = { {"MOVER"},{"COPIAR"},{"SAIR"} };

/*==============================================================*/
void bloco_extenso ()
{
	esconde_mouse ();
	faz_fundo ();
	outtextxy (POS_XT_RET+90,POS_YT_RET+11," Bloco muito extenso !");
	putch (0x07);
	aparece_mouse ();
	get_buttom_pressed ();
	get_mouse_release (0);
	get_mouse_release (1);
	esconde_mouse ();
	tira_fundo ();
	aparece_mouse ();
}

/*==============================================================*/

void move ()
{
	unsigned long size;
	void far     *b_janela,*b_antigo,*ptr_area;
	int      tamx,tamy,prim,antx,anty;
	int      saida;

	get_mouse_release (0);
	setcolor (cor_rasc);
	get_buttom_pressed ();
	while ((stbut & 2) != 2)
	{
		esconde_mouse ();
		get_mouse_release (0);
		saida = define_box ();
		aparece_mouse ();
		if (saida == 1)
		{
			get_mouse_release (0);
			size = imagesize (xt,yt,xb,yb);
			if (size < 50000)              // nao funciona com o depurador !!
			{
				setwritemode (XOR_PUT);
				esconde_mouse ();
				rectangle (xt,yt,xb,yb);
				tamx = xb - xt;
				tamy = yb - yt;
				b_janela = farmalloc (size);
				b_antigo = farmalloc (size);
				getimage (xt,yt,xb,yb,b_janela);			/* guarda area */
				rectangle (xt,yt,xb,yb);
				antx = anty = stbut = prim = 0;
				do
				{
					get_mouse_position ();
					if ((antx != pos_x) || (anty != pos_y) || (prim == 0))
					{
					   if (prim == 0)
						   prim = 1;
					   else
						   putimage (antx,anty,b_antigo,COPY_PUT);             /* restaura area antiga */
					   getimage (pos_x,pos_y,pos_x+tamx,pos_y+tamy,b_antigo);  /* guarda area nova */
					   putimage (pos_x,pos_y,b_janela,COPY_PUT);               /* escreve area a ser copiada */
					   antx = pos_x;                                           /* salva posicao anterior */
					   anty = pos_y;
					}
				}
				while (((stbut & 1) != 1) && ((stbut & 2) != 2));
				if ((stbut & 2) == 2)
				{
					 putimage (antx,anty,b_antigo,COPY_PUT);
					 putimage (xt,yt,b_janela,COPY_PUT);
					 rectangle (xt,yt,xb,yb);
					 get_mouse_release (1);
				}
				else
				{
					 apg_ret (xt,yt,xb,yb);
					 setwritemode (XOR_PUT);
					 putimage (pos_x,pos_y,b_janela,COPY_PUT);               /* escreve area a ser copiada */
					 rectangle (pos_x,pos_y,pos_x+tamx,pos_y+tamy);
					 get_mouse_release (0);
				}
				farfree (b_janela);                                                /* libera buffers */
				farfree (b_antigo);
				setwritemode (COPY_PUT);
				aparece_mouse ();
				get_buttom_pressed ();
			}
			else
			{
				bloco_extenso ();
				stbut |= 2;
			}
		}
	}
	setcolor (cor_fg);
}

/*=====================================================*/

void copia ()
{
	unsigned long size,tamx,tamy,resta;
	void  far     *b_janela,*b_antigo;
	int           prim,antx,anty,saida;

	get_mouse_release (0);
	setcolor (cor_rasc);
	get_buttom_pressed ();
	while ((stbut & 2) != 2)
	{
		esconde_mouse ();
		get_mouse_release (0);
		saida = define_box ();
		aparece_mouse ();
		if (saida == 1)   /* confirmou area p/manipulacao */
		{
			get_mouse_release (0);
			size = imagesize (xt,yt,xb,yb);
			if (size < 50000)
			{
				setwritemode (XOR_PUT);
				esconde_mouse ();
				rectangle (xt,yt,xb,yb);
				tamx = xb - xt;
				tamy = yb - yt;
				b_janela = farmalloc (size);  /* foreground */
				b_antigo = farmalloc (size);  /* background */
				getimage (xt,yt,xb,yb,b_janela);
				rectangle (xt,yt,xb,yb);
				aparece_mouse ();
				antx = anty = 0;
				do
				{
					get_mouse_release (0);
					esconde_mouse ();
					stbut = prim = 0;
					do
					{
						get_mouse_position ();
						if ((antx != pos_x) || (anty != pos_y) || (prim == 0))
						/* posicao e' diferente ou e' a 1a. vez */
						{
							if (prim == 0)
								prim = 1;
							else
								putimage (antx,anty,b_antigo,COPY_PUT);
							getimage (pos_x,pos_y,pos_x+tamx,pos_y+tamy,b_antigo);
							putimage (pos_x,pos_y,b_janela,COPY_PUT);
							antx = pos_x;
							anty = pos_y;
						}
					}
					while (((stbut & 1) != 1) && ((stbut & 2) != 2));
					if ((stbut & 2) != 2) /* desistiu, fica na mesma */
					{
						rectangle (antx,anty,antx+tamx,anty+tamy);
						get_mouse_release (0);
					}
					else  /* selecionou p/ onde vai a area copiada */
					{
						putimage (pos_x,pos_y,b_antigo,COPY_PUT);
						get_mouse_release (1);
					}
					aparece_mouse ();
					get_buttom_pressed ();
				}
				while ((stbut & 2) != 2);
				get_mouse_release (1);
				farfree (b_janela);
				farfree (b_antigo);
				get_buttom_pressed ();
			}
			else
			{
				bloco_extenso ();
				stbut |= 2; /* forca uma desistencia, para sair do while */
			}
		}
	}
}

/*========================================================================*/

void bloco ()
{
	int opcao,fim_bloco;

	fim_bloco = FALSE;
	do
	{
		get_mouse_release (0);
		for (opcao = 0;opcao < QTD_OPCOES_BL;opcao++)
			menu [opcao] = &menu_bloco [opcao][0];
		opcao = opcoes (POS_MENU_X,POS_MENU_Y,QTD_OPCOES_BL);
		switch (opcao)
		{
			case 0: move ();
					break;

			case 1: copia ();
					break;

			case 2: fim_bloco = TRUE;
					get_mouse_release (0);
					break;

			default: break;
		}
	}
    while (!fim_bloco);
}


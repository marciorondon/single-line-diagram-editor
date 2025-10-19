


		  /******************************/
	      /*          MEDLIN.C          */
	      /* MODULO DE EDICAO DE LINHAS */
	      /******************************/


#include <graphics.h>
#include <dos.h>

#define FALSE 0
#define TRUE 0xff
#define MOUSE 0x33
#define TAM_MAX_MENU 20

int extern pos_x,
	   pos_y,
	   stbut,
	   cor_rasc,
	   cor_fg,
	   cor_bg,
	   tipo_linha;

int fim_drag;


char *menu [TAM_MAX_MENU];

#define QTD_OPCOES_LIN 4
#define POS_MENU_X 40
#define POS_MENU_Y  30
char menu_linha [QTD_OPCOES_LIN][15] = { {"DRAG"},{"HORIZONTAL"},{"VERTICAL"},{"SAIR"} };


/*===========================================================================*/

void drag_lin ()

{
	int x0,y0,xant,yant;

	get_buttom_pressed ();
	while ((stbut & 2) != 2)  /* se nao pressionour botao direito */
	{
	   get_mouse_release (0);
	   esconde_mouse ();
	   do
	   {
		   setwritemode (XOR_PUT);		/* escreve sem apagar */
		   setcolor (cor_rasc);
		   xant = x0 = pos_x;                  /* marca ponto inicial */
		   yant = y0 = pos_y;
		   do
		   {
			   get_mouse_position ();
			   line (x0,y0,xant,yant);            /* desfaz linha anterior  */
			   line (x0,y0,pos_x,pos_y);          /* faz nova linha */
			   xant = pos_x;
			   yant = pos_y;
		   }
		   while (((stbut & 1) != 1) && ((stbut & 2) != 2));
		   if ((stbut & 2) == 2)
		   {
			  line (x0,y0,pos_x,pos_y);                /* apaga linha residual */
			  line (x0,y0,x0,y0);
			  setwritemode (COPY_PUT);
			  setcolor (cor_fg);
		   }
		   else
		   {
			  setwritemode (COPY_PUT);
			  setcolor (cor_fg);
			  setlinestyle (SOLID_LINE,0,tipo_linha);
			  line (x0,y0,pos_x,pos_y);
			  setlinestyle (SOLID_LINE,0,NORM_WIDTH);

		   }
	   }
	   while ((stbut & 2) != 2);
	   aparece_mouse ();
	   get_buttom_pressed ();
	}
}

/*==================================================================*/

void horiz_vert_lin (char flag)

{
	int x0,y0,xant,yant;

	get_buttom_pressed ();
	while ((stbut & 2) != 2)
	{
		get_mouse_release (0);
		esconde_mouse ();
		setwritemode (XOR_PUT);
		setcolor (cor_rasc);
		if (flag == 'H')
		{
			xant = x0 = pos_x;                   /* marca ponto inicial */
			y0 = pos_y;
			do
			{
				get_mouse_position ();
				line (x0,y0,xant,y0);              /* desfaz linha anterior  */
				line (x0,y0,pos_x,y0);             /* faz nova linha */
				xant = pos_x;
			}
			while (((stbut & 1) != 1) && ((stbut & 2) != 2));
			pos_y = y0;
		}
		else
		{
			yant = y0 = pos_y;
			x0 = pos_x;
			do
			{
				get_mouse_position ();
				line (x0,y0,x0,yant);              /* desfaz linha anterior  */
				line (x0,y0,x0,pos_y);             /* faz nova linha */
				yant = pos_y;
			}
			while (((stbut & 1) != 1) && ((stbut & 2) != 2));
			pos_x = x0;
		}
		if ((stbut & 2) == 2)
		{
			line (x0,y0,pos_x,pos_y);
			line (x0,y0,x0,y0);
			setwritemode (COPY_PUT);
			setcolor (cor_fg);
		}
		else
		{
			setwritemode (COPY_PUT);
			setcolor (cor_fg);
			setlinestyle (SOLID_LINE,0,tipo_linha);
			line (x0,y0,pos_x,pos_y);
			setlinestyle (SOLID_LINE,0,NORM_WIDTH);
		}
		aparece_mouse();
		get_buttom_pressed ();
	}
}

/*==================================================================*/

void linha ()
{
	int opcao,fim_linha;

	fim_linha = FALSE;
	do
	{
		get_mouse_release (0); 		                          /* espera soltar botao */
		for (opcao = 0;opcao < QTD_OPCOES_LIN;opcao++)            /* preenche texto para menu */
			menu [opcao] = &menu_linha[opcao][0];
		opcao = opcoes (POS_MENU_X,POS_MENU_Y,QTD_OPCOES_LIN);    /* escreve menu e aguarda opcao */
		switch (opcao)
		{

			case 0: drag_lin();
					break;

			case 1: horiz_vert_lin('H');
					break;

			case 2: horiz_vert_lin('V');
					break;

			case 3: fim_linha = TRUE;
					get_mouse_release (0);
					break;

			default: break;
		}
	}
	while (!fim_linha);
}


/*===============================================================================*/

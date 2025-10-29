

			/******************************/
			/*         MEDITPR.C          */
			/* MODULO PRINCIPAL DO EDITOR */
			/******************************/

#include <graphics.h>
#include <alloc.h>
#include <conio.h>
#include <dos.h>

#define INI_OP_Y 0
#define INI_OP_X 0
#define FIM_OP_Y 19
#define FIM_OP_X 639
#define NR_OP 16
#define TRUE 0xff
#define FALSE 0

int g_mode,
	g_driver,
	top_x,
	xt,
	yt,
	xb,
	yb,
	k,
	i,
	x,
	tam_op_x,
	opcao,
	cor_rasc,
	cor_fg,
	cor_bg,
	salvou,
	escolha,
	sai;

int extern pos_x,pos_y,stbut,letra,tipo_linha,direcao,
	   mult_x,mult_y,div_x,div_y,tp_small,op;
unsigned size;
void *b_temp,*b_janela;
char num[5];
char extern texto [];

char menu_principal [16][5] = { {"CIR"},
								{"LIN"},
								{"RET"},
								{"DEF"},
								{"APG"},
								{"BLC"},
								{"ARQ"},
								{"ICN"},
								{"TXT"},
								{"DIN"},
								{"IMP"},
								{"FIM"},
								{" "},
								{" "},
								{" "},
								{" "}  };


char msg_light [] = {"Light"};

int vet_tela [] = {0,0,639,0,639,349,0,349,0,0};

int vet0 [] = {150,40,182,40,105,100,205,100,190,115,55,115,150,40};
int vet1 [] = {50,120,185,120,90,201,69,190,140,135,30,135,50,120};
/*=================================================================*/

void tela_inicial ()
{

	cleardevice ();
	setcolor (GREEN);
	setfillstyle (INTERLEAVE_FILL,GREEN);
	fillpoly (5,vet_tela);
	setcolor (WHITE);
	setfillstyle (SOLID_FILL,WHITE);
	fillellipse (120,120,100,80);
	settextstyle (TRIPLEX_FONT,HORIZ_DIR,0);
	setusercharsize (4,1,4,1);
	outtextxy (270,50,msg_light);
	setcolor (GREEN);
	setfillstyle (INTERLEAVE_FILL,GREEN);
	setlinestyle (DOTTED_LINE,0,NORM_WIDTH);
	fillpoly (7,vet0);
	fillpoly (7,vet1);
	setlinestyle (SOLID_LINE,0,NORM_WIDTH);
	setcolor (WHITE);
	line (20,225,590,225);
	setlinestyle (SOLID_LINE,0,THICK_WIDTH);
	line (20,230,590,230);
	setusercharsize (1,1,1,1);
	outtextxy (25,250,"EDITOR DE DIAGRAMAS DE OPERACAO");
	settextstyle (DEFAULT_FONT,HORIZ_DIR,0);
	setlinestyle (SOLID_LINE,0,NORM_WIDTH);
	outtextxy (150,295,"Autores: Marcio Egydio S. Rondon - DTAO");
	outtextxy (2,340,"Versao 4.1");
	outtextxy (350,340,"Para iniciar, digite qualquer tecla.");
	getch ();
}

/*=================================================================*/

void mostra_def ()
{
	int x,y;

	y = (FIM_OP_Y - INI_OP_Y) / 2;
	x = ((tam_op_x+7) * 14) + 5;
	setfillstyle (SOLID_FILL,BLACK);
	bar(x+1,INI_OP_Y+3,x+tam_op_x-1,FIM_OP_Y-3);
	setcolor (cor_fg);
	setlinestyle (SOLID_LINE,0,tipo_linha);
	line (x+4,y,x+tam_op_x-4,y);
	setlinestyle (SOLID_LINE,0,NORM_WIDTH);
//	y =  (letra == SMALL_FONT)  ? 0 : 1;
	settextstyle (letra,HORIZ_DIR,0);         // o paramentro charsize (0) era o y da linha anterior
	x = ((tam_op_x+7) * 17) + 5;
	disable ();
	bar (x+1,INI_OP_Y+1,x+tam_op_x-1,FIM_OP_Y-1);
	enable ();
	if (letra == SMALL_FONT)
	{
	   op = 0;
	   setusercharsize (mult_x,div_x,mult_y,div_y);
	   switch ( tp_small )
	   {
		case 0:
			x += 12;
			y = 4;
			break;

		case 1:
			x += 12;
			y = 4;
			break;

		case 2:
			x += 12;
			y = 4;
			break;
	   }
	   settextstyle (letra,HORIZ_DIR,op);
	   outtextxy (x,y,texto);
	}
	else
	{
		x += 10;
		y = 0;
		setusercharsize (1,2,1,2);
		outtextxy (x,y,texto);
		setusercharsize (1,1,1,1);
	}
	settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
}

/*============================================================*/

int define_box () /* quadrado rascunho para desenho de circulo, etc.. */
{
   int difx,dify,x0,y0,xtan,ytan,xban,yban;
   int saida,cor_atual;

		cor_atual = getcolor ();
		setcolor (cor_rasc);
		setwritemode (XOR_PUT); /* modo escrita grafica para movimentacao - IMPORTANTE */
		xtan=xban=x0=pos_x;
		ytan=yban=y0=pos_y;
		do
		{                                       /* identifica quadrante */
		  difx = pos_x - x0;
		  dify = pos_y - y0;
		  if (difx >= 0)
		  {
			 xt = x0;
			 xb = pos_x;
		  }
		  else
		  {
			 xt = pos_x;
			 xb = x0;
		  }
		  if (dify >= 0)
		  {
			 yt = y0;
			 yb = pos_y;
		  }
		  else
		  {
			 yt = pos_y;
			 yb = y0;
		  }
		  rectangle(xtan,ytan,xban,yban);             /* desfaz quadrado anterior */
		  rectangle (xt,yt,xb,yb);                    /* traca quadrado novo */
		  xtan = xt;                                  /* guarda pontos anteriores */
		  ytan = yt;
		  xban = xb;
		  yban = yb;
		  get_mouse_position ();
		}
		while (((stbut & 1) != 1) && ((stbut & 2) != 2));
		rectangle (xt,yt,xb,yb); /* apaga retangulo rascunho e... */
		setwritemode (COPY_PUT); /* mudou o modo para desenhar */
		setcolor (cor_atual);
		saida = ((stbut & 1) == 1) ? 1 : 2;
		return (saida);  /* ret = 1, escreve ; ret = 2, desiste */
}

/*=================================================================*/

void apaga()
{
	int saida;

	get_mouse_release (0);
	get_buttom_pressed ();
	while ((stbut & 2) != 2)
	{
		get_mouse_release (0);
		esconde_mouse ();
		saida = define_box();
		if (saida == 1)   /* confirmou area p/delecao */
			apg_ret (xt,yt,xb,yb);
		aparece_mouse ();
		get_buttom_pressed ();
	}
	get_mouse_release (1);
}

/*=================================================================*/

void circ()
{

  int  x0,y0,ry,rx,saida;

  get_mouse_release (0);
  get_buttom_pressed ();
  while ((stbut & 2) != 2) /* se nao apertou botao direito */
  {
	   get_mouse_release (0);
	   esconde_mouse ();
	   saida = define_box ();
	   if (saida == 1)
	   {
		   x0 = (xt+xb)/2;  /* coordenadas do centro do circulo */
		   y0 = (yt+yb)/2;
		   rx  = (xb-xt)/2; /* raio */
		   ry  = (yb-yt)/2;
		   ellipse (x0,y0,0,360,rx,ry);
	   }
	   aparece_mouse ();
	   get_buttom_pressed  ();
  }
  get_mouse_release (1);
}

/*============================================================*/

int sel_opcao ()
{
	int flag;

	flag = 0;
	do
	{
		get_mouse_buttom (0);                         /* verifica se apertou botao 1 */
		if ((pos_y >= INI_OP_Y + 3) &&
			(pos_y <= FIM_OP_Y - 3) &&
			(pos_x >= 5) &&
			(pos_x <= ((tam_op_x + 7) * 12)))
		{
			disable ();
			escolha = (pos_x-5)/(tam_op_x+7);    /* identifica opcao */
			enable ();
			top_x = ((tam_op_x+7) * escolha) + 5; /* coord.x do vertice da tecla */
			esconde_mouse ();
			aperta_tecla (top_x,INI_OP_Y+3,top_x+tam_op_x,FIM_OP_Y-3,&b_temp); /* efeito visual da tecla */
			settextstyle (SMALL_FONT,HORIZ_DIR,0);
			setusercharsize (4,4,4,4);
			setfillstyle (SOLID_FILL,WHITE);
			disable ();
			bar (top_x+7,INI_OP_Y+3+2,top_x+7+16,INI_OP_Y+3+2+8); /* apaga nome da tecla pressionada */
			enable ();
			setcolor (LIGHTBLUE);
			outtextxy (top_x+8,INI_OP_Y+3+2,&menu_principal [escolha][0]); /* reescreve tecla com cor azul */
			setcolor (cor_fg);
			settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
			aparece_mouse ();  /* reescreve mouse */
			flag = 0xff;
		}
		get_mouse_release (0); /* garante que botao foi solto */
	}
	while (flag == 0);
	return (escolha);
}

/*=======================================================================*/

void dr_retan ()

{
	int saida;

	get_buttom_pressed ();
	while ((stbut & 2) != 2)
	{
		get_mouse_release (0);
		esconde_mouse ();
		saida = define_box ();
		if (saida == 1)
		   rectangle (xt,yt,xb,yb);
		aparece_mouse ();
		get_buttom_pressed ();
	}
	get_mouse_release (1);
}

/*==================================================================*/

void desfaz_op()
{
	esconde_mouse ();
	top_x = ((tam_op_x+7) * opcao) + 5;
	solta_tecla (top_x,INI_OP_Y+3,&b_temp);
	aparece_mouse ();
}

/*=======================================================================*/

void main ()
{

	g_mode = 1;
	g_driver = 3;
	if (registerbgidriver (EGAVGA_driver) < 0) exit (1);
	if (registerbgifont (sansserif_font) < 0) exit (1);
	if (registerbgifont (triplex_font) < 0) exit (1);
	if (registerbgifont (small_font) < 0) exit (1);

	initgraph (&g_driver,&g_mode," ");
	chk_mouse ();

	tela_inicial ();
	cor_rasc = WHITE;
	cor_bg = BLACK;
	salvou = FALSE;
	tam_op_x = 28;
	setbkcolor (cor_bg);
	clearviewport ();

	setfillstyle (SOLID_FILL,CYAN);
	disable ();                      /* desabilita e habilita qdo. for usar "bar" */
	bar (INI_OP_X,INI_OP_Y,FIM_OP_X,FIM_OP_Y);
	enable ();
	setcolor (BLACK);
	outtextxy (((tam_op_x+7) * 13),INI_OP_Y+6,"Pena:");
	outtextxy (((tam_op_x+7) * 16)-10,INI_OP_Y+6,"Letra:");
	letra = SMALL_FONT;
	mult_x = mult_y = div_x = div_y = 4;
	tp_small = 2;
	direcao = HORIZ_DIR;
	settextstyle (letra,HORIZ_DIR,0);
	for (i = 0; i < 12 ; i++)
	{
		x = ((tam_op_x+7) * i)+5;
		faz_tecla (x,INI_OP_Y+3,x+tam_op_x,FIM_OP_Y-3);  /* vertices das teclas */
		setcolor (BLACK);
		outtextxy (x+5,INI_OP_Y+3,&menu_principal [i][0]);
	}
	setcolor (cor_fg);
	aparece_mouse ();
	cor_fg = LIGHTGREEN;
	letra = SMALL_FONT;
	setcolor (cor_fg);
	mostra_def ();
	sai = FALSE;
	do
	{
		get_mouse_release (0);
		get_mouse_release (1);
		opcao = sel_opcao ();
		desloc_mouse_y (FIM_OP_Y+2,349);  /* limito desloc.y para atuar na area de desenho */
/*		posiciona_mouse (320,175);  */
		switch (opcao)
		{

			case 0: circ ();
					salvou = FALSE;
					break;

			case 1: linha ();
					salvou = FALSE;
					break;

			case 2: dr_retan ();
					salvou = FALSE;
					break;

			case 3: define ();
					break;

			case 4: apaga ();
					salvou = FALSE;
					break;

			case 5: bloco ();
					salvou = FALSE;
					break;

			case 6: tela ();
					break;

			case 7: icone ('E');
					salvou = FALSE;
					break;

			case 8: insere_texto ();
					salvou = FALSE;
					break;

			case 9: dinamic ();
					break;

			case 10:hardcopy ();
					break;

			case 11:sai = TRUE;
					break;

			default: break;
		}
		desfaz_op();  /* exibe tecla de forma normal */
		desloc_mouse_y (0,349);  /* volta faixa de desloc.y mouse para toda a tela */
	}
	while (!sai);
	esconde_mouse ();
	clearviewport();
	closegraph (); /* retorna … condi‡ao inicial do video, antes do init.. */
	exit (0);
}


		  /*********************************************/
		  /*                 MEDIDIN.C                 */
		  /* MODULO DE DEFINICAO DOS CAMPOS DINAMICOS  */
		  /*********************************************/


#include <alloc.h>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <dos.h>
#include <string.h>
#include <mem.h>

#include "jantxt.h"

#define FALSE 0
#define TRUE  0xff

#define POS_XT_RET 137
#define POS_YT_RET 310
#define POS_XB_RET 514
#define POS_YB_RET 337


#define TAM_ICONE 1434

/* menu da opcao ALT */

#define QTD_OPCOES_ALT 3
#define POS_ALT_X 360
#define POS_ALT_Y  30

char menu_alt [QTD_OPCOES_ALT][15] = { {"INCLUIR"},{"EXCLUIR"},{"SAIR"} };

/* menu da opcao DIN */

#define QTD_OPCOES_DIN 3
#define POS_DIN_X 360
#define POS_DIN_Y  30

#define QTD_OPCOES_VAL 3
#define POS_VAL_X POS_DIN_X
#define POS_VAL_Y POS_DIN_Y

char menu_val [QTD_OPCOES_VAL][15] = { {"TENSAO"}, {"ENERGIA"}, {"SAIR"} };
char menu_din [QTD_OPCOES_DIN][15] = { {"DIGITAL"},{"ANALOGICO"},{"SAIR"} };
void extern *menu [];
void extern *area;
unsigned extern tamanho;
char extern nome_arq [];
char *pont;

int extern pos_x,pos_y,stbut,salvou,cor_fg,teclou_esc;
int eixo_x,eixo_y,starq_dig,starq_ana,abandona_icn,
	y_ponto,x_ponto;

char nome_arq_dig [30],nome_arq_ana [30];
char msg_ponto [] = {"PONTO: "};
char ext_arq [] = {"."};
unsigned char extern facaf[],disjf[],facaf_p[],disjf_p[];

char nome_icn [10];
char tipo;

int est;

typedef struct
	{
		int ponto;
		int eixox;
		int eixoy;
		char tipo;
	} tela_dig;

tela_dig  ponto_dig;

char extern msg_texto[41],barra_o[12],barra_d[12];

typedef struct
{
	char numero [6];
	char nome [15];
} nome_numero;

// nome_numero extern dadflow [164];

typedef struct
{
	int tipo;
	int x;
	int y;
	char origem  [15];
	char destino [15];
	char unidade [5];
	int ckt;
} reg_ana;

reg_ana ponto_ana;

/*============================================================*/

void mostra_icone (char *nome,int x, int y)
{
	int h_arq;
	void *b_icone;
	unsigned size;

	size = TAM_ICONE;
	b_icone = malloc (size);				        /* reserva area para icone */
	h_arq = open (nome,O_RDONLY|O_BINARY,S_IREAD);	/* abre aquivo para leitura do icone */
	if (h_arq != -1)                        		/* verifica se houve erro */
		read (h_arq,b_icone,size);					/* le arquivo */
	else
	{
		printf ("Erro no disco = %d !",errno);			/* substituir por rotina trata erro no disco */
	}
	close (h_arq);
	esconde_mouse ();
	putimage (x,y,b_icone,COPY_PUT);
	aparece_mouse ();
	free (b_icone);
}

/*=============================================================*/

void esconde_dig ()
{
	int k;
	long tam_arq_dig;
	void *ptr_dig;
	tela_dig *ptr_aux_dig;

	strcpy (nome_arq_dig,nome_arq);
	pont = strstr (nome_arq_dig,ext_arq);
	*(pont+1) = 'd';
	*(pont+2) = 'i';
	*(pont+3) = 'g';
	starq_dig = open (nome_arq_dig,O_RDWR|O_BINARY|O_APPEND,S_IREAD|S_IWRITE);
	if (starq_dig != -1)
	{
		tam_arq_dig = filelength (starq_dig);
		ptr_dig = (tela_dig *) malloc (tam_arq_dig);
		read (starq_dig,ptr_dig,tam_arq_dig);
		lseek (starq_dig,0,SEEK_SET);
		k = 0;
		ptr_aux_dig = ptr_dig;
		do
		{
			switch (ptr_aux_dig -> tipo)
			{
				case 'F': putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,facaf,COPY_PUT);
						  break;

				case 'D': putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,disjf,COPY_PUT);
						  break;

				case '0': putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,facaf_p,COPY_PUT);
						  break;

				case '1': putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,disjf_p,COPY_PUT);
						  break;

				default:  break;
			}
			ptr_aux_dig += 1;
			k += sizeof (tela_dig);
		}
		while (k < tam_arq_dig);
		free (ptr_dig);
		close (starq_dig);
	}
}


/*==================================================================*/

void exclui_dig ()
{
	long tam_arq_dig,nbytes;
	int difx,dify,k;
	void *ptr_dig;
	tela_dig *ptr_aux_dig;

	get_buttom_pressed ();
	if ((stbut & 2) == 2)
		return;
	tam_arq_dig = filelength (starq_dig);
	ptr_dig = (tela_dig *) malloc (tam_arq_dig);
	read (starq_dig,ptr_dig,tam_arq_dig);
	lseek (starq_dig,0,SEEK_SET);
	k=0;
	ptr_aux_dig = ptr_dig;
	do  /* procura no arq. DIG o registro com o local clicado */
	{
		difx = pos_x - (ptr_aux_dig -> eixox);
		dify = pos_y - (ptr_aux_dig -> eixoy);
		ptr_aux_dig += 1;
		k += sizeof (tela_dig);
	}
	while (((difx < 0) || (difx > 20) || (dify < 0) || (dify > 10)) &&
			(k < tam_arq_dig));

	if ((difx >= 0) && (difx <= 20) && (dify >= 0) && (dify <= 10))  /* achou */
	{
		ptr_aux_dig -= 1;
		switch (ptr_aux_dig -> tipo)
		{
			case 'F':  /* faca */
				  esconde_mouse ();
				  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,facaf,COPY_PUT);
				  aparece_mouse ();
				  break;

			case 'D': /*disjuntor */
				  esconde_mouse ();
				  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,disjf,COPY_PUT);
				  aparece_mouse ();
				  break;

			case '0':                          /* faca pequena */
				  esconde_mouse ();
				  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,facaf_p,COPY_PUT);
				  aparece_mouse ();
				  break;

			case '1':                          /* disjuntor pequeno */
				  esconde_mouse ();
				  putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,disjf_p,COPY_PUT);
				  aparece_mouse ();
				  break;



			default:	break;
		}
		nbytes = tam_arq_dig - k;   /* decrementa tamanho pq. excluiu */
		if (nbytes != 0)            /* nao e'o ultimo - move os outros registros */
		{
			memmove (ptr_aux_dig,ptr_aux_dig+1,nbytes);
			write (starq_dig,ptr_dig,tam_arq_dig - sizeof (tela_dig));
		}
		chsize (starq_dig,tam_arq_dig - sizeof (tela_dig));
	}
	free (ptr_dig);
}

/*====================================================================*/

void exclui_din ()
{

/*  tenta abrir arquivo */

	strcpy (nome_arq_dig,nome_arq);
	pont = strstr (nome_arq_dig,ext_arq);
	*(pont+1) = 'd';
	*(pont+2) = 'i';
	*(pont+3) = 'g';
	starq_dig = open (nome_arq_dig,O_RDWR|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
	exclui_dig ();
	close (starq_dig);
}

/*====================================================================*/

void inclui_dig ()
{


/* 	seleciona icone */

	tipo = icone ('D');  /* indica p/ rotina 'icone', quem a chamou */
	if (!abandona_icn)
	{                        /* monta registro do arquivo com
								os campos digitais da tela */
		ponto_dig.ponto = est;
		ponto_dig.eixox = eixo_x;
		ponto_dig.eixoy = eixo_y;
		ponto_dig.tipo  = tipo;
							 /*  grava registro */
		write (starq_dig,&ponto_dig,sizeof (ponto_dig));
	}
}

/*===================================================================*/

void inclui_din ()
{

/*  tenta abrir arquivo */

	strcpy (nome_arq_dig,nome_arq);
	pont = strstr (nome_arq_dig,ext_arq);
	*(pont+1) = 'd';
	*(pont+2) = 'i';
	*(pont+3) = 'g';
	starq_dig = open (nome_arq_dig,O_RDWR|O_CREAT|O_BINARY|O_APPEND,S_IREAD|S_IWRITE);
	inclui_dig ();
	close (starq_dig);
}

/*====================================================================*/

void digital ()
{
	int opcao,fim_tela;

	get_mouse_release (0);
	fim_tela = FALSE;
	do
	{
		for (opcao = 0; opcao < QTD_OPCOES_ALT; opcao++)
			menu [opcao] = &menu_alt [opcao][0];
		opcao = opcoes (POS_ALT_X,POS_ALT_Y,QTD_OPCOES_ALT);
		switch (opcao)
		{
			case 0: inclui_din ();
					get_mouse_release (0);
					break;

			case 1: exclui_din ();
					get_mouse_release (0);
					break;

			case 2: fim_tela = TRUE;
					get_mouse_release (0);
					break;

			default: break;
		}
	}
	while (!fim_tela);
}

/*==================================================*/

void esconde_ana ()
{
	int k;
	long tam_arq_ana;
	void *ptr_ana;
	reg_ana *ptr_aux_ana;
	int compri,altura;

	strcpy (nome_arq_ana,nome_arq);
	pont = strstr (nome_arq_ana,ext_arq);
	*(pont+1) = 'A';
	*(pont+2) = 'N';
	*(pont+3) = 'A';
	starq_ana = open (nome_arq_ana,O_RDWR|O_BINARY|O_APPEND,S_IREAD|S_IWRITE);
	if (starq_ana != -1)
	{
		settextstyle (SMALL_FONT,HORIZ_DIR,0);
		altura = textheight ("+XXXX.X");
		compri = textwidth ("+XXXX.X");
		settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
		tam_arq_ana = filelength (starq_ana);
		ptr_ana = (reg_ana *) malloc (tam_arq_ana);
		read (starq_ana,ptr_ana,tam_arq_ana);
		lseek (starq_ana,0,SEEK_SET);
		k = 0;
		ptr_aux_ana = ptr_ana;
		esconde_mouse ();
		do
		{
			apg_ret (ptr_aux_ana->x,ptr_aux_ana->y,
					 ptr_aux_ana->x+compri,ptr_aux_ana->y+altura);
			ptr_aux_ana += 1;
			k += sizeof (reg_ana);
		}
		while (k < tam_arq_ana);
		aparece_mouse ();
		free (ptr_ana);
		close (starq_ana);
	}
}
/*==================================================*/
/*
int procura_indice (char *nome)
{
	int i,achou;

	achou = FALSE;
	for (i=0; i<165 ; i++)
	{
		if (strcmp(nome,&dadflow[i].nome) == 0)
		{
			achou = TRUE;
			return (i);
		}
	}
	if (!achou)
		return (0xFF);
}
*/
/*==================================================*/

void tensao ()
{
	char t;
	int indice;

	faz_retangulo ();
/*	do
	{
*/		do
		{
			setfillstyle (SOLID_FILL,CYAN);
			disable ();
			bar (TOPX+6,TOPY+6,BOTX-6,BOTY-6);
			enable ();
			outtextxy (TOPX+115,TOPY+8,"TENSAO");
			outtextxy (TOPX+56,TOPY+36,"BARRA :");
			le_texto ('V');                                   /* le numero da barra */
			if (msg_texto [0] != 0)
			{
				outtextxy (TOPX+90,TOPY+65,"CONFIRMA (S/N) ?");
				t = getch ();
			}
		}
		while (((t !='s') && (t !='S')) && (!teclou_esc));   /* enquanto nao confirma ou nao sai */
//		if (!teclou_esc)                     /* se nao abandonou entrada */
/*		{
			indice = procura_indice (msg_texto);
			if (indice == 0xFF)
			{
				disable ();
				bar (TOPX+6,TOPY+6,BOTX-6,BOTY-6);
				enable ();
				outtextxy (TOPX+70,TOPY+50,"NAO ENCONTRADO");
				putch (0x07);
				delay (500);
			}
		}
	}
	while ((indice == 0xFF) && (!teclou_esc));   */
	retira_retangulo ();
	if (!teclou_esc)
	{
		ponto_ana.tipo = 0;
		ponto_ana.x = x_ponto;
		ponto_ana.y = y_ponto;
		strcpy (ponto_ana.origem,msg_texto);
		strcpy (ponto_ana.unidade,"kV");
		write (starq_ana,&ponto_ana,sizeof (reg_ana));
	}
	else
	{
		settextstyle (SMALL_FONT,HORIZ_DIR,0);
		esconde_mouse ();
		apg_ret (x_ponto,y_ponto,x_ponto+textwidth ("+XXXX.X"),y_ponto+textheight ("+XXXX.X"));
		aparece_mouse ();
		settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
	}
}

/*==================================================*/

void energia ()
{
	char t,c,k;
	int indice_o,indice_d,geracao;

	geracao = FALSE;
/*	do
	{  */
		esconde_mouse ();
		faz_retangulo ();
		do
		{
			setfillstyle (SOLID_FILL,CYAN);
			disable ();
			bar (TOPX+6,TOPY+6,BOTX-6,BOTY-6);
			enable ();
			outtextxy (TOPX+115,TOPY+8,"ENERGIA");
			outtextxy (TOPX+30,TOPY+23,"Barra Origem :");
			outtextxy (TOPX+30,TOPY+33,"Barra Destino :");
			le_texto ('E');                                   /* le identificacao das barras */
			if (msg_texto [0] != 0)
			{
				do                                            /* identifica potencia ativa ou reativa */
				{   setcolor (RED);
					outtextxy (TOPX+80,TOPY+45,"A");
					setcolor (BLACK);
					outtextxy (TOPX+88,TOPY+45,"tiva");
					setcolor (RED);
					outtextxy (TOPX+160,TOPY+45,"R");
					setcolor (BLACK);
					outtextxy (TOPX+168,TOPY+45,"eativa");
					c = getch ();
				}
				while ((c != 'A') && (c != 'a') && (c != 'R') && (c != 'r') &&
					  (c != 0x1B));
				if (c == 0x1B)
					teclou_esc = TRUE;
				else
				{
					do                                    /* identifica circuito */
					{
						setcolor (BLACK);
						outtextxy (TOPX+30,TOPY+57,"Circuito:");
						setcolor (RED);
						outtextxy (TOPX+130,TOPY+57,"1");
						outtextxy (TOPX+150,TOPY+57,"2");
						outtextxy (TOPX+170,TOPY+57,"3");
						outtextxy (TOPX+190,TOPY+57,"4");
						outtextxy (TOPX+210,TOPY+57,"5");
						k = getch ();
					}
					while (((k < '1') || (k > '5')) &&
							(k != 0x1B));
					if (k == 0x1B)
						teclou_esc = TRUE;
					else
					{
						setcolor (BLACK);
						outtextxy (TOPX+90,TOPY+68,"CONFIRMA (S/N) ?");
						t = getch ();
					}
				}
			}
		}
		while (((t !='s') && (t !='S')) && (!teclou_esc));
/*		if (!teclou_esc)
		{
			indice_o = procura_indice (barra_o);
			if (indice_o == 0xFF)
			{
				disable ();
				bar (TOPX+6,TOPY+6,BOTX-6,BOTY-6);
				enable ();
				outtextxy (TOPX+120,TOPY+20,barra_o);
				outtextxy (TOPX+90,TOPY+35,"BARRA INEXISTENTE");
				putch (0x07);
				delay (1000);
			}
			if (barra_d != NULL)
			{
				indice_d = procura_indice (barra_d);
				if (indice_d == 0xFF)
				{
					disable ();
					bar (TOPX+6,TOPY+6,BOTX-6,BOTY-6);
					enable ();
					outtextxy (TOPX+120,TOPY+20,barra_d);
					outtextxy (TOPX+90,TOPY+35,"BARRA INEXISTENTE");
					putch (0x07);
					delay (1000);
				}
			}
			else
			{
				indice_d = 0;
				geracao = TRUE;
			}
		}   */
		if (barra_d == NULL)  /* se barra destino == NULL -> geracao */
			geracao = TRUE;
		retira_retangulo ();
		aparece_mouse ();
/*	}
	while (((indice_o == 0xFF) || (indice_d == 0xFF)) && (!teclou_esc));   */
	if (!teclou_esc)
	{
		ponto_ana.tipo = 1;
		ponto_ana.x = x_ponto;
		ponto_ana.y = y_ponto;
		strcpy (ponto_ana.origem,barra_o);
		if (geracao)
			ponto_ana.destino [0] = 0;
		else
			strcpy (ponto_ana.destino,barra_d);
		if ((c == 'A') || (c == 'a'))
			strcpy (ponto_ana.unidade,"MW");
		else
			strcpy (ponto_ana.unidade,"MVar");
		ponto_ana.ckt = k - 0x31;              // converte de ASCII para HEXA offset = 1  (PDG)
		write (starq_ana,&ponto_ana,sizeof (reg_ana));
	}
	else
	{
		settextstyle (SMALL_FONT,HORIZ_DIR,0);
		esconde_mouse ();
		apg_ret (x_ponto,y_ponto,x_ponto+textwidth ("+XXXX.X"),y_ponto+textheight ("+XXXX.X"));
		aparece_mouse ();
		settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
	}
}


/*==================================================*/

void inclui_ana ()
{
	void *b_texto,*b_fundo,*b_antigo;
	int altura,compri,prim,antx,anty,xt,yt,xb,yb,abandona,
		fim_tela,opcao;
	unsigned size;

	strcpy (nome_arq_ana,nome_arq);
	pont = strstr (nome_arq_ana,ext_arq);
	*(pont+1) = 'A';
	*(pont+2) = 'N';
	*(pont+3) = 'A';
	starq_ana = open (nome_arq_ana,O_RDWR|O_CREAT|O_BINARY|O_APPEND,S_IREAD|S_IWRITE);
	esconde_mouse ();
	abandona = FALSE;
	posiciona_mouse (0,50);
	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	altura = textheight ("+XXXX.X");
	compri = textwidth ("+XXXX.X");
	size = altura*compri;
	b_texto = malloc (size);
	b_fundo = malloc (size);
	xt = 0;
	yt = 50;
	xb = xt + compri;
	yb = yt + altura;
	setcolor (WHITE);
	getimage (xt,yt,xb,yb,b_fundo);
	apg_ret (xt,yt,xb,yb);
	outtextxy (xt,yt,"+XXXX.X");
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
				putimage (antx,anty,b_antigo,COPY_PUT);
			getimage (pos_x,pos_y,pos_x+compri,pos_y+altura,b_antigo);
			putimage (pos_x,pos_y,b_texto,COPY_PUT);
			antx = pos_x;
			anty = pos_y;
		}
	}
	while (((stbut & 1) != 1) && ((stbut & 2) != 2));
	if  ((stbut & 2) == 2)
	{
		get_mouse_release (1);
		putimage (antx,anty,b_antigo,COPY_PUT);
		abandona = TRUE;
	}
	else
		get_mouse_release (0);
	free (b_texto);
	free (b_antigo);
	settextstyle (DEFAULT_FONT,HORIZ_DIR,1);
	setwritemode (COPY_PUT);
	aparece_mouse ();
	if (!abandona)
	{
		x_ponto = pos_x;
		y_ponto = pos_y;
		fim_tela = FALSE;
		do
		{
			for (opcao = 0;opcao < QTD_OPCOES_VAL;opcao++)
				menu [opcao] = &menu_val [opcao][0];
			opcao = opcoes (POS_VAL_X,POS_VAL_Y,QTD_OPCOES_VAL);
			switch (opcao)
			{
				case 0: tensao ();
						fim_tela = TRUE;
						break;

				case 1: energia ();
						fim_tela = TRUE;
						break;

				case 2: fim_tela = TRUE;
			}
		}
		while (!fim_tela);
		close (starq_ana);
	}
}
/*==================================================*/

void exclui_ana ()
{
	long tam_arq_ana,nbytes;
	void *ptr_ana;
	int k;
	reg_ana *ptr_aux_ana;
	int difx,dify,compri,altura;

	strcpy (nome_arq_ana,nome_arq);
	pont = strstr (nome_arq_ana,ext_arq);
	*(pont+1) = 'A';
	*(pont+2) = 'N';
	*(pont+3) = 'A';
	starq_ana = open (nome_arq_ana,O_RDWR|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
	get_buttom_pressed ();
	if ((stbut & 2) == 2)
		return;
	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	altura = textheight ("+XXXX.X");
	compri = textwidth ("+XXXX.X");
	tam_arq_ana = filelength (starq_ana);
	ptr_ana = (reg_ana *) malloc (tam_arq_ana);
	read (starq_ana,ptr_ana,tam_arq_ana);
	lseek (starq_ana,0,SEEK_SET);
	k = 0;
	ptr_aux_ana = ptr_ana;
	do
	{
		difx = pos_x - (ptr_aux_ana->x);
		dify = pos_y - (ptr_aux_ana->y);
		ptr_aux_ana += 1;
		k += sizeof (reg_ana);
	}
	while (((difx < 0) || (difx > compri) || (dify < 0) || (dify > altura)) &&
			(k < tam_arq_ana));
	if ((difx >= 0) && (difx <= compri) && (dify >= 0) && (dify <= altura))
	{
		ptr_aux_ana -= 1;
		esconde_mouse ();
		apg_ret (ptr_aux_ana->x,ptr_aux_ana->y,
				 ptr_aux_ana->x+compri,ptr_aux_ana->y+altura);
		aparece_mouse ();

		nbytes = tam_arq_ana - k;
		if (nbytes != 0)                                    /* se for o ultimo so altera */
		{                                                   /* o tamanho do arquivo  */
			memmove (ptr_aux_ana,ptr_aux_ana + 1,nbytes);
			write (starq_ana,ptr_ana,tam_arq_ana-sizeof(reg_ana));
		}
		chsize (starq_ana,tam_arq_ana-sizeof(reg_ana));
	}
	close (starq_ana);
	free (ptr_ana);
	settextstyle  (DEFAULT_FONT,HORIZ_DIR,1);
}

/*==================================================*/

void analogico ()
{
	int opcao,fim_tela;

	get_mouse_release (0);
	fim_tela = FALSE;
	do
	{
		for (opcao = 0;opcao < QTD_OPCOES_ALT; opcao++)
			menu [opcao] = &menu_alt [opcao][0];
		opcao = opcoes (POS_ALT_X,POS_ALT_Y,QTD_OPCOES_ALT);
		switch (opcao)
		{
			case 0: inclui_ana ();
					get_mouse_release (0);
					break;

			case 1: exclui_ana ();
					get_mouse_release (0);
					break;

			case 2: fim_tela = TRUE;
					get_mouse_release (0);
					break;

			default: break;
		}
	}
	while (!fim_tela);
}
/*==================================================*/
void msg_arq_nsalvo ()
{
		faz_fundo ();
		outtextxy (POS_XT_RET+75,POS_YT_RET+11,"Arquivo nao foi salvo !");
		putch (0x07);
		get_buttom_pressed ();
		get_mouse_release (0);
		get_mouse_release (1);
		esconde_mouse ();
		tira_fundo ();
		aparece_mouse ();
}

/*==================================================*/
void dinamic ()
{
	int opcao,fim_tela;

	get_mouse_release (0);
	if (salvou)
	{
		fim_tela = FALSE;
		do
		{
			for (opcao = 0; opcao < QTD_OPCOES_DIN; opcao++)
				menu [opcao] = &menu_din [opcao][0];
			opcao = opcoes (POS_DIN_X,POS_DIN_Y,QTD_OPCOES_DIN);
			switch (opcao)
			{
				case 0: digital ();
						get_mouse_release (0);
						break;

				case 1: analogico ();
						get_mouse_release (0);
						break;

				case 2: fim_tela = TRUE;
						get_mouse_release (0);
						break;

				default: break;
			}
		}
		while (!fim_tela);
	}
	else
	{
		msg_arq_nsalvo ();
	}
}

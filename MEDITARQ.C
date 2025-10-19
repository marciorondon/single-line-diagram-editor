		  /******************************/
		  /*      MEDITARQ.C            */
		  /* MODULO DE TRAT. ARQUIVOS   */
		  /******************************/


#include <mem.h>
#include <dir.h>
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
#include <ctype.h>

#define TAM_NOME 9
#define MAX 16
#define TOPX 160
#define TOPY 68
#define BOTX 479
#define BOTY 131
#define HY BOTY-TOPY
#define QTD_H 4
#define QTD_V 4
#define XINIC 8
#define YINIC 4
#define DESLOC_V 16
#define DESLOC_H 80
#define X 2
#define Y 4

#define POS_XT_RET 200
#define POS_YT_RET 100
#define POS_XB_RET 510
#define POS_YB_RET 136

#define QTD_OPCOES_ARQ 3
#define POS_ARQ_X 240
#define POS_ARQ_Y  30

#define FALSE 0
#define TRUE  0xff
#define TAM_ICONE 1434


/* variaveis de ff() */

int box [] = {BOTX+1,TOPY-1,
			  BOTX+20,TOPY-1,
			  BOTX+20,BOTY+1,
			  BOTX+1,BOTY+1,
			  BOTX+1,BOTY-TOPY/2,
			  BOTX+20,BOTY-TOPY/2};

int seta_up [] = {	BOTX+4+6,TOPY+5+3,
					BOTX+4+6+4,TOPY+5+3+4,
					BOTX+4+6-4,TOPY+5+3+4,
					BOTX+4+6,TOPY+5+3};

int seta_down [] = {BOTX+4+6,TOPY+52,
					BOTX+4+6+3,TOPY+52-3,
					BOTX+4+6-3,TOPY+52-3,
					BOTX+4+6,TOPY+52};


typedef struct
	{
		unsigned int qtd;
		unsigned char valor;
	} st_basica;

st_basica registro;

int extern j,plano,salvou;
char extern far *ptr_video;

char *nomes;
int qtd_nomes,indice;
char ext[] = {".EDT"};

typedef struct
{
	int tipo;
	int x;
	int y;
	char origem [15];
	char destino [15];
	char unidade [5];
	int ckt;
} reg_ana;

/*---------------------*/

char menu_arq [QTD_OPCOES_ARQ][15] = { {"LER"}, {"SALVAR"},{"SAIR"} };
char extern *menu [];
char msg_gr_arq [] = {"Entre com o nome do arquivo (sem ext)"};
char msg_nome_arq [] = {"Nome:"};
char *area;
char nome_arq[20];
unsigned tamanho;

int extern pos_x,pos_y,stbut,cor_fg;
unsigned char extern trafo[],facad[],facaf[],disjd[],disjf[],facad_p[],disjd_p[];
typedef struct
	{
		int ponto;
		int eixox;
		int eixoy;
		char tipo;
	} tela_dig;


tela_dig *ptr_dig,*ptr_aux_dig;


tela_dig  extern ponto_dig;
long tam_arq_dig;



void dir_janela (int i)
{
	int cont,y,k;

	y=YINIC;
	cont = k = 0;
	while ((i<qtd_nomes) && (k<MAX))
	{
		if (cont < 4)
		{
			outtextxy (XINIC+(cont*DESLOC_H),y,&nomes[i*TAM_NOME]);
			cont++;
			i++;
			k++;
		}
		else
		{
			y+=DESLOC_V;
			cont=0;
		}
	}
}
/*==================================================================*/

void ordena ()
{
	int k,w;
	char nome_aux [9];

	for (k = 0;k < (qtd_nomes-1);k++)
	{
		for (w = k+1; w < qtd_nomes;w++)
		{
			if (strcmp (&nomes[k*TAM_NOME],&nomes[w*TAM_NOME]) > 0)
			{
			   strcpy (&nome_aux,&nomes[k*TAM_NOME]);
			   strcpy (&nomes[k*TAM_NOME],&nomes[w*TAM_NOME]);
			   strcpy (&nomes[w*TAM_NOME],&nome_aux);

			}
		}
	}
}

/*==================================================================*/


void ff(char *espec)
{


	int tam_opcao,tam_janela,tam_tecla;
	char *pnt,*buffer_opcao,*tecla,*janela;
	int qtd,inicio,fim,aux1,aux2,aux3,aux4,topo;
	int ref,refx,refy,ref_ant,refx_ant,refy_ant,ref_mouse;
	struct ffblk ffblk;
	int achou,ind,opcao,desistiu;
	void *b_seta;

	desistiu = FALSE;
	qtd_nomes = 0;									/* qtd de nomes encontrados */
	achou = findfirst (espec,&ffblk,0);             /* procura pelo primeiro arquivo */
	if (achou == 0)									/* se nao achou nao faz nada */
	{

		esconde_mouse();
		get_mouse_release(0);
		tam_janela = imagesize(TOPX-4,TOPY-3,BOTX+23,BOTY+3);	/* salva area da tela onde aparecera a janela */
		janela = malloc (tam_janela);
		getimage (TOPX-4,TOPY-3,BOTX+23,BOTY+3,janela);
		apg_ret (TOPX-4,TOPY-3,BOTX+23,BOTY+3);
		tam_opcao = imagesize (2,2,DESLOC_H-2,DESLOC_V-4);		/* reserva area para iluminar opcao corrente */
		buffer_opcao = malloc (tam_opcao);
		ind = 0;									/* posicao na matriz de nomes */
		qtd_nomes++;								/* qtd de nomes encontrados */
		nomes = malloc (TAM_NOME);  				/* reserva 9 posicoes para colocar nulo no final */
		pnt = strstr (ffblk.ff_name,".");      		/* procura pelo '.' no nome do arquivo */
		*pnt = NULL;                            	/* armazena nulo no lugar do '.' */
		qtd = pnt + 1 - ffblk.ff_name;         		/* qtd de bytes ate o nulo */
		strncpy (&nomes[ind],ffblk.ff_name,qtd);	/* copia esta qtd incluido o nulo */
		achou = findnext (&ffblk);			 		/* procura o proximo */
		while (achou == 0)
		{
			qtd_nomes++;							/* atualiza contadores  */
			ind+=TAM_NOME;
			nomes = realloc (nomes,ind+TAM_NOME);  		/* realoca mais area para armazenar o novo nome */
			pnt = strstr (ffblk.ff_name,".");  			/* procura pelo '.' no nome do arquivo */
			*pnt = NULL;
			qtd = pnt + 1 - ffblk.ff_name;     			/* qtd de bytes ate o nulo */
			strncpy (&nomes[ind],ffblk.ff_name,qtd);	/* copia esta qtd incluido o nulo */
			achou = findnext (&ffblk);          		/* procura o proximo */
		}

		ordena ();

		setcolor (BROWN);							/* desenha janela em marrom */
		setfillstyle (SOLID_FILL,BROWN);
		rectangle (TOPX-1,TOPY-1,BOTX+1,BOTY+1);
		rectangle (TOPX-4,TOPY-3,BOTX+23,BOTY+3);
		drawpoly (6,box);
		floodfill (BOTX+2,TOPY+2,BROWN);
		floodfill (BOTX+2,BOTY-2,BROWN);

		faz_tecla (BOTX+4,TOPY+5,BOTX+4+12,TOPY+5+15);

		faz_tecla (BOTX+4,TOPY+35+5,BOTX+4+12,TOPY+35+5+15);

		setcolor (BLACK);
		setfillstyle (SOLID_FILL,BLACK);						/* desenha seta para cima */
		fillpoly (4,seta_up);
		line (BOTX+4+6,TOPY+5+3+4,BOTX+4+6,TOPY+5+3+4+5);
		line (BOTX+4+5,TOPY+5+3+4,BOTX+4+5,TOPY+5+3+4+5);
		line (BOTX+4+7,TOPY+5+3+4,BOTX+4+7,TOPY+5+3+4+5);
		fillpoly (4,seta_down);									/* desenha seta para baixo */
		putpixel (BOTX+4+6,TOPY+52,BLACK);
		line (BOTX+4+6,TOPY+52-4,BOTX+4+6,TOPY+52-9);
		line (BOTX+4+5,TOPY+52-4,BOTX+4+5,TOPY+52-9);
		line (BOTX+4+7,TOPY+52-4,BOTX+4+7,TOPY+52-9);
		desloc_mouse_x (TOPX,BOTX+19);							/* prepara mouse */
		desloc_mouse_y (TOPY,BOTY);
		posiciona_mouse (TOPX,TOPY);
		setviewport (TOPX,TOPY,BOTX,BOTY,1);					/* abre viewport para apresentar a janela */
		clearviewport();
		setcolor (RED);
		setfillstyle (SOLID_FILL,CYAN);
		floodfill (0,0,BROWN);
		topo = ref_ant = refx_ant = refy_ant = ref_mouse = ind = 0;
		dir_janela(topo);							/* preenche janela com nomes dos arquivos */
		inicio = TRUE;
		do
		{
			get_mouse_position();
			refx = pos_x;
			refx -= TOPX;
			refx /= DESLOC_H;
			refy = pos_y;
			refy -= TOPY;
			refy /= DESLOC_V;
			if (pos_x > BOTX)						/* verifica se mouse esta na area das setas */
			{
				if (pos_y < (HY/2))
					ref = 16;						/* mouse na seta para cima */
				else
					ref = 17;                       /* mouse na seta para baixo */
			}
			else
				ref = (QTD_V*refy)+refx;            /* ref de 0 ate 15 = mouse em alguma posicao da janela de nomes */
			switch (ref)
			{
				case  0: case  1: case  2: case  3:     /* de 0 a 15 o tratamento e o mesmo */
				case  4: case  5: case  6: case  7:
				case  8: case  9: case 10: case 11:
				case 12: case 13: case 14: case 15:

							if ((ref_mouse == 16)||(ref_mouse == 17))     /* se retornou das teclas */
							{
								esconde_mouse();
								ref_mouse = 0;
							}
							if ((ref_ant != ref) || (inicio == TRUE))     				/* se nova posicao ou 									*/
							{                                               			/* inicio = true (primeira vez ou retornou das teclas) 	*/
								aux1 = DESLOC_H * refx_ant;                 			/* calcula posicoes para controlar o movimento na janela */
								aux2 = DESLOC_V * refy_ant;
								aux3 = DESLOC_H * refx;
								aux4 = DESLOC_V * refy;
								if (inicio != TRUE)
									putimage (aux1+2,aux2+2,buffer_opcao,COPY_PUT); 	/* apaga iluminacao da opcao anterior */
								else
									inicio = FALSE;
								getimage (aux3+2,aux4+2,aux3+DESLOC_H-2,aux4+DESLOC_V-4,buffer_opcao);	/* le area da opcao corrente */
								putimage (aux3+2,aux4+2,buffer_opcao,NOT_PUT);                          /* escreve mesma area com iluminacao */
								refx_ant = refx;														/* salva referencias */
								refy_ant = refy;
								ref_ant  = ref;
							}
							if ((stbut & 1) == 1)            		/* verifica se pressionou botao do mouse */
							{
								get_mouse_release(0);
								if ((topo+ref) < qtd_nomes)         /* se mouse esta em cima de um nome */
									fim=TRUE;
							}
							if ((stbut & 2) == 2)
							{
								get_mouse_release (1);
								fim = desistiu = TRUE;
							}
							break;

				case 16:    if ((ref_ant != ref) && (inicio == FALSE))       								/* se nova posicao e  */
							{                                                   							/* veio da janela 	  */
								putimage (DESLOC_H*refx_ant+2,DESLOC_V*refy_ant+2,buffer_opcao,COPY_PUT);   /* coloca ultima posicao da janela em condicao normal */
								aparece_mouse();															/* mostra mouse para selecionar tecla */
								inicio = TRUE;
								ref_mouse = ref;
							}
							if (((stbut & 1) == 1) && (topo != 0))	/* se pressionou botao e pode fazer scroll */
							{
								setviewport (0,0,639,349,1);							/* volta tela como era antes */
								esconde_mouse ();
								aperta_tecla (BOTX+4,TOPY+5,BOTX+4+12,TOPY+5+15,&b_seta);
								delay (190);
								solta_tecla (BOTX+4,TOPY+5,&b_seta);
								aparece_mouse ();
								setviewport (TOPX,TOPY,BOTX,BOTY,1);
								setcolor (CYAN); 					/* escreve mesma janela na cor do fundo */
								dir_janela (topo);
								setcolor (RED);						/* escreve janela 4 posicoes acima */
								topo -= 4;
								dir_janela (topo);
								get_mouse_release(0);

							}
							if ((stbut & 2) == 2)
							{
								get_mouse_release (1);
								esconde_mouse ();
								fim = desistiu = TRUE;
							}
							break;

				case 17:    if ((ref_ant != ref) && (inicio == FALSE))
							{
								putimage (DESLOC_H*refx_ant+2,DESLOC_V*refy_ant+2,buffer_opcao,COPY_PUT);
								aparece_mouse();
								inicio = TRUE;
								ref_mouse = ref;
							}
							if (((stbut & 1) == 1) && (topo+MAX < qtd_nomes))
							{
								setviewport (0,0,639,349,1);							/* volta tela como era antes */
								esconde_mouse ();
								aperta_tecla (BOTX+4,TOPY+35+5,BOTX+4+12,TOPY+35+5+15,&b_seta);
								delay (190);
								solta_tecla (BOTX+4,TOPY+35+5,&b_seta);
								aparece_mouse ();
                                setviewport (TOPX,TOPY,BOTX,BOTY,1);
								setcolor (CYAN);
								dir_janela (topo);
								setcolor (RED);
								topo += 4;
								dir_janela (topo);
								get_mouse_release (0);

							}
							if ((stbut & 2) == 2)
							{
								get_mouse_release (1);
								esconde_mouse ();
								fim = desistiu = TRUE;
							}
							break;
			}
		}
		while ( fim != TRUE);  								 	/* executa ate selecionar um nome */
		if (desistiu)
			nome_arq [0] = 0;
		else
		{
			strncpy (nome_arq,&nomes[(topo+ref)*(TAM_NOME)],TAM_NOME);    /* monta extensao do arquivo */
			espec = strstr (espec,".");
			strncat (nome_arq,espec,4);
		}
		free (nomes);											/* libera areas */
		free (buffer_opcao);
		setviewport (0,0,639,349,1);							/* volta tela como era antes */
		putimage (TOPX-4,TOPY-3,janela,COPY_PUT);
		free (janela);
		setcolor (cor_fg);
		aparece_mouse();
		desloc_mouse_x (0,639);
		desloc_mouse_y (0,349);
	}
}

/*=====================================================*/

void le_icone_dig (char *nome,int x, int y)
{
	int h_arq;
	void *b_icone;
	unsigned size;

	size = TAM_ICONE;
	b_icone = malloc (size);				        /* reserva area para icone */
	h_arq = open (nome,O_RDONLY|O_BINARY,S_IREAD);	/* abre aquivo para leitura do icone */
	if (h_arq != -1)                        		/* verifica se houve erro */
	{
		read (h_arq,b_icone,size);					/* le arquivo */
		close (h_arq);
		putimage (x,y,b_icone,COPY_PUT);
	}
	free (b_icone);
}

/*=========================================================*/

void mostra_ana (char *arquivo)
{
	int k,starq_ana;
	long tam_arq_ana;
	char c = '.';
	char *pont_nome;
	void *ptr_ana;
	char arq_ana [30];
	char ana[] = {"ANA"};
	reg_ana *ptr_aux_ana;

	strcpy (arq_ana,arquivo);
	pont_nome = strchr (arq_ana,c);
	strcpy ((pont_nome+1),ana);
	starq_ana = open (arq_ana,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
	tam_arq_ana = filelength (starq_ana);
	ptr_ana = (reg_ana *) malloc (tam_arq_ana);
	read (starq_ana,ptr_ana,tam_arq_ana);
	close (starq_ana);
	ptr_aux_ana = ptr_ana;
	settextstyle (SMALL_FONT,HORIZ_DIR,0);
	setcolor (WHITE);
	esconde_mouse ();
	for (k = 0;k < (tam_arq_ana/sizeof (reg_ana));k++)
	{
		outtextxy (ptr_aux_ana->x,ptr_aux_ana->y,"+XXXX.X");
		ptr_aux_ana += 1;
	}
	free (ptr_ana);
	aparece_mouse ();
}

/*=========================================================*/

void mostra_dig (char *arquivo)
{


	int k,starq_dig;
	char c = '.';
	char *pont_nome;
	char arq_dig[30];
	char dig[] = {"DIG"};

	strcpy (arq_dig,arquivo);
	pont_nome = strchr (arq_dig,c);
	strcpy ((pont_nome+1),dig);
	starq_dig = open (arq_dig,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
	tam_arq_dig = filelength (starq_dig);
	ptr_dig = (tela_dig *) malloc (tam_arq_dig);
	read (starq_dig,ptr_dig,tam_arq_dig);
	close (starq_dig);
	esconde_mouse ();
	ptr_aux_dig = ptr_dig;
	for (k = 0;k < (tam_arq_dig/sizeof (tela_dig));k++)
	{
/*		if (ptr_aux_dig -> tipo == 'F')
			putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,facad,COPY_PUT);
		else
					putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,disjd,COPY_PUT); */
		switch (ptr_aux_dig->tipo)
		{
		    case 'F':
			     putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,facad,COPY_PUT);
			     break;

			case 'D':
			     putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,disjd,COPY_PUT);
			     break;

			case '0':
				 putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,facad_p,COPY_PUT);
				 break;

			case '1':
				 putimage (ptr_aux_dig -> eixox,ptr_aux_dig -> eixoy,disjd_p,COPY_PUT);
				 break;


		}

		ptr_aux_dig += 1;
	}
	free (ptr_dig);
	aparece_mouse ();
}

/*====================================================================*/

void compac (int h_arq)
{

	int cont;
	unsigned char byte;

	tam_arq_dig = 0;
	for (plano = 0;plano < 4;plano++)
	{
		ptr_video = (unsigned char *)0xA0000640;       /* aponta para o inicio da tela */
		outport (0x3CE,04);                            /* seleciona plano */
		outport (0x3CF,plano);
		byte = *ptr_video;                             /* le dado */
		j  = cont = 1;
		while (j < 26400)                              /* enquanto nao acaba tela */
		{
			ptr_video++;                               /* proximo dado */
			if (byte == *ptr_video)                    /* se conteudo e' igual */
				cont++;                                /* incrementa contador */
			else
			{                                          /* diferente */
				registro.qtd = cont;                   /* grava registro no disco */
				registro.valor = byte;
				write (h_arq,&registro.qtd,sizeof(st_basica));
				tam_arq_dig += 1;
				cont = 1;
				byte = *ptr_video;                     /* prepara novo ciclo */
			}
			j++;
		}
		if (cont != 1)          /* se acabou a tela e nao gravou */
		{
			registro.qtd = cont;
			registro.valor = byte;
			write (h_arq,&registro.qtd,sizeof(st_basica));
			tam_arq_dig += 1;
		}
	}
}

/*============================================================*/

int sobrepoe ()
{

	int esc,sai;
	void *b_tec;

	tamanho = imagesize (TOPX,TOPY,BOTX,BOTY);
	area = malloc (tamanho);
	getimage (TOPX,TOPY,BOTX,BOTY,area);
	setcolor (BROWN);                                      /* faz retangulos */
	setfillstyle (SOLID_FILL,BROWN);
	disable ();
	bar (TOPX,TOPY,BOTX,BOTY);
	enable ();
	setfillstyle (SOLID_FILL,CYAN);
	disable ();
	bar (TOPX+4,TOPY+4,BOTX-4,BOTY-4);
	enable ();
	setcolor (BLACK);
	rectangle (TOPX+1,TOPY+1,BOTX-1,BOTY-1);
	outtextxy (TOPX+120,TOPY+10,&nome_arq[0]);
	outtextxy (TOPX+75,TOPY+25,"Arquivo ja existente !");

	faz_tecla (TOPX+40,TOPY+40,TOPX+40+76,TOPY+40+12);
	setcolor (BLACK);
	outtextxy (TOPX+40+5,TOPY+40+2,"Sobrepoe");

	faz_tecla (TOPX+200,TOPY+40,TOPX+200+76,TOPY+40+12);
	setcolor (BLACK);
	outtextxy (TOPX+200+8,TOPY+40+2,"Cancela");

	aparece_mouse ();
	sai = FALSE;
	do
	{
		get_mouse_buttom (0);
		if ((pos_x > TOPX+40) && (pos_x < TOPX+40+76) &&   /* identifica escolha */
		   (pos_y > TOPY+40) && (pos_y < TOPY+40+10))
		{
			esconde_mouse ();
			aperta_tecla (TOPX+40,TOPY+40,TOPX+40+76,TOPY+40+12,&b_tec);
			delay (190);
			solta_tecla (TOPX+40,TOPY+40,&b_tec);
			aparece_mouse ();
			esc = sai = TRUE;
		}
		else
		{
			if ((pos_x > TOPX+200) && (pos_x < TOPX+200+76) &&
			   (pos_y > TOPY+40) && (pos_y < TOPY+40+10))
			{
				esconde_mouse ();
				aperta_tecla (TOPX+200,TOPY+40,TOPX+200+76,TOPY+40+12,&b_tec);
				delay (190);
				solta_tecla (TOPX+200,TOPY+40,&b_tec);
				aparece_mouse ();
				sai = TRUE;
				esc = FALSE;
			}
		}
		get_mouse_release (0);
	}
	while (!sai);
	esconde_mouse ();
	putimage (TOPX,TOPY,area,COPY_PUT);
	free(area);
	return (esc);
}

/*===========================================================*/

void deleta (int *indice)
{
	int comp;

	if (*indice != 0)
	{
		(*indice)--;
		nome_arq [*indice] = 0;
		comp = textwidth (nome_arq);
		disable ();
		bar (POS_XT_RET+5+40+comp,POS_YT_RET+24,POS_XT_RET+5+40+comp+8,POS_YT_RET+32);
		enable ();
	}
	else
		putch (07);
}

/*============================================================*/

void le_nome ()
{

	int tecla,indx,sai;
	char c;

	indx = nome_arq[0] = sai = FALSE;
	do
	{
		tecla = getch ();
		if ((isalnum (tecla)) || (tecla == '_'))          /* || (tecla == 0x5C) || (tecla == 0x3A)) */
		{                                                 /*                \                  :    */
			c = toupper(tecla);                           /* se tecla alfanumerica */
			nome_arq [indx] = c;                          /* converte  para maiuscula */
			nome_arq [indx+1] = 0;                        /* guarda na string */
			indx++;
			outtextxy (POS_XT_RET+5+40,POS_YT_RET+24,&nome_arq[0]);
			if (indx == 20)
				sai = TRUE;
		}
		else
		{
			switch (tecla)
			{
				case 0x0D : nome_arq [indx] = 0;      /* Enter */
							sai = TRUE;
							break;

				case 0x1B : sai = TRUE;               /* ESC   */
							nome_arq [0] = 0;
							break;

				case 0x08 : deleta (&indx);                   /* Back Space */
							break;

				case 0:     tecla = getch ();
							switch (tecla)
							{
								case 0x53:                      /* Seta */
								case 0x4B: deleta (&indx);      /* Del  */
										   break;
							}
							break;

				default:    break;
			}
		}
	}
	while (!sai);
}
/*===========================================================*/
void limpa_teclado ()
{
	while (kbhit())
	{
		getch ();
	}
}
/*===========================================================*/

void faz_janela_nome ()
{
		tamanho = imagesize (POS_XT_RET,POS_YT_RET,POS_XB_RET,POS_YB_RET);   /*  trata area onde aparecera o menu */
		area = malloc (tamanho);

		getimage (POS_XT_RET,POS_YT_RET,POS_XB_RET,POS_YB_RET,area);

		setfillstyle (SOLID_FILL,BROWN);
		disable ();
		bar (POS_XT_RET,POS_YT_RET,POS_XB_RET,POS_YB_RET);
		enable ();
		setfillstyle (SOLID_FILL,CYAN);
		disable ();
		bar (POS_XT_RET+4,POS_YT_RET+4,POS_XB_RET-4,POS_YB_RET-4);
		enable ();
		setcolor (BLACK);
		rectangle (POS_XT_RET+1,POS_YT_RET+1,POS_XB_RET-1,POS_YB_RET-1);

		outtextxy (POS_XT_RET+5,POS_YT_RET+6,&msg_gr_arq [0]);
		outtextxy (POS_XT_RET+5,POS_YT_RET+24,&msg_nome_arq [0]);
		le_nome ();
		putimage (POS_XT_RET,POS_YT_RET,area,COPY_PUT);
		free (area);
}

/*============================================================*/

void grava_arq ()
{

	int starq,sai;
	struct ffblk ffblk;

	faz_janela_nome ();
	esconde_mouse ();
	sai = FALSE;
	do
	{

		if (nome_arq [0] != 0)
		{
			if (strlen(nome_arq) > 8)                               /* monta o nome com extensao EDT */
				strcpy (&nome_arq[8],ext);
			else
				strcat (nome_arq,ext);
			starq = findfirst (nome_arq,&ffblk,0);
			if (starq == -1)  /* arquivo de tela novo */
			{
				esconde_dig ();  /* esconde dinamicas p/ gravar somente o esqueleto */
				esconde_ana ();
				starq = open (nome_arq,O_RDWR|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
				compac (starq);
				sai = salvou = TRUE;
				close (starq);
				mostra_dig (nome_arq);  /* reexibe dinamicas */
				mostra_ana (nome_arq);

			}
			else  /* arquivo ja' existe */
			{
				if (sobrepoe())
				{
					esconde_dig ();
					esconde_ana ();
					starq = open (nome_arq,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
					compac (starq);
					chsize (starq,(sizeof (st_basica) * tam_arq_dig));  /* muda tamanho do arquivo pois houve alteracao */
					sai = salvou = TRUE;
					close (starq);
					mostra_dig (nome_arq);
					mostra_ana (nome_arq);
				}
			}
		}
		else
			sai = TRUE;
	}
	while (!sai);
	aparece_mouse ();
}

/*===============================================================*/
void descompac (char *arq)
{
	FILE *in;
	int i;

		in = fopen (arq,"rb");
		for (plano = 1;plano < 9;plano <<=1)
		{
			ptr_video = (unsigned char *)0xA0000640; /* aponta inicio da tela, inicio da mem.video */
			outportb (0x3C4,2);                      /* seleciona plano */
			outportb (0x3C5,plano);
			j = 0;
			while (j < 26400)
			{
				fread (&registro,3,1,in);
				for (i = 0;i < registro.qtd;i++)              /* escreve dado na tela  */
				{
					*(ptr_video+j) = registro.valor;
					j++;
				}
			}
		}
		fclose (in);
		outportb (0x3C4,2);
		outportb (0x3C5,0xFF);
}
/*===============================================================*/

void le_arq()
{



	ff("*.EDT");                                                   /* seleciona arquivo */
	if (nome_arq [0] != 0)
	{
		esconde_mouse ();
		descompac (nome_arq);
		salvou = TRUE;
	}
	aparece_mouse ();
}

/*===============================================================*/

void tela ()
{
	int opcao,fim_tela;

		get_mouse_release (0);
		for (opcao = 0; opcao < QTD_OPCOES_ARQ; opcao++)
			menu [opcao] = &menu_arq [opcao][0];
		opcao = opcoes (POS_ARQ_X,POS_ARQ_Y,QTD_OPCOES_ARQ);
		switch (opcao)
		{
			case 0: le_arq();
					mostra_dig (nome_arq);
					mostra_ana (nome_arq);
					break;

			case 1: grava_arq ();
					break;

			case 2: get_mouse_release (0);
					break;

			default: break;
		}
}




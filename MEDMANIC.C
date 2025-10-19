
						/*************************/
						/*      MEDMANIC.C       */
						/* MODULO DE MANIPULACAO */
						/*      DE ICONES        */
						/*************************/
#include <dos.h>
#include <alloc.h>
#include <graphics.h>
#include <fcntl.h>
#include <sys\stat.h>

#define TRUE  0xFF
#define FALSE 0

#define TAM_ICONE  1434

#define QTD_OPCOES_DIN  3
#define QTD_OPCOES_IC  4
#define QTD_OPCOES_ED_IC 3
#define POS_MENU_X    280
#define POS_MENU_Y     30

char menu_ed_ic [QTD_OPCOES_ED_IC][8] = { {"LER"},
										  {"EDITAR"},
										  {"SAIR"} };

char menu_icone [QTD_OPCOES_IC][15] = { {"SECC"},
										{"DISJ"},
										{"OUTROS"},
										{"SAIR"} };


char menu_dig [QTD_OPCOES_DIN][15] = { {"SECC"},
									   {"DISJ"},
									   {"SAI"} };
char extern *menu[];
int tam_ic_x, tam_ic_y;

int extern pos_x,pos_y,stbut,abandona_icn,salvou;
int extern errno;
unsigned char extern trafo[],facad[],facaf[],disjd[],disjf[],cabo1[],cabo2[],
					 capa[],deltay[],tc[],tp[],ydelta[],terra[],chfusi[],deltadel[],
					 disjext[],fusiv[],trafot[],facad_p[],facaf_p[],disjd_p[],disjf_p[];

/*-----------------------------------------------*/
/*	variaveis  usadas  para  saber a posicao do  */
/*	mouse quando for definido um campo dinamico  */
/*-----------------------------------------------*/
int extern eixo_x,eixo_y;
char extern nome_arq [];
char extern sub_dir [];

/*===============================================================*/
void le_icone (unsigned char *b_icone)
{
	int antx,anty,prim;
	void *b_antigo;
	unsigned quad;


	get_mouse_release (0);					/* espera soltar botao */
	esconde_mouse ();
	posiciona_mouse (500,290);
	antx = 590;
	anty = 390;
	quad = imagesize (pos_x,pos_y,pos_x+tam_ic_x,pos_y+tam_ic_y);
	b_antigo = malloc (quad);
	setwritemode (XOR_PUT);
	abandona_icn = stbut = prim = FALSE;
	do
	{
		get_mouse_position ();
		if ((antx != pos_x) || (anty != pos_y) || (prim == 0))
		{
			if (prim ==  0)
				prim = 1;
			else
				putimage (antx,anty,b_antigo,COPY_PUT);
			getimage (pos_x,pos_y,pos_x+tam_ic_x,pos_y+tam_ic_y,b_antigo);
			putimage (pos_x,pos_y,b_icone,COPY_PUT);
			antx = eixo_x = pos_x;
			anty = eixo_y = pos_y;
		}
	}
	while (((stbut & 1) != 1) && ((stbut & 2) != 2));
	if ((stbut & 2) == 2)
	{
		get_mouse_release (1);
		putimage (antx,anty,b_antigo,COPY_PUT);
		abandona_icn = TRUE;
	}
	else
		get_mouse_release (0);
	setwritemode (COPY_PUT);
	free (b_antigo);
	aparece_mouse ();
}

/*=====================================================================*/

char icone (char origem)
{
	int opcao,starq;
	unsigned int tam_arq;
	char *buff;

	get_mouse_release (0);
	if (origem == 'D')
	{
		for (opcao = 0;opcao < QTD_OPCOES_DIN;opcao++)
			menu [opcao] = &menu_dig [opcao][0];
		opcao = opcoes (POS_MENU_X,POS_MENU_Y,QTD_OPCOES_DIN);
	}
	else
	{
		for (opcao = 0;opcao < QTD_OPCOES_ED_IC;opcao++)
			menu [opcao] = &menu_ed_ic [opcao][0];
		opcao = opcoes (POS_MENU_X,POS_MENU_Y,QTD_OPCOES_ED_IC);
	}
	if (origem != 'D')
	{
		switch (opcao)
		{
			case 0 : for (opcao = 0;opcao < QTD_OPCOES_IC;opcao++)                        /* LER - pula para o else */
						menu [opcao] = &menu_icone [opcao][0];
					opcao = opcoes (POS_MENU_X,POS_MENU_Y,QTD_OPCOES_IC);
					break;


			case 1: esconde_mouse ();                                                     /* EDITAR o icone */
					starq = open ("TELA.TMP",O_RDWR|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
					compac (starq);
					close (starq);
					editar_icone ();
					esconde_mouse ();
//					starq = open ("TELA.TMP",O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
					descompac ("TELA.TMP");                                                    /* descompacta os dados */
//					close (starq);
					remove ("TELA.TMP");
					aparece_mouse ();

			case 2: opcao = 0xFF;                                                         /* SAI */

			default:break;
		}
	}
	if (opcao != 0xFF)
	{
		switch (opcao)
		{
				case 0:	tam_ic_x = 9;						   /* Seccionadora */
						tam_ic_y = 5;
						desloc_mouse_x (0,623);
						if (origem == 'D')
							le_icone (facad_p);
						else
							le_icone (facaf_p);
						origem = '0';                         /* sec. pequena */
						desloc_mouse_x (0,639);
						break;

				case 1:	tam_ic_x = 8;                               /* Disjuntor */
						tam_ic_y = 6;
						desloc_mouse_x (0,619);
						if (origem == 'D')
							le_icone (disjd_p);
						else
							le_icone (disjf_p);
						desloc_mouse_x (0,639);
						origem = '1';                              /* Disjuntor pequeno */
						break;

				case 2: ff ("icones\\*.ICN");                        /* Outros  */
						if (nome_arq [0] != 0)
						{
							sub_dir [7] = 0;
							strcat (sub_dir,nome_arq);
							starq = open (sub_dir,O_RDWR|O_BINARY,S_IREAD|S_IWRITE);
							tam_arq = filelength (starq);
							buff = malloc (tam_arq);
							read (starq,buff,tam_arq);
							close (starq);
							tam_ic_x = buff [0];
							tam_ic_y = buff [2];
							desloc_mouse_x (0,639-tam_ic_x);
							desloc_mouse_y (20,349-tam_ic_y);
							le_icone (buff);
							desloc_mouse_x (0,639);
							desloc_mouse_y (0,349);
							free (buff);
						}
						break;

				case 3:                                              /* SAI */

				default:break;
		}
	}
	return (origem);
}

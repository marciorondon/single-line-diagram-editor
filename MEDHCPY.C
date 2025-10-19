


					/**********************/
					/*       MEDHCPY      */
					/* MODULO DE HARDCOPY */
					/**********************/



#include <dos.h>
#include <graphics.h>

#define POS_XT_RET 137
#define POS_YT_RET 310
#define POS_XB_RET 514
#define POS_YB_RET 337

int col,lin,plano,j,a;
char far *ptr_video;
char bufimp;

/*================================*/

void hardcopy ()   /* espec'ifico p/impressora Epson 8 pinos */
{
	int k;

	if (((get_printer_status () & 0xFF00) == 0x9000) ||
		((get_printer_status () & 0xFF00) == 0x1000))
	{
		esconde_mouse ();
		ptr_video = (unsigned char *)0xA0000000;    /* aponta inicio da tela */
		biosprint (1,0,0);
		biosprint (0,0x1B,0);					    /* ESC */
		biosprint (0,0x41,0);					    /* A   */
		biosprint (0,0x08,0);					    /* 08  */
		for (col = 0; col < 80; col++)
		{   										/* programa impressora */
			biosprint (0,0x1B,0);					/* ESC */
			biosprint (0,0x5A,0);					/* Z - Densidade Quadrupla - Emilia PC */
//			biosprint (0,0x4C,0);                   /* L - Alice */
			biosprint (0,0x28,0);                   /* n1 = 40 */
			biosprint (0,0x05,0);                   /* n2 = 5  */
			for (lin = 0;lin < 330;lin++)   /* 350 -20 linhas -> nao imprime menu */
			{
				j = ((350-lin) * 80) + col;         /* aponta byte */
				bufimp = 0;
				for (plano = 0;plano < 4;plano++)
				{
					outportb (0x3CE,04);            /* programa placa EGA/VGA */
					outportb (0x3CF,plano);         /* seleciona plano de leitura */
					bufimp |= *(ptr_video + j);     /* prepara agulhas para impressora */
				}
				for (k = 0; k < 4;k++)              /* imprime 4 vezes */
					biosprint (0,bufimp,0);			/* impressao quadrupla */
			}
			biosprint (0,0x0a,0);                   /* line feed */
		}
		biosprint (0,0x0C,0);                       /* form feed */
		aparece_mouse ();
	}
	else
	{
		esconde_mouse ();
		faz_fundo ();
		outtextxy (POS_XT_RET+90,POS_YT_RET+11,"Impressora com problemas !");
		putch (0x07);
		aparece_mouse ();
		get_buttom_pressed ();
		get_mouse_release (0);
		get_mouse_release (1);
		esconde_mouse ();
		tira_fundo ();
		aparece_mouse ();
	}
}

/*===============================================*/


void hcopy_manual ()
{
	int k;

	ptr_video = (unsigned char *)0xA0000000;    /* aponta inicio da tela */
	biosprint (1,0,0);
	biosprint (0,0x1B,0);					    /* ESC */
	biosprint (0,0x41,0);					    /* A */
	biosprint (0,0x08,0);					    /* 08 */
	for (col = 0; col < 80; col++)
	{   										/* programa impressora */
		biosprint (0,0x1B,0);					/* ESC */
		biosprint (0,0x5A,0);					/* Z - Densidade Quadrupla */
		biosprint (0,0x78,0);                   /* n1 = 120 */
		biosprint (0,0x05,0);                   /* n2 = 5  */
		for (lin = 0;lin < 350;lin++)
		{
			j = ((349-lin) * 80) + col;         /* aponta byte */
			bufimp = 0;
			for (plano = 0;plano < 4;plano++)
			{
				outportb (0x3CE,04);            /* programa placa EGA */
				outportb (0x3CF,plano);         /* seleciona plano de leitura */
				bufimp = *(ptr_video + j);     /* prepara agulhas para impressora */
				biosprint (0,bufimp,0);
			}
		}
		biosprint (0,0x0a,0);                   /* line feed */
	}
	biosprint (0,0x0C,0);                       /* form feed */
	aparece_mouse ();
}


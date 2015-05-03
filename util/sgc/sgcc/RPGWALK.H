#ifndef _INC_RPGWALK_

#define _INC_RPGWALK_

#define RWK_FC_UP 0
#define RWK_FC_DOWN 1
#define RWK_FC_LEFT 2
#define RWK_FC_RIGHT 3

typedef struct{
	int pos_x, pos_y; /* Coordenadas atuais (em pixels) */
	uint blk_x, blk_y; /* Coordenadas atuais (em blocos) */
	int step_x, step_y; /* Caminhando em que dire‡Æo? A que velocidade? */
	uint stepcnt;
	uint walkdelay, walkdlyctrl;
    uchar facing;
	uint sprnum, palnum;
	uint basetile, baseframe, frame;
    uchar frmctrl, frmdly;
}walker;

void init_walker(wlk,basetile);
void handle_walker(wlk);

void walker_walkto(walker,direction,blockcnt);

#endif

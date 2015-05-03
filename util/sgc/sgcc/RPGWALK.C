#include "genesis.h"
#include "utils.h"
#include "rpgwalk.h"

void init_walker(wlk,basetile)
walker *wlk;
uint basetile;
{
	wlk->pos_x = 0;
	wlk->pos_y = 0;

	wlk->blk_x = 0;
	wlk->blk_y = 0;

	wlk->step_x = 0;
	wlk->step_y = 1;

	wlk->stepcnt = 64;

	wlk->walkdelay = 1;
	wlk->walkdlyctrl = 1;

	wlk->facing = RWK_FC_DOWN;

	wlk->sprnum = 1;
	wlk->palnum = 1;

	wlk->basetile = basetile;
	wlk->baseframe = basetile;	
	wlk->frame = 0;

	wlk->frmctrl = 10;
	wlk->frmdly =  10;
}

void handle_walker(wlk)
walker *wlk;
{
	if(wlk->stepcnt){
		if(!wlk->walkdlyctrl){
			wlk->pos_x += wlk->step_x;
			wlk->pos_y += wlk->step_y;

			wlk->stepcnt--;
			wlk->walkdlyctrl = wlk->walkdelay;
		}else{
			wlk->walkdlyctrl--;
		}

	}
}

uint frame_step_mapping[]={
0x0000,
0x0008,
0x0000,
0x0010
};

uint frame_dir_mapping[]={
0x0048,
0x0000,
0x0018,
0x0030
};

void draw_walker(wlk)
walker *wlk;
{
	def_sprite(wlk->sprnum,wlk->pos_x,wlk->pos_y,0x0700,
			   BUILD_ATTR(wlk->palnum,0,0,0)|(wlk->baseframe+frame_step_mapping[wlk->frame]));

	if(wlk->stepcnt){
		if(!wlk->frmctrl){
			wlk->frame++;
			if(wlk->frame == 4){
				wlk->frame = 0;
			}

			wlk->frmctrl = wlk->frmdly;
		}else{
			wlk->frmctrl--;
		}
	}else{
		wlk->frame = 0;
	}
}

void walker_checkbaseframe(wlk)
walker *wlk;
{
	wlk->baseframe = wlk->basetile + frame_dir_mapping[wlk->facing];
}

void walker_walkto(wlk,direction,blockcnt)
walker *wlk;
uint direction;
uint blockcnt;
{
	switch(direction){
		case RWK_FC_UP:
			wlk->facing = direction;

			wlk->step_x = 0;
			wlk->step_y = -1;

			wlk->stepcnt = 16*blockcnt;

			walker_checkbaseframe(wlk);
		break;
		case RWK_FC_DOWN:
			wlk->facing = direction;

			wlk->step_x = 0;
			wlk->step_y = 1;

			wlk->stepcnt = 16*blockcnt;

			walker_checkbaseframe(wlk);
		break;
		case RWK_FC_LEFT:
			wlk->facing = direction;

			wlk->step_x = -1;
			wlk->step_y = 0;

			wlk->stepcnt = 16*blockcnt;

			walker_checkbaseframe(wlk);
		break;
		case RWK_FC_RIGHT:
			wlk->facing = direction;

			wlk->step_x = 1;
			wlk->step_y = 0;

			wlk->stepcnt = 16*blockcnt;

			walker_checkbaseframe(wlk);
		break;
	}
}
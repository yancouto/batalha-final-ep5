#include "../robot_fight.h"

static Direction look_dir;

int valid(Position p, int m, int n) {
	return p.x >= 0 && p.x < m && p.y >= 0 && p.y < n;
}

void prepareGame(Grid *g, Position p) {
	int i;
	look_dir = 0;
	for(i = 0; i < 6; i++) {
		Position s = getNeighbor(p, i);
		while(valid(s, g->m, g->n)) {
			if(g->map[s.x][s.y].type == ROBOT) {
				look_dir = i;
				return;
			}
			s = getNeighbor(s, i);
		}
	}
}

Action processTurn(Grid *g, Position p) {
	Robot *r = &g->map[p.x][p.y].object.robot;
	if(r->dir != look_dir)
		return TURN_RIGHT;
	else if(r->bullets > 0)
		return SHOOT_CENTER;
	else
		return STAND;
}
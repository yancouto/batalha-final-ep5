#include "../robot_fight.h"

static Direction lookDir;
static char charging;

int valid(Position p, int m, int n) {
	return p.x >= 0 && p.x < m && p.y >= 0 && p.y < n;
}

void prepareGame(Grid *g, Position p, int turnCount) {
	charging = 0;
}

Action bestTurn(Direction from, Direction to) {
	if(((6 + from - to) % 6) < 3) return TURN_LEFT;
	else return TURN_RIGHT;
}

char findControl(Grid *g, Position p) {
	int i;
	lookDir = 0;
	for(i = 0; i < 6; i++) {
		Position s = getNeighbor(p, i);
		while(valid(s, g->m, g->n)) {
			if(g->map[s.x][s.y].type == NONE && g->map[s.x][s.y].isControlPoint) {
				lookDir = i;
				return 1;
			}
			s = getNeighbor(s, i);
		}
	}
	return 0;
}

char find(Grid *g, Position p, TileType t) {
	int i;
	lookDir = 0;
	for(i = 0; i < 6; i++) {
		Position s = getNeighbor(p, i);
		while(valid(s, g->m, g->n)) {
			if(g->map[s.x][s.y].type == t) {
				lookDir = i;
				return 1;
			}
			s = getNeighbor(s, i);
		}
	}
	return 0;
}

Action processTurn(Grid *g, Position p, int turnsLeft) {
	Robot *r = &g->map[p.x][p.y].object.robot;
	if(r->bullets == 0 || charging) {
		charging = 1;
		if(g->map[p.x][p.y].isControlPoint) {
			if(r->bullets > 30)
				charging = 0;
			return STAND;
		}
		if(findControl(g, p)) {
			if(lookDir == r->dir)
				return WALK;
			else
				return bestTurn(r->dir, lookDir);	
		}
		else if(valid(getNeighbor(p, r->dir), g->m, g->n))
			return WALK;
		else
			return TURN_LEFT;
	}
	else if(find(g, p, ROBOT)) {
		if(lookDir == r->dir)
			return SHOOT_CENTER;
		else
			return bestTurn(r->dir, lookDir);
	}
	else if(valid(getNeighbor(p, r->dir), g->m, g->n))
		return WALK;
	else
		return TURN_LEFT;
}
#include "TileM.h"

CPicture SInfoBox::g_tile;
CPicture SInfoBox::g_town;
CPicture SInfoBox::g_pasture;
//CPicture SInfoBox::g_box;
CPicture SInfoBox::g_resource;
//CPicture SInfoBox::g_trade;
CPicture SInfoBox::g_goods;
CPicture SInfoBox::g_building;
CPicture SInfoBox::g_shadeS;
CPicture SInfoBox::g_shadeL;
CPicture SInfoBox::g_build;
CPicture SInfoBox::g_demolish;
CPicture SInfoBox::g_mineral;
CPicture SInfoBox::g_sArea;

SInfoBox::SInfoBox(){
	mode = KEEP;
	open = true;

	g_tile.Load("Chikuwa3/Tiles.png", TERRAINS, 1, GRID, GRID, TERRAINS);
	g_town.Load("Chikuwa3/Towns.png", 1, TOWNS + 1, GRID, GRID, TOWNS + 1);
	g_pasture.Load("Chikuwa3/Pasture.png", TERRAINS, 3, GRID, GRID, TERRAINS * 3);
	g_building.Load("Chikuwa3/Buildings.png", BUILDINGS, TOWNS + 1, 40, 40, BUILDINGS * (TOWNS + 1));
	//g_box.Load("Chikuwa3/Box.png");
	g_shadeS.Load("Chikuwa3/Shade40.png");
	g_shadeL.Load("Chikuwa3/Shade50.png");
	g_resource.Load("Chikuwa3/RIcons.png", RESOURCES, 1, 20, 20, RESOURCES);
	//g_trade.Load("Chikuwa3/TIcons.png", TRADE, 1, 20, 20, TRADE);
	g_goods.Load("Chikuwa3/Goods.png", GOODS, 1, G_SIZE, G_SIZE, GOODS);
	g_build.Load("Chikuwa3/Build.png");
	g_demolish.Load("Chikuwa3/Demolish.png");
	g_mineral.Load("Chikuwa3/Mineral.png", MNR_TYPE, 1, R_SIZE, R_SIZE, MNR_TYPE);
	g_sArea.Load("Chikuwa3/SpArea.png", 1, SP_AREAS + 1, GRID, GRID, SP_AREAS + 1);
}

void SInfoBox::DrawIB(){

	if ((Event.LMouse.GetClick(WINDOW_WIDTH - CROSS_L - 1, -1, WINDOW_WIDTH, CROSS_L) || Event.RMouse.GetClick()) && !open){
		mode = CLOSE;
	}
}

void SInfoBox::I_UpDate() {
	mode = KEEP;
	open = true;
}

/////

CPicture SFoundBox::g_boxF;
CPicture SFoundBox::g_cbut;
CPicture SFoundBox::g_pasture;
CPicture SFoundBox::g_saBut;
CPicture SFoundBox::g_naBut;
CPicture SFoundBox::g_search;

SFoundBox::SFoundBox(ETerrain type, STown town, bool cFlag, bool pas_s, bool pas_c, bool pas_p, bool sFlag){
	terrain = type;
	townInfo = town;
	pType = 0;
	saType = 0;
	cut = cFlag;
	search = sFlag;
	pasture[0] = pas_s;
	pasture[1] = pas_c;
	pasture[2] = pas_p;
	spArea = false;
	spAAble[0] = false;
	for (int i = 1; i <= SP_AREAS; i++) {
		spAAble[i] = CheckSpAAble(i);
	}

	g_boxF.Load("Chikuwa3/Box_F.png");
	g_cbut.Load("Chikuwa3/CButton.png");
	g_pasture.Load("Chikuwa3/Pasture.png", TERRAINS, 3, GRID, GRID, TERRAINS * 3);
	g_saBut.Load("Chikuwa3/SaButton.png");
	g_naBut.Load("Chikuwa3/NaButton.png");
	g_search.Load("Chikuwa3/Search.png");
}

bool SFoundBox::CheckSpAAble(int n) {
	/*int sum = 0;
	for (int i = 1; i <= TOWNS; i++) {
		if (saData.rPType[n][i]) {
			sum += townInfo.tTypePop[i];
		}
	}
	switch (n) {
	case 6:
		if (townInfo.cPas < 6 || townInfo.goodsPro[2] - townInfo.goodsCon[2] < 6) {
			return false;
		}
		break;

	case 7:
		if (townInfo.income[WOOD] < 45) {
			return false;
		}
		break;

	case 8:
		if (townInfo.goodsPro[20] - townInfo.goodsCon[20] < 1 || townInfo.goodsPro[17] - townInfo.goodsCon[17] < 6) {
			return false;
		}
		break;

	default:
		break;
	}
	if (sum >= saData.reqPop[n]) {*/
		return true;
	/*}
	else {
		return false;
	}*/
}

bool SFoundBox::CheckSAEnough(int type) {
	for (int i = 0; i < RESOURCES; i++) {
		if (townInfo.resource[i] < saData.cost[type][i]) {
			return false;
		}
	}

	return true;
}

void SFoundBox::PutSAB(int x, int y, int type) {
	g_sArea.Draw(x, y, type);
	
	DrawFormatString(x - 35 - I_SIZE, y - 40, BLACK, "%s", saData.name[type]);
	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);

	bool flag = true;
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(x - (I_SIZE + 45), y + I_SIZE * i, i);
		if (townInfo.resource[i] < saData.cost[type][i]) {
			flag = false;
			DrawFormatString(x - 40, y + I_SIZE * i + 2, RED, "-%d", saData.cost[type][i]);
		}
		else {
			DrawFormatString(x - 40, y + I_SIZE * i + 2, BLACK, "-%d", saData.cost[type][i]);
		}
	}
	if (flag) {
		if (!open && Event.LMouse.GetClick(x, y, x + GRID, y + GRID) && spAAble[type]) {
			saType = type;
			mode = BLD_SA;
		}
	}
}

void SFoundBox::DrawSAReq(int x, int y, int type, bool ok) {
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 231;
	int col = 0;
	int fixY = 0;
	if (saData.expL[type] > 0) {
		fixY++;
	}
	if (ok) {
		col = BLACK;
	}
	else {
		col = RED;
	}
	if (Event.LMouse.GetOn(x - 1, y - 1, x + GRID, y + GRID)) {
		if (mx < lim) {
			DrawBox(mx, my, mx + 215, my + (saData.reqL[type] + saData.expL[type] + 1 + fixY) * 25 + 5, LIGHTYELLOW, true);
			DrawString(mx + 5, my + 5, "条件", BLACK);
			for (int i = 0; i < saData.reqL[type]; i++) {
				DrawFormatString(mx + 5, my + 30 + i * 25, col, "%s", saData.req[type][i]);
			}
			if (saData.expL[type] > 0) {
				DrawString(mx + 5, my + (saData.reqL[type] + 1) * 25 + 5, "効果", BLACK);
			}
			for (int i = 0; i < saData.expL[type]; i++) {
				DrawFormatString(mx + 5, my + 5 + (i + saData.reqL[type] + 2) * 25, col, "%s", saData.exp[type][i]);
			}
		}
		else {
			DrawBox(lim, my, lim + 215, my + (saData.reqL[type] + saData.expL[type] + 1 + fixY) * 25 + 5, LIGHTYELLOW, true);
			DrawString(lim + 5, my + 5, "条件", BLACK);
			for (int i = 0; i < saData.reqL[type]; i++) {
				DrawFormatString(lim + 5, my + 30 + i * 25, col, "%s", saData.req[type][i]);
			}
			if (saData.expL[type] > 0) {
				DrawString(lim + 5, my + (saData.reqL[type] + 1) * 25 + 5, "効果", BLACK);
			}
			for (int i = 0; i < saData.expL[type]; i++) {
				DrawFormatString(lim + 5, my + 5 + (i + saData.reqL[type] + 2) * 25, col, "%s", saData.exp[type][i]);
			}
		}
	}
}


bool SFoundBox::CheckTEnough(ETown type){
	for(int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < tData.cost[type][NEW][i]){
			return false;
		}
	}

	return true;
}

void SFoundBox::PutPB(int x, int y, int type) {
	g_pasture.Draw(x, y, terrain + type*TERRAINS);

	DrawFormatString(x - 35 - I_SIZE, y - 40, BLACK, "%s", pData.name[type]);
	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);
	DrawString(x + GRID + 5, y - 20, "産出", BLACK);
	DrawPData(x, y, type);

	bool flag = true;
	for (int i = 0; i < RESOURCES; i++) {
		if (townInfo.resource[i] < pData.cost[type][i]) {
			flag = false;
		}
	}

	if (flag) {
		if (!open && Event.LMouse.GetClick(x, y, x + GRID, y + GRID)) {
			pType = type;
			mode = BLD_P;
		}
	}
}

void SFoundBox::DrawPData(int x, int y, int type) {
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < pData.cost[type][i]) {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", pData.cost[type][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", pData.cost[type][i]);
		}
	}
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(x + GRID + 5, y + i * I_SIZE, i);
		DrawFormatString(x + GRID + 5 + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "%d", pData.income[type][i]);
	}
	if (pData.goods[type] != 0) {
		g_goods.Draw(x + GRID + 5, y + RESOURCES * I_SIZE, pData.goods[type]);
	}
}

void SFoundBox::PutTB(int x, int y, ETown type){
	g_town.Draw(x, y, type);
	
	DrawFormatString(x - 35 - I_SIZE, y - 40, BLACK, "%s", tData.name[type]);
	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);
	DrawString(x + GRID + 5, y - 20, "産出", BLACK);
	DrawTData(x + GRID + 5, y, type);

	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[type][NEW][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", tData.cost[type][NEW][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", tData.cost[type][NEW][i]);
		}
	}

	if (CheckTEnough(type)){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			town = type;
			mode = EST;
		}
	}
}

void SFoundBox::DrawTData(int x, int y, ETown type){
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "%d", tData.income[type][NEW][i]);
	}
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tData.trade[type][NEW][i]);
	}*/
	if (tData.goods[type][0] != 0) {
		g_goods.Draw(x, y + RESOURCES * I_SIZE, tData.goods[type][0]);
	}
	if (tData.goods[type][0] == 1) {
		DrawFormatString(x + G_SIZE + 5, y + RESOURCES * I_SIZE + 10, CYAN, "%d", tData.trade[type][0]);
	}
	//DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tData.trade[type][NEW][i]);
}

void SFoundBox::DrawFB(int money/*, EMineral mineral*/) {
	g_boxF.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	/*if (terrain == HILL){
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, mineral);
		switch (mineral) {
		case GOLD:
			DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 10, 450 + R_SIZE / 2 + 5, "金", BLACK);
			break;

		case IRON:
			DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 - 5, 450 + R_SIZE / 2 + 5, "鉄鉱石", BLACK);
			break;

		default:
			DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 3, 450 + R_SIZE / 2 + 5, "石材", BLACK);
			break;
		}
	}*/

	switch (terrain) {
	case HILL_S:
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, ROCK);
		DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 3, 450 + R_SIZE / 2 + 5, "石材", BLACK);
		break;

	case HILL_G:
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, GOLD);
		DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 + 10, 450 + R_SIZE / 2 + 5, "金", BLACK);
		break;

	case HILL_I:
		g_mineral.Draw(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2, 450 - R_SIZE / 2, IRON);
		DrawString(WINDOW_WIDTH - WINDOW_HEIGHT + WINDOW_HEIGHT / 2 - R_SIZE / 2 - 5, 450 + R_SIZE / 2 + 5, "鉄鉱石", BLACK);
		break;

	default:
		break;
	}

	if (spArea) {
		g_naBut.Draw(340, 87);
		if (!open && Event.LMouse.GetClick(340, 87, 340 + 105, 87 + 105)) {
			spArea = false;
		}
		DrawString(400, 30, "絵にカーソルを合わせると条件を見られます", BLACK);
		for (int i = 1; i < SP_AREAS; i++) {
			if (!townInfo.onlyArea[i]) {
				PutSAB(430 + 130 * (i % 4) - GRID / 2, 130 * (i / 4 + 1), i);
			}
			else if (i == 7) {
				PutSAB(430 + 130 * (i % 4) - GRID / 2, 130 * (i / 4 + 1), 8);
			}
		}
		for (int i = 1; i < SP_AREAS; i++) {
			if (!townInfo.onlyArea[i]) {
				DrawSAReq(430 + 130 * (i % 4) - GRID / 2, 130 * (i / 4 + 1), i, spAAble[i]);
			}
			else if (i == 7) {
				DrawSAReq(430 + 130 * (i % 4) - GRID / 2, 130 * (i / 4 + 1), 8, spAAble[8]);
			}
		}
	}
	else{
		g_saBut.Draw(340, 87);
		if (!open && Event.LMouse.GetClick(340, 87, 340 + 105, 87 + 105)) {
			spArea = true;
		}

		if (townInfo.towns < townInfo.townMax) {
			switch (terrain) {
			case PLAIN:

				if (!spArea) {
					PutTB(570, 100, FARM);
					PutTB(750, 100, COMM);
					PutTB(390, 270, PAS_S);
					PutTB(570, 270, PAS_C);
					PutTB(750, 270, PAS_P);
				}

				break;

			case FOREST:
				PutTB(540, 120, F_VIL);
				break;

			/*case HILL:
			switch (mineral) {
			case GOLD:
			PutButton(540, 120, MINE_G, money);
			break;

			case IRON:
			PutButton(540, 120, MINE_I, money);
			break;

			default:
			PutButton(540, 120, MINE_S, money);
			break;
			}
			break;*/

			case HILL_S:
				PutTB(540, 120, MINE_S);
				break;

			case HILL_G:
				PutTB(540, 120, MINE_G);
				break;

			case HILL_I:
				PutTB(540, 120, MINE_I);
				break;

			default:
				break;
			}
		}
		else {
			DrawString(500, 200, "これ以上都市を造れません！", BLACK);
		}
	}

	if (cut) {
		g_cbut.Draw(450, 465);
		DrawString(445, 522, "コスト", BLACK);
		g_resource.Draw(505, 520, MONEY);

		if (money >= 50) {
			DrawString(530, 522, "50", BLACK);
			if (Event.LMouse.GetClick(449, 464, 550, 515) && !open) {
				mode = CUT;
			}
		}
		else {
			DrawString(530, 522, "50", RED);
		}
	}
	
	if (search) {
		g_search.Draw(700, 465);
		DrawString(695, 522, "コスト", BLACK);
		g_resource.Draw(755, 520, MONEY);
		if (money >= 80) {
			DrawString(780, 522, "80", BLACK);
			if (Event.LMouse.GetClick(699, 464, 800, 515) && !open) {
				mode = SEARCH;
			}
		}
		else {
			DrawString(780, 522, "80", RED);
		}
	}

	if (pasture[0]) {
		PutPB(470, 460, 0);
	}
	if (pasture[1]) {
		PutPB(670, 460, 1);
	}
	if (pasture[2]) {
		PutPB(730, 460, 2);
	}
	

	open = false;
}

void SFoundBox::UpDate(ETerrain type, STown town, bool cFlag, bool pas_s, bool pas_c, bool pas_p, bool sFlag){
	I_UpDate();
	terrain = type;
	townInfo = town;
	pType = 0;
	saType = 0;
	cut = cFlag;
	search = sFlag;
	spArea = false;
	spAAble[0] = false;
	pasture[0] = pas_s;
	pasture[1] = pas_c;
	pasture[2] = pas_p;
	for (int i = 1; i <= SP_AREAS; i++) {
		spAAble[i] = CheckSpAAble(i);
	}
}

/////

CPicture STownBox::g_boxT;
CPicture STownBox::g_lvUp;
CPicture STownBox::g_lvDn;
CPicture STownBox::g_demoL[2];
CPicture STownBox::g_SB;
CPicture STownBox::g_item;
CPicture STownBox::g_search;

STownBox::STownBox(STown town, STile tile, bool sFlag){
	//devLv = 0;
	townInfo = town;
	tileInfo = tile;
	clickDemo = false;
	search = sFlag;

	g_boxT.Load("Chikuwa3/Box_T.png");
	g_lvUp.Load("Chikuwa3/Dev.png");
	g_lvDn.Load("Chikuwa3/DG.png");
	g_demoL[0].Load("Chikuwa3/Demo_L.png");
	g_demoL[1].Load("Chikuwa3/AskDL.png");
	g_SB.Load("Chikuwa3/SpBuildings.png", SP_BUILDS, 1, B_SIZE, B_SIZE, SP_BUILDS);
	g_item.Load("Chikuwa3/Items.png", ITEMS, 1, G_SIZE, G_SIZE, ITEMS);
	g_search.Load("Chikuwa3/Search.png");
}

void STownBox::PutRemoveButton(int x, int y){
	DrawString(x + GRID + 15, y - 30, "コスト", BLACK);
	g_resource.Draw(x + GRID + 15, y + I_SIZE - 30, MONEY);

	int cost = (tData.cost[tileInfo.town][NEW][MONEY] + tData.cost[tileInfo.town][LVUP][MONEY] * tileInfo.townLv) / 2;
	if (townInfo.resource[MONEY] >= cost){
		DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE - 28, BLACK, "%d", cost);

		if (Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			if (clickDemo){
				mode = REMV;
			}
			else {
				clickDemo = true;
			}
		}

		if (!Event.RMouse.GetOn(x - 1, y - 1, x + GRID, y + GRID) && clickDemo){
			clickDemo = false;
		}
	}
	else {
		DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE - 28, RED, "%d", cost);
	}

	DrawString(x + GRID + 15, y + I_SIZE * 2 - 25, "回収", BLACK);
	g_resource.Draw(x + GRID + 15, y + I_SIZE * 3 - 25, WOOD);
	DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE * 3 - 21, BLACK, "%d", (tData.cost[tileInfo.town][NEW][WOOD] + tData.cost[tileInfo.town][LVUP][WOOD] * tileInfo.townLv) / 4);
	g_resource.Draw(x + GRID + 15, y + I_SIZE * 4 - 25, STONE);
	DrawFormatString(x + GRID + I_SIZE + 20, y + I_SIZE * 4 - 21, BLACK, "%d", (tData.cost[tileInfo.town][NEW][STONE] + tData.cost[tileInfo.town][LVUP][STONE] * tileInfo.townLv) / 4);

}

void STownBox::DrawDev(int x, int y/*, int lv*/){
	DrawString(x, y - 20, "上昇値", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "+%d", tData.income[tileInfo.town][LVUP][i]);
	}
	if (tData.goods[tileInfo.town][LVUP] != 0) {
		g_goods.Draw(x, y + RESOURCES*I_SIZE, tData.goods[tileInfo.town][LVUP]);
	}
	if (tData.goods[tileInfo.town][LVUP] == 1) {
		DrawFormatString(x + G_SIZE + 5, y + RESOURCES*I_SIZE + 10, CYAN, "%d", tData.trade[tileInfo.town][LVUP]);
	}
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "+%d", tData.trade[tileInfo.town][LVUP][i]);
	}*/
}

void STownBox::DrawDG(int x, int y/*, int lv*/){
	DrawString(x, y - 20, "下降値", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x, y + i * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + i * I_SIZE + 2, BLACK, "-%d", tData.income[tileInfo.town][LVUP][i]);
	}
	g_goods.Draw(x, y + RESOURCES*I_SIZE, tData.goods[tileInfo.town][LVUP]);
	if (tData.goods[tileInfo.town][LVUP] == 1) {
		DrawFormatString(x + G_SIZE + 5, y + RESOURCES*I_SIZE + 10, CYAN, "%d", tData.trade[tileInfo.town][LVUP]);
	}
	/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(x, y + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(x + I_SIZE + 5, y + (i + RESOURCES) * I_SIZE + 2, BLACK, "-%d", tData.trade[tileInfo.town][LVUP][i]);
	}*/
}

bool STownBox::CheckDEnough(){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutDevButton(int x, int y/*, int lv*/){
	g_lvUp.Draw(x, y);

	DrawString(x - 35 - I_SIZE, y - 20, "コスト", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "%d", tData.cost[tileInfo.town][LVUP][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "%d", tData.cost[tileInfo.town][LVUP][i]);
		}
	}

	if (CheckDEnough()){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + 50, y + 50)){
			mode = DEV;
			//devLv = lv;
		}
	}
}

void STownBox::PutDGButton(int x, int y/*, int lv*/){
	g_lvDn.Draw(x, y);

	/*DrawString(x - 65 - I_SIZE, y - 20, "コスト・回収", BLACK);
	for (int i = 0; i < RESOURCES; i++){
		g_resource.Draw(x - (I_SIZE + 35), y + I_SIZE * i, i);

		if (townInfo.resource[i] < tData.cost[tileInfo.town][LVUP][i]){
			DrawFormatString(x - 30, y + I_SIZE * i + 2, RED, "-%d", tData.cost[tileInfo.town][LVUP][i]);
		}
		else {
			DrawFormatString(x - 30, y + I_SIZE * i + 2, BLACK, "-%d", tData.cost[tileInfo.town][LVUP][i]);
		}
	}*/
	DrawString(x - 40 - I_SIZE, y - 20, "コスト", BLACK);
	g_resource.Draw(x - (I_SIZE + 40), y, MONEY);
	if (townInfo.resource[MONEY] < tData.cost[tileInfo.town][LVUP][MONEY] / 2){
		DrawFormatString(x - 35, y + 2, RED, "%d", tData.cost[tileInfo.town][LVUP][MONEY] / 2);
	}
	else {
		DrawFormatString(x - 35, y + 2, BLACK, "%d", tData.cost[tileInfo.town][LVUP][MONEY] / 2);
	}

	DrawString(x - 40 - I_SIZE, y + 25, "回収", BLACK);
	g_resource.Draw(x - (I_SIZE + 40), y + 50, WOOD);
	DrawFormatString(x - 35, y + 50, BLACK, "%d", tData.cost[tileInfo.town][LVUP][WOOD] / 4);
	g_resource.Draw(x - (I_SIZE + 40), y + 75, STONE);
	DrawFormatString(x - 35, y + 75, BLACK, "%d", tData.cost[tileInfo.town][LVUP][STONE] / 4);

	if (townInfo.resource[MONEY] >= tData.cost[tileInfo.town][LVUP][MONEY] / 2){
		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + 50, y + 50)){
			mode = DG;
			//devLv = lv;
		}
	}
}

void STownBox::PutItemB(int x, int y, int type) {
	g_item.Draw(x, y, type - 1);
	int g = 0;
	switch (type) {
	case 1:
		g = 16;
		break;

	case 2:
		g = 20;
		break;

	default:
		break;
	}

	if (tileInfo.itemUse[type]) {
		if (Event.LMouse.GetClick(x - 1, y - 1, x + G_SIZE, y + G_SIZE)) {
			mode = END_USE;
			iNum = type;
		}
	}
	else {
		g_shadeS.Draw(x, y);

		if (townInfo.goodsPro[g] - townInfo.itemCon[type - 1] >= 1) {
			if (Event.LMouse.GetClick(x - 1, y - 1, x + G_SIZE, y + G_SIZE)) {
				mode = USE;
				iNum = type;
			}
		}
		else {
			DrawString(x + 3, y + G_SIZE + 5, "不足", RED);
		}
	}
}

void STownBox::DrawBuildings(int x, int y, int build){
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 241;
	if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
		if (mx < lim){
			DrawBox(mx, my, mx + 225, my + 160, LIGHTYELLOW, true);

			DrawFormatString(mx + 5, my + 5, BLACK, "%s", bData.name[tileInfo.town][build]);
			if (tileInfo.built[build]) {
				DrawString(mx + 5, my + I_SIZE + 5, "撤去コスト", BLACK);
				g_resource.Draw(mx + 5, my + I_SIZE * 2 + 5, MONEY);
				if (townInfo.resource[MONEY] < bData.cost[tileInfo.town][build][MONEY] / 2) {
					DrawFormatString(mx + 10 + I_SIZE, my + I_SIZE * 2 + 7, RED, "%d", bData.cost[tileInfo.town][build][MONEY] / 2);
				}
				else {
					DrawFormatString(mx + 10 + I_SIZE, my + I_SIZE * 2 + 7, BLACK, "%d", bData.cost[tileInfo.town][build][MONEY] / 2);
				}
			}
			else {
				DrawString(mx + 5, my + I_SIZE + 5, "建設コスト", BLACK);
				for (int i = 0; i < RESOURCES; i++){
					g_resource.Draw(mx + 5 + i * 55, my + I_SIZE * 2 + 5, i);
					if (townInfo.resource[i] < bData.cost[tileInfo.town][build][i]){
						DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", bData.cost[tileInfo.town][build][i]);
					}
					else {
						DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", bData.cost[tileInfo.town][build][i]);
					}
				}
			}
		
			DrawString(mx + 5, my + 10 + I_SIZE * 3, "産出", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(mx + 5 + i * 55, my + 10 + I_SIZE * 4, i);

				DrawFormatString(mx + 10 + I_SIZE + i * 55, my + 12 + I_SIZE * 4, BLACK, "%d", bData.income[tileInfo.town][build][i]);
			}
			/*for (int i = 0; i < TRADE; i++){
				g_trade.Draw(mx + 5 + i * 55, my + 15 + I_SIZE * 5, i);

				DrawFormatString(mx + 10 + I_SIZE + i * 55, my + 17 + I_SIZE * 5, BLACK, "%d", bData.trade[tileInfo.town][build][i]);
			}*/
			if (bData.goods[tileInfo.town][build] != 0) {
				g_goods.Draw(mx + 5, my + 15 + I_SIZE * 5, bData.goods[tileInfo.town][build]);
			}
			if (bData.goods[tileInfo.town][build] == 1) {
				DrawFormatString(mx + G_SIZE + 10, my + 25 + I_SIZE * 5, CYAN, "%d", bData.trade[tileInfo.town][build]);
			}
			if ((tileInfo.town == PAS_S || tileInfo.town == PAS_C || tileInfo.town == PAS_P) && build == 0) {
				DrawString(mx + 5, my + 20 + I_SIZE * 5, "周りに放牧地が", BLACK);
				DrawString(mx + 5, my + 20 + I_SIZE * 6, "造れるようになる", BLACK);
			}
		}
		else {
			DrawBox(lim, my, lim + 225, my + 160, LIGHTYELLOW, true);
			
			DrawFormatString(lim + 5, my + 5, BLACK, "%s", bData.name[tileInfo.town][build]);
			if (tileInfo.built[build]) {
				DrawString(lim + 5, my + I_SIZE + 5, "撤去コスト", BLACK);
				g_resource.Draw(lim + 5, my + I_SIZE * 2 + 5, MONEY);
				if (townInfo.resource[MONEY] < bData.cost[tileInfo.town][build][MONEY] / 2) {
					DrawFormatString(lim + 10 + I_SIZE, my + I_SIZE * 2 + 7, RED, "%d", bData.cost[tileInfo.town][build][MONEY] / 2);
				}
				else {
					DrawFormatString(lim + 10 + I_SIZE, my + I_SIZE * 2 + 7, BLACK, "%d", bData.cost[tileInfo.town][build][MONEY] / 2);
				}
			}
			else {
				DrawString(lim + 5, my + I_SIZE + 5, "建設コスト", BLACK);
				for (int i = 0; i < RESOURCES; i++) {
					g_resource.Draw(lim + 5 + i * 55, my + I_SIZE * 2 + 5, i);
					if (townInfo.resource[i] < bData.cost[tileInfo.town][build][i]) {
						DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", bData.cost[tileInfo.town][build][i]);
					}
					else {
						DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", bData.cost[tileInfo.town][build][i]);
					}
				}
			}
		
			DrawString(lim + 5, my + 10 + I_SIZE * 3, "産出", BLACK);
			for (int i = 0; i < RESOURCES; i++){
				g_resource.Draw(lim + 5 + i * 55, my + 10 + I_SIZE * 4, i);

				DrawFormatString(lim + 10 + I_SIZE + i * 55, my + 12 + I_SIZE * 4, BLACK, "%d", bData.income[tileInfo.town][build][i]);
			}
			/*for (int i = 0; i < TRADE; i++){
				g_trade.Draw(lim + 5 + i * 55, my + 15 + I_SIZE * 5, i);

				DrawFormatString(lim + 10 + I_SIZE + i * 55, my + 17 + I_SIZE * 5, BLACK, "%d", bData.trade[tileInfo.town][build][i]);
			}*/
			if (bData.goods[tileInfo.town][build] != 0) {
				g_goods.Draw(lim + 5, my + 15 + I_SIZE * 5, bData.goods[tileInfo.town][build]);
			}
			if (bData.goods[tileInfo.town][build] == 1) {
				DrawFormatString(lim + G_SIZE + 10, my + 25 + I_SIZE * 5, CYAN, "%d", bData.trade[tileInfo.town][build]);
			}
		}
	}
}

bool STownBox::CheckBEnough(int bNum){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < bData.cost[tileInfo.town][bNum][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutBuildingButton(int x, int y, int bNum, bool isBuilt){
	if (isBuilt){
		if (Event.RMouse.GetOn(x, y, x + B_SIZE, y + B_SIZE)){
			g_demolish.Draw(x, y);
		}

		if (townInfo.resource[MONEY] >= bData.cost[tileInfo.town][bNum][MONEY] / 2){
			if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
				mode = DEMO;
				buildNum = bNum;
			}
		}
	}
	else{
		g_shadeS.Draw(x, y);

		if (CheckBEnough(bNum)){
			if (Event.RMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				g_build.Draw(x, y);
			}

			if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				mode = BUILD;
				buildNum = bNum;
			}
		}
	}	
}

void STownBox::DrawSBuildings(int x, int y, int build, bool popReq, bool ex){
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 291;
	int l = 0;
	if (ex) {
		if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
			if (mx < lim){
				DrawBox(mx, my, mx + 275, my + 100, LIGHTYELLOW, true);

				DrawFormatString(mx + 5, my + 5, BLACK, "%s", sbData.name[build]);

				if (sbData.onlyOne[build]){
					DrawString(mx + 5, my + 10 + I_SIZE, "地域に一つだけ建てられる", BLACK);
					l = 1;
				}

				DrawString(mx + 5, my + 15 + I_SIZE * (1 + l), "効果", BLACK);
				DrawFormatString(mx + 5, my + 15 + I_SIZE * (2 + l), BLACK, "%s", sbData.exp[build]);
			}
			else {
				DrawBox(lim, my, lim + 275, my + 100, LIGHTYELLOW, true);
			
				DrawFormatString(lim + 5, my + 5, BLACK, "%s", sbData.name[build]);

				if (sbData.onlyOne[build]){
					DrawString(lim + 5, my + 10 + I_SIZE, "地域に１つだけ建てられる", BLACK);
					l = 1;
				}

				DrawString(lim + 5, my + 15 + I_SIZE * (1 + l), "効果", BLACK);
				DrawFormatString(lim + 5, my + 15 + I_SIZE * (2 + l), BLACK, "%s", sbData.exp[build]);
			}
		}
	}
	else {
		if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
			if (mx < lim){
				DrawBox(mx, my, mx + 275, my + 180, LIGHTYELLOW, true);

				DrawFormatString(mx + 5, my + 5, BLACK, "%s", sbData.name[build]);
				DrawString(mx + 5, my + I_SIZE + 5, "建設コスト", BLACK);
				for (int i = 0; i < RESOURCES; i++){
					g_resource.Draw(mx + 5 + i * 55, my + I_SIZE * 2 + 5, i);
					if (townInfo.resource[i] < sbData.cost[build][i]){
						DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", sbData.cost[build][i]);
					}
					else {
						DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", sbData.cost[build][i]);
					}
				}
		
				if (popReq){
					DrawString(mx + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
					DrawFormatString(mx + 5, my + 10 + I_SIZE * 4, BLACK, "%s", sbData.req[build]);
				}else{
					DrawString(mx + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
					DrawFormatString(mx + 5, my + 10 + I_SIZE * 4, RED, "%s", sbData.req[build]);
				}

				if (sbData.onlyOne[build]){
					DrawString(mx + 5, my + 10 + I_SIZE * 5, "地域に一つだけ建てられる", BLACK);
					l = 1;
				}

				DrawString(mx + 5, my + 15 + I_SIZE * (5 + l), "効果", BLACK);
				DrawFormatString(mx + 5, my + 15 + I_SIZE * (6 + l), BLACK, "%s", sbData.exp[build]);
			}
			else {
				DrawBox(lim, my, lim + 275, my + 180, LIGHTYELLOW, true);
			
				DrawFormatString(lim + 5, my + 5, BLACK, "%s", sbData.name[build]);
				DrawString(lim + 5, my + I_SIZE + 5, "建設コスト", BLACK);
				for (int i = 0; i < RESOURCES; i++){
					g_resource.Draw(lim + 5 + i * 55, my + I_SIZE * 2 + 5, i);
					if (townInfo.resource[i] < sbData.cost[build][i]){
						DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", sbData.cost[build][i]);
					}
					else {
						DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", sbData.cost[build][i]);
					}
				}
			
				if (popReq){
					DrawString(lim + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
					DrawFormatString(lim + 5, my + 10 + I_SIZE * 4, BLACK, "%s", sbData.req[build]);
				}else{
					DrawString(lim + 5, my + 10 + I_SIZE * 3, "条件", BLACK);
					DrawFormatString(lim + 5, my + 10 + I_SIZE * 4, RED, "%s", sbData.req[build]);
				}

				if (sbData.onlyOne[build]){
					DrawString(lim + 5, my + 10 + I_SIZE * 5, "地域に１つだけ建てられる", BLACK);
					l = 1;
				}

				DrawString(lim + 5, my + 15 + I_SIZE * (5 + l), "効果", BLACK);
				DrawFormatString(lim + 5, my + 15 + I_SIZE * (6 + l), BLACK, "%s", sbData.exp[build]);
			}
		}
	}
}

bool STownBox::CheckSBEnough(int bNum){
	for (int i = 0; i < RESOURCES; i++){
		if (townInfo.resource[i] < sbData.cost[bNum][i]){
			return false;
		}
	}

	return true;
}

void STownBox::PutSpBuildButton(int x, int y, int bNum, int tbNum, bool isBuilt){
	if (isBuilt){
		/*if (Event.RMouse.GetOn(x, y, x + B_SIZE, y + B_SIZE)){
			g_demolish.Draw(x, y);
		}

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + GRID, y + GRID)){
			mode = DEMO;
			buildNum = bNum;
		}*/
	}
	else{
		if (CheckSBEnough(bNum)){
			if (Event.RMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				g_build.Draw(x, y);
			}

			if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + B_SIZE, y + B_SIZE)){
				mode = SBUILD;
				buildNum = tbNum;
				sBuildNum = bNum;
			}
		}
	}
}

void STownBox::DrawItemInfo(int x, int y, int tNum, int iNum) {
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 241;
	if (Event.LMouse.GetOn(x - 1, y - 1, x + B_SIZE, y + B_SIZE)) {
		if (mx < lim) {
			DrawBox(mx, my, mx + 225, my + tData.iExpL[tNum][iNum] * 20 + 30, LIGHTYELLOW, true);
			DrawString(mx + 5, my + 5, "対象", BLACK);
			for (int i = 0; i < tData.iExpL[tNum][iNum]; i++) {
				DrawFormatString(mx + 5, my + 25 + 20 * i, BLACK, "%s", tData.iExp[tNum][iNum][i]);
			}
		}
		else {
			DrawBox(lim, my, lim + 225, my + tData.iExpL[tNum][iNum] * 20 + 30, LIGHTYELLOW, true);

			DrawBox(lim, my, lim + 225, my + tData.iExpL[tNum][iNum] * 20 + 30, LIGHTYELLOW, true);
			DrawString(lim + 5, my + 5, "対象", BLACK);
			for (int i = 0; i < tData.iExpL[tNum][iNum]; i++) {
				DrawFormatString(lim + 5, my + 25 + 20 * i, BLACK, "%s", tData.iExp[tNum][iNum][i]);
			}
		}
	}
}

void STownBox::DrawTB(){
	g_boxT.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	DrawFormatString(415, 210, BLACK, "%s", tData.name[tileInfo.town]);
	DrawFormatString(415, 235, BLACK, "都市人口:%d/%d", tileInfo.townLv + 1, tileInfo.devLim);
	if (search) {
		g_search.Draw(350, 305);
		DrawString(345, 362, "コスト", BLACK);
		g_resource.Draw(405, 360, MONEY);
		if (townInfo.resource[MONEY] >= 80) {
			DrawString(430, 362, "80", BLACK);
			if (Event.LMouse.GetClick(349, 304, 450, 355) && !open) {
				mode = SEARCH;
				search = false;
			}
		}
		else {
			DrawString(430, 362, "80", RED);
		}

		DrawString(470, 255, "産出", BLACK);
		for (int i = 0; i < RESOURCES; i++) {
			g_resource.Draw(480, 275 + i * I_SIZE, i);
			DrawFormatString(480 + I_SIZE + 5, 275 + i * I_SIZE + 2, BLACK, "%d", tileInfo.produce[i]);
		}
		/*for (int i = 0; i < TRADE; i++){
		g_trade.Draw(425, 275 + (i + RESOURCES) * I_SIZE, i);
		DrawFormatString(425 + I_SIZE + 5, 275 + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tileInfo.trade[i]);
		}*/
		int count = 0, g[GOODS] = {};
		for (int i = 1; i < GOODS; i++) {
			if (tileInfo.goods[i] > 0) {
				g[count] = i;
				count++;
			}
		}

		for (int i = 0; i < count; i++) {
			if (g[i] == 1) {
				g_goods.Draw(426 + 202 / (count + 1) * (i + 1) - G_SIZE / 2, 275 + RESOURCES * I_SIZE, g[i]);
				DrawFormatString(431 + 202 / (count + 1) * (i + 1) + G_SIZE / 2, 275 + RESOURCES * I_SIZE + 12, CYAN, "%d", tileInfo.goods[g[i]]);
			}
			else {
				g_goods.Draw(426 + 202 / (count + 1) * (i + 1) - G_SIZE / 2, 275 + RESOURCES * I_SIZE, g[i]);
				DrawFormatString(431 + 202 / (count + 1) * (i + 1) + G_SIZE / 2, 275 + RESOURCES * I_SIZE + 12, BLACK, "%d", tileInfo.goods[g[i]]);
			}
		}
	}
	else {
		DrawString(440, 255, "産出", BLACK);
		for (int i = 0; i < RESOURCES; i++) {
			g_resource.Draw(450, 275 + i * I_SIZE, i);
			DrawFormatString(450 + I_SIZE + 5, 275 + i * I_SIZE + 2, BLACK, "%d", tileInfo.produce[i]);
		}
		/*for (int i = 0; i < TRADE; i++){
			g_trade.Draw(425, 275 + (i + RESOURCES) * I_SIZE, i);
			DrawFormatString(425 + I_SIZE + 5, 275 + (i + RESOURCES) * I_SIZE + 2, BLACK, "%d", tileInfo.trade[i]);
		}*/
		int count = 0, g[GOODS] = {};
		for (int i = 1; i < GOODS; i++) {
			if (tileInfo.goods[i] > 0) {
				g[count] = i;
				count++;
			}
		}

		for (int i = 0; i < count; i++) {
			if (g[i] == 1) {
				g_goods.Draw(316 + 282 / (count + 1) * (i + 1) - G_SIZE / 2, 275 + RESOURCES * I_SIZE, g[i]);
				DrawFormatString(321 + 282 / (count + 1) * (i + 1) + G_SIZE / 2, 275 + RESOURCES * I_SIZE + 12, CYAN, "%d", tileInfo.goods[g[i]]);
			}
			else {
				g_goods.Draw(316 + 282 / (count + 1) * (i + 1) - G_SIZE / 2, 275 + RESOURCES * I_SIZE, g[i]);
				DrawFormatString(321 + 282 / (count + 1) * (i + 1) + G_SIZE / 2, 275 + RESOURCES * I_SIZE + 12, BLACK, "%d", tileInfo.goods[g[i]]);
			}
		}
	}

	for (int i = 0; i < BUILDINGS; i++){
		g_building.Draw(450 - B_SIZE / 2 + i * 150, 60, i + tileInfo.town * BUILDINGS);
	}

	PutBuildingButton(430, 60, 0, tileInfo.built[0]);
	PutBuildingButton(580, 60, 1, tileInfo.built[1]);
	PutBuildingButton(730, 60, 2, tileInfo.built[2]);

	switch (tileInfo.town){
	case FARM:
		g_SB.Draw(430, 120, 0);
		if (!tileInfo.built[3]){
			g_shadeS.Draw(430, 120);
		}

		g_SB.Draw(580, 120, 4);
		if (!tileInfo.built[4]) {
			g_shadeS.Draw(580, 120);
		}
		break;

	case COMM:
		if (!townInfo.onlyOne[T_HALL] || tileInfo.built[3]) {
			g_SB.Draw(430, 120, 1);
		}
		if (!tileInfo.built[3] && !townInfo.onlyOne[T_HALL]){
			g_shadeS.Draw(430, 120);
		}

		if (!townInfo.onlyOne[COURT] || tileInfo.built[4]) {
			g_SB.Draw(580, 120, 5);
		}
		if (!tileInfo.built[4] && !townInfo.onlyOne[COURT]) {
			g_shadeS.Draw(580, 120);
		}
		break;

	case MINE_G:
	case MINE_I:
		g_SB.Draw(580, 120, 2);
		if (!tileInfo.built[4]) {
			g_shadeS.Draw(580, 120);
		}

	case MINE_S:
		g_SB.Draw(430, 120, 3);
		if (!tileInfo.built[3]){
			g_shadeS.Draw(430, 120);
		}
		break;

	default:
		break;
	}
	
	DrawBuildings(430, 60, 0);
	DrawBuildings(580, 60, 1);
	DrawBuildings(730, 60, 2);
	
	if (tileInfo.townLv + 1 < tileInfo.devLim){
		DrawDev(670 + GRID + 5, 270/*, tileInfo.townLv + 1*/);
		PutDevButton(670, 270/*, tileInfo.townLv + 1*/);

		if (tData.bufItem[tileInfo.town][1] || tData.bufItem[tileInfo.town][2]) {
			DrawString(800, 235, "道具の活用", BLACK);
		}
		if (tData.bufItem[tileInfo.town][1]) {
			PutItemB(820, 260, 1);
			if (tData.bufItem[tileInfo.town][2]) {
				PutItemB(820, 260 + G_SIZE + 25, 2);
			}
			DrawItemInfo(820, 260, (int)(tileInfo.town), 1);
			if (tData.bufItem[tileInfo.town][2]) {
				DrawItemInfo(820, 260 + G_SIZE + 25, (int)(tileInfo.town), 2);
			}
		}
		else if (tData.bufItem[tileInfo.town][2]) {
			PutItemB(820, 260, 2);
			DrawItemInfo(820, 260, (int)(tileInfo.town), 2);
		}
	}
	else {
		DrawString(660, 280, "これ以上開発できません", BLACK);

		if (tData.bufItem[tileInfo.town][1] || tData.bufItem[tileInfo.town][2]) {
			DrawString(710, 310, "道具の活用", BLACK);
		}
		if (tData.bufItem[tileInfo.town][1]) {
			if (tData.bufItem[tileInfo.town][2]) {
				PutItemB(710, 330, 1);
				PutItemB(750, 330, 2);
				DrawItemInfo(710, 330, (int)(tileInfo.town), 1);
				DrawItemInfo(750, 330, (int)(tileInfo.town), 2);
			}
			else {
				PutItemB(730, 330, 1);
				DrawItemInfo(730, 330, (int)(tileInfo.town), 1);
			}
		}
		else if (tData.bufItem[tileInfo.town][2]) {
			PutItemB(730, 330, 2);
			DrawItemInfo(730, 330, (int)(tileInfo.town), 2);
		}
	}

	bool flag = false;
	switch (tileInfo.town){
	case FARM:
		if (/*townInfo.devSum >= 10 &&*/ tileInfo.townLv >= 4){
			PutSpBuildButton(430, 120, 0, 3, tileInfo.built[3]);
			DrawSBuildings(430, 120, 0, true, tileInfo.built[3]);
		}else{
			DrawSBuildings(430, 120, 0, false, tileInfo.built[3]);
		}
		if (tileInfo.townLv >= 7) {
			PutSpBuildButton(580, 120, 4, 4, tileInfo.built[4]);
			DrawSBuildings(580, 120, 4, true, tileInfo.built[4]);
		}
		else {
			DrawSBuildings(580, 120, 4, false, tileInfo.built[4]);
		}
		break;

	case COMM:
		if (!townInfo.onlyOne[T_HALL]){
			if (townInfo.towns >= 5 && tileInfo.townLv >= 4){
				PutSpBuildButton(430, 120, 1, 3, tileInfo.built[3]);
				DrawSBuildings(430, 120, 1, true, tileInfo.built[3]);
			}else{
				DrawSBuildings(430, 120, 1, false, tileInfo.built[3]);
			}
		}
		else if (tileInfo.built[3]) {
			DrawSBuildings(430, 120, 1, true, tileInfo.built[3]);
		}

		if (!townInfo.onlyOne[COURT]) {
			if (townInfo.towns >= 10 && tileInfo.townLv >= 4) {
				PutSpBuildButton(580, 120, 5, 4, tileInfo.built[4]);
				DrawSBuildings(580, 120, 5, true, tileInfo.built[4]);
			}
			else {
				DrawSBuildings(580, 120, 5, false, tileInfo.built[4]);
			}
		}
		else if (tileInfo.built[4]) {
			DrawSBuildings(580, 120, 5, false, tileInfo.built[4]);
		}
		break;

	case MINE_G:
	case MINE_I:
		if (tileInfo.townLv >= 4 && tileInfo.built[0]) {
			PutSpBuildButton(580, 120, 2, 4, tileInfo.built[4]);
			DrawSBuildings(580, 120, 2, true, tileInfo.built[4]);
		}
		else {
			DrawSBuildings(580, 120, 2, false, tileInfo.built[4]);
		}

	case MINE_S:
		if (townInfo.maxMines >= 5){
			PutSpBuildButton(430, 120, 3, 3, tileInfo.built[3]);
			DrawSBuildings(430, 120, 3, true, tileInfo.built[3]);
		}
		else{
			DrawSBuildings(430, 120, 3, false, tileInfo.built[3]);
		}
		break;

	default:
		break;
	}

	if (tileInfo.townLv >= 1){
		DrawDG(460 + GRID + 5, 450/*, tileInfo.townLv + 1*/);
		PutDGButton(460, 470/*, tileInfo.townLv + 1*/);
	}
	
	DrawString(635, 465, "元の地形", BLACK);
	g_tile.Draw(640, 490, tileInfo.terrain);
	DrawString(715, 465, "▼壊す", BLACK);
	g_demoL[0].Draw(720, 490);
	if (clickDemo){
		g_demoL[1].Draw(720, 490);
	}

	if (!open){
		PutRemoveButton(720, 490);
	}

	open = false;
}

void STownBox::UpDate(STown town, STile tile) {
	I_UpDate();
	townInfo = town;
	tileInfo = tile;
	clickDemo = false;

	if (tileInfo.town == MINE_S && tileInfo.built[3]) {
		search = true;
	}
}

/////

CPicture SPastureBox::g_boxP;
CPicture SPastureBox::g_pasture;
CPicture SPastureBox::g_demoP;

SPastureBox::SPastureBox(STile tile, int m, int n) {
	g_boxP.Load("Chikuwa3/Box_P.png");
	//g_pasture.Load();
	g_demoP.Load("Chikuwa3/Demo_P.png");
	g_pasture.Load("Chikuwa3/Pasture.png", TERRAINS, 3, GRID, GRID, TERRAINS * 3);

	money = m;
	num = n;
	tileInfo = tile;
}

void SPastureBox::DrawPB() {
	g_boxP.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	DrawFormatString(470, 125, BLACK, "%s", pData.name[num]);
	g_pasture.Draw(470, 150, tileInfo.terrain + num * TERRAINS);

	DrawString(670, 100, "産出", BLACK);
	for (int i = 0; i < RESOURCES; i++) {
		g_resource.Draw(670, 125 + I_SIZE * i, i);
		DrawFormatString(670 + I_SIZE + 5, 127 + I_SIZE * i, BLACK, "%d", pData.income[num][i]);
	}
	g_goods.Draw(670, 125 + I_SIZE * RESOURCES, pData.goods[num]);

	g_demoP.Draw(570, 400);
	DrawString(575, 380, "▼壊す", BLACK);
	DrawString(570, 465, "コスト", BLACK);
	g_resource.Draw(570, 490, MONEY);

	if (money >= 30) {
		DrawString(600, 492, "30", BLACK);

		if (Event.LMouse.GetClick(569, 399, 630, 460) && !open) {
			mode = DEMO;
		}
	}
	else {
		DrawString(600, 492, "30", RED);
	}

	open = false;
}

/////

CPicture SRiverBox::g_boxR;
CPicture SRiverBox::g_RB;
CPicture SRiverBox::g_buildL;
CPicture SRiverBox::g_demoL;

SRiverBox::SRiverBox(STown town, STile tile) {
	g_boxR.Load("Chikuwa3/Box_R.png");
	g_RB.Load("Chikuwa3/RBuilding.png", R_BUILDS, 1, RB_SIZE, RB_SIZE, R_BUILDS);
	g_buildL.Load("Chikuwa3/BuildL.png");
	g_demoL.Load("Chikuwa3/DemoL.png");

	townInfo = town;
	tileInfo = tile;
	buildNum = 0;
}

bool SRiverBox::CheckRBEnough(int bNum) {
	for (int i = 0; i < RESOURCES; i++) {
		if (townInfo.resource[i] < rbData.cost[bNum][i]) {
			return false;
		}
	}

	return true;
}

void SRiverBox::PutRBButton(int x, int y, int bNum, bool isBuilt) {
	g_RB.Draw(x, y, bNum);

	if(isBuilt){
		if (Event.RMouse.GetOn(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
			g_demoL.Draw(x, y);
		}

		if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE) && townInfo.resource[MONEY] >= rbData.cost[bNum][MONEY] / 2) {
			mode = DEMO;
			buildNum = bNum;
		}
	}
	else {

		g_shadeL.Draw(x, y);
		if (CheckRBEnough(bNum)) {
			if (Event.RMouse.GetOn(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
				g_buildL.Draw(x, y);
			}

			if (!open && Event.LMouse.GetClick(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
				mode = BUILD;
				buildNum = bNum;
			}
		}
	}
}

void SRiverBox::DrawDataBox(int x, int y, int bNum) {
	int mx = Event.RMouse.GetX();
	int my = Event.RMouse.GetY();
	int lim = WINDOW_WIDTH - 246;
	if (Event.RMouse.GetOn(x - 1, y - 1, x + RB_SIZE, y + RB_SIZE)) {
		if (mx < lim) {
			DrawBox(mx, my, mx + 230, my + 140, LIGHTYELLOW, true);

			DrawFormatString(mx + 5, my + 5, BLACK, "%s", rbData.name[bNum]);
			if (!tileInfo.built[bNum]) {
				DrawString(mx + 5, my + I_SIZE + 5, "建設コスト", BLACK);
				for (int i = 0; i < RESOURCES; i++) {
					g_resource.Draw(mx + 5 + i * 55, my + I_SIZE * 2 + 5, i);
					if (townInfo.resource[i] < rbData.cost[bNum][i]) {
						DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", rbData.cost[bNum][i]);
					}
					else {
						DrawFormatString(mx + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", rbData.cost[bNum][i]);
					}
				}
			}
			else {
				DrawString(mx + 5, my + I_SIZE + 5, "撤去コスト", BLACK);
				g_resource.Draw(mx + 5, my + I_SIZE * 2 + 5, MONEY);
				if (townInfo.resource[MONEY] < rbData.cost[bNum][MONEY] / 2) {
					DrawFormatString(mx + 10 + I_SIZE, my + I_SIZE * 2 + 7, RED, "%d", rbData.cost[bNum][MONEY] / 2);
				}
				else {
					DrawFormatString(mx + 10 + I_SIZE, my + I_SIZE * 2 + 7, BLACK, "%d", rbData.cost[bNum][MONEY] / 2);
				}
			}

			DrawString(mx + 5, my + I_SIZE * 3 + 10, "効果", BLACK);
			for (int i = 0; i < 2; i++) {
				DrawFormatString(mx + 5, my + I_SIZE * 4 + i * I_SIZE + 10, BLACK, "%s", rbData.exp[bNum][i]);
			}
		}
		else {
			DrawBox(lim, my, lim + 230, my + 140, LIGHTYELLOW, true);

			DrawFormatString(lim + 5, my + 5, BLACK, "%s", rbData.name[bNum]);
			if (!tileInfo.built[bNum]) {
				DrawString(lim + 5, my + I_SIZE + 5, "建設コスト", BLACK);
				for (int i = 0; i < RESOURCES; i++) {
					g_resource.Draw(lim + 5 + i * 55, my + I_SIZE * 2 + 5, i);
					if (townInfo.resource[i] < rbData.cost[bNum][i]) {
						DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, RED, "%d", rbData.cost[bNum][i]);
					}
					else {
						DrawFormatString(lim + 10 + I_SIZE + i * 55, my + I_SIZE * 2 + 7, BLACK, "%d", rbData.cost[bNum][i]);
					}
				}
			}
			else {
				DrawString(lim + 5, my + I_SIZE + 5, "撤去コスト", BLACK);
				g_resource.Draw(lim + 5, my + I_SIZE * 2 + 5, MONEY);
				if (townInfo.resource[MONEY] < rbData.cost[bNum][MONEY] / 2) {
					DrawFormatString(lim + 10 + I_SIZE, my + I_SIZE * 2 + 7, RED, "%d", rbData.cost[bNum][MONEY] / 2);
				}
				else {
					DrawFormatString(lim + 10 + I_SIZE, my + I_SIZE * 2 + 7, BLACK, "%d", rbData.cost[bNum][MONEY] / 2);
				}
			}

			DrawString(lim + 5, my + I_SIZE * 3 + 10, "効果", BLACK);
			for (int i = 0; i < 2; i++) {
				DrawFormatString(lim + 5, my + I_SIZE * 4 + i * I_SIZE + 10, BLACK, "%s", rbData.exp[bNum][i]);
			}
		}
	}
}

void SRiverBox::DrawRB(ETown u, ETown d, ETown l, ETown r) {
	g_boxR.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	if (u == FARM || d == FARM || l == FARM || r == FARM) {
		PutRBButton(470, 65, 0, tileInfo.built[0]);
	}
	/*if ((u != WILD && u != COMM) || (d != WILD && d != COMM) || (l != WILD && l != COMM) || (r != WILD && r != COMM)) {
		PutRBButton(580, 65, 1, tileInfo.built[1]);
	}*/
	if (u == COMM || d == COMM || l == COMM || r == COMM) {
		PutRBButton(670, 65, 2, tileInfo.built[2]);
	}

	if (u == FARM || d == FARM || l == FARM || r == FARM) {
		DrawDataBox(470, 65, 0);
	}
	/*if ((u != WILD && u != COMM) || (d != WILD && d != COMM) || (l != WILD && l != COMM) || (r != WILD && r != COMM)) {
		DrawDataBox(580, 65, 1);
	}*/
	if (u == COMM || d == COMM || l == COMM || r == COMM) {
		DrawDataBox(670, 65, 2);
	}

	open = false;
}

void SRiverBox::UpDate(STown town, STile tile) {
	I_UpDate();
	townInfo = town;
	tileInfo = tile;
	buildNum = 0;
}

/////

CPicture STradeBox::g_boxG;
CPicture STradeBox::g_arrow;

STradeBox::STradeBox(STown town) {
	townInfo = town;

	g_boxG.Load("Chikuwa3/Box_G.png");
	g_arrow.Load("Chikuwa3/Arrow.png");
}

void STradeBox::DrawArrow(int x, int y, int n) {
	g_arrow.Draw(x, y);
	DrawString(x + 11, y + 25, "消", BLACK);
	DrawString(x + 11, y + 42, "費", BLACK);
	DrawVString(x + 10, y + 56, "：", BLACK);
	DrawFormatString(x + 15, y + 69, BLACK, "%d", n);
}

void STradeBox::DrawTB() {
	g_boxG.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	for (int i = 0; i < GOODS / 5; i++) {
		DrawString(330, 75 + i * 120, "価値", ORANGE);
		DrawString(330, 100 + i * 120, "生産", DARKGREEN);
		DrawString(330, 120 + i * 120, "消費", RED);
	}
	for (int i = 2; i < GOODS; i++) {
		if ((i != 21 || townInfo.spFarm) && i != 5 && i != 4) {
			int show = i - 2;
			g_goods.Draw(390 + show % 5 * 100, 30 + show / 5 * 120, i);
			DrawFormatString(392 + show % 5 * 100, 75 + show / 5 * 120, ORANGE, "%.2lf", gData.value[i]);
			DrawCenterString(409 + show % 5 * 100, 100 + show / 5 * 120, DARKGREEN, "%d", townInfo.goodsPro[i]);
			DrawCenterString(409 + show % 5 * 100, 120 + show / 5 * 120, RED, "%d", townInfo.goodsCon[i]);
		}
	}

	DrawArrow(440, 40, 3);
	DrawArrow(440, 160, 1);
	DrawArrow(540, 160, 3);
	DrawArrow(740, 160, 2);
	DrawArrow(440, 280, 2);
	DrawArrow(640, 280, 2);
	DrawArrow(740, 280, 1);
	DrawArrow(440, 400, 2);

	g_goods.Draw(400, 510, 1);
	DrawFormatString(450, 522, CYAN, "%d", townInfo.goodsPro[1]);
	double a = 1.0 - ((double)townInfo.goodsPro[1]) / 100;
	DrawFormatString(500, 512, DARKGREEN, "交易収入:%.2lf × %.2lf = %.2lf", townInfo.exSum, 1 + ((double)townInfo.goodsPro[1]) / 100, townInfo.exFin);
	DrawFormatString(500, 537, RED, "交易支出:%.2lf × %.2lf = %.2lf", townInfo.inSum, a * 2.5, townInfo.inFin);

	open = false;
}

/////

CPicture SBuyBox::g_boxB;
CPicture SBuyBox::g_brbut;

int mod[RESOURCES] = { 0, 4, 5, 5 };

SBuyBox::SBuyBox(STown town) {
	townInfo = town;
	bought = false;
	rType = MONEY;
	amount = 0;
	cost = 0;

	g_boxB.Load("Chikuwa3/Box_B.png");
	g_brbut.Load("Chikuwa3/BuyRButton.png", RESOURCES, 1, BRB_SIZE, BRB_SIZE, RESOURCES);
}

void SBuyBox::DrawBB(int money) {
	g_boxB.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	bought = false;
	rType = MONEY;
	amount = 0;
	cost = 0;

	DrawFormatString(300 + 1 * 150 - BRB_SIZE / 2, 170, ORANGE, "コスト:100");
	for (int i = 1; i < RESOURCES; i++) {
		PutBuyButton(300 + i * 150 - BRB_SIZE / 2, 200, 0, (EResource)i, 100, 100 / mod[i], money);
	}

	open = false;
}

void SBuyBox::PutBuyButton(int x, int y, int trade, EResource type, int c, int a, int money) {
	DrawFormatString(x, y + BRB_SIZE + 5, BLACK, "買い入れ量:%d", a);

	if (money >= c) {
		if (Event.LMouse.GetClick(x - 1, y - 1, x + BRB_SIZE, y + BRB_SIZE)) {
			bought = true;
			rType = type;
			amount = a;
			cost = c;
		}

		g_brbut.Draw(x, y, type);
	}
	else {
		g_brbut.Draw(x, y, type);

		DrawString(x, y + BRB_SIZE + 30, "お金が足りません", RED);
	}
}

/////

CPicture SSpABox::g_boxs;
CPicture SSpABox::g_lvUp;

SSpABox::SSpABox(STown town, STile tile) {
	g_boxs.Load("Chikuwa3/Box_S.png");
	g_lvUp.Load("Chikuwa3/Dev.png");

	townInfo = town;
	tileInfo = tile;
}

bool SSpABox::CheckDAble(int type) {
	switch (type) {
	case 6:
		if (townInfo.tTypePop[PAS_S] + townInfo.tTypePop[PAS_C] + townInfo.tTypePop[PAS_P] >= tileInfo.saLv * 10 + 10 && townInfo.goodsPro[2] - townInfo.goodsCon[2] >= 7 + tileInfo.saLv) {
			for (int i = 0; i < RESOURCES; i++) {
				if (townInfo.resource[i] < saData.cost[6][i] / 2) {
					return false;
				}
			}
			return true;
		}
		break;

	case 7:
		for (int i = 0; i < RESOURCES; i++) {
			if (townInfo.resource[i] < saData.cost[7][i] / 2) {
				return false;
			}
		}
		return true;
		break;

	case 8:
		if (townInfo.goodsPro[20] - townInfo.goodsCon[20] >= 1 && townInfo.goodsPro[17] - townInfo.goodsCon[17] >= 4) {
			for (int i = 0; i < RESOURCES; i++) {
				if (townInfo.resource[i] < saData.cost[8][i] / 2) {
					return false;
				}
			}
			return true;
		}
		break;

	default:
		break;
	}

	return false;
}

void SSpABox::PutDevB(int x, int y) {
	if (!open && Event.LMouse.GetClick(x + 1, y + 1, x + 50, y + 50)) {
		mode = DEV;
	}
}

void SSpABox::DrawSB() {
	g_boxs.Draw(WINDOW_WIDTH - WINDOW_HEIGHT, 0);
	DrawIB();

	DrawFormatString(500, 50, BLACK, "%s", saData.name[tileInfo.saNum]);
	DrawFormatString(520, 75, BLACK, "規模：%d", tileInfo.saLv + 1);
	if (tileInfo.saNum == 6) {
		DrawString(500, 100, "産出", BLACK);
		g_goods.Draw(500, 125, 21);
		DrawFormatString(500 + G_SIZE + 5, 136, BLACK, "%d", tileInfo.saLv + 1);

		DrawString(600, 100, "消費", BLACK);
		g_resource.Draw(600, 135, FOOD);
		DrawFormatString(600 + I_SIZE + 5, 137, BLACK, "%d", tileInfo.saLv * 10 + 10);

		if (tileInfo.saLv < 4) {
			g_lvUp.Draw(575, 330);
			if (CheckDAble(6)){
				PutDevB(575, 330);
			}

			DrawString(450, 400, "条件", BLACK);
			if (townInfo.tTypePop[PAS_S] + townInfo.tTypePop[PAS_C] + townInfo.tTypePop[PAS_P] >= 20 + (tileInfo.saLv + 1) * 5) {
				DrawFormatString(450, 425, BLACK, "牧場町人口：%d", 20 + (tileInfo.saLv + 1) * 5);
			}
			else {
				DrawFormatString(450, 425, RED, "牧場町人口：%d", 20 + (tileInfo.saLv + 1) * 5);
			}
			if (townInfo.goodsPro[2] - townInfo.goodsCon[2] >= 7 + tileInfo.saLv) {
				DrawFormatString(450, 445, BLACK, "交易品目：小麦×%d", 7 + tileInfo.saLv);
			}
			else {
				DrawFormatString(450, 445, RED, "交易品目：小麦×%d", 7 + tileInfo.saLv);
			}

			DrawString(610, 400, "コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++) {
				g_resource.Draw(610, 400 + (i + 1) * I_SIZE, i);
				int cost = saData.cost[6][i] / 2;
				if (townInfo.resource[i] < cost) {
					DrawFormatString(610 + I_SIZE + 5, 400 + (i + 1) * I_SIZE + 2, RED, "%d", cost);
				}
				else {
					DrawFormatString(610 + I_SIZE + 5, 400 + (i + 1) * I_SIZE + 2, BLACK, "%d", cost);
				}
			}

			DrawString(700, 400, "効果", BLACK);
			g_goods.Draw(700, 425, 21);
			DrawString(700 + G_SIZE + 5, 436, "+1", BLACK);
			g_resource.Draw(700, 425 + G_SIZE + 5, FOOD);
			DrawString(700 + I_SIZE + 5, 425 + G_SIZE + 7, "-10", BLACK);
		}
	}
	if (tileInfo.saNum == 7) {
		DrawString(500, 100, "産出", BLACK);
		g_goods.Draw(500, 125, 20);
		DrawFormatString(500 + G_SIZE + 5, 136, BLACK, "%d", min(tileInfo.saLv + 1, townInfo.forests));

		DrawString(600, 100, "森林伐採", BLACK);
		DrawFormatString(600, 135, BLACK, "１ターンに%dマス", min(tileInfo.saLv + 1, townInfo.forests));

		if (tileInfo.townLv < 4) {
			g_lvUp.Draw(575, 330);
			if (CheckDAble(7)) {
				PutDevB(575, 330);
			}
			DrawString(460, 400, "コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++) {
				g_resource.Draw(460, 400 + (i + 1) * I_SIZE, i);
				int cost = saData.cost[7][i] / 2;
				if (townInfo.resource[i] < cost) {
					DrawFormatString(460 + I_SIZE + 5, 400 + (i + 1) * I_SIZE + 2, RED, "%d", cost);
				}
				else {
					DrawFormatString(460 + I_SIZE + 5, 400 + (i + 1) * I_SIZE + 2, BLACK, "%d", cost);
				}
			}
			DrawString(550, 400, "効果", BLACK);
			g_goods.Draw(550, 425, 20);
			DrawString(550 + G_SIZE + 5, 436, "+1", BLACK);
			DrawString(550, 430 + G_SIZE, "ターン毎の森林伐採 +1マス", BLACK);
		}
	}
	if (tileInfo.saNum == 8) {
		DrawString(500, 100, "産出", BLACK);
		g_goods.Draw(500, 125, 18);
		DrawFormatString(500 + G_SIZE + 5, 136, BLACK, "%d", townInfo.tex);

		DrawString(600, 100, "消費", BLACK);
		g_goods.Draw(600, 125, 20);
		DrawFormatString(600 + G_SIZE + 5, 137, BLACK, "%d", townInfo.tex / 2);

		if (tileInfo.saLv < 4) {
			g_lvUp.Draw(575, 330);
			if (CheckDAble(8)) {
				PutDevB(575, 330);
			}

			DrawString(400, 400, "条件", BLACK);
			if (townInfo.goodsPro[20] - townInfo.goodsCon[20] >= 1) {
				DrawString(400, 425,"交易品目(道具)：木炭+1", BLACK);
			}
			else {
				DrawString(400, 425, "交易品目(道具)：木炭+1", RED);
			}
			if (townInfo.goodsPro[17] - townInfo.goodsCon[17] >= 6) {
				DrawString(400, 445, "交易品目：羊毛+6", BLACK);
			}
			else {
				DrawString(400, 445, "交易品目：羊毛+6", RED);
			}

			DrawString(610, 400, "コスト", BLACK);
			for (int i = 0; i < RESOURCES; i++) {
				g_resource.Draw(610, 400 + (i + 1) * I_SIZE, i);
				int cost = saData.cost[8][i] / 2;
				if (townInfo.resource[i] < cost) {
					DrawFormatString(610 + I_SIZE + 5, 400 + (i + 1) * I_SIZE + 2, RED, "%d", cost);
				}
				else {
					DrawFormatString(610 + I_SIZE + 5, 400 + (i + 1) * I_SIZE + 2, BLACK, "%d", cost);
				}
			}

			DrawString(700, 400, "効果", BLACK);
			g_goods.Draw(700, 425, 18);
			DrawString(700 + G_SIZE + 5, 436, "+3", BLACK);
			g_goods.Draw(700, 425 + G_SIZE, 20);
			DrawString(700 + G_SIZE + 5, 425 + G_SIZE + 7, "-1", BLACK);
		}
	}

	open = false;
}

void SSpABox::UpDate(STown town, STile tile) {
	I_UpDate();
	townInfo = town;
	tileInfo = tile;

	if (tileInfo.saNum == 8) {
		townInfo.goodsCon[17] += 6;
		townInfo.goodsCon[20] += 1;
	}
}
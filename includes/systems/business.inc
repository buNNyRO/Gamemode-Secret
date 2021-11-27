#include < YSI\y_hooks >

#define MAX_BUSINESSES 				(30)

new businessName[17][] = {
	"NULL",
	"Banca",
	"Market 24/7",
	"Clothes Store",
	"Casino",
	"Bar",
	"Club",
	"Pizza",
	"Burger",
	"Cluckin's bell",
	"GYM",
	"Gun Shop",
	"Sex Shop",
	"Benzinarie",
	"Tunning",
	"PNS",
	"CNN"
};

new Iterator:serverBusinesses<MAX_BUSINESSES>;
enum bInfo
{
	bID,
	bOwner[30],
	bOwnerID,
	bModel,
	bPrice,
	Float: bEX,
	Float: bEY,
	Float: bEZ,
	Float: bIX,
	Float: bIY,
	Float: bIZ,
	bInterior,
	Text3D:BizzLabel,
	bPickup,
	bEArea,
	bIArea
};

new businessVariables[MAX_BUSINESSES][bInfo];

function loadDynamicBIZZ() {
	if(!cache_num_rows()) return print("SQL >> No rows found in 'businesses' table.");
	for(new i = 1; i < cache_num_rows() + 1; i++) {
		cache_get_value_name_int(i-1, "id", businessVariables[i][bID]);
		cache_get_value_name(i-1, "owner", businessVariables[i][bOwner]);
		cache_get_value_name(i-1, "ownerid", businessVariables[i][bOwnerID]);
		cache_get_value_name_int(i-1, "model", businessVariables[i][bModel]);
		cache_get_value_name_int(i-1, "price", businessVariables[i][bPrice]);
		cache_get_value_name_float(i-1, "eX", businessVariables[i][bEX]); 
		cache_get_value_name_float(i-1, "eY", businessVariables[i][bEY]); 
		cache_get_value_name_float(i-1, "eZ", businessVariables[i][bEZ]); 
		cache_get_value_name_float(i-1, "iX", businessVariables[i][bIX]); 
		cache_get_value_name_float(i-1, "iY", businessVariables[i][bIY]); 
		cache_get_value_name_float(i-1, "iZ", businessVariables[i][bIZ]); 
		cache_get_value_name_int(i-1, "interior", businessVariables[i][bInterior]);

		businessVariables[i][bEArea] = CreateArea(businessVariables[i][bEX], businessVariables[i][bEY], businessVariables[i][bEZ], 3.0, 0, 0);
		businessVariables[i][bIArea] = CreateArea(businessVariables[i][bIX], businessVariables[i][bIY], businessVariables[i][bIZ], 3.0, businessVariables[i][bInterior], businessVariables[i][bID]);
		businessLabel(i);
		if(!Iter_Contains(serverBusinesses, i)) Iter_Add(serverBusinesses, i);
	}
	printf("SQL >> Loaded %d businesses.", Iter_Count(serverBusinesses));
	return 1;
}

CMD:gotobiz(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] < 1) return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");
	new id;
	if(sscanf(params,"d", id)) return SCM(playerid, -1, SYNTAX"/gotobiz [id]");
	SetPlayerPos(playerid, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ]);
	return 1;
}

CMD:movebiz(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] < 5) return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");
	new id;
	if(sscanf(params,"d", id)) return SCM(playerid, -1, SYNTAX"/gotobiz [id]");
	new Float: x, Float: y, Float: z;
	GetPlayerPos(playerid, x, y, z);
	businessVariables[id][bEX] = x;
	businessVariables[id][bEY] = y;
	businessVariables[id][bEZ] = z;
	DestroyArea(businessVariables[id][bEArea]);
	DestroyDynamicPickup(businessVariables[id][bPickup]);
	DestroyDynamic3DTextLabel(businessVariables[id][BizzLabel]);
	businessVariables[id][bEArea] = CreateArea(businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 3.0, 0, 0);
	businessLabel(id);
	mysql_tquery(SQL, va_return("UPDATE `businesses` SET `eX` = '%f' ,`eY` = '%f' ,`eZ` = '%f' WHERE `id` = '%d'", businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], businessVariables[id][bID]), "", "" );	
	return 1;
}

CMD:createbiz(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] < 6) return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");
	new price, model[30];
	if(sscanf(params,"s[30]d", model, price)) {
		SCM(playerid, -1, SYNTAX"/createbiz [model] [price]");
		SendClientMessage(playerid, -1, "[{DDE02B}-{FFFFFF}] banca, market, clothes, casino, bar, club, pizza, burger, cluckin, gym, gunshop, sexshop");
		SendClientMessage(playerid, -1, "[{DDE02B}-{FFFFFF}] benzinarie, tunning, pns, cnn");
		return 1;
	}
	new Float: x, Float: y, Float: z, id = Iter_Count(serverBusinesses)+1;
	GetPlayerPos(playerid, x, y, z);
	switch(YHash(model)) {
		case _H<banca>: businessVariables[id][bIX] = 2306, businessVariables[id][bIY] = -16, businessVariables[id][bIZ] = 27, businessVariables[id][bInterior] = 0, businessVariables[id][bModel] = 1;
		case _H<market>: businessVariables[id][bIX] = -31.0246, businessVariables[id][bIY] = -91.3283, businessVariables[id][bIZ] = 1003.55, businessVariables[id][bInterior] = 18, businessVariables[id][bModel] = 2;
		case _H<clothes>: businessVariables[id][bIX] = 207.0009, businessVariables[id][bIY] = -139.9626, businessVariables[id][bIZ] = 1003.5078, businessVariables[id][bInterior] = 3, businessVariables[id][bModel] = 3;
		case _H<casino>: businessVariables[id][bIX] = 1133.1929, businessVariables[id][bIY] = -15.3691, businessVariables[id][bIZ] = 1000.6797, businessVariables[id][bInterior] = 12, businessVariables[id][bModel] = 4;
		case _H<bar>: businessVariables[id][bIX] = 501.8877, businessVariables[id][bIY] = -67.8034, businessVariables[id][bIZ] = 998.7578, businessVariables[id][bInterior] = 11, businessVariables[id][bModel] = 5;
		case _H<club>: businessVariables[id][bIX] = 493.3731, businessVariables[id][bIY] = -24.4225, businessVariables[id][bIZ] = 1000.6797, businessVariables[id][bInterior] = 17, businessVariables[id][bModel] = 6;
		case _H<pizza>: businessVariables[id][bIX] = 372.4742, businessVariables[id][bIY] = -132.5471, businessVariables[id][bIZ] = 1001.4922, businessVariables[id][bInterior] = 5, businessVariables[id][bModel] = 7;
		case _H<burger>: businessVariables[id][bIX] = 363.2178, businessVariables[id][bIY] = -74.9702, businessVariables[id][bIZ] = 1001.5078, businessVariables[id][bInterior] = 10, businessVariables[id][bModel] = 8;
		case _H<cluckin>: businessVariables[id][bIX] = 364.9319, businessVariables[id][bIY] = -11.3886, businessVariables[id][bIZ] = 1001.8516, businessVariables[id][bInterior] = 0, businessVariables[id][bModel] = 9;
		case _H<gym>: businessVariables[id][bIX] = 772.3702, businessVariables[id][bIY] = -5.1847, businessVariables[id][bIZ] = 1000.7288, businessVariables[id][bInterior] = 5, businessVariables[id][bModel] = 10;
		case _H<gunshop>: businessVariables[id][bIX] = 296.9385, businessVariables[id][bIY] = -111.6911, businessVariables[id][bIZ] = 1001.5156, businessVariables[id][bInterior] = 6, businessVariables[id][bModel] = 11;
		case _H<sexshop>: businessVariables[id][bIX] = -100.3728, businessVariables[id][bIY] = -24.6370, businessVariables[id][bIZ] = 1000.7188, businessVariables[id][bInterior] = 3, businessVariables[id][bModel] = 12;
		case _H<benzinarie>: businessVariables[id][bModel] = 13;
		case _H<tunning>: businessVariables[id][bModel] = 14;
		case _H<pns>: businessVariables[id][bModel] = 15; 
		case _H<cnn>: businessVariables[id][bModel] = 16;
		default: return SCM(playerid, -1, EROR"Nu ai pus nici un tip de companie.");
	}
	businessVariables[id][bID] = id;
	businessVariables[id][bPrice] = price;
	businessVariables[id][bEX] = x;
	businessVariables[id][bEY] = y;
	businessVariables[id][bEZ] = z;

	new ownerString[10];
	format(ownerString, sizeof ownerString, businessVariables[id][bOwner], "AdmBot");

	businessVariables[id][bEArea] = CreateArea(businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 3.0, 0, 0);
	businessVariables[id][bIArea] = CreateArea(businessVariables[id][bIX], businessVariables[id][bIY], businessVariables[id][bIZ], 3.0, businessVariables[id][bInterior], businessVariables[id][bID]);
	businessLabel(id);
	if(!Iter_Contains(serverBusinesses, id)) Iter_Add(serverBusinesses, id);
	
	dbQuery[0] = (EOS);
	mysql_format(SQL, dbQuery, sizeof(dbQuery), "INSERT INTO `businesses` (`owner`, `model`, `price`, `eX`, `eY`, `eZ`, `iX`, `iY`, `iZ`, `interior`) VALUES ('AdmBot', '%d', '%d', '%f', '%f', '%f', '%f', '%f', '%f', '%d')", businessVariables[id][bModel], businessVariables[id][bPrice], businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], businessVariables[id][bIX], businessVariables[id][bIY], businessVariables[id][bIZ], businessVariables[id][bInterior]);	
	mysql_tquery(SQL, dbQuery, "", "");	
	
	va_SendClientMessage(playerid, -1, "{DDE02B}Compania %d de tip %s a fost creata.", businessVariables[id][bID], businessName[businessVariables[id][bModel]]);
	return 1;
}

CMD:buybiz(playerid, params[]) {
	if(PlayerInfo[playerid][pBizz] != 0) return SCM(playerid, -1, EROR"Esti proprietar-ul unei companii.");
	if(!IsPlayerInAnyArea(playerid)) return SCM(playerid, -1, EROR"Nu esti in aproprierea unei companii.");
	foreach(new i : serverBusinesses) {
		if(GetPlayerArea(playerid) == businessVariables[i][bEArea]) {
			if(GetPlayerMoney(playerid) < businessVariables[i][bPrice]) return SCM(playerid, -1, EROR"Nu ai banii necesari pentru cumpararea companiei.");
				
			PlayerInfo[playerid][pBizz] = i;
			businessVariables[i][bOwnerID] = PlayerInfo[playerid][pSQLID];
			va_SendClientMessage(playerid, -1, "Ai cumparat compania (%d)%s cu %s Lei, felicitari!", businessVariables[i][bID], businessName[businessVariables[i][bOwnerID]], FormatNumber(businessVariables[i][bOwnerID]));
				
			new ownerString[10];
			format(ownerString, sizeof ownerString, businessVariables[i][bOwner], GetName(playerid));
			businessLabel(i);
			
			mysql_tquery(SQL, va_return("UPDATE `businesses` SET `ownerid` = '%d', `owner` = '%s', `price` = '0'", PlayerInfo[playerid][pSQLID], businessVariables[i][bOwner]), "", "");	
			mysql_tquery(SQL, va_return("UPDATE `users` SET `businnes` = '%d'", PlayerInfo[playerid][pBizz]), "", "");	

		}
	}
	return 1;
}

stock IsBizzStatic(id) {
	switch(businessVariables[id][bModel]) {
		case 1..12: return 0;
		case 13..16: return 1;
	}
	return 1;
}

businessLabel(id) {
	switch(businessVariables[id][bModel]) {
		case 1..12: {
			businessVariables[id][bPickup] = CreateDynamicPickup(1239, 1, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 0, 0, -1, 50.0);
			businessVariables[id][BizzLabel] = CreateDynamic3DTextLabel(va_return("{DDE02B}%s{FFFFFF}\nID: {DDE02B}%d{FFFFFF}\nOwner: {DDE02B}%s{FFFFFF}\nPrice: {DDE02B}%s Lei{FFFFFF}", businessName[businessVariables[id][bModel]], businessVariables[id][bID], businessVariables[id][bOwner], FormatNumber(businessVariables[id][bPrice])), 0xFFFFFFFF, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 100.0); 
		}
		case 13: {
			businessVariables[id][bPickup] = CreateDynamicPickup(1650, 1, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 0, 0, -1, 50.0);
			businessVariables[id][BizzLabel] = CreateDynamic3DTextLabel(va_return("{ff533f}%s{FFFFFF}(%d)\nOwner: {ff533f}%s{FFFFFF}\nPrice: {ff533f}%s Lei{FFFFFF}\nApasa {ff533f}H{FFFFFF} pentru a umple rezervorul.", businessName[businessVariables[id][bModel]], businessVariables[id][bID], businessVariables[id][bOwner], FormatNumber(businessVariables[id][bPrice])), 0xFFFFFFFF, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 100.0); 
		}
		case 14: {
			businessVariables[id][bPickup] = CreateDynamicPickup(1238, 1, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 0, 0, -1, 50.0);
			businessVariables[id][BizzLabel] = CreateDynamic3DTextLabel(va_return("{c14443}%s{FFFFFF}(%d)\nOwner: {c14443}%s{FFFFFF}\nPrice: {c14443}%s Lei{FFFFFF}\nApasa {c14443}H{FFFFFF} pentru a intra la mecanic.", businessName[businessVariables[id][bModel]], businessVariables[id][bID], businessVariables[id][bOwner], FormatNumber(businessVariables[id][bPrice])), 0xFFFFFFFF, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 100.0); 
		}
		case 15: {
			businessVariables[id][bPickup] = CreateDynamicPickup(19621, 1, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 0, 0, -1, 50.0);
			businessVariables[id][BizzLabel] = CreateDynamic3DTextLabel(va_return("{791e12}%s{FFFFFF}(%d)\nOwner: {791e12}%s{FFFFFF}\nPrice: {791e12}%s Lei{FFFFFF}\nApasa {791e12}H{FFFFFF} pentru a repara masina.", businessName[businessVariables[id][bModel]], businessVariables[id][bID], businessVariables[id][bOwner], FormatNumber(businessVariables[id][bPrice])), 0xFFFFFFFF, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 100.0); 
		}
		case 16: {
			businessVariables[id][bPickup] = CreateDynamicPickup(1317, 1, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ]-0.5, 0, 0, -1, 50.0);
			businessVariables[id][BizzLabel] = CreateDynamic3DTextLabel(va_return("{a34cc7}%s{FFFFFF}\nOwner: {a34cc7}%s{FFFFFF}\nPrice: {a34cc7}%s Lei{FFFFFF}\nTrebuie sa pui {a34cc7}/ad [text]{FFFFFF} pentru a pune un anunt.", businessName[businessVariables[id][bModel]], businessVariables[id][bOwner], FormatNumber(businessVariables[id][bPrice])), 0xFFFFFFFF, businessVariables[id][bEX], businessVariables[id][bEY], businessVariables[id][bEZ], 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 100.0); 
		}
	}	
	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if(newkeys&GetPVarInt(playerid, "BugKey") && GetPVarInt(playerid, "BugKeyTime") > gettime()) return 1;
	if(newkeys) SetPVarInt(playerid, "BugKey", newkeys), SetPVarInt(playerid, "BugKeyTime", gettime()+1);
	if(newkeys == KEY_SECONDARY_ATTACK) {
	 	foreach(new i : serverBusinesses) {
	 		if(!IsBizzStatic(i)) {
				if(GetPlayerArea(playerid) == businessVariables[i][bIArea]) SetPlayerPos(playerid, businessVariables[i][bEX], businessVariables[i][bEY], businessVariables[i][bEZ]), SetPlayerVirtualWorld(playerid, 0), SetPlayerInterior(playerid, 0);
				else if(GetPlayerArea(playerid) == businessVariables[i][bEArea]) SetPlayerPos(playerid, businessVariables[i][bIX], businessVariables[i][bIY], businessVariables[i][bIZ]), SetPlayerVirtualWorld(playerid, businessVariables[i][bID]), SetPlayerInterior(playerid, businessVariables[i][bInterior]);
			}
		}
	}
	return 1;
}
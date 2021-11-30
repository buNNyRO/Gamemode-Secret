#define server_hostname 			"Outer Banks"
#define server_version  			"v0.0.3"
#define server_version_name  		"blank"
#define server_mapname 				"LV"
#define server_weburl   			"secret.ro"

#include <a_samp>
#include <a_mysql>
#include <streamer>
#include <sscanf2>

#include <easyArea>
#include <easyDialog>
#include <easyCheckpoint>
#include <Pawn.CMD>

#include <YSI\y_va>
#include <YSI\y_timers>
#include <YSI\y_bit>
#include <YSI\y_iterate>
#include <YSI\y_stringhash>

#include "../includes/defines"
#include "../includes/enums"
#include "../includes/variables"
#include "../includes/dialogs"

public OnGameModeInit() {
	SQL = mysql_connect(mysql_host, mysql_user, mysql_pass, mysql_data);
	if(mysql_errno() != 0) print("SQL >> Could not connect to database.");
	else print("SQL >> Database connection established !");

	serverSys();
	serverMaps();
	globalTD();
	
	new h,m,s;
	gettime(h, m, s);
	SetWorldTime(h);

	SetNameTagDrawDistance(20.0);
	EnableStuntBonusForAll(false);
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_OFF);
    ManualVehicleEngineAndLights();
    DisableInteriorEnterExits();
	AllowInteriorWeapons(true);
	UsePlayerPedAnims();

	SendRconCommand("hostname "server_hostname"");
	SetGameModeText(""server_version_name" "server_version"");
	SendRconCommand("mapname "server_mapname"");
	SendRconCommand("weburl "server_weburl"");

	AddPlayerClass(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	return 1;
}

public OnGameModeExit() {
	mysql_close(SQL);
	return 1;
}

public OnPlayerRequestClass(playerid, classid) {
	if(Iter_Contains(serverLogged, playerid)) return SpawnPlayer(playerid);
	return 1;
}

public OnPlayerConnect(playerid) {
	PlayAudioStreamForPlayer(playerid, "https://uploadir.com/u/4d4j2zsd");

	serverRemoveMaps(playerid);
	playerTD(playerid);
	TogglePlayerSpectating(playerid, 1);

	SetSpawnInfo(playerid, 0, 60, 1675.7025,1447.7917,10.7866,269.5700, -1, -1, -1, -1, -1, -1);

	mysql_tquery(SQL, query("SELECT * FROM `users` WHERE `name` = '%s'", GetName(playerid)), "CheckAccount", "d", playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason) {
	Iter_Remove(serverLogged, playerid);
	if(PlayerInfo[playerid][pAdmin] > 0) {
		Iter_Remove(serverAdmins, playerid);
		Iter_Remove(serverStaff, playerid);
	}

	if(PlayerInfo[playerid][pHelper] > 0) {
		Iter_Remove(serverHelpers, playerid);
		Iter_Remove(serverStaff, playerid);
	}

	return 1;
}

public OnPlayerSpawn(playerid) {
	SetPlayerPos(playerid, 1675.7025,1447.7917,10.7866);
	SetPlayerFacingAngle(playerid, 269.5700);
	SetCameraBehindPlayer(playerid);
	SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason) {

	return 1;
}

public OnVehicleSpawn(vehicleid) {

	return 1;
}

public OnVehicleDeath(vehicleid, killerid) {

	return 1;
}

public OnPlayerText(playerid, text[]) {
	if(!Iter_Contains(serverLogged, playerid)) return 1;
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {

	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid) {

	if(vehicleid == PlayerInfo[playerid][pExamVehicle]) examPlayerFailed(playerid);
	
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate) {

	if(newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER) {
		if(IsValidVehicle(PlayerInfo[playerid][pExamVehicle])) {
			examPlayerFailed(playerid);
		}
	}	
	return 1;
}

public OnPlayerEnterCheckpoint(playerid) {

	if(PlayerInfo[playerid][pExamCheckpoint] > 0 && IsPlayerInVehicle(playerid, PlayerInfo[playerid][pExamVehicle])) {
		DisablePlayerCheckpoint(playerid);
		PlayerInfo[playerid][pExamCheckpoint] ++;

		if(!Iter_Contains(serverExam, PlayerInfo[playerid][pExamCheckpoint])) {
			dbQuery[0] = (EOS);
			PlayerInfo[playerid][pDrivingLicense] = 100;

			mysql_format(SQL, dbQuery, sizeof dbQuery, "UPDATE `users` SET `licenses` = '%d|%d|%d|%d|%d|%d|%d|%d' WHERE `id` = '%d'", PlayerInfo[playerid][pDrivingLicense], PlayerInfo[playerid][pDrivingLicenseSuspend], PlayerInfo[playerid][pGunLicense], PlayerInfo[playerid][pGunLicenseSuspend], PlayerInfo[playerid][pFlyLicense], PlayerInfo[playerid][pFlyLicenseSuspend], PlayerInfo[playerid][pSailingLicenseSuspend], PlayerInfo[playerid][pSailingLicenseSuspend], PlayerInfo[playerid][pSQLID]);
			mysql_tquery(SQL, dbQuery, "", "");	

			PlayerTextDrawHide(playerid, Info);
			DestroyVehicle(PlayerInfo[playerid][pExamVehicle]);
			PlayerInfo[playerid][pExamVehicle] = INVALID_VEHICLE_ID;
			PlayerInfo[playerid][pExamCheckpoint] = 0;
			SetPlayerPos(playerid, 1707.1661,1362.4440,10.7520);

			SendClientMessage(playerid, COLOR_WHITE, ">>"PRIMARY"Finish"ALB": Ai terminat examen-ul auto. Ai primit licenta de condus pentru 100 ore.");
			return true;
		}

		SetPlayerCheckpoint(playerid, examVariables[PlayerInfo[playerid][pExamCheckpoint]][dPosX], examVariables[PlayerInfo[playerid][pExamCheckpoint]][dPosY], examVariables[PlayerInfo[playerid][pExamCheckpoint]][dPosZ], 3.0);
		
		va_PlayerTextDrawSetString(playerid, Info, "DRIVING LICENSE~n~Checkpoints: ~y~%d~w~/~y~%d", (PlayerInfo[playerid][pExamCheckpoint] - 1), Iter_Count(serverExam));
	}
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags) {
	if(!Iter_Contains(serverLogged, playerid)) return SCM(playerid, -1, GRAY"SERVER:"ALB" You not logged.");
    if(result == -1) {
        SCMf(playerid, -1, GRAY"(/%s)"ALB": Unknown command.", cmd);
        return 0; 
    }
    return 1; 
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid) {

    return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source) {

	return 1;
}

#include "../includes/maps"
#include "../includes/funcs"
#include "../includes/textdraws"
#include "../includes/timers" 	
#include "../includes/commands" 	
#include "../includes/systems/business" 
#include "../includes/systems/gps" 	
#include "../includes/systems/personalv" 	
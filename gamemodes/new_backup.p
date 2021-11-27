/*
	𝙶𝚃𝙰: 𝚁𝚎𝚟𝚒𝚜𝚎𝚍 (v0.1)

	Authors: - dustyhawk
			 - BuNNy
*/		

// ( server settings >
#define server_hostname 			"GTA: Revised - Official MP Server"
#define server_version  			"v0.1"
#define server_mapname 				"Bayside"
#define server_weburl   			"www.revised.ro"

#define password_salt 				"jkbr238fj37"

#define mysql_host 					"localhost"
#define mysql_user 					"root"
#define mysql_data 					"suntnr1"
#define mysql_pass 					""

// ( server includes >
#include < a_samp >

#include < a_mysql >
#include < a_mysql_yinline >

#include < streamer >
#include < sscanf2 >

#include < YSI\y_va >
#include < YSI\y_timers >
#include < YSI\y_bit >
#include < YSI\y_iterate >
#include < YSI\y_commands >

main() { }

// ( max values > 
#define MAX_ADMIN_LEVEL 			(6)
#define MAX_HELPER_LEVEL 			(3)
#define MAX_REPORTS					(15)

#define MAX_DMV_CP 					(30)

// ( defines >
#define function%0(%1) forward%0(%1); public%0(%1)

// ( enums >
enum {
	DIALOG_MAIL, DIALOG_GENDER, DIALOG_PASSWORD, DIALOG_LOGIN
};

enum ENUM_PLAYER_DATA {
	pSQLID, pUsername[32], pMail[64], pGender, pPassword[65],
	pAdmin, pHelper, pExamVehicle, pExamCheckpoint
};

// ( variables >
new MySQL: SQL,

	dbQuery[1024],

	PlayerText:LoginBG[6], PlayerText:RegisterTD[16], PlayerText:TutorialTD[12], PlayerText:ServerStatsTD, Text:LogoTD[2],

	mailStep[ MAX_PLAYERS ], genderStep[ MAX_PLAYERS ], passwordStep[ MAX_PLAYERS ],

	reportText[MAX_PLAYERS][128], reportDeelay[MAX_PLAYERS],
	newbieText[MAX_PLAYERS][128], newbieDeelay[MAX_PLAYERS],

	playerIsInTutorial[ MAX_PLAYERS ] = 0,

	Iterator:serverAdmins<MAX_PLAYERS>, Iterator:serverHelpers<MAX_PLAYERS>, Iterator:serverStaff<MAX_PLAYERS>, Iterator:serverReports<MAX_PLAYERS>, 
	Iterator:serverNewbies<MAX_PLAYERS>, Iterator: serverExam<MAX_DMV_CP>,

	playerVariables[ MAX_PLAYERS ][ ENUM_PLAYER_DATA ];

#include < business >

public OnGameModeInit() {
	SQL = mysql_connect(mysql_host, mysql_user, mysql_pass, mysql_data);

	if(mysql_errno() != 0)
		print("(SQL): Could not connect to database.");
	else
		print("(SQL): Database connection established !");

	serverSys();

	SetNameTagDrawDistance(20.0);
	EnableStuntBonusForAll(false);
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_OFF);
    ManualVehicleEngineAndLights();
    DisableInteriorEnterExits();
	AllowInteriorWeapons(true);
	UsePlayerPedAnims();

	SendRconCommand("hostname "server_hostname"");
	SetGameModeText("GTA: Revised ("server_version")");
	SendRconCommand("mapname "server_mapname"");
	SendRconCommand("weburl "server_weburl"");

	globalTDs();
	commandsAlts();

	AddPlayerClass(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	return 1;
}

public OnGameModeExit() {
	mysql_close(SQL);
	return 1;
}

public OnPlayerRequestClass(playerid, classid) {
	if(playerIsInTutorial[playerid] == 1) SpawnPlayer(playerid);
	return 1;
}

public OnPlayerConnect(playerid) {
	TogglePlayerSpectating(playerid, 1);
	playerTDs(playerid);

	passwordStep[playerid] = 0; mailStep[playerid] = 0; genderStep[playerid] = 0;

	dbQuery[0] = (EOS);
	mysql_format(SQL, dbQuery, sizeof dbQuery, "SELECT * FROM `users` WHERE `name` = '%e'", GetName(playerid));
    inline OnAccountDataReceived()
    {
        new rows;
	    cache_get_row_count(rows);
	    if(rows) {
	    	clearChat(playerid, 20);
	    	for(new i = 0; i < 5; i++) PlayerTextDrawShow(playerid, LoginBG[i]);
	        defer beginConnect(playerid);
	    } else {
	    	playerIsInTutorial[ playerid ] = 1;

	    	PlayAudioStreamForPlayer(playerid, "https://uploadir.com/u/vlmf20t1");

	    	clearChat(playerid, 20);

	    	PlayerTextDrawShow(playerid, TutorialTD[10]);
	    	PlayerTextDrawShow(playerid, TutorialTD[11]);

			defer tutorialStepOne(playerid);
	    }
    }
    mysql_tquery_inline(SQL, dbQuery, using inline OnAccountDataReceived);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	passwordStep[playerid] = 0; mailStep[playerid] = 0; genderStep[playerid] = 0;

	if(playerVariables[ playerid ][ pAdmin ] > 0) {
		Iter_Remove(serverAdmins, playerid);
		Iter_Remove(serverStaff, playerid);
	}

	if(playerVariables[ playerid ][ pHelper ] > 0) {
		Iter_Remove(serverHelpers, playerid);
		Iter_Remove(serverStaff, playerid);
	}

	return 1;
}

public OnPlayerSpawn(playerid) {
	SetPlayerPos(playerid, 2756.5881, -1177.4579, 69.4039);
	SetPlayerFacingAngle(playerid, 89.6747);
	SetCameraBehindPlayer(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if(playerIsInTutorial[playerid] == 1) return 0;
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid) {
    if(playertextid == RegisterTD[12]) {
		ShowPlayerDialog(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, "SERVER: Mail", "Trebuie sa introduci un mail valid pentru a continua.\n", "Proceed", "");        
    }
    
    if(playertextid == RegisterTD[13]) {
        ShowPlayerDialog(playerid, DIALOG_GENDER, DIALOG_STYLE_MSGBOX, "SERVER: Gender", "Trebuie sa selectezi un gen pentru a continua.\n", "Masculin", "Feminin");
    }
    
    if(playertextid == RegisterTD[14]) {
        ShowPlayerDialog(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, "SERVER: Parola", "Trebuie sa introduci o parola pentru a te inregistra.\n", "Proceed", "");
    }
   	
   	if(playertextid == RegisterTD[2]) {
    	if(mailStep[playerid] == 0) return SendClientMessage(playerid, -1, "Trebuie sa introduci un mail valid pentru a te inregistra.");
    	if(genderStep[playerid] == 0) return SendClientMessage(playerid, -1, "Trebuie sa selectezi un gen pentru a te inregistra.");
    	if(passwordStep[playerid] == 0) return SendClientMessage(playerid, -1, "Trebuie sa introduci o parola valida pentru a te inregistra.");

    	for(new i = 0; i < 15; i++) PlayerTextDrawHide(playerid, RegisterTD[i]);
		CancelSelectTextDraw(playerid);
		
		new ip[18];
		GetPlayerIp(playerid, ip, sizeof(ip));
			   
		mysql_format(SQL, dbQuery, sizeof dbQuery, "INSERT INTO `users` (`name`, `password`, `mail`, `gender`, ip) VALUES ('%e', '%e', '%s', '%d', '%e')", GetName(playerid), playerVariables[playerid][pPassword], playerVariables[playerid][pMail], playerVariables[playerid][pGender], ip);
		mysql_pquery(SQL, dbQuery, "", "");
			   
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Login", "Bine ai (re)venit !\nTrebuie sa iti introduci parola contului tau mai jos.\n", "Proceed", "Cancel");
    }
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	dbQuery[0] = (EOS);

	switch(dialogid) {
        case DIALOG_PASSWORD: {
            if(response) {

                if(!strlen(inputtext))
                    return ShowPlayerDialog(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, "SERVER: Parola incorecta", "Trebuie sa introduci o parola valida pentru a te inregistra.\n", "Proceed", "");
               
               	if(strlen(inputtext) < 6 || strlen(inputtext) > 32)
               		return ShowPlayerDialog(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, "SERVER: Parola incorecta", "Parola trebuie sa contina intre 3-32 caractere.\n\nTrebuie sa introduci o parola valida pentru a te inregistra.\n", "Proceed", "");
                
                new passwordHash[65];
                SHA256_PassHash(inputtext, password_salt, passwordHash, sizeof passwordHash);

                playerVariables[playerid][pPassword] = passwordHash;
                PlayerTextDrawSetString(playerid, RegisterTD[14], "parola introdusa");
                passwordStep[playerid] = 1;
            }
            return 1;
        }
        case DIALOG_MAIL: {
            if(response) {

                if(!strlen(inputtext))
                    return ShowPlayerDialog(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, "SERVER: Mail", "Trebuie sa introduci un mail valid pentru a te inregistra.\n", "Proceed", "");
               
                if(!IsValidEmail(inputtext))
                	return ShowPlayerDialog(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, "SERVER: Mail", "Trebuie sa introduci un mail valid pentru a te inregistra.\n", "Proceed", "");

               	if(strlen(inputtext) < 6 || strlen(inputtext) > 64)
               		return ShowPlayerDialog(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, "SERVER: Mail", "Mail-ul trebuie sa contina intre 6-64 caractere.\n\nTrebuie sa introduci un mail valid pentru a te inregistra.\n", "Proceed", "");

                mailStep[playerid] = 1;
                format(playerVariables[playerid][pMail], 64, "%s", inputtext);

                PlayerTextDrawSetString(playerid, RegisterTD[12], playerVariables[playerid][pMail]);
            }
            return 1;
        }
        case DIALOG_GENDER: {
            if(!response) {
                playerVariables[playerid][pGender] = 1;
                PlayerTextDrawSetString(playerid, RegisterTD[13], "Feminin");
                genderStep[playerid] = 1;
            }
            if(response) {             
                playerVariables[playerid][pGender] = 0;
                PlayerTextDrawSetString(playerid, RegisterTD[13], "Masculin");
                genderStep[playerid] = 1;
            }
            return 1;
        }
        case DIALOG_LOGIN:
        {
            if(!response) return Kick(playerid);
            if(response)
            {
                if(!strlen(inputtext))
                        return ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Parola incorecta", "Bine ai (re)venit !\nTrebuie sa iti introduci parola contului tau mai jos.\n", "Proceed", "Cancel");

                new passwordHash[65];
                SHA256_PassHash(inputtext, password_salt, passwordHash, sizeof passwordHash);
                mysql_format(SQL, dbQuery, sizeof dbQuery, "SELECT * FROM `users` WHERE `name`='%e' AND `password` = '%e'", GetName(playerid), passwordHash);
				inline OnAccountDataDone()
			    {
			        new rows;
				    cache_get_row_count(rows);
				    if(rows) {
				    	cache_get_value_name_int(0, "id", playerVariables[ playerid ][ pSQLID ]);
				        cache_get_value_name(0, "password", playerVariables[ playerid ][ pPassword ]);
						cache_get_value_name(0, "mail", playerVariables[ playerid ][ pMail ]);
						cache_get_value_name_int(0, "gender", playerVariables[ playerid ][ pGender ]);
						cache_get_value_name_int(0, "admin", playerVariables[ playerid ][ pAdmin ]);
						cache_get_value_name_int(0, "helper", playerVariables[ playerid ][ pHelper ]);

						format(playerVariables[ playerid ][ pUsername ], 32, "%s", GetName(playerid));

						TogglePlayerSpectating(playerid, 0);
						StopAudioStreamForPlayer(playerid);

						TextDrawShowForPlayer(playerid, LogoTD[0]);
						TextDrawShowForPlayer(playerid, LogoTD[1]);

						va_SendClientMessage(playerid, -1, "Bine ai (re)venit, %s !", GetName(playerid));

						if(playerVariables[ playerid ][ pAdmin ] > 0) {
							Iter_Add(serverAdmins, playerid);
							Iter_Add(serverStaff, playerid);
							va_SendClientMessage(playerid, -1, "SERVER: Te-ai logat ca admin de nivel %d.", playerVariables[ playerid ][ pAdmin ]);
						}

						if( playerVariables[ playerid ][ pAdmin ] == MAX_ADMIN_LEVEL) {
							PlayerTextDrawShow(playerid, ServerStatsTD);
						}

						if(playerVariables[ playerid ][ pHelper ] > 0) {
							Iter_Add(serverHelpers, playerid);
							Iter_Add(serverStaff, playerid);
							va_SendClientMessage(playerid, -1, "SERVER: Te-ai logat ca helper de nivel %d.", playerVariables[ playerid ][ pHelper ]);
						}

						playerIsInTutorial[ playerid ] = 0;

				        SpawnPlayer(playerid);
				    } else {
				        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Parola incorecta", "Te rog sa iti introduci parola corecta, pentru a te loga pe server.", "Logare", "Exit");
				    }
			    }
			    mysql_tquery_inline(SQL, dbQuery, using inline OnAccountDataDone);
            }
            return 1;
        }
    }
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

// ( funcs >
serverSys()
{
	mysql_tquery(SQL, "SELECT * FROM `dmv_cp`", "loadDynamicDMV", "");
	mysql_tquery(SQL, "SELECT * FROM `businesses`", "loadDynamicBIZZ", "");
	return true;
}

enum ENUM_DMV_DATA {
	dID, Float: dPosX, Float: dPosY, Float: dPosZ
};
new examVariables[ MAX_DMV_CP ][ ENUM_DMV_DATA ];

function loadDynamicDMV()
{
	if(!cache_num_rows()) return print("SQL >> No rows found in 'dmv_cp' table.");
	for(new i = 1; i < cache_num_rows() + 1; i++) {
		Iter_Add(serverExam, i);

		cache_get_value_name_int(i - 1, "id", examVariables[ i ][ dID ]);
		cache_get_value_name_float(i - 1, "posx", examVariables[ i ][ dPosX ]);
		cache_get_value_name_float(i - 1, "posy", examVariables[ i ][ dPosY ]);
		cache_get_value_name_float(i - 1, "posz", examVariables[ i ][ dPosZ ]);
	}
	printf("SQL >> Loaded %d dmv checkpoints.", Iter_Count(serverExam));
	return true;
}

stock GetName(playerid) {
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof name);
	return name;
}

stock IsValidEmail(email[])
{
    new a_count, length = strlen(email);
    if(length < 5)
        return false;
    for(new i = 0; i < strlen(email); i++)
    {
        if(email[i] == '@')
        {
            a_count++;
            if(a_count > 1)
            {
                return false;
            }
        }
        if( (email[i] < 'A' && email[i] > 'Z') || (email[i] < 'a' && email[i] > 'z') )
        {
            if(email[i] != '.')
            {
                return false;
            }
        }
        if(i == length-1 && email[i] == '.')
            return false;
    }
    return true;
}

stock clearChat(playerid, lines) {
	for(new i = 0; i < lines; i++) SendClientMessage(playerid, -1, " ");
	return 1;
}

stock sendSyntax(playerid, const syntax[]) return va_SendClientMessage(playerid, -1, "{C8C8C8}Sintaxa: {ffffff}%s", syntax);
stock sendError(playerid, const error[]) return va_SendClientMessage(playerid, -1, "{C8C8C8}Eroare: {ffffff}%s", error);
stock sendGreyMsg(playerid, const msg[]) return va_SendClientMessage(playerid, -1, "{C8C8C8}%s", msg);

stock sendStaff(typeid, colour, const text[], va_args<>) {
	switch(typeid) {
		case 0: foreach(new x : serverAdmins) return va_SendClientMessage(x, colour, va_return(text, va_start<3>));
		case 1: foreach(new x : serverStaff) return va_SendClientMessage(x, colour, va_return(text, va_start<3>));
	}
	return 1;
}

commandsAlts() {
	Command_AddAltNamed("adminchat", "a");
	Command_AddAltNamed("closereport", "cr");
	Command_AddAltNamed("newbie", "n");
	Command_AddAltNamed("newbiereply", "nr");
	return 1;
}

// ( textdraws >
playerTDs(playerid) {
	RegisterTD[0] = CreatePlayerTextDraw(playerid, 319.777893, 140.720108, "box");
	PlayerTextDrawLetterSize(playerid, RegisterTD[0], 0.000000, 11.866667);
	PlayerTextDrawTextSize(playerid, RegisterTD[0], 0.000000, 126.000000);
	PlayerTextDrawAlignment(playerid, RegisterTD[0], 2);
	PlayerTextDrawColor(playerid, RegisterTD[0], -2139062017);
	PlayerTextDrawUseBox(playerid, RegisterTD[0], 1);
	PlayerTextDrawBoxColor(playerid, RegisterTD[0], 842150655);
	PlayerTextDrawSetShadow(playerid, RegisterTD[0], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[0], 0);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[0], 255);
	PlayerTextDrawFont(playerid, RegisterTD[0], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[0], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[0], 0);

	RegisterTD[1] = CreatePlayerTextDraw(playerid, 319.777832, 269.392425, "box");
	PlayerTextDrawLetterSize(playerid, RegisterTD[1], 0.000000, -2.400006);
	PlayerTextDrawTextSize(playerid, RegisterTD[1], 0.000000, 126.000000);
	PlayerTextDrawAlignment(playerid, RegisterTD[1], 2);
	PlayerTextDrawColor(playerid, RegisterTD[1], -2139062017);
	PlayerTextDrawUseBox(playerid, RegisterTD[1], 1);
	PlayerTextDrawBoxColor(playerid, RegisterTD[1], 336860415);
	PlayerTextDrawSetShadow(playerid, RegisterTD[1], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[1], 0);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[1], -1061109505);
	PlayerTextDrawFont(playerid, RegisterTD[1], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[1], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[1], 0);

	RegisterTD[2] = CreatePlayerTextDraw(playerid, 319.572479, 252.818176, "register");
	PlayerTextDrawLetterSize(playerid, RegisterTD[2], 0.264444, 1.117155);
	PlayerTextDrawTextSize(playerid, RegisterTD[2], 15.0, 100.0);
	PlayerTextDrawAlignment(playerid, RegisterTD[2], 2);
	PlayerTextDrawColor(playerid, RegisterTD[2], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[2], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[2], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[2], 255);
	PlayerTextDrawFont(playerid, RegisterTD[2], 3);
	PlayerTextDrawSetProportional(playerid, RegisterTD[2], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[2], 0);
	PlayerTextDrawSetSelectable(playerid, RegisterTD[2], true);

	RegisterTD[3] = CreatePlayerTextDraw(playerid, 262.100036, 152.812362, "Nume");
	PlayerTextDrawLetterSize(playerid, RegisterTD[3], 0.194666, 0.932977);
	PlayerTextDrawAlignment(playerid, RegisterTD[3], 1);
	PlayerTextDrawColor(playerid, RegisterTD[3], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[3], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[3], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[3], 255);
	PlayerTextDrawFont(playerid, RegisterTD[3], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[3], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[3], 0);

	RegisterTD[4] = CreatePlayerTextDraw(playerid, 262.199981, 177.050674, "Mail");
	PlayerTextDrawLetterSize(playerid, RegisterTD[4], 0.194666, 0.932977);
	PlayerTextDrawAlignment(playerid, RegisterTD[4], 1);
	PlayerTextDrawColor(playerid, RegisterTD[4], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[4], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[4], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[4], 255);
	PlayerTextDrawFont(playerid, RegisterTD[4], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[4], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[4], 0);

	RegisterTD[5] = CreatePlayerTextDraw(playerid, 262.199981, 199.352035, "Gen");
	PlayerTextDrawLetterSize(playerid, RegisterTD[5], 0.194666, 0.932977);
	PlayerTextDrawAlignment(playerid, RegisterTD[5], 1);
	PlayerTextDrawColor(playerid, RegisterTD[5], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[5], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[5], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[5], 255);
	PlayerTextDrawFont(playerid, RegisterTD[5], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[5], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[5], 0);

	RegisterTD[6] = CreatePlayerTextDraw(playerid, 261.755523, 221.453384, "Parola");
	PlayerTextDrawLetterSize(playerid, RegisterTD[6], 0.194666, 0.932977);
	PlayerTextDrawAlignment(playerid, RegisterTD[6], 1);
	PlayerTextDrawColor(playerid, RegisterTD[6], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[6], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[6], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[6], 255);
	PlayerTextDrawFont(playerid, RegisterTD[6], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[6], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[6], 0);

	RegisterTD[7] = CreatePlayerTextDraw(playerid, 334.444488, 168.307540, "box");
	PlayerTextDrawLetterSize(playerid, RegisterTD[7], 0.000000, -2.222229);
	PlayerTextDrawTextSize(playerid, RegisterTD[7], 0.000000, 81.000000);
	PlayerTextDrawAlignment(playerid, RegisterTD[7], 2);
	PlayerTextDrawColor(playerid, RegisterTD[7], -2139062017);
	PlayerTextDrawUseBox(playerid, RegisterTD[7], 1);
	PlayerTextDrawBoxColor(playerid, RegisterTD[7], 336860415);
	PlayerTextDrawSetShadow(playerid, RegisterTD[7], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[7], 0);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[7], -1061109505);
	PlayerTextDrawFont(playerid, RegisterTD[7], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[7], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[7], 0);

	RegisterTD[8] = CreatePlayerTextDraw(playerid, 334.589050, 192.309844, "box");
	PlayerTextDrawLetterSize(playerid, RegisterTD[8], 0.000000, -2.222229);
	PlayerTextDrawTextSize(playerid, RegisterTD[8], 0.000000, 81.000000);
	PlayerTextDrawAlignment(playerid, RegisterTD[8], 2);
	PlayerTextDrawColor(playerid, RegisterTD[8], -2139062017);
	PlayerTextDrawUseBox(playerid, RegisterTD[8], 1);
	PlayerTextDrawBoxColor(playerid, RegisterTD[8], 336860415);
	PlayerTextDrawSetShadow(playerid, RegisterTD[8], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[8], 0);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[8], -1061109505);
	PlayerTextDrawFont(playerid, RegisterTD[8], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[8], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[8], 0);

	RegisterTD[9] = CreatePlayerTextDraw(playerid, 334.589050, 215.214294, "box");
	PlayerTextDrawLetterSize(playerid, RegisterTD[9], 0.000000, -2.222229);
	PlayerTextDrawTextSize(playerid, RegisterTD[9], 0.000000, 81.000000);
	PlayerTextDrawAlignment(playerid, RegisterTD[9], 2);
	PlayerTextDrawColor(playerid, RegisterTD[9], -2139062017);
	PlayerTextDrawUseBox(playerid, RegisterTD[9], 1);
	PlayerTextDrawBoxColor(playerid, RegisterTD[9], 336860415);
	PlayerTextDrawSetShadow(playerid, RegisterTD[9], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[9], 0);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[9], -1061109505);
	PlayerTextDrawFont(playerid, RegisterTD[9], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[9], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[9], 0);

	RegisterTD[10] = CreatePlayerTextDraw(playerid, 334.633605, 237.321014, "box");
	PlayerTextDrawLetterSize(playerid, RegisterTD[10], 0.000000, -2.222229);
	PlayerTextDrawTextSize(playerid, RegisterTD[10], 0.000000, 81.000000);
	PlayerTextDrawAlignment(playerid, RegisterTD[10], 2);
	PlayerTextDrawColor(playerid, RegisterTD[10], -2139062017);
	PlayerTextDrawUseBox(playerid, RegisterTD[10], 1);
	PlayerTextDrawBoxColor(playerid, RegisterTD[10], 336860415);
	PlayerTextDrawSetShadow(playerid, RegisterTD[10], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[10], 0);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[10], -1061109505);
	PlayerTextDrawFont(playerid, RegisterTD[10], 1);
	PlayerTextDrawSetProportional(playerid, RegisterTD[10], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[10], 0);

	RegisterTD[11] = CreatePlayerTextDraw(playerid, 333.934020, 153.455154, GetName(playerid));
	PlayerTextDrawLetterSize(playerid, RegisterTD[11], 0.163111, 0.908088);
	PlayerTextDrawAlignment(playerid, RegisterTD[11], 2);
	PlayerTextDrawColor(playerid, RegisterTD[11], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[11], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[11], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[11], 255);
	PlayerTextDrawFont(playerid, RegisterTD[11], 2);
	PlayerTextDrawSetProportional(playerid, RegisterTD[11], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[11], 0);

	RegisterTD[12] = CreatePlayerTextDraw(playerid, 333.934051, 177.555160, "scrie un mail");
	PlayerTextDrawLetterSize(playerid, RegisterTD[12], 0.163111, 0.908088);
	PlayerTextDrawTextSize(playerid, RegisterTD[12], 15.0, 100.0);
	PlayerTextDrawAlignment(playerid, RegisterTD[12], 2);
	PlayerTextDrawColor(playerid, RegisterTD[12], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[12], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[12], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[12], 255);
	PlayerTextDrawFont(playerid, RegisterTD[12], 2);
	PlayerTextDrawSetProportional(playerid, RegisterTD[12], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[12], 0);
	PlayerTextDrawSetSelectable(playerid, RegisterTD[12], true);

	RegisterTD[13] = CreatePlayerTextDraw(playerid, 333.934051, 200.306549, "selecteaza un gen");
	PlayerTextDrawLetterSize(playerid, RegisterTD[13], 0.163111, 0.908088);
	PlayerTextDrawTextSize(playerid, RegisterTD[13], 15.0, 100.0);
	PlayerTextDrawAlignment(playerid, RegisterTD[13], 2);
	PlayerTextDrawColor(playerid, RegisterTD[13], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[13], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[13], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[13], 255);
	PlayerTextDrawFont(playerid, RegisterTD[13], 2);
	PlayerTextDrawSetProportional(playerid, RegisterTD[13], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[13], 0);
	PlayerTextDrawSetSelectable(playerid, RegisterTD[13], true);

	RegisterTD[14] = CreatePlayerTextDraw(playerid, 333.934051, 222.407897, "scrie o parola");
	PlayerTextDrawLetterSize(playerid, RegisterTD[14], 0.163111, 0.908088);
	PlayerTextDrawTextSize(playerid, RegisterTD[14], 15.0, 100.0);
	PlayerTextDrawAlignment(playerid, RegisterTD[14], 2);
	PlayerTextDrawColor(playerid, RegisterTD[14], -1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[14], 0);
	PlayerTextDrawSetOutline(playerid, RegisterTD[14], 1);
	PlayerTextDrawBackgroundColor(playerid, RegisterTD[14], 255);
	PlayerTextDrawFont(playerid, RegisterTD[14], 2);
	PlayerTextDrawSetProportional(playerid, RegisterTD[14], 1);
	PlayerTextDrawSetShadow(playerid, RegisterTD[14], 0);
	PlayerTextDrawSetSelectable(playerid, RegisterTD[14], true);

	LoginBG[0] = CreatePlayerTextDraw(playerid, -24.333349, -0.311084, "revised:bglogin");
	PlayerTextDrawLetterSize(playerid, LoginBG[0], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, LoginBG[0], 690.000000, 463.000000);
	PlayerTextDrawAlignment(playerid, LoginBG[0], 1);
	PlayerTextDrawColor(playerid, LoginBG[0], -1);
	PlayerTextDrawSetShadow(playerid, LoginBG[0], 0);
	PlayerTextDrawSetOutline(playerid, LoginBG[0], 0);
	PlayerTextDrawBackgroundColor(playerid, LoginBG[0], 255);
	PlayerTextDrawFont(playerid, LoginBG[0], 4);
	PlayerTextDrawSetProportional(playerid, LoginBG[0], 0);
	PlayerTextDrawSetShadow(playerid, LoginBG[0], 0);

	LoginBG[1] = CreatePlayerTextDraw(playerid, 67.777778, 170.586730, "box");
	PlayerTextDrawLetterSize(playerid, LoginBG[1], 0.000000, 4.355550);
	PlayerTextDrawTextSize(playerid, LoginBG[1], 186.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, LoginBG[1], 1);
	PlayerTextDrawColor(playerid, LoginBG[1], -110);
	PlayerTextDrawUseBox(playerid, LoginBG[1], 1);
	PlayerTextDrawBoxColor(playerid, LoginBG[1], 102);
	PlayerTextDrawSetShadow(playerid, LoginBG[1], 0);
	PlayerTextDrawSetOutline(playerid, LoginBG[1], 0);
	PlayerTextDrawBackgroundColor(playerid, LoginBG[1], 152);
	PlayerTextDrawFont(playerid, LoginBG[1], 1);
	PlayerTextDrawSetProportional(playerid, LoginBG[1], 1);
	PlayerTextDrawSetShadow(playerid, LoginBG[1], 0);

	LoginBG[2] = CreatePlayerTextDraw(playerid, 78.888908, 179.546615, "GTA: Revised este un GTA aproape~n~diferit, care are la baza modarea~n~jocului GTA San Andreas");
	PlayerTextDrawLetterSize(playerid, LoginBG[2], 0.183999, 0.788621);
	PlayerTextDrawAlignment(playerid, LoginBG[2], 1);
	PlayerTextDrawColor(playerid, LoginBG[2], -1);
	PlayerTextDrawSetShadow(playerid, LoginBG[2], 0);
	PlayerTextDrawSetOutline(playerid, LoginBG[2], 0);
	PlayerTextDrawBackgroundColor(playerid, LoginBG[2], 255);
	PlayerTextDrawFont(playerid, LoginBG[2], 1);
	PlayerTextDrawSetProportional(playerid, LoginBG[2], 1);
	PlayerTextDrawSetShadow(playerid, LoginBG[2], 0);

	LoginBG[3] = CreatePlayerTextDraw(playerid, 77.555541, 350.613708, "REVISED");
	PlayerTextDrawLetterSize(playerid, LoginBG[3], 0.400000, 1.600000);
	PlayerTextDrawAlignment(playerid, LoginBG[3], 1);
	PlayerTextDrawColor(playerid, LoginBG[3], -1);
	PlayerTextDrawSetShadow(playerid, LoginBG[3], 0);
	PlayerTextDrawSetOutline(playerid, LoginBG[3], 1);
	PlayerTextDrawBackgroundColor(playerid, LoginBG[3], 255);
	PlayerTextDrawFont(playerid, LoginBG[3], 3);
	PlayerTextDrawSetProportional(playerid, LoginBG[3], 1);
	PlayerTextDrawSetShadow(playerid, LoginBG[3], 0);

	LoginBG[4] = CreatePlayerTextDraw(playerid, 91.333396, 357.782592, "Roleplay");
	PlayerTextDrawLetterSize(playerid, LoginBG[4], 0.403111, 1.286399);
	PlayerTextDrawAlignment(playerid, LoginBG[4], 1);
	PlayerTextDrawColor(playerid, LoginBG[4], -1);
	PlayerTextDrawSetShadow(playerid, LoginBG[4], 0);
	PlayerTextDrawSetOutline(playerid, LoginBG[4], 1);
	PlayerTextDrawBackgroundColor(playerid, LoginBG[4], 255);
	PlayerTextDrawFont(playerid, LoginBG[4], 0);
	PlayerTextDrawSetProportional(playerid, LoginBG[4], 1);
	PlayerTextDrawSetShadow(playerid, LoginBG[4], 0);

	LoginBG[5] = CreatePlayerTextDraw(playerid, 597.555908, 434.408813, "Loading...");
	PlayerTextDrawLetterSize(playerid, LoginBG[5], 0.219110, 1.032533);
	PlayerTextDrawAlignment(playerid, LoginBG[5], 1);
	PlayerTextDrawColor(playerid, LoginBG[5], -1);
	PlayerTextDrawSetShadow(playerid, LoginBG[5], 0);
	PlayerTextDrawSetOutline(playerid, LoginBG[5], 1);
	PlayerTextDrawBackgroundColor(playerid, LoginBG[5], 255);
	PlayerTextDrawFont(playerid, LoginBG[5], 1);
	PlayerTextDrawSetProportional(playerid, LoginBG[5], 1);
	PlayerTextDrawSetShadow(playerid, LoginBG[5], 0);
	PlayerTextDrawSetSelectable(playerid, LoginBG[5], true);

	TutorialTD[0] = CreatePlayerTextDraw(playerid, 153.999969, 244.395263, "dustyhawk");
	PlayerTextDrawLetterSize(playerid, TutorialTD[0], 0.224888, 0.997688);
	PlayerTextDrawAlignment(playerid, TutorialTD[0], 1);
	PlayerTextDrawColor(playerid, TutorialTD[0], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[0], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[0], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[0], 255);
	PlayerTextDrawFont(playerid, TutorialTD[0], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[0], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[0], 0);

	TutorialTD[1] = CreatePlayerTextDraw(playerid, 140.316757, 233.773422, "lead developers");
	PlayerTextDrawLetterSize(playerid, TutorialTD[1], 0.313333, 1.151999);
	PlayerTextDrawAlignment(playerid, TutorialTD[1], 1);
	PlayerTextDrawColor(playerid, TutorialTD[1], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[1], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[1], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[1], 255);
	PlayerTextDrawFont(playerid, TutorialTD[1], 0);
	PlayerTextDrawSetProportional(playerid, TutorialTD[1], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[1], 0);

	TutorialTD[2] = CreatePlayerTextDraw(playerid, 154.099975, 254.095855, "bunny");
	PlayerTextDrawLetterSize(playerid, TutorialTD[2], 0.224888, 0.997688);
	PlayerTextDrawAlignment(playerid, TutorialTD[2], 1);
	PlayerTextDrawColor(playerid, TutorialTD[2], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[2], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[2], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[2], 255);
	PlayerTextDrawFont(playerid, TutorialTD[2], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[2], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[2], 0);

	TutorialTD[3] = CreatePlayerTextDraw(playerid, 443.427825, 164.582351, "web developer");
	PlayerTextDrawLetterSize(playerid, TutorialTD[3], 0.313333, 1.151999);
	PlayerTextDrawAlignment(playerid, TutorialTD[3], 1);
	PlayerTextDrawColor(playerid, TutorialTD[3], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[3], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[3], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[3], 255);
	PlayerTextDrawFont(playerid, TutorialTD[3], 0);
	PlayerTextDrawSetProportional(playerid, TutorialTD[3], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[3], 0);

	TutorialTD[4] = CreatePlayerTextDraw(playerid, 469.560089, 176.397537, "adryan");
	PlayerTextDrawLetterSize(playerid, TutorialTD[4], 0.224888, 0.997688);
	PlayerTextDrawAlignment(playerid, TutorialTD[4], 1);
	PlayerTextDrawColor(playerid, TutorialTD[4], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[4], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[4], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[4], 255);
	PlayerTextDrawFont(playerid, TutorialTD[4], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[4], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[4], 0);

	TutorialTD[5] = CreatePlayerTextDraw(playerid, 359.872497, 285.392395, "lead modders");
	PlayerTextDrawLetterSize(playerid, TutorialTD[5], 0.313333, 1.151999);
	PlayerTextDrawAlignment(playerid, TutorialTD[5], 1);
	PlayerTextDrawColor(playerid, TutorialTD[5], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[5], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[5], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[5], 255);
	PlayerTextDrawFont(playerid, TutorialTD[5], 0);
	PlayerTextDrawSetProportional(playerid, TutorialTD[5], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[5], 0);

	TutorialTD[6] = CreatePlayerTextDraw(playerid, 375.333526, 296.761901, "adryan");
	PlayerTextDrawLetterSize(playerid, TutorialTD[6], 0.224888, 0.997688);
	PlayerTextDrawAlignment(playerid, TutorialTD[6], 1);
	PlayerTextDrawColor(playerid, TutorialTD[6], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[6], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[6], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[6], 255);
	PlayerTextDrawFont(playerid, TutorialTD[6], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[6], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[6], 0);

	TutorialTD[7] = CreatePlayerTextDraw(playerid, 375.488922, 306.717468, "bunny");
	PlayerTextDrawLetterSize(playerid, TutorialTD[7], 0.224888, 0.997688);
	PlayerTextDrawAlignment(playerid, TutorialTD[7], 1);
	PlayerTextDrawColor(playerid, TutorialTD[7], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[7], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[7], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[7], 255);
	PlayerTextDrawFont(playerid, TutorialTD[7], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[7], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[7], 0);

	TutorialTD[8] = CreatePlayerTextDraw(playerid, 375.488861, 316.775115, "raypeax");
	PlayerTextDrawLetterSize(playerid, TutorialTD[8], 0.224888, 0.997688);
	PlayerTextDrawAlignment(playerid, TutorialTD[8], 1);
	PlayerTextDrawColor(playerid, TutorialTD[8], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[8], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[8], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[8], 255);
	PlayerTextDrawFont(playerid, TutorialTD[8], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[8], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[8], 0);

	TutorialTD[9] = CreatePlayerTextDraw(playerid, 375.488861, 326.432800, "dustyhawk");
	PlayerTextDrawLetterSize(playerid, TutorialTD[9], 0.224888, 0.997688);
	PlayerTextDrawAlignment(playerid, TutorialTD[9], 1);
	PlayerTextDrawColor(playerid, TutorialTD[9], -1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[9], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[9], 1);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[9], 255);
	PlayerTextDrawFont(playerid, TutorialTD[9], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[9], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[9], 0);


	TutorialTD[10] = CreatePlayerTextDraw(playerid, 293.555572, -58.391109, "box");
	PlayerTextDrawLetterSize(playerid, TutorialTD[10], 1.012888, 167.253662);
	PlayerTextDrawTextSize(playerid, TutorialTD[10], 0.000000, 2304.000000);
	PlayerTextDrawAlignment(playerid, TutorialTD[10], 2);
	PlayerTextDrawColor(playerid, TutorialTD[10], -1);
	PlayerTextDrawUseBox(playerid, TutorialTD[10], 1);
	PlayerTextDrawBoxColor(playerid, TutorialTD[10], 255);
	PlayerTextDrawSetShadow(playerid, TutorialTD[10], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[10], 607);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[10], 255);
	PlayerTextDrawFont(playerid, TutorialTD[10], 1);
	PlayerTextDrawSetProportional(playerid, TutorialTD[10], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[10], 0);

	TutorialTD[11] = CreatePlayerTextDraw(playerid, 320.033508, 191.493331, "bun venit");
	PlayerTextDrawLetterSize(playerid, TutorialTD[11], 0.464000, 1.719466);
	PlayerTextDrawAlignment(playerid, TutorialTD[11], 2);
	PlayerTextDrawColor(playerid, TutorialTD[11], -5963521);
	PlayerTextDrawSetShadow(playerid, TutorialTD[11], 0);
	PlayerTextDrawSetOutline(playerid, TutorialTD[11], 0);
	PlayerTextDrawBackgroundColor(playerid, TutorialTD[11], 255);
	PlayerTextDrawFont(playerid, TutorialTD[11], 3);
	PlayerTextDrawSetProportional(playerid, TutorialTD[11], 1);
	PlayerTextDrawSetShadow(playerid, TutorialTD[11], 0);

	ServerStatsTD = CreatePlayerTextDraw(playerid, 637.555908, 433.911071, "Loading...");
	PlayerTextDrawLetterSize(playerid, ServerStatsTD, 0.262222, 1.211733);
	PlayerTextDrawAlignment(playerid, ServerStatsTD, 3);
	PlayerTextDrawColor(playerid, ServerStatsTD, -1);
	PlayerTextDrawSetShadow(playerid, ServerStatsTD, 0);
	PlayerTextDrawSetOutline(playerid, ServerStatsTD, 1);
	PlayerTextDrawBackgroundColor(playerid, ServerStatsTD, 255);
	PlayerTextDrawFont(playerid, ServerStatsTD, 3);
	PlayerTextDrawSetProportional(playerid, ServerStatsTD, 1);
	PlayerTextDrawSetShadow(playerid, ServerStatsTD, 0);
	return 1;
}

globalTDs() {
	LogoTD[0] = TextDrawCreate(3.333329, 429.070770, "Revised");
	TextDrawLetterSize(LogoTD[0], 0.296444, 1.281421);
	TextDrawAlignment(LogoTD[0], 1);
	TextDrawColor(LogoTD[0], -1);
	TextDrawSetShadow(LogoTD[0], 0);
	TextDrawSetOutline(LogoTD[0], 1);
	TextDrawBackgroundColor(LogoTD[0], 255);
	TextDrawFont(LogoTD[0], 3);
	TextDrawSetProportional(LogoTD[0], 1);
	TextDrawSetShadow(LogoTD[0], 0);

	LogoTD[1] = TextDrawCreate(13.205572, 434.875610, "Roleplay");
	TextDrawLetterSize(LogoTD[1], 0.313333, 1.151999);
	TextDrawAlignment(LogoTD[1], 1);
	TextDrawColor(LogoTD[1], -1);
	TextDrawSetShadow(LogoTD[1], 0);
	TextDrawSetOutline(LogoTD[1], 1);
	TextDrawBackgroundColor(LogoTD[1], 255);
	TextDrawFont(LogoTD[1], 0);
	TextDrawSetProportional(LogoTD[1], 1);
	TextDrawSetShadow(LogoTD[1], 0);
}

// ( timers >
timer beginConnect[3000](playerid) 
{
	TogglePlayerSpectating(playerid, 0);
	for(new i = 0; i < 6; i++) PlayerTextDrawHide(playerid, LoginBG[i]);
	ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "SERVER: Login", "Bine ai (re)venit !\nTrebuie sa iti introduci parola contului tau mai jos.\n", "Proceed", "Cancel");
	return 1;
}

timer SpawnPlayer[50](playerid) return SpawnPlayer(playerid);

// ====================> ( server introduction > 
timer tutorialStepOne[5000](playerid) {
	PlayerTextDrawHide(playerid, TutorialTD[10]);
	PlayerTextDrawHide(playerid, TutorialTD[11]);

	InterpolateCameraPos(playerid, 159.628265, -1775.425537, 11.718588, 237.322296, -1777.143676, 14.932574, 10000);
	InterpolateCameraLookAt(playerid, 163.367523, -1772.156616, 11.142436, 240.698547, -1773.649169, 13.753839, 10000);

	PlayerTextDrawShow(playerid, TutorialTD[0]);
	PlayerTextDrawShow(playerid, TutorialTD[1]);
	PlayerTextDrawShow(playerid, TutorialTD[2]);

	defer tutorialStepTwo(playerid);

	return 1;
}

timer tutorialStepTwo[10000](playerid) {
	InterpolateCameraPos(playerid, 376.026428, -2004.987670, 69.849624, 374.065917, -2062.869628, 58.556320, 10000);
	InterpolateCameraLookAt(playerid, 376.602416, -2004.185424, 64.948120, 375.591186, -2059.725830, 54.979938, 10000);

	PlayerTextDrawHide(playerid, TutorialTD[0]);
	PlayerTextDrawHide(playerid, TutorialTD[1]);
	PlayerTextDrawHide(playerid, TutorialTD[2]);

	PlayerTextDrawShow(playerid, TutorialTD[3]);
	PlayerTextDrawShow(playerid, TutorialTD[4]);

	defer tutorialStepThree(playerid);

	return 1;
}

timer tutorialStepThree[10000](playerid) {
	InterpolateCameraPos(playerid, 365.516510, -1953.367065, 39.701690, 402.767974, -1689.899658, 40.576271, 10000);
	InterpolateCameraLookAt(playerid, 366.942932, -1948.732788, 38.481582, 407.061126, -1687.342529, 40.402866, 10000);

	PlayerTextDrawHide(playerid, TutorialTD[3]);
	PlayerTextDrawHide(playerid, TutorialTD[4]);

	PlayerTextDrawShow(playerid, TutorialTD[5]);
	PlayerTextDrawShow(playerid, TutorialTD[6]);
	PlayerTextDrawShow(playerid, TutorialTD[7]);
	PlayerTextDrawShow(playerid, TutorialTD[8]);
	PlayerTextDrawShow(playerid, TutorialTD[9]);

	defer tutorialStepFinish(playerid);

	return 1;
}

timer tutorialStepFinish[10000](playerid) {
	InterpolateCameraPos(playerid, 317.239746, -2042.789916, 8.661369, 338.663024, -2105.711181, 4.928667, 15000);
	InterpolateCameraLookAt(playerid, 320.088043, -2046.651733, 7.256582, 342.956695, -2103.168457, 4.615070, 15000);

	PlayerTextDrawHide(playerid, TutorialTD[5]);
	PlayerTextDrawHide(playerid, TutorialTD[6]);
	PlayerTextDrawHide(playerid, TutorialTD[7]);
	PlayerTextDrawHide(playerid, TutorialTD[8]);
	PlayerTextDrawHide(playerid, TutorialTD[9]);


	for(new i = 0; i < 15; i++) PlayerTextDrawShow(playerid, RegisterTD[i]);
	SelectTextDraw(playerid, 0xA3B4C5FF);

	return 1;
}
// ====================> ( server introduction > 

task oneSecond[1000]() {
	foreach(new playerid : Player) {
		if(playerVariables[ playerid ][ pAdmin ] == MAX_ADMIN_LEVEL) {
			 va_PlayerTextDrawSetString(playerid, ServerStatsTD, "TICKS: %d_____QUERIES: %d_____ANIM: %d", GetServerTickRate(), mysql_unprocessed_queries(), GetPlayerAnimationIndex(playerid));
		}
		if(reportDeelay[ playerid ] != 0) {
			reportDeelay[playerid] --;
			if(reportDeelay[ playerid ] == 1) sendGreyMsg(playerid, "[deelay]: acum poti sa folosesti comanda '/report' din nou.");
		}
		if(newbieDeelay[ playerid ] != 0) {
			newbieDeelay[playerid] --;
			if(newbieDeelay[ playerid ] == 1) sendGreyMsg(playerid, "[deelay]: acum poti sa folosesti comanda '/(n)ewbie' din nou.");
		}
	}

	return 1;
}

stock va_PlayerTextDrawSetString(playerid, PlayerText:text, const fmat[], va_args<>) {
	new string[145];
    va_format(string, sizeof (string), fmat, va_start<3>);
    return PlayerTextDrawSetString(playerid, text, string);
}

// ( commands >

// ===============> ( helper cmds >
YCMD:helperchat(playerid, params[], help) {
	if(playerVariables[ playerid ][ pHelper ] < 1 || playerVariables[ playerid ][ pAdmin ] < 1) 
		return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");

	new helpertext[128];
	if(sscanf(params,"s[128]", helpertext)) return sendSyntax(playerid, "/(h)elperchat [text]");

	if(playerVariables[ playerid ][ pHelper ] < 1) {
		sendStaff(1, -1, "(helper %d) %s: %s", playerVariables[ playerid ][ pHelper ], GetName(playerid), helpertext);
	} else {
		sendStaff(1, -1, "(admin %d) %s: %s", playerVariables[ playerid ][ pAdmin ], GetName(playerid), helpertext);
	}	
	return 1;
}

YCMD:newbie(playerid, params[], help) {
	new question[128];
	if(sscanf(params,"s[128]", question)) return sendSyntax(playerid, "/(n)ewbie [question]");
	if(strlen(question) < 4 || strlen(question) > 128) return sendError(playerid, "Intrebarea trebuie sa contina 4-128 caractere.");

	/*if(playerVariables[ playerid ][ pAdmin ] > 0 || playerVariables[ playerid ][ pHelper ] > 0) 
		return sendError(playerid, "Nu poti sa folosesti comanda '/(n)ewbie' deoarece esti admin / helper.");*/

	if(newbieDeelay[ playerid ] != 0) return sendError(playerid, "Nu poti sa folosesti aceasta comanda deoarece ai deelay.");

	if(Iter_Count(serverNewbies) == MAX_REPORTS) return sendError(playerid, "Limita maxima de intrebari a fost atinsa.");
	if(Iter_Contains(serverNewbies, playerid)) return sendError(playerid, "Ai trimis deja o intrebare, trebuie sa astepti pana cand o sa primesti un raspuns.");

	format(newbieText[playerid], 128, "%s", question);
	va_SendClientMessage(playerid, -1, "{095393}Jucatorul %s intreaba: %s", GetName(playerid), question);

	Iter_Add(serverNewbies, playerid); 
	return 1;
}

YCMD:questions(playerid, params[], help) {
	if(playerVariables[ playerid ][ pAdmin ] >= 1 || playerVariables[ playerid ][ pHelper ] >= 1) {
		SendClientMessage(playerid, -1, "{2F8EF0}----- Intrebari -----");
		foreach(new x : serverNewbies) {
			va_SendClientMessage(playerid, -1, "(%d) %s - %s", x, GetName(x), newbieText[ x ]);
		}
		va_SendClientMessage(playerid, -1, "{2F8EF0}----- Sunt in total %d intrebari -----", Iter_Count(serverNewbies));
	}	
	else return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");
	return 1;
}

YCMD:newbiereply(playerid, params[], help) {
	if(playerVariables[ playerid ][ pAdmin ] >= 1 || playerVariables[ playerid ][ pHelper ] >= 1) {
		new nuserid, replyquestion[128];

		if(sscanf(params,"us[128]", nuserid, replyquestion)) return sendSyntax(playerid, "/(n)ewbie(r)eply [player name] [text]");
		if(strlen(replyquestion) < 4 || strlen(replyquestion) > 128) return sendError(playerid, "Text-ul introdus trebuie sa contina 4-128 caractere.");

		if(!Iter_Contains(serverNewbies, nuserid)) return sendError(playerid, "Acest jucator nu are o intrebare.");

		if(playerVariables[ playerid ][ pHelper ] < 1) {
			va_SendClientMessageToAll(-1, "{095393}Jucatorul %s intreaba: %s", GetName(nuserid), newbieText[ nuserid ]);
			va_SendClientMessageToAll(-1, "{095393}Helper-ul %s raspunde: %s", GetName(playerid), replyquestion);
			va_SendClientMessage(nuserid, -1, "{EFB507}Helper-ul %s raspunde: %s", GetName(playerid), replyquestion);
		} else {
			va_SendClientMessageToAll(-1, "{095393}Jucatorul %s intreaba: %s", GetName(nuserid), newbieText[ nuserid ]);
			va_SendClientMessageToAll(-1, "{095393}Admin-ul %s raspunde: %s", GetName(playerid), replyquestion);
			va_SendClientMessage(nuserid, -1, "{EFB507}Admin-ul %s raspunde: %s", GetName(playerid), replyquestion);
		}	

		newbieDeelay[ nuserid ] = 30;

		Iter_Remove(serverNewbies, nuserid); 
	}
	else return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

// ===============> ( admin cmds >
/*YCMD:adminhelp(playerid, params[], help) {
	switch(playerVariables[ playerid ][ pAdmin ]) {
		case
		admin 1: /(a)dminchat /reports /(c)lose(r)eport
	}
	return 1;
}
*/
YCMD:adminchat(playerid, params[], help) {
	if(playerVariables[ playerid ][ pAdmin ] >= 1) {
		new admintext[128];
		if(sscanf(params,"s[128]", admintext)) return sendSyntax(playerid, "/(a)dminchat [text]");

		sendStaff(0, -1, "(%d) %s: %s", playerVariables[ playerid ][ pAdmin ], GetName(playerid), admintext);
	}
	else return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

YCMD:vspawn(playerid, params[], help) {
	if(playerVariables[ playerid ][ pAdmin ] < 1) return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");
	new model;
	if(sscanf(params, "i", model)) return sendSyntax(playerid, "/vspawn [model(400-611)]");
	if(model < 400 || model > 611) return sendError(playerid, "Model invalid.");
    
    new Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X,Y,Z);
     
    new carid = CreateVehicle(model, X,Y,Z, 0.0,  -1, -1, -1);

	LinkVehicleToInterior(carid, GetPlayerInterior(playerid));
	SetVehicleVirtualWorld(carid, GetPlayerVirtualWorld(playerid));
	PutPlayerInVehicle(playerid, carid, 0);	
	return 1;
}

YCMD:admins(playerid, params[], help) {
	SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Adminstratori conectati {2F8EF0}-----");

	foreach(new x : serverAdmins) {
		va_SendClientMessage(playerid, -1, "(%d) %s - admin de nivel %d", x, GetName(x), playerVariables[ x ][ pAdmin ]);
	}

	SendClientMessage(playerid, -1, "{2F8EF0}___________________________________");
	SendClientMessage(playerid, -1, "Pentru orice nelamurire foloseste comanda '/report'.");
	va_SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Sunt in total %d admini conectati {2F8EF0}-----", Iter_Count(serverAdmins));
	return 1;
}

YCMD:helpers(playerid, params[], help) {
	SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Helperi conectati {2F8EF0}-----");

	foreach(new x : serverHelpers) {
		va_SendClientMessage(playerid, -1, "(%d) %s - helper de nivel %d", x, GetName(x), playerVariables[ x ][ pHelper ]);
	}

	SendClientMessage(playerid, -1, "{2F8EF0}___________________________________");
	SendClientMessage(playerid, -1, "Pentru orice nelamurire foloseste comanda '/newbie'.");
	va_SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Sunt in total %d helperi conectati {2F8EF0}-----", Iter_Count(serverHelpers));
	return 1;
}

YCMD:report(playerid, params[], help) {
	new report[128];
	if(sscanf(params,"s[128]", report)) return sendSyntax(playerid, "/report [text]");
	if(strlen(report) < 4) return sendError(playerid, "Report-ul trebuie sa contina minim 4 caractere.");

	/*if(playerVariables[ playerid ][ pAdmin ] > 0 || playerVariables[ playerid ][ pHelper ] > 0) 
		return sendError(playerid, "Nu poti sa folosesti aceasta comanda deoarece esti admin / helper.");*/

	if(reportDeelay[ playerid ] != 0) return sendError(playerid, "Nu poti sa folosesti aceasta comanda deoarece ai deelay.");

	if(Iter_Count(serverReports) == MAX_REPORTS) return sendError(playerid, "Limita maxima de report-uri a fost atinsa.");
	if(Iter_Contains(serverReports, playerid)) return sendError(playerid, "Ai trimis deja un report, trebuie sa astepti pana cand acesta o sa fie rezolvat.");

	format(reportText[playerid], 128, "%s", report);
	SendClientMessage(playerid, -1, "Report-ul tau a fost trimis adminilor conectati.");

	Iter_Add(serverReports, playerid); 
	return 1;
}

YCMD:reports(playerid, params[], help) {
	if(playerVariables[ playerid ][ pAdmin ] >= 1) {
		if(Iter_Count(serverReports) == 0) return sendError(playerid, "In acest moment nu mai exista niciun report activ.");
		
		SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Report-uri active {2F8EF0}-----");
		foreach(new x : serverReports) {
			va_SendClientMessage(playerid, -1, "(%d) %s - %s", x, GetName(x), reportText[ x ]);
		}
	}
	else return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

YCMD:closereport(playerid, params[], help) {	
	if(playerVariables[ playerid ][ pAdmin ] >= 1) {
		new reportuserid, closereport[128];

		if(sscanf(params,"us[128]", reportuserid, closereport)) return sendSyntax(playerid, "/(c)lose(r)eport [player name] [text]");
		if(strlen(closereport) < 4) return sendError(playerid, "Text-ul introdus trebuie sa contina minim 4 caractere.");

		if(!Iter_Contains(serverReports, reportuserid)) return sendError(playerid, "Acest jucator nu are un report.");

		va_SendClientMessage(playerid, -1, "Admin-ul %s a inchis report-ul lui %s, answer: %s", GetName(playerid), GetName(reportuserid), closereport);
		va_SendClientMessage(reportuserid, -1, "Admin-ul %s a raspuns report-ului: %s", GetName(playerid), closereport);

		reportDeelay[ reportuserid ] = 30;

		Iter_Remove(serverReports, reportuserid); 
	}
	else return sendError(playerid, "Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}
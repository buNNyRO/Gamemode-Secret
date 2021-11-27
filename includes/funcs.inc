serverSys() {
	mysql_tquery(SQL, "SELECT * FROM `businesses`", "loadDynamicBIZZ", "");
	mysql_tquery(SQL, "SELECT * FROM `dmv_cp`", "loadDynamicDMV", "");
	return true;
}

function loadDynamicDMV() {
	if(!cache_num_rows()) return print("SQL >> No rows found in 'dmv_cp' table.");
	for(new i = 1; i < cache_num_rows() + 1; i++) {
		Iter_Add(serverExam, i);

		cache_get_value_name_int(i - 1, "id", examVariables[i][dID]);
		cache_get_value_name_float(i - 1, "posx", examVariables[i][dPosX]);
		cache_get_value_name_float(i - 1, "posy", examVariables[i][dPosY]);
		cache_get_value_name_float(i - 1, "posz", examVariables[i][dPosZ]);
	}
	printf("SQL >> Loaded %d dmv checkpoints.", Iter_Count(serverExam));
	return true;
}

stock formats(const text[], va_args<>) {
	String[0] = (EOS); va_format(String, sizeof String, text, va_start<1>);
	return String;
}

stock query(const text[], va_args<>) {
	dbQuery[0] = (EOS); va_format(dbQuery, sizeof dbQuery, text, va_start<1>);
	return dbQuery;
}

function CheckAccount(playerid) {
	// InterpolateCameraPos(playerid, 1796.492675, 796.915527, 40.102069, 1800.619262, 2315.061035, 48.685932, 60000);
	// InterpolateCameraLookAt(playerid, 1796.492675, 796.915527, 40.102069, 1800.619262, 2315.061035, 48.685932, 60000);

	InterpolateCameraPos(playerid, 1796.304565, 748.975952, 23.093755, 1797.538696, 2310.052490, 27.493625, 60000);
	InterpolateCameraLookAt(playerid, 1796.304565, 748.975952, 23.093755, 1797.538696, 2310.052490, 27.493625, 10000);	
    clearChat(playerid, 20);
    new rows; cache_get_row_count(rows);
    if(rows) defer beginConnect(playerid);
    else defer TutorialStart(playerid);
    return 1;
}

function AccountLoad(playerid) {
    new rows;
    cache_get_row_count(rows);
    if(!rows) Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, PRIMARY"Login"ALB" Parola", "Bine ai (re)venit !\nTrebuie sa iti introduci parola contului tau mai jos.\n", "Login", "Cancel");
    else {
    	cache_get_value_name_int(0, "id", PlayerInfo[playerid][pSQLID]);
        cache_get_value_name(0, "password", PlayerInfo[playerid][pPassword]);
		cache_get_value_name(0, "mail", PlayerInfo[playerid][pMail]);
		cache_get_value_name_int(0, "gender", PlayerInfo[playerid][pGender]);
		cache_get_value_name_int(0, "admin", PlayerInfo[playerid][pAdmin]);
		cache_get_value_name_int(0, "helper", PlayerInfo[playerid][pHelper]);
		cache_get_value_name_int(0, "businnes", PlayerInfo[playerid][pBizz]);

		new string[48];
		cache_get_value_name(0, "licenses", string, 48);
		sscanf(string, "p<|>dddddddd", PlayerInfo[playerid][pDrivingLicense], PlayerInfo[playerid][pDrivingLicenseSuspend], PlayerInfo[playerid][pGunLicense], PlayerInfo[playerid][pGunLicenseSuspend], PlayerInfo[playerid][pFlyLicense], PlayerInfo[playerid][pFlyLicenseSuspend], PlayerInfo[playerid][pSailingLicenseSuspend], PlayerInfo[playerid][pSailingLicenseSuspend]);

		format(PlayerInfo[playerid][pUsername], 32, "%s", GetName(playerid));

		TogglePlayerSpectating(playerid, 0);
		StopAudioStreamForPlayer(playerid);

		if(PlayerInfo[playerid][pAdmin] > 0) {
			Iter_Add(serverAdmins, playerid);
			Iter_Add(serverStaff, playerid);
			va_SendClientMessage(playerid, -1, SECCONDARY"STAFF"ALB" >> Te-ai logat ca admin de nivel %d.", PlayerInfo[playerid][pAdmin]);
		}

		if(PlayerInfo[playerid][pHelper] > 0) {
			Iter_Add(serverHelpers, playerid);
			Iter_Add(serverStaff, playerid);
			va_SendClientMessage(playerid, -1, SECCONDARY"STAFF"ALB" >> Te-ai logat ca helper de nivel %d.", PlayerInfo[playerid][pHelper]);
		}

		va_PlayerTextDrawSetString(playerid, Logo, "%s/RPG.BLACK~p~MOON~w~.RO", GetName(playerid));
		PlayerTextDrawShow(playerid, Logo);

		TogglePlayerSpectating(playerid, 0);
        SpawnPlayer(playerid);
    }
	return 1;
}

stock GetName(playerid) {
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof name);
	return name;
}

stock FormatNumber(iNum, const szChar[] = ".") {
    new
        szStr[16]
    ;
    format(szStr, sizeof(szStr), "%d", iNum);

    for(new iLen = strlen(szStr) - 3; iLen > 0; iLen -= 3)
    {
        strins(szStr, szChar, iLen);
    }
    return szStr;
}

stock IsValidEmail(email[]) {
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

stock sendStaff(typeid, colour, const text[], va_args<>) {
	switch(typeid) {
		case 0: foreach(new x : serverAdmins) return va_SendClientMessage(x, colour, va_return(text, va_start<3>));
		case 1: foreach(new x : serverStaff) return va_SendClientMessage(x, colour, va_return(text, va_start<3>));
	}
	return 1;
}

stock va_PlayerTextDrawSetString(playerid, PlayerText:text, const fmat[], va_args<>) {
    return PlayerTextDrawSetString(playerid, text, va_return(fmat, va_start<3>));
}

stock sendNearbyMessage(playerid, Float:range, color, const text[], va_args<>) {
	new Float:Xpos, Float:Y, Float: Z; 
	GetPlayerPos(playerid, Xpos, Y, Z);
	foreach(new x: Player) { 
		if(IsPlayerConnected(x) && IsPlayerInRangeOfPoint(x, range, Xpos, Y, Z) && GetPlayerVirtualWorld(x) == GetPlayerVirtualWorld(playerid) && GetPlayerInterior(x) == GetPlayerInterior(playerid)) { 
			SendClientMessage(x, color, va_return(text, va_start<4>)); 
		} 
	}
	return 1; 
}

stock changeEngineStatus(playerid) {
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;

	if(engineStatus[GetPlayerVehicleID(playerid)] == 1) {
		engineStatus[GetPlayerVehicleID(playerid)] = 0;
		GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
		SetVehicleParamsEx(GetPlayerVehicleID(playerid), VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
		sendNearbyMessage(playerid, 25.0, COLOR_PURPLE, "* %s a oprit motorul vehiculului %s.", GetName(playerid), vehicleName[GetVehicleModel(GetPlayerVehicleID(playerid)) - 400]);
	}
	else {
		engineStatus[GetPlayerVehicleID(playerid)] = 1;
		GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
		SetVehicleParamsEx(GetPlayerVehicleID(playerid), VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		sendNearbyMessage(playerid, 25.0, COLOR_PURPLE, "* %s a pornit motorul vehiculului %s.", GetName(playerid), vehicleName[GetVehicleModel(GetPlayerVehicleID(playerid)) - 400]);
	}
	return 1;
}

stock examPlayerFailed(playerid) {
	if(IsValidVehicle(PlayerInfo[playerid][pExamVehicle])) {
		DestroyVehicle(PlayerInfo[playerid][pExamVehicle]);
	}	

	DisablePlayerCheckpoint(playerid);

	PlayerTextDrawHide(playerid, Info);

	PlayerInfo[playerid][pExamVehicle] = INVALID_VEHICLE_ID;
	PlayerInfo[playerid][pExamCheckpoint] = 0;

	SetPlayerPos(playerid, 2781.2178, -1813.0525, 11.8438);
	SetPlayerVirtualWorld(playerid, 0);
	
	SendClientMessage(playerid, -1, "Ai esuat examen-ul pentru permis-ul auto. Trebuie sa te duci din nou la DMV pentru a da testul din nou.");
	return 1;
}
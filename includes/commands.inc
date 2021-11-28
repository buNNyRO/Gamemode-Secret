bunny:engine(playerid, params[]) {
	if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, -1, EROR"Nu esti intr-un vehicul.");
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SCM(playerid, -1, EROR"Nu esti soferul unui vehicul.");

	changeEngineStatus(playerid);
	return 1;
}

bunny:exam(playerid, params[]) {
	new cp = Iter_First(serverExam);

	/*if(gCheckpoint[playerid] > 0 || TargetFind[playerid] != INVALID_PLAYER_ID)
		return SendClientMessage(playerid, COLOR_WHITE, "Error: You have an active checkpoint, type /killcp to eliminate checkpoint.");*/

	if(PlayerInfo[playerid][pDrivingLicense] > 0)
		return SCM(playerid, -1, EROR"Nu poti sa incepi examenul deoarece ai deja o licenta pentru condus valida.");

	if(!IsPlayerInRangeOfPoint(playerid, 3.0, 1707.1661,1362.4440,10.7520) || GetPlayerVirtualWorld(playerid) != 0)
		return SCM(playerid, -1, EROR"Nu esti in zona DMV-ului. Foloseste comanda '/locations' pentru a ajunge la aceasta locatie.");

	if(IsPlayerInAnyVehicle(playerid))
		return SCM(playerid, -1, EROR"Nu poti sa folosesti aceasta comanda cand esti intr-o masina.");

	if(!Iter_Count(serverExam))
		return SCM(playerid, -1, EROR"Este o problema tehnica la aceasta comanda, foloseste comanda '/report' pentru a raporta acest lucru.");

	SetPlayerVirtualWorld(playerid, (playerid + 1));
	PlayerInfo[playerid][pExamVehicle] = CreateVehicle(410, 2795.6907, -1848.8398, 9.5496, 268.0729, 84, 63, -1);

	SetVehicleVirtualWorld(PlayerInfo[playerid][pExamVehicle], (playerid + 1));
	PutPlayerInVehicle(playerid, PlayerInfo[playerid][pExamVehicle], 0);

	SetPlayerCheckpoint(playerid, examVariables[cp][dPosX], examVariables[cp][dPosY], examVariables[cp][dPosZ], 3.0);
	PlayerInfo[playerid][pExamCheckpoint] = Iter_First(serverExam);

	PlayerTextDrawShow(playerid, Info);
	va_PlayerTextDrawSetString(playerid, Info, "DRIVING LICENSE~n~Checkpoints: ~y~%d~w~/~y~%d", PlayerInfo[playerid][pExamCheckpoint] - 1, Iter_Count(serverExam));
	
	SendClientMessage(playerid, -1, "Examen-ul pentru permis-ul auto a inceput.");
	SendClientMessage(playerid, -1, "Tasteaza comanda '/engine' sau apasa pe tasta '2' pentru a porni motorul masinii.");	
	return 1;
}

bunny:helperchat(playerid, params[]) {
	if(PlayerInfo[playerid][pHelper] < 1 || PlayerInfo[playerid][pAdmin] < 1) 
		return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");

	new helpertext[128];
	if(sscanf(params,"s[128]", helpertext)) return SCM(playerid, -1, SYNTAX"/(h)elperchat [text]");

	if(PlayerInfo[playerid][pHelper] < 1) {
		sendStaff(1, -1, "(helper %d) %s: %s", PlayerInfo[playerid][pHelper], GetName(playerid), helpertext);
	} else {
		sendStaff(1, -1, "(admin %d) %s: %s", PlayerInfo[playerid][pAdmin], GetName(playerid), helpertext);
	}	
	return 1;
}

alias:newbie("n")
bunny:newbie(playerid, params[]) {
	new question[128];
	if(sscanf(params,"s[128]", question)) return SCM(playerid, -1, SYNTAX"/(n)ewbie [question]");
	if(strlen(question) < 4 || strlen(question) > 128) return SCM(playerid, -1, EROR"Intrebarea trebuie sa contina 4-128 caractere.");

	/*if(PlayerInfo[playerid][pAdmin] > 0 || PlayerInfo[playerid][pHelper] > 0) 
		return SCM(playerid, -1, EROR"Nu poti sa folosesti comanda '/(n)ewbie' deoarece esti admin / helper.");*/

	if(newbieDeelay[playerid] != 0) return SCM(playerid, -1, EROR"Nu poti sa folosesti aceasta comanda deoarece ai deelay.");

	if(Iter_Count(serverNewbies) == MAX_REPORTS) return SCM(playerid, -1, EROR"Limita maxima de intrebari a fost atinsa.");
	if(Iter_Contains(serverNewbies, playerid)) return SCM(playerid, -1, EROR"Ai trimis deja o intrebare, trebuie sa astepti pana cand o sa primesti un raspuns.");

	format(newbieText[playerid], 128, "%s", question);
	va_SendClientMessage(playerid, -1, "{095393}Jucatorul %s intreaba: %s", GetName(playerid), question);

	Iter_Add(serverNewbies, playerid); 
	return 1;
}

bunny:questions(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 1 || PlayerInfo[playerid][pHelper] >= 1) {
		SendClientMessage(playerid, -1, "{2F8EF0}----- Intrebari -----");
		foreach(new x : serverNewbies) {
			va_SendClientMessage(playerid, -1, "(%d) %s - %s", x, GetName(x), newbieText[x]);
		}
		va_SendClientMessage(playerid, -1, "{2F8EF0}----- Sunt in total %d intrebari -----", Iter_Count(serverNewbies));
	}	
	else return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");
	return 1;
}

alias:newbiereply("nr")
bunny:newbiereply(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 1 || PlayerInfo[playerid][pHelper] >= 1) {
		new nuserid, replyquestion[128];

		if(sscanf(params,"us[128]", nuserid, replyquestion)) return SCM(playerid, -1, SYNTAX"/(n)ewbie(r)eply [player name] [text]");
		if(strlen(replyquestion) < 4 || strlen(replyquestion) > 128) return SCM(playerid, -1, EROR"Text-ul introdus trebuie sa contina 4-128 caractere.");

		if(!Iter_Contains(serverNewbies, nuserid)) return SCM(playerid, -1, EROR"Acest jucator nu are o intrebare.");

		if(PlayerInfo[playerid][pHelper] < 1) {
			va_SendClientMessageToAll(-1, "{095393}Jucatorul %s intreaba: %s", GetName(nuserid), newbieText[nuserid]);
			va_SendClientMessageToAll(-1, "{095393}Helper-ul %s raspunde: %s", GetName(playerid), replyquestion);
			va_SendClientMessage(nuserid, -1, "{EFB507}Helper-ul %s raspunde: %s", GetName(playerid), replyquestion);
		} else {
			va_SendClientMessageToAll(-1, "{095393}Jucatorul %s intreaba: %s", GetName(nuserid), newbieText[nuserid]);
			va_SendClientMessageToAll(-1, "{095393}Admin-ul %s raspunde: %s", GetName(playerid), replyquestion);
			va_SendClientMessage(nuserid, -1, "{EFB507}Admin-ul %s raspunde: %s", GetName(playerid), replyquestion);
		}	

		newbieDeelay[nuserid] = 30;

		Iter_Remove(serverNewbies, nuserid); 
	}
	else return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

/*bunny:adminhelp(playerid, params[]) {
	switch(PlayerInfo[playerid][pAdmin]) {
		case
		admin 1: /(a)dminchat /reports /(c)lose(r)eport
	}
	return 1;
}
*/

bunny:gotoxyz(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] < 3) return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");
	new Float: x, Float: y, Float: z, interior, virtual;
	if(sscanf(params,"fffdd", x,y,z,virtual,interior)) return SCM(playerid, -1, SYNTAX"/gotoxyz [x] [y] [z] [vw] [i]");
	SetPlayerPos(playerid, x,y,z);
	SetPlayerInterior(playerid, interior);
	SetPlayerVirtualWorld(playerid, virtual);
	return 1;
}

alias:adminchat("a")
bunny:adminchat(playerid, params[]) {
	PlayAudioStreamForPlayer(playerid, "https://uploadir.com/u/6q5yfgdt");

	if(PlayerInfo[playerid][pAdmin] >= 1) {
		new admintext[128];
		if(sscanf(params,"s[128]", admintext)) return SCM(playerid, -1, SYNTAX"/(a)dminchat [text]");

		sendStaff(0, -1, "(%d) %s: %s", PlayerInfo[playerid][pAdmin], GetName(playerid), admintext);
	}
	else return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

bunny:vspawn(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 1) {
		new model;
		if(sscanf(params, "i", model)) return SCM(playerid, -1, SYNTAX"/vspawn [model(400-611)]");
		if(model < 400 || model > 611) return SCM(playerid, -1, EROR"Model invalid.");
	    
	    new Float:X, Float:Y, Float:Z;
		GetPlayerPos(playerid, X,Y,Z);
	     
	    new carid = CreateVehicle(model, X,Y,Z, 0.0,  -1, -1, -1);

		LinkVehicleToInterior(carid, GetPlayerInterior(playerid));
		SetVehicleVirtualWorld(carid, GetPlayerVirtualWorld(playerid));
		PutPlayerInVehicle(playerid, carid, 0);
	}
	else return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

bunny:admins(playerid, params[]) {
	SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Adminstratori conectati {2F8EF0}-----");

	foreach(new x : serverAdmins) {
		va_SendClientMessage(playerid, -1, "(%d) %s - admin de nivel %d", x, GetName(x), PlayerInfo[x][pAdmin]);
	}

	SendClientMessage(playerid, -1, "{2F8EF0}___________________________________");
	SendClientMessage(playerid, -1, "Pentru orice nelamurire foloseste comanda '/report'.");
	va_SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Sunt in total %d admini conectati {2F8EF0}-----", Iter_Count(serverAdmins));
	return 1;
}

bunny:helpers(playerid, params[]) {
	SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Helperi conectati {2F8EF0}-----");

	foreach(new x : serverHelpers) {
		va_SendClientMessage(playerid, -1, "(%d) %s - helper de nivel %d", x, GetName(x), PlayerInfo[x][pHelper]);
	}

	SendClientMessage(playerid, -1, "{2F8EF0}___________________________________");
	SendClientMessage(playerid, -1, "Pentru orice nelamurire foloseste comanda '/newbie'.");
	va_SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Sunt in total %d helperi conectati {2F8EF0}-----", Iter_Count(serverHelpers));
	return 1;
}

bunny:report(playerid, params[]) {
	new report[128];
	if(sscanf(params,"s[128]", report)) return SCM(playerid, -1, SYNTAX"/report [text]");
	if(strlen(report) < 4) return SCM(playerid, -1, EROR"Report-ul trebuie sa contina minim 4 caractere.");

	/*if(PlayerInfo[playerid][pAdmin] > 0 || PlayerInfo[playerid][pHelper] > 0) 
		return SCM(playerid, -1, EROR"Nu poti sa folosesti aceasta comanda deoarece esti admin / helper.");*/

	if(reportDeelay[playerid] != 0) return SCM(playerid, -1, EROR"Nu poti sa folosesti aceasta comanda deoarece ai deelay.");

	if(Iter_Count(serverReports) == MAX_REPORTS) return SCM(playerid, -1, EROR"Limita maxima de report-uri a fost atinsa.");
	if(Iter_Contains(serverReports, playerid)) return SCM(playerid, -1, EROR"Ai trimis deja un report, trebuie sa astepti pana cand acesta o sa fie rezolvat.");

	format(reportText[playerid], 128, "%s", report);
	SendClientMessage(playerid, -1, "Report-ul tau a fost trimis adminilor conectati.");

	Iter_Add(serverReports, playerid); 
	return 1;
}

bunny:reports(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 1) {
		if(Iter_Count(serverReports) == 0) return SCM(playerid, -1, EROR"In acest moment nu mai exista niciun report activ.");
		
		SendClientMessage(playerid, -1, "{2F8EF0}----- {ffffff}Report-uri active {2F8EF0}-----");
		foreach(new x : serverReports) {
			va_SendClientMessage(playerid, -1, "(%d) %s - %s", x, GetName(x), reportText[x]);
		}
	}
	else return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

alias:closereport("cr")
bunny:closereport(playerid, params[]) {	
	if(PlayerInfo[playerid][pAdmin] >= 1) {
		new reportuserid, closereport[128];

		if(sscanf(params,"us[128]", reportuserid, closereport)) return SCM(playerid, -1, SYNTAX"/(c)lose(r)eport [player name] [text]");
		if(strlen(closereport) < 4) return SCM(playerid, -1, EROR"Text-ul introdus trebuie sa contina minim 4 caractere.");

		if(!Iter_Contains(serverReports, reportuserid)) return SCM(playerid, -1, EROR"Acest jucator nu are un report.");

		va_SendClientMessage(playerid, -1, "Admin-ul %s a inchis report-ul lui %s, answer: %s", GetName(playerid), GetName(reportuserid), closereport);
		va_SendClientMessage(reportuserid, -1, "Admin-ul %s a raspuns report-ului: %s", GetName(playerid), closereport);

		reportDeelay[reportuserid] = 30;

		Iter_Remove(serverReports, reportuserid); 
	}
	else return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}

bunny:addexamcp(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] == 6) {
		if(Iter_Count(serverExam) >= MAX_DMV_CP)
			return SCM(playerid, -1, EROR"Limita maxima de checkpoint-uri a fost atinsa.");

		dbQuery[0] = (EOS);
		new i = (Iter_Count(serverExam) + 1);
		GetPlayerPos(playerid, examVariables[i][dPosX], examVariables[i][dPosY], examVariables[i][dPosZ]);

		mysql_tquery(SQL, query("INSERT INTO `dmv_cp` (`posx`, `posy`, `posz`) VALUES ('%f', '%f', '%f')", examVariables[i][dPosX], examVariables[i][dPosY], examVariables[i][dPosZ]), "", "");
		Iter_Add(serverExam, i);
		examVariables[i][dID] = i;

		sendStaff(0, COLOR_LIGHTRED, "Admbunny: %s a adaugat un nou checkpoint cu id-ul %d la examen-ul auto.", GetName(playerid), i);
	}
	else return SCM(playerid, -1, EROR"Nu ai gradul administrativ necesar pentru a folosii aceasta comanda.");	
	return 1;
}
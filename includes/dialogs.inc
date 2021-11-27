Dialog:DIALOG_MAIL(playerid, response, listitem, inputtext[]) {
    if(!response) Dialog_Show(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, PRIMARY"Register:"ALB" Mail", "Trebuie sa adaugi un email valid.\n", "Next", "");
    if(!strlen(inputtext)) return Dialog_Show(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, PRIMARY"Register:"ALB" Mail", "Trebuie sa introduci un mail valid pentru a te inregistra.\n", "Next", "");
    if(!IsValidEmail(inputtext)) return Dialog_Show(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, PRIMARY"Register:"ALB" Mail", "Trebuie sa introduci un mail valid pentru a te inregistra.\n", "Next", "");
   	if(strlen(inputtext) < 6 || strlen(inputtext) > 64) return Dialog_Show(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, PRIMARY"Register:"ALB" Mail", "Mail-ul trebuie sa contina intre 6-64 caractere.\n\nTrebuie sa introduci un mail valid pentru a te inregistra.\n", "Next", "");
   	format(PlayerInfo[playerid][pMail], 65, inputtext);
    Dialog_Show(playerid, DIALOG_GENDER, DIALOG_STYLE_MSGBOX, PRIMARY"Register:"ALB" Gender", "Trebuie sa selectezi un gen.\n", "Masculin", "Feminin");
    return 1;
}
Dialog:DIALOG_GENDER(playerid, response, listitem, inputtext[]) {
    if(!response) PlayerInfo[playerid][pGender] = 1;
    else PlayerInfo[playerid][pGender] = 0;
    Dialog_Show(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, PRIMARY"Register"ALB" Parola", "Trebuie sa adaugi o parola valida pentru a te inregistra.\n", "Finish", "");
    return 1;
}
Dialog:DIALOG_PASSWORD(playerid, response, listitem, inputtext[]) {
    if(!response) Dialog_Show(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, PRIMARY"Register"ALB" Parola", "Trebuie sa adaugi o parola valida pentru a te inregistra.\n", "Finish", "");
    if(!strlen(inputtext)) return Dialog_Show(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, PRIMARY"Register"ALB" Parola", "Trebuie sa introduci o parola valida pentru a te inregistra.\n", "Finish", "");
   	if(strlen(inputtext) < 6 || strlen(inputtext) > 32) return Dialog_Show(playerid, DIALOG_PASSWORD, DIALOG_STYLE_PASSWORD, PRIMARY"Register"ALB" Parola", "Parola trebuie sa contina intre 3-32 caractere.\n\nTrebuie sa introduci o parola valida pentru a te inregistra.\n", "Finish", "");
    
    SHA256_PassHash(inputtext, password_salt, PlayerInfo[playerid][pPassword], 65);

    // inserarea
    new ip[18];
	GetPlayerIp(playerid, ip, sizeof(ip));
		   
	mysql_tquery(SQL, query("INSERT INTO `users` (`name`, `password`, `mail`, `gender`, `ip`) VALUES ('%s', '%s', '%s', '%d', '%s')", GetName(playerid), PlayerInfo[playerid][pPassword], PlayerInfo[playerid][pMail], PlayerInfo[playerid][pGender], ip), "", "");
		   
	Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, PRIMARY"Login"ALB" Parola", "Bine ai (re)venit !\nTrebuie sa iti introduci parola contului tau mai jos.\n", "Login", "Cancel");
    return 1;
}
Dialog:DIALOG_LOGIN(playerid, response, listitem, inputtext[]) {
    if(!response) return Kick(playerid);
    if(!strlen(inputtext)) return Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, PRIMARY"Login"ALB" Parola", "Bine ai (re)venit !\nTrebuie sa iti introduci parola contului tau mai jos.\n", "Login", "Cancel");

    new passwordHash[65];
    SHA256_PassHash(inputtext, password_salt, passwordHash, sizeof passwordHash);
	mysql_tquery(SQL, query("SELECT * FROM `users` WHERE `name`='%s' AND `password` = '%s'", GetName(playerid), passwordHash), "AccountLoad", "d", playerid);
    return 1;
}
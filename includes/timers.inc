timer beginConnect[3000](playerid) {
	Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, PRIMARY"Login"ALB" Parola", "Bine ai (re)venit !\nTrebuie sa iti introduci parola contului tau mai jos.\n", "Login", "Cancel");
	return 1;
}

timer TutorialStart[7000](playerid) {
	Dialog_Show(playerid, DIALOG_MAIL, DIALOG_STYLE_INPUT, PRIMARY"Register"ALB" Mail", "Trebuie sa adaugi un email valid.\n", "Next", "");
	return 1;
}

task TimerSecond[1000]() {
	foreach(new playerid : Player) {
		if(reportDeelay[playerid] != 0) { // la deelay am cea mai eficienta metoda :))
			reportDeelay[playerid] --;
			if(reportDeelay[playerid] == 1) SCM(playerid, -1, GRAY"[deelay]: acum poti sa folosesti comanda '/report' din nou.");
		}
		if(newbieDeelay[playerid] != 0) {
			newbieDeelay[playerid] --;
			if(newbieDeelay[playerid] == 1) SCM(playerid, -1, GRAY"[deelay]: acum poti sa folosesti comanda '/(n)ewbie' din nou.");
		}
	}

	return 1;
}
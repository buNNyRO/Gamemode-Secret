playerTD(playerid) {
	Logo = CreatePlayerTextDraw(playerid, 638.755981, 436.022460, "");
	PlayerTextDrawLetterSize(playerid, Logo, 0.202221, 1.311287);
	PlayerTextDrawAlignment(playerid, Logo, 3);
	PlayerTextDrawColor(playerid, Logo, -1);
	PlayerTextDrawSetShadow(playerid, Logo, 0);
	PlayerTextDrawSetOutline(playerid, Logo, 1);
	PlayerTextDrawBackgroundColor(playerid, Logo, 59);
	PlayerTextDrawFont(playerid, Logo, 2);
	PlayerTextDrawSetProportional(playerid, Logo, 1);
	PlayerTextDrawSetShadow(playerid, Logo, 0);

	Info = CreatePlayerTextDraw(playerid, 321.400177, 391.895538, "");
	PlayerTextDrawLetterSize(playerid, Info, 0.276888, 1.256533);
	PlayerTextDrawAlignment(playerid, Info, 2);
	PlayerTextDrawColor(playerid, Info, -1);
	PlayerTextDrawSetShadow(playerid, Info, 0);
	PlayerTextDrawSetOutline(playerid, Info, 1);
	PlayerTextDrawBackgroundColor(playerid, Info, 126);
	PlayerTextDrawFont(playerid, Info, 2);
	PlayerTextDrawSetProportional(playerid, Info, 1);
	PlayerTextDrawSetShadow(playerid, Info, 0);
	return 1;
}

globalTD() {

	return 1;
}
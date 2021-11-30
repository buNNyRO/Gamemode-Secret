#define TIME_DESPAWN 600

new Iterator:vehPers<MAX_VEHICLES>,
	HaveVeh[MAX_PLAYERS][MAX_SLOTS];
	// vehCount[MAX_VEHICLES];

// new Float:UnstruckPos[8][4] = {
// {2303.7434,1487.4240,42.2396,269.0810},{2350.7048,1415.8242,42.2419,88.4962},
// {2290.9602,1451.5975,38.9287,88.3734},{2263.6038,1408.8861,36.2006,268.6508},
// {2263.6790,1487.1947,41.2094,269.1717},{2351.3589,1490.8306,42.2405,90.6968},
// {2279.1816,1517.3660,42.2416,179.4244},{2303.7434,1487.4240,42.2396,269.0810}
// };

alias:vehicles("v","cars","mycars")
bunny:vehicles(playerid, params[]) {
	String[0] = (EOS); new x = 0, title[50];
	format(String, sizeof String, "Vehicle\tStatus\tDespawn time\n");
	foreach(new id : vehPers) {
		if(strfind(GetName(playerid), CarInfo[id][cOwner], true) != -1) {
			format(String, sizeof String, "%s%s\t%s\t%s\n", String, vehicleName[CarInfo[id][cModel]-400], (CarInfo[id][cVehID] != INVALID_VEHICLE_ID) ? "{2AE707}[spawned]{FFFFFF}" : "{E70707}[despawned]{FFFFFF}", CarInfo[id][cTime]);	
			HaveVeh[playerid][x] = id; x++;
		}
	}
	printf("%s", String);
	if(HaveVeh[playerid][0]) {
		format(title, sizeof title, "Manage your vehicles [%d/%d]{FFFF00}[+%d]", x, MAX_SLOTS, MAX_SLOTS-x);
		ShowPlayerDialog(playerid, DIALOG_VEHICLES, DIALOG_STYLE_TABLIST_HEADERS, title, String, "Select", "Cancel");
	}
	else SCM(playerid, -1, "Nu ai nici un vehicul personal.");
	return 1;
}

// bunny:park(playerid, params[]) {
// 	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SCM(playerid, -1, "Trebuie sa fii la volanul unei masini pentru a utiliza aceasta comanda.");
// 	new vehicle = GetPlayerVehicleID(playerid), id = vehCount[vehicle];
// 	if(CarInfo[id][VehID] != vehicle) return SCM(playerid, COLOR_YELLOW, "Poti folosi aceasta comanda doar in masina ta personala.");
// 	new Float: x, Float: y, Float: z, Float: a; GetVehiclePos(vehicle, x, y, z); GetVehicleZAngle(vehicle, a);
// 	park_car(id, x, y, z, a);
// 	SCMf(playerid, -1, "You parked your %s (%d).", vehicleName[CarInfo[id][Model]-400], CarInfo[id][VehID]);
// 	return 1;
// }

// function VehicleDialogs(playerid, dialogid, response, listitem, inputtext[]) {
// 	switch(dialogid) {
// 		case DIALOG_VEHICLES: {
// 			// 0 | Vehicle Info 1
// 			// 1 | Tow Vehicle
// 			// 2 | Find Vehicle
// 			// 3 | Buy Insurance 2
// 			// 4 | Buy Upgrade 3
// 			// 5 | Clear age and KM - 20 premium points 4
// 			// 6 | Change vehicle number plate 5
// 			// 7 | Unstuck vehicle
// 			// 8 | Despawn vehicle
// 			if(!response) return 1;
// 			new id = HaveVeh[playerid][listitem];
// 			HaveVeh[playerid][0] = HaveVeh[playerid][listitem];
// 			String[0] = (EOS);
// 			format(String, sizeof String, "Manage your %s %s", aVehicleNames[CarInfo[id][Model]-400], (CarInfo[id][VehID] != INVALID_VEHICLE_ID) ? "(spawned)" : "(despawned)");
// 			ShowPlayerDialog(playerid, DIALOG_VEHICLES_MENU, DIALOG_STYLE_LIST, String, "Vehicle Info\nTow Vehicle\nFind Vehicle\nBuy Insurance\nBuy Upgrade\nClear age and KM - {FFFF06}20 premium points{FFFFFF}\nChange vehicle number plate\nUnstuck vehicle\nDespawn vehicle", "Select", "Cancel");
// 		}
// 		case DIALOG_VEHICLES_MENU: {
// 			if(!response) return 1;
// 			new id = HaveVeh[playerid][0];
// 			switch(listitem) {
// 				case 0: {
// 					String[0] = (EOS);
// 					format(String, sizeof String, "Vehicle Model: %s %s\nVehicle Age: %d\nDistance Traveled: %0.1f\nInsurace price: %d\nInsurace points: %d\nColors: %d,%d", aVehicleNames[CarInfo[id][Model]-400], (CarInfo[id][VehID] != INVALID_VEHICLE_ID) ? "(spawned)" : "(despawned)", CarInfo[id][AGE], CarInfo[id][KM], CarInfo[id][INSprice], CarInfo[id][INS], CarInfo[id][cOne], CarInfo[id][cTwo]);
// 					ShowPlayerDialog(playerid, DIALOG_VEHICLES_MENU+1, DIALOG_STYLE_MSGBOX, "Vehicle info", String, "OK", "");
// 				}
// 				case 1: {
// 					if(IsVehicleOccupied(CarInfo[id][VehID])) return SCM(playerid, -1, "Imposibil de despawnat. Acest vehicul nu este liber!");
// 					CarInfo[id][UserID] = playerid;
// 					spawn_car(id);
// 					SCMf(playerid, -1, "Your car (Model: %s) has been now spawned!", aVehicleNames[CarInfo[id][Model]-400]);
// 				}
// 				case 2: {
// 					if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Foloseste mai intai 'tow car' pentru a putea gasii masina!");
// 					if(IsPlayerInVehicle(playerid, CarInfo[id][VehID])) return SCM(playerid, -1, "Te afli in vehiculul care il cauti!");
// 					SCMf(playerid, -1, "A checkpoint has been set to your %s.", aVehicleNames[CarInfo[id][Model]-400]);
// 					new Float:x,Float:y,Float:z;
// 					GetVehiclePos(CarInfo[id][VehID], x, y, z);
// 					SetPlayerCheckpointEx(playerid, x, y, z, 7.0);
// 					CP[playerid] = DIALOG_VEHICLES;
// 				}
// 				case 3: {
// 					if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Acest vehicul nu este spawnat!");
// 					if(CarInfo[id][INS] >= 10) return SCM(playerid, -1, "Ai deja 10 puncte de asigurare!");
// 					ShowPlayerDialog(playerid, DIALOG_VEHICLES_MENU+2, DIALOG_STYLE_MSGBOX, "Vehicle insurance", "Esti sigur ca vrei sa cumperi 1 punct de asigurarea?", "Da", "Nu");
// 				}
// 				case 4: SCM(playerid, -1, "Nui gata bai!");
// 				case 5: {
// 					if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Acest vehicul nu este spawnat!");
// 					ShowPlayerDialog(playerid, DIALOG_VEHICLES_MENU+4, DIALOG_STYLE_MSGBOX, "Vehicle reset", "Esti sigur ca vrei sa resetezi detaliile vehicului?", "Da", "Nu");
// 				}
// 				case 6: {
// 					if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Acest vehicul nu este spawnat!");
// 					ShowPlayerDialog(playerid, DIALOG_VEHICLES_MENU+5, DIALOG_STYLE_INPUT, "Vehicle plate", "Pentru a iti seta number plate te va costa $10.000", "OK", "Cancel");
// 				}
// 				case 7: {
// 					if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Foloseste mai intai 'tow car' pentru a putea da unstruck masinii!");
// 					if(IsVehicleOccupied(CarInfo[id][VehID])) return SCM(playerid, -1, "Imposibil de despawnat. Acest vehicul nu este liber!");
// 					unstruck_car(id);
// 				}
// 				case 8: {
// 					if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Acest vehicul nu este spawnat!");
// 					if(IsVehicleOccupied(CarInfo[id][VehID])) return SCM(playerid, -1, "Imposibil de despawnat. Acest vehicul nu este liber!");
// 					despawn_car(id);
// 					SCMf(playerid, -1, "Vehiculul tau %s a fost despawnat.", aVehicleNames[CarInfo[id][Model]-400]);
// 				}
// 			}
// 		}
// 		case DIALOG_VEHICLES_MENU+1: {
// 			if(!response) return 1;
// 			else return 1;
// 		}
// 		case DIALOG_VEHICLES_MENU+2: {
// 			if(!response) return 1;
// 			new id = HaveVeh[playerid][0];
// 			if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Acest vehicul nu este spawnat!");
// 			if(CarInfo[id][INS] >= 10) return SCM(playerid, -1, "Ai deja 10 puncte de asigurare!");
// 			CarInfo[id][INS]++;
// 			SCMf(playerid, -1, "Ai platit $%s pentru un punct de asigurare, acum ai %d/10.", FormatNumber(CarInfo[id][INSprice]), CarInfo[id][INS]);
// 			CarUpdate(id, "`INS` = '%d'", CarInfo[id][INS]);
// 		}
// 		case DIALOG_VEHICLES_MENU+4: {
// 			if(!response) return 1;
// 			if(PlayerInfo[playerid][pPremiumPoints] < 20) return SCM(playerid, -1, "Nu ai 20 premium points pentru a putea reseta vehiculul!");
// 			new id = HaveVeh[playerid][0];
// 			if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Acest vehicul nu este spawnat!");
// 			CarInfo[id][INS] = 10;
// 			CarInfo[id][INSprice] = 0;
// 			CarInfo[id][AGE] = 0;
// 			CarInfo[id][KM] = 0;
// 			CarInfo[id][GAS] = 0;
// 			SCM(playerid, -1, "Ai platit 20 premium points pentru resetarea vehiculului.");
// 			PlayerInfo[playerid][pPremiumPoints] -= 20;
// 			CarUpdate(id, "`INS` = '%d', `INSprice` = '%d', `AGE` = '%d', `KM` = '%.0f', `GAS` = '%.0f'", CarInfo[id][INS], CarInfo[id][INSprice], CarInfo[id][AGE], CarInfo[id][KM], CarInfo[id][GAS]);
// 			Update(playerid, pPremiumPointsx);
// 		}
// 		case DIALOG_VEHICLES_MENU+5: {
// 			if(!response) return 1;
// 			new id = HaveVeh[playerid][0];
// 			if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return SCM(playerid, -1, "Acest vehicul nu este spawnat!");
// 			if(!IsPlayerInVehicle(playerid, CarInfo[id][VehID])) return SCM(playerid, -1, "Nu te afli in vehiculul pe care l-ai selectat!");
// 			if(strlen(inputtext) >= 36 || strlen(inputtext) < 3) return SCM(playerid, -1, "Trebuie sa ai maxim intre 3 - 36 caractere!");
// 			format(CarInfo[id][PLATE], 36, inputtext);
// 			CarUpdate(id, "`PLATE` = '%s'", CarInfo[id][PLATE]);
// 		}
// 	}
// 	return 1;
// }

// function StartVeh(playerid) {
//     if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SCM(playerid, -1, "Trebuie sa fii la volanul unei masini pentru a utiliza aceasta comanda.");
// 	if(gettime() < GetPVarInt(playerid, "EngineDeelay")) return SCMf(playerid, COLOR_YELLOW, "Trebuie sa astepti %d secunde pana vei putea folosi din nou aceasta comanda.", GetPVarInt(playerid, "EngineDeelay") - gettime());
// 	new idcar = GetPlayerVehicleID(playerid);
// 	if(CarInfo[vehCount[idcar]][VehID] == idcar && CarInfo[vehCount[idcar]][INS] <= 0) return SCM(playerid, -1, "Acest vehicul nu are asigurarea platita.");
// 	if(FunCar[idcar][GAS] <= 0) return SCM(playerid, -1, "Acest vehicul nu are benzina.");
// 	if(IsABike(idcar)) return SCM(playerid, COLOR_WHITE, "Nu poti folosi aceasta comanda intr-o bicicleta.");
// 	new vehicle = GetVehicleModel(idcar) - 400;
// 	VehicleEngine(idcar, VEHICLE_PARAMS_ON);
// 	ProxDetector(30.0, playerid, "* %s %s motorul vehiculului %s.", COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,GetName(playerid),(FunCar[idcar][Engine] ? "a pornit" : "a oprit"),aVehicleNames[vehicle]);
// 	SetPVarInt(playerid, "EngineDeelay", gettime() + 3);
// 	return 1;
// }

// function spawn_car(id) {
// 	if(CarInfo[id][VehID] != INVALID_VEHICLE_ID) DestroyVehicle(CarInfo[id][VehID]);
// 	CarInfo[id][VehID] = CreateVehicleEx(CarInfo[id][Model], CarInfo[id][lX], CarInfo[id][lY], CarInfo[id][lZ], CarInfo[id][lA], CarInfo[id][cOne], CarInfo[id][cTwo], -1);
// 	SetVehicleNumberPlate(CarInfo[id][VehID], CarInfo[id][PLATE]);
// 	CarInfo[id][VehTime] = TIME_DESPAWN;
// 	FunCar[CarInfo[id][VehID]][GAS] = CarInfo[id][GAS];
// 	vehCount[CarInfo[id][VehID]] = id;
// 	return 1;
// }

// function despawn_car(id) {
// 	if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return 1;
// 	DestroyVehicle(CarInfo[id][VehID]);
// 	CarInfo[id][VehID] = INVALID_VEHICLE_ID;
// 	CarInfo[id][UserID] = INVALID_PLAYER_ID;
// 	CarInfo[id][VehTime] = TIME_DESPAWN;
// 	return 1;
// }

// function park_car(id, Float: x, Float: y, Float: z, Float: a) {
// 	if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return 1;
// 	DestroyVehicle(CarInfo[id][VehID]);
// 	CarInfo[id][VehID] = INVALID_VEHICLE_ID;
// 	CarInfo[id][UserID] = INVALID_PLAYER_ID;
// 	CarInfo[id][VehTime] = TIME_DESPAWN;
// 	CarInfo[id][lX] = x; CarInfo[id][lY] = y; CarInfo[id][lZ] = z; CarInfo[id][lA] = a;
// 	CarUpdate(id, "`lX` = '%f', `lY` = '%f', `lZ` = '%f', `lA` = '%f'", CarInfo[id][lX], CarInfo[id][lY], CarInfo[id][lZ], CarInfo[id][lA]);
// 	spawn_car(id);
// 	return 1;
// }

// function unstruck_car(id) {
// 	if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return 1;
// 	new unr = random(sizeof UnstruckPos);
// 	SetVehiclePos(CarInfo[id][VehID], UnstruckPos[unr][0], UnstruckPos[unr][1], UnstruckPos[unr][2]);
// 	SetVehicleZAngle(CarInfo[id][VehID], UnstruckPos[unr][3]);
// 	SCM(CarInfo[id][UserID], COLOR_YELLOW, "You successfull unstucked your vehicle. Use /v -> find vehicle to find your car.");
// 	CarInfo[id][lX] = UnstruckPos[unr][0]; CarInfo[id][lY] = UnstruckPos[unr][1]; CarInfo[id][lZ] = UnstruckPos[unr][2]; CarInfo[id][lA] = UnstruckPos[unr][3];
// 	CarUpdate(id, "`lX` = '%f', `lY` = '%f', `lZ` = '%f', `lA` = '%f'", CarInfo[id][lX], CarInfo[id][lY], CarInfo[id][lZ], CarInfo[id][lA]);CarInfo[id][lY] = UnstruckPos[unr][1];CarInfo[id][lZ] = UnstruckPos[unr][2];
// 	return 1;
// }

// function reset_car(id) {
// 	if(CarInfo[id][VehID] == INVALID_VEHICLE_ID) return 1;
// 	DestroyVehicle(CarInfo[id][VehID]);
// 	CarInfo[id][ID] = 0; 
// 	CarInfo[id][Model] = 0; 
// 	format(CarInfo[id][Owner], 36, "AdmBot");
// 	format(CarInfo[id][PLATE], 36, "Dealership");
// 	CarInfo[id][User] = 0;
// 	CarInfo[id][UserID] = INVALID_PLAYER_ID;
// 	CarInfo[id][VehID] = INVALID_VEHICLE_ID;
// 	CarInfo[id][VehTime] = TIME_DESPAWN;
// 	CarInfo[id][lX] = 0; 
// 	CarInfo[id][lY] = 0;
// 	CarInfo[id][lZ] = 0;
// 	CarInfo[id][lA] = 0;
// 	CarInfo[id][cOne] = 0;
// 	CarInfo[id][cTwo] = 0;
// 	return 1;
// }

// task vehicleDespawn[1000]() {	
// 	foreach(new id : vehPers) {
// 		if(!IsVehicleOccupied(CarInfo[id][VehID]) && CarInfo[id][VehID] != INVALID_VEHICLE_ID) {
// 			if(CarInfo[id][VehTime] != 0) CarInfo[id][VehTime]--;
// 			if(CarInfo[id][VehTime] == 0) despawn_car(id), SCMf(CarInfo[id][UserID], -1, "Vehiculul tau %s a fost despawnat din cauza inactivitatii.", aVehicleNames[CarInfo[id][Model]-400]);
// 		}
// 	}
//     foreach(new i : PlayerInVehicle) {
//     	new vehicle = GetPlayerVehicleID(i), spe = Carspeed(i), Float:health, vParams[7];
// 		GetVehicleParamsEx(vehicle, vParams[0], vParams[1], vParams[2], vParams[3], vParams[4], vParams[5], vParams[6]);
// 		GetVehicleHealth(vehicle, health);

// 		PTDF(i, VehicleHUD[12], "%d~n~KM/H", spe);
//         PTDF(i, VehicleHUD[15], "~up~ %.0f L", FunCar[vehicle][GAS]);
//         PTDF(i, VehicleHUD[14], "%.0f HP", health);
//         PTDF(i, VehicleHUD[17], "%s", vParams[3] == 1 ? "~r~Locked" : "~g~Unlocked");
//         if(CarInfo[vehCount[vehicle]][VehID] == vehicle) PTDF(i, VehicleHUD[16], "%.0fKM", CarInfo[vehCount[vehicle]][KM]);
//         else PlayerTextDrawSetString(i, VehicleHUD[16], "--");

// 		if(!IsAPlane(vehicle) || !IsABoat(vehicle) || !IsABike2(vehicle) || ExamCar[i] != vehicle || JobVehicle[i] != vehicle) {
// 			if(PlayerInfo[i][pRunOutTime] < gettime()) {
// 				PlayerInfo[i][pRunOutTime] = gettime() + RunOutTime;

// 				if(FunCar[vehicle][GAS] >= 1) {
// 		   		    if(FunCar[vehicle][Engine] == 1) {
// 						if(spe > 2 && !IsPlayerPaused(i)) {
// 		            		FunCar[vehicle][GAS]--;
// 							if(CarInfo[vehCount[vehicle]][VehID] == vehicle) {
// 					            CarInfo[vehCount[vehicle]][GAS] = FunCar[vehicle][GAS];
// 								////////////////////////////////////////////////////////////////////////////
// 								CarInfo[vehCount[vehicle]][KM] += (spe*0.27)/100;
// 								CarUpdate(vehCount[vehicle], "`GAS` = '%f', `KM` = '%f'", CarInfo[vehCount[vehicle]][GAS], CarInfo[vehCount[vehicle]][KM]);
// 							}
// 						}
// 					}
// 		   		} else {
// 	              	NoFuel[i] = 1;
// 	              	TogglePlayerControllable(i, 0);
// 		        	GameTextForPlayer(i,"No fuel",1500,3);
// 		        	VehicleEngine(vehicle, false);
// 	      	    	SetVehicleParamsEx(vehicle,VEHICLE_PARAMS_OFF,vParams[1], vParams[2], vParams[3], vParams[4], vParams[5], vParams[6]);
// 	      	    	SetPVarInt(i, "Engine", 0);
// 				}
// 			}
// 		}   	
//     }		
// 	return 1;
// }

// // stock & hook

function loadCar(playerid) {
	if(!cache_num_rows()) return print("SQL >> No rows found in 'cars' table.");
	for(new i = 1; i < cache_num_rows() + 1; i++) {
		// if( <= MAX_VEH_PER--S)
 		cache_get_value_name_int(i-1, "id", CarInfo[i][cID]);
 		cache_get_value_name_int(i-1, "model", CarInfo[i][cModel]);
 		cache_get_value_name(i-1, "owner", CarInfo[i][cOwner]);
 		cache_get_value_name(i-1, "user", CarInfo[i][cUser]);
 		cache_get_value_name_float(i-1, "x", CarInfo[i][cX]);
 		cache_get_value_name_float(i-1, "y", CarInfo[i][cY]);
 		cache_get_value_name_float(i-1, "z", CarInfo[i][cZ]);
 		cache_get_value_name_float(i-1, "a", CarInfo[i][cA]);
 		cache_get_value_name_int(i-1, "color1", CarInfo[i][cColor1]);
 		cache_get_value_name_int(i, "color2", CarInfo[i][cColor2]);

 		cache_get_value_name_int(i-1, "ins_price", CarInfo[i][cIns]);
 		cache_get_value_name_int(i-1, "ins", CarInfo[i][cInsPrice]);
 		cache_get_value_name_float(i-1, "km", CarInfo[i][cKM]);
 		cache_get_value_name_int(i-1, "age", CarInfo[i][cAge]);
 		cache_get_value_name_float(i-1, "gas", CarInfo[i][cGas]);
 		cache_get_value_name(i-1, "plate", CarInfo[i][cPlate]);

 		Iter_Add(vehPers, CarInfo[i][cID]);
 		CarInfo[i][cVehID] = INVALID_VEHICLE_ID;
 		CarInfo[i][cUserID] = playerid;
 		CarInfo[i][cTime] = TIME_DESPAWN;
 		printf("Veh: %d, Model: %d, total -: %d", CarInfo[i][cID], CarInfo[i][cModel], Iter_Count(vehPers));
 	}
	return 1;
}

// function CarUpdate(id, const varname[], va_args<>) {
//     new fzMySQL[128]; gQuery[0] = (EOS);
//     va_format(fzMySQL, sizeof fzMySQL, varname, va_start<2>);
//     format(gQuery, sizeof gQuery, "UPDATE `cars` SET %s WHERE ID = '%d'", fzMySQL, id);
//     mysql_tquery(SQL, gQuery);
//     return 1;
// }

// hook OnPlayerEnterCheckpoint(playerid) {
// 	switch(CP[playerid]) {
// 		case DIALOG_VEHICLES: {
// 			SCM(playerid, -1, "You have reached the checkpoint.");
// 			DisablePlayerCheckpointEx(playerid);
// 			CP[playerid] = 0;
// 		}
// 	}
// 	return 1;
// }

// hook OnPlayerStateChange(playerid, newstate, oldstate) {
// 	new vehicleid = GetPlayerVehicleID(playerid);
// 	switch(newstate) {
// 		case PLAYER_STATE_DRIVER: {
// 			if(CarInfo[vehCount[vehicleid]][VehID] == vehicleid) {
// 				new id = vehCount[vehicleid];
// 				if(CarInfo[vehCount[vehicleid]][INS] <= 0) {
// 					if(CarInfo[id][UserID] == playerid) ShowPlayerDialog(playerid, DIALOG_VEHICLES_MENU+2, DIALOG_STYLE_MSGBOX, "Vehicle insurance", "Esti sigur ca vrei sa cumperi 1 punct de asigurarea?", "Da", "Nu");
// 					else VehicleEngine(vehicleid, VEHICLE_PARAMS_OFF);
// 				}
// 				if(CarInfo[id][UserID] != INVALID_PLAYER_ID) SCMf(playerid, -1, "This %s (ID %d) is owned by %s | Age: %d days | Odometer: %.0f km | Colors: %d, %d | Insurance: %s (%d points)", aVehicleNames[CarInfo[id][Model]-400], CarInfo[id][VehID], CarInfo[id][Owner], CarInfo[id][AGE], CarInfo[id][KM], CarInfo[id][cOne], CarInfo[id][cTwo], FormatNumber(CarInfo[id][INSprice]), CarInfo[id][INS]);
// 				CarInfo[id][VehTime] = TIME_DESPAWN;
// 			}
// 			// speed
// 	   		Iter_Add(PlayerInVehicle, playerid);
// 			PlayerTextDrawSetPreviewModel(playerid, VehicleHUD[3], GetVehicleModel(vehicleid));
// 			va_PlayerTextDrawSetString(playerid, VehicleHUD[13], "%s", aVehicleNames[GetVehicleModel(vehicleid) - 400]);
// 			for(new i; i < 18; i++) PlayerTextDrawShow(playerid, VehicleHUD[i]);
// 			// speed
// 		}
// 		case PLAYER_STATE_ONFOOT: {
// 			// speed
// 	   		Iter_Remove(PlayerInVehicle, playerid);
// 			for(new i; i < 18; i++) PlayerTextDrawHide(playerid, VehicleHUD[i]);
// 			// speed
// 		}	
// 	}
// 	return 1;
// }

// hook OnVehicleDeath(vehicleid, killerid) {
// 	if(CarInfo[vehCount[vehicleid]][VehID] == vehicleid) {
// 		new id = vehCount[vehicleid];
// 		CarInfo[id][VehTime] = TIME_DESPAWN;
// 		CarInfo[id][INS]--;
// 		CarInfo[id][INSprice] += CarInfo[id][AGE]*100+CarInfo[id][KM]*100+random(3000);
// 		CarUpdate(id, "`INS` = '%d', `INSprice` = '%d'", CarInfo[id][INS], CarInfo[id][INSprice]);
// 	}
// 	return 1;
// }

// hook OnPlayerExitVehicle(playerid, vehicleid) {
// 	CarInfo[vehCount[vehicleid]][VehTime] = TIME_DESPAWN;
// 	return 1;
// }

// stock IsVehicleOccupied(veh) {
// 	foreach(new i : PlayerInVehicle) if(IsPlayerInVehicle(i,veh)) return 1;
// 	return 0;
// }
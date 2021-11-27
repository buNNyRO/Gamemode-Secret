/*
 *                              rl_areas 0.1.2
 *        All right reserved! © By: RaFaeL
 *
 * Owner                : RaFaeL
 * Time                 : 27.01.2013
 *
 * Thanks to:
 *                      RaFaeL							 - Scripting, PAWN
 *                      SA:MP Team past, present, future - SA:MP.
 *
 *
 * Functions:
 *          IsValidAreaID(areaid) - Check if areaID is vaild
 *          CreateArea(X, Y, Z, Radius = DEFAULT_RADIUS, Interior = -1, VirtualWorld = -1) - Create new area (return: areaid)
 *          DestroyArea(areaid) - Destroy created area
 *          IsPlayerInArea(playerid, areaid) - Check if player in specific area
 *          IsPlayerInAnyArea(playerid) - Check if player in any area
 *          GetPlayerArea(playerid) - Get player area
 *
 * CallBacks:
 *          OnPlayerEnterArea(playerid, areaid) - When player get's into area
 *          OnPlayerExitArea(playerid, areaid) - When player get's out of area
 *
 * Update:
 *      27.01.2013:
 *          Other:              - First Release
 *                              - Bug fixed
 *
 */
 
#if defined rl_areas
	#error areas is already defined!
#endif
#define rl_areas

#define MAX_AREAS      50
#define STREAM_RATE    500
#define DEFAULT_RADIUS 5.0

#define IsValidAreaID(%0) (%0 > 0 && %0 < MAX_AREAS && aInfo[%0][_aUsed])

enum aENUM {
		bool:_aUsed,
		Float:_aX,
		Float:_aY,
		Float:_aZ,
		Float:_aRadius,
		_aInterior,
		_aVirtualWorld
	};
	
new
	aInfo[MAX_AREAS][aENUM],
	SERVICE_timer = -1,
	actualAreas[MAX_PLAYERS] = {-1, ...};
	
forward OnPlayerExitArea(playerid, areaid);
forward OnPlayerEnterArea(playerid, areaid);
	
stock CreateArea(Float:X, Float:Y, Float:Z, Float:Radius = DEFAULT_RADIUS, Interior = -1, VirtualWorld = -1) {
	new 
		areaid = 1;
	while(areaid < MAX_AREAS && aInfo[areaid][_aUsed]) areaid++;
	if(areaid >= MAX_AREAS) return printf("[warning] You reached maximum number of areas (now: %d)", MAX_AREAS);
	
	if(SERVICE_timer == -1) {
		SERVICE_timer = SetTimer("SERVICE_check", STREAM_RATE, true);
	}
	
	aInfo[areaid][_aUsed] = true;
	
	aInfo[areaid][_aX] = X;
	aInfo[areaid][_aY] = Y;
	aInfo[areaid][_aZ] = Z;
	aInfo[areaid][_aRadius] = Radius;
	
	aInfo[areaid][_aInterior] = Interior;
	aInfo[areaid][_aVirtualWorld] = VirtualWorld;
	
	return areaid;
}
stock DestroyArea(areaid) {
	if(!IsValidAreaID(areaid)) return 0;
	aInfo[areaid][_aUsed] = false;
	return 1;
}

stock IsPlayerInArea(playerid, areaid)
	return (areaid > 0 && actualAreas[playerid] == areaid);
	
stock IsPlayerInAnyArea(playerid)
	return (actualAreas[playerid] != -1);
	
stock GetPlayerArea(playerid)
	return actualAreas[playerid];
	
forward SERVICE_check();
public SERVICE_check() {
	for(new i = 0, j = MAX_PLAYERS; i <j; i++) if(IsPlayerConnected(i)) {
		new
			Float:prevdist = 100000.00,
			areaid = -1;
		
		for(new area = 0, armax = MAX_AREAS; area < armax; area++) if(aInfo[area][_aUsed] &&
		(aInfo[area][_aVirtualWorld] == -1 || GetPlayerVirtualWorld(i) == aInfo[area][_aVirtualWorld]) && (aInfo[area][_aInterior] == -1 || GetPlayerInterior(i) == aInfo[area][_aInterior]) &&
		IsPlayerInRangeOfPoint(i, aInfo[area][_aRadius], aInfo[area][_aX], aInfo[area][_aY], aInfo[area][_aZ]) && GetPlayerDistanceFromPoint(i, aInfo[area][_aX], aInfo[area][_aY], aInfo[area][_aZ]) < prevdist) {
			prevdist = GetPlayerDistanceFromPoint(i, aInfo[area][_aX], aInfo[area][_aY], aInfo[area][_aZ]);
			areaid = area;
		}
		
		if(actualAreas[i] != areaid) {
			if(actualAreas[i] != 0) CallLocalFunction("OnPlayerExitArea", "dd", i, actualAreas[i]);
			actualAreas[i] = areaid;
			if(areaid != 0) CallLocalFunction("OnPlayerEnterArea", "dd", i, actualAreas[i]);
		}
	}
	return 1;
}
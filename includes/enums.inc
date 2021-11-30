enum {
	DIALOG_MAIL, DIALOG_GENDER, DIALOG_PASSWORD, DIALOG_LOGIN, DIALOG_VEHICLES, DIALOG_VEHICLES_MENU, DIALOG_VIPNAME, DIALOG_VIPNAME_EDIT
};


enum ENUM_PLAYER_DATA {
	pSQLID, pUsername[32], pMail[64], pGender, pPassword[65], pAdmin, pHelper, pExamVehicle, pExamCheckpoint, pBizz, pSkin,
	pDrivingLicense, pGunLicense, pFlyLicense, pSailingLicense, pDrivingLicenseSuspend, pGunLicenseSuspend, pFlyLicenseSuspend, pSailingLicenseSuspend
};
new PlayerInfo[MAX_PLAYERS][ENUM_PLAYER_DATA];


enum ENUM_CARS_DATA {
	cID, cModel, cOwner[32], cUser, cUserID, cVehID, cTime, Float:cX, Float:cY, Float:cZ, Float:cA, cColor1, cColor2, cInsPrice,
	cIns, Float:cKM, cAge, Float:cGas, cPlate[32]
};
new CarInfo[MAX_VEH_PERS][ENUM_CARS_DATA];


enum ENUM_DMV_DATA {
	dID, Float: dPosX, Float: dPosY, Float: dPosZ
};
new examVariables[MAX_DMV_CP][ENUM_DMV_DATA];


enum {
	DIALOG_MAIL, DIALOG_GENDER, DIALOG_PASSWORD, DIALOG_LOGIN
};

enum ENUM_PLAYER_DATA {
	pSQLID, pUsername[32], pMail[64], pGender, pPassword[65], pAdmin, pHelper, pExamVehicle, pExamCheckpoint, pBizz, pSkin,
	pDrivingLicense, pGunLicense, pFlyLicense, pSailingLicense, pDrivingLicenseSuspend, pGunLicenseSuspend, pFlyLicenseSuspend, pSailingLicenseSuspend
};

enum ENUM_DMV_DATA {
	dID, Float: dPosX, Float: dPosY, Float: dPosZ
};
new examVariables[ MAX_DMV_CP ][ ENUM_DMV_DATA ];
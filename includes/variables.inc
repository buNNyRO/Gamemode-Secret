new MySQL: SQL,

	dbQuery[512],
	String[5012],

	engine, lights, alarm, doors, bonnet, boot, objective, engineStatus[MAX_VEHICLES],

	reportText[MAX_PLAYERS][128], reportDeelay[MAX_PLAYERS],
	newbieText[MAX_PLAYERS][128], newbieDeelay[MAX_PLAYERS],

//iteratori
	Iterator:serverAdmins<MAX_PLAYERS>, Iterator:serverHelpers<MAX_PLAYERS>, Iterator:serverStaff<MAX_PLAYERS>, Iterator:serverReports<MAX_PLAYERS>, 
	Iterator:serverNewbies<MAX_PLAYERS>, Iterator: serverExam<MAX_DMV_CP>, Iterator: serverLogged<MAX_PLAYERS>,
//pText
	PlayerText:Logo, PlayerText:Info;

new vehicleName[212][] = {
	{"Landstalker"}, {"Bravura"}, {"Buffalo"}, {"Linerunner"}, {"Perrenial"}, {"Sentinel"}, {"Dumper"}, {"Firetruck"},
	{"Trashmaster"}, {"Stretch"}, {"Manana"}, {"Infernus"},	{"Voodoo"}, {"Pony"}, {"Mule"}, {"Cheetah"},
	{"Ambulance"}, {"Leviathan"}, {"Moonbeam"}, {"Esperanto"}, {"Taxi"}, {"Washington"}, {"Bobcat"}, {"Mr Whoopee"},
	{"BF Injection"}, {"Hunter"}, {"Premier"}, {"Enforcer"}, {"Securicar"}, {"Banshee"}, {"Predator"}, {"Bus"},
	{"Rhino"}, {"Barracks"}, {"Hotknife"}, {"Trailer 1"}, {"Previon"}, {"Coach"},	{"Cabbie"}, {"Stallion"},
	{"Rumpo"}, {"RC Bandit"}, {"Romero"}, {"Packer"}, {"Monster"}, {"Admiral"}, {"Squalo"}, {"Seasparrow"},
	{"Pizzaboy"}, {"Tram"},	{"Trailer 2"}, {"Turismo"}, {"Speeder"}, {"Reefer"}, {"Tropic"}, {"Flatbed"},
	{"Yankee"}, {"Caddy"}, {"Solair"}, {"Berkley's RC Van"}, {"Skimmer"}, {"PCJ-600"}, {"Faggio"}, {"Freeway"},
	{"RC Baron"}, {"RC Raider"}, {"Glendale"}, {"Oceanic"}, {"Sanchez"}, {"Sparrow"}, {"Patriot"}, {"Quad"},
	{"Coastguard"}, {"Dinghy"}, {"Hermes"},	{"Sabre"}, {"Rustler"}, {"ZR-350"}, {"Walton"}, {"Regina"},
	{"Comet"}, {"BMX"},	{"Burrito"}, {"Camper"}, {"Marquis"}, {"Baggage"}, {"Dozer"}, {"Maverick"},
	{"News Chopper"}, {"Rancher"}, {"FBI-Rancher"}, {"Virgo"}, {"Greenwood"}, {"Jetmax"}, {"Hotring"}, {"Sandking"},
	{"Blista Compact"},	{"Police Maverick"}, {"Boxville"}, {"Benson"}, {"Mesa"}, {"RC Goblin"}, {"Hotring Racer A"}, {"Hotring Racer B"},
	{"Bloodring Banger"}, {"Rancher"}, {"Super GT"}, {"Elegant"}, {"Journey"}, {"Bike"}, {"Mountain Bike"}, {"Beagle"},
	{"Cropdust"}, {"Stunt"}, {"Tanker"}, {"Roadtrain"}, {"Nebula"}, {"Majestic"}, {"Buccaneer"}, {"Shamal"},
	{"Hydra"}, {"FCR-900"},	{"NRG-500"}, {"HPV-1000"}, {"Cement Truck"}, {"Tow Truck"}, {"Fortune"}, {"Cadrona"},
	{"FBI-Truck"}, {"Willard"},	{"Forklift"}, {"Tractor"}, {"Combine"}, {"Feltzer"}, {"Remington"}, {"Slamvan"},
	{"Blade"}, {"Freight"},	{"Streak"}, {"Vortex"}, {"Vincent"}, {"Bullet"}, {"Clover"}, {"Sadler"},
	{"Firetruck LA"}, {"Hustler"}, {"Intruder"}, {"Primo"},	{"Cargobob"}, {"Tampa"}, {"Sunrise"}, {"Merit"},
	{"Utility"}, {"Nevada"}, {"Yosemite"}, {"Windsor"},	{"Monster A"}, {"Monster B"}, {"Uranus"}, {"Jester"},
	{"Sultan"}, {"Stratum"}, {"Elegy"}, {"Raindance"}, {"RC Tiger"}, {"Flash"}, {"Tahoma"}, {"Savanna"},
	{"Bandito"}, {"Freight Flat"}, {"Streak Carriage"}, {"Kart"}, {"Mower"}, {"Duneride"}, {"Sweeper"}, {"Broadway"},
	{"Tornado"}, {"AT-400"}, {"DFT-30"}, {"Huntley"}, {"Stafford"}, {"BF-400"}, {"Newsvan"}, {"Tug"},
	{"Trailer 3"}, {"Emperor"}, {"Wayfarer"}, {"Euros"}, {"Hotdog"}, {"Club"}, {"Freight Carriage"}, {"Trailer 3"},
	{"Andromada"}, {"Dodo"}, {"RC Cam"}, {"Launch"}, {"Police Car"},	{"Police Car"},	{"Police Car"},	{"Ranger"},
	{"Picador"}, {"S.W.A.T. Van"}, {"Alpha"}, {"Phoenix"},	{"Glendale"}, {"Sadler"}, {"Luggage Trailer A"}, {"Luggage Trailer B"},
	{"Stair Trailer"}, {"Boxville"}, {"Farm Plow"},	{"Utility Trailer"}
};
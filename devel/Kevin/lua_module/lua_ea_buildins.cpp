/* LUA_EA module ***Kevin*** lua_ea_buildins.cpp v1.0 */

#ifndef _LUA_BUILDINS_
#define _LUA_BUILDINS_

// scriptend()
// Exits definitively the script
static int buildin_scriptend(lua_State *NL)
{
	return lua_yield(NL, 0);
}

// addnpc("NPC name","name postfix","map.gat",x,y,dir,sprite,"function")
// Add an standard NPC that triggers a function when clicked
static int buildin_addnpc(lua_State *NL)
{
	char *name,*exname,*map,*function;
	unsigned short m,x,y,dir,class_;
	
	name=(char *)aCallocA(strlen((char *)lua_tostring(NL, 1)), sizeof(char));
	name[0]=0;
	exname=(char *)aCallocA(strlen((char *)lua_tostring(NL, 2)), sizeof(char));
	exname[0]=0;
	map=(char *)aCallocA(strlen((char *)lua_tostring(NL, 3)), sizeof(char));
	map[0]=0;
	function=(char *)aCallocA(strlen((char *)lua_tostring(NL, 8)), sizeof(char));
	function[0]=0;
	
	strcpy(name,(char *)lua_tostring(NL,1));
	strcpy(exname,(char *)lua_tostring(NL,2));
	strcpy(map,(char *)lua_tostring(NL,3));
	m=map_mapname2mapid(map);
	x=lua_tonumber(NL,4);
	y=lua_tonumber(NL,5);
	dir=lua_tonumber(NL,6);
	class_=lua_tonumber(NL,7);
	strcpy(function,(char *)lua_tostring(NL,8));
	
	lua_ea_npc_add(name,exname,m,x,y,dir,class_,function);
	
	aFree(name);
	aFree(exname);
	aFree(map);
	aFree(function);

	return 0;
}

// addareascript("Area script name","map.gat",x1,y1,x2,y2,"function")
// Add an invisible area that triggers a function when entered
static int buildin_addareascript(lua_State *NL)
{
	char name[24],map[16],function[50];
	unsigned short m,x1,y1,x2,y2;

	sprintf(name,"%s",lua_tostring(NL,1));
	sprintf(map,"%s",lua_tostring(NL,2));
	m=map_mapname2mapid(map);
	x1=(lua_tonumber(NL,5)>lua_tonumber(NL,3))?lua_tonumber(NL,3):lua_tonumber(NL,5);
	y1=(lua_tonumber(NL,6)>lua_tonumber(NL,4))?lua_tonumber(NL,4):lua_tonumber(NL,6);
	x2=(lua_tonumber(NL,5)>lua_tonumber(NL,3))?lua_tonumber(NL,5):lua_tonumber(NL,3);
	y2=(lua_tonumber(NL,6)>lua_tonumber(NL,4))?lua_tonumber(NL,6):lua_tonumber(NL,4);
	sprintf(function,"%s",lua_tostring(NL,7));

	lua_ea_areascript_add(name,m,x1,y1,x2,y2,function);

	return 0;
}

// addwarp("Warp name","map.gat",x,y,"destmap.gat",destx,desty,xradius,yradius)
// Add a warp that moves players to somewhere else when entered

/*static int buildin_addwarp(lua_State *NL) // not sure about warps yet with eapp, will look into it
{
	char name[24],map[16],destmap[16];
	short m,x,y;
	short destx,desty,xs,ys;
	
	sprintf(name,"%s",lua_tostring(NL,1));
	sprintf(map,"%s",lua_tostring(NL,2));
	m=map_mapname2mapid(map);
	x=lua_tonumber(NL,3);
	y=lua_tonumber(NL,4);
	sprintf(destmap,"%s",lua_tostring(NL,5));
	destx=lua_tonumber(NL,6);
	desty=lua_tonumber(NL,7);
	xs=lua_tonumber(NL,8);
	ys=lua_tonumber(NL,9);

	lua_ea_warp_add(name,m,x,y,destmap,destx,desty,xs,ys);

	return 0;
}

// addspawn("Monster name","map.gat",x,y,xradius,yradius,id,amount,delay1,delay2,"function")
// Add a monster spawn etc
static int buildin_addspawn(lua_State *NL) //or mob spawns
{
	char name[24],map[16],function[50];
	short m,x,y,xs,ys,class_,num,d1,d2;

	sprintf(name,"%s",lua_tostring(NL,1));
	sprintf(map,"%s",lua_tostring(NL,2));
	m=map_mapname2mapid(map);
	x=lua_tonumber(NL,3);
	y=lua_tonumber(NL,4);
	xs=lua_tonumber(NL,5);
	ys=lua_tonumber(NL,6);
	class_=lua_tonumber(NL,7);
	num=lua_tonumber(NL,8);
	d1=lua_tonumber(NL,9);
	d2=lua_tonumber(NL,10);
	sprintf(function,"%s",lua_tostring(NL,11));

	lua_ea_spawn_add(name,m,x,y,xs,ys,class_,num,d1,d2,function);

	return 0;
}*/

// npcmes("Text",[id])
// Print the text into the NPC dialog window of the player
static int buildin_npcmes(lua_State *NL)
{
	struct map_session_data *sd;
	char mes[512];

	sprintf(mes,"%s",lua_tostring(NL, 1)); 
	sd = lua_ea_get_target(NL, 2);

	clif_scriptmes(sd, sd->lua_ea_player_data.script_id, mes);

	return 0;
}

// npcclose([id])
// Display a [Close] button in the NPC dialog window of the player and pause the script until the button is clicked
static int buildin_npcclose(lua_State *NL)
{
	struct map_session_data *sd;

	sd = lua_ea_get_target(NL, 1);

	clif_scriptclose(sd,sd->lua_ea_player_data.script_id);

	sd->lua_ea_player_data.lua_ea_state = CLOSE;
	return lua_yield(NL, 0);
}

// npcnext([id])
// Display a [Next] button in the NPC dialog window of the player and pause the script until the button is clicked
static int buildin_npcnext(lua_State *NL)
{
	struct map_session_data *sd;

	sd = lua_ea_get_target(NL, 1);

	clif_scriptnext(sd,sd->lua_ea_player_data.script_id);

	sd->lua_ea_player_data.lua_ea_state = NEXT;
	return lua_yield(NL, 0);
}

// npcinput(type,[id])
// Display an NPC input window asking the player for a value
static int buildin_npcinput(lua_State *NL)
{
	struct map_session_data *sd;
	int type;

	type = lua_tonumber(NL, 1);
	sd = lua_ea_get_target(NL, 2);

	switch(type){
		case 0:
			clif_scriptinput(sd,sd->lua_ea_player_data.script_id);
			break;
		case 1:
			clif_scriptinputstr(sd,sd->lua_ea_player_data.script_id);
			break;
	}
	
	sd->lua_ea_player_data.lua_ea_state = INPUT;
	return lua_yield(NL, 1);
}

// npcmenu("menu_name1",return_value1,...)
// Display an NPC input window asking the player for a value
static int buildin_npcmenu(lua_State *NL)
{
	struct map_session_data *sd;
	char *buf;
	char *menu;
	int len=0, n, i;
	int value, index;

	sd = lua_ea_get_target(NL, 2);
	
	//setup table to store values
	lua_pushliteral(NL, "menu");
	lua_newtable(NL);
	lua_rawset(NL, LUA_GLOBALSINDEX);

	lua_pushliteral(NL, "n");
	lua_rawget(NL, 1);
	n = lua_tonumber(NL, -1);
	lua_pop(NL, 1);

	if(n%2 == 1) {
		lua_pushstring(NL, "Incorrect number of arguments for function 'npcmenu'\n");
		lua_error(NL);
		return -1;
	}

	if(!sd->lua_ea_player_data.npc_menu_data.current) {
		sd->lua_ea_player_data.npc_menu_data.current=0;
	}

	for(i=0; i<n; i+=2) {
		lua_pushnumber(NL, i+1);
		lua_rawget(NL, 1);
		menu = (char *)lua_tostring(NL, -1);
		lua_pop(NL, 1);
		len += strlen(menu);
	}

	buf=(char *)aCallocA(len+1, sizeof(char));
	buf[0]=0;

	for(i=0, index=0; i<n; i+=2, index++) {
		lua_pushnumber(NL, i+1);
		lua_rawget(NL, 1);
		menu = (char *)lua_tostring(NL, -1);
		lua_pop(NL, 1);

		lua_pushnumber(NL, i+2);
		lua_rawget(NL, 1);
		value = lua_tonumber(NL, -1);
		lua_pop(NL, 1);
		
		lua_pushliteral(NL, "menu");
		lua_rawget(NL, LUA_GLOBALSINDEX);
		lua_pushnumber(NL, index);
		lua_pushnumber(NL, value);
		lua_rawset(NL, -3);
		lua_pop(NL, 1);
		
		strcat(buf,menu);
		strcat(buf,":");
	}

	clif_scriptmenu(sd,sd->lua_ea_player_data.script_id,buf);
	aFree(buf);

	sd->lua_ea_player_data.lua_ea_state = MENU;
	return lua_yield(NL, 1);
}

// npcshop(item_id1,item_price1,...)
// Start a shop with buylist of item_id selling for item_price
static int buildin_npcshop(lua_State *NL)
{
	struct map_session_data *sd;
	int n, i, j;

	sd = lua_ea_get_target(NL, 2);

	lua_pushliteral(NL, "n");
	lua_rawget(NL, 1);
	n = lua_tonumber(NL, -1);
	lua_pop(NL, 1);

	if(n%2 == 1) {
		lua_pushstring(NL, "Incorrect number of arguments for function 'npcmenu'\n");
		lua_error(NL);
		return -1;
	}

	sd->lua_ea_player_data.shop_data.n = n/2;

	for(i=1, j=0; i<=n; i+=2, j++) {
		lua_pushnumber(NL, i);
		lua_rawget(NL, 1);
		sd->lua_ea_player_data.shop_data.nameid[j] = lua_tonumber(NL, -1);
		lua_pop(NL, 1);

		lua_pushnumber(NL, i+1);
		lua_rawget(NL, 1);
		sd->lua_ea_player_data.shop_data.value[j] = lua_tonumber(NL, -1);
		lua_pop(NL, 1);
	}

	clif_npcbuysell(sd, sd->lua_ea_player_data.script_id);

	sd->lua_ea_player_data.lua_ea_state = SHOP;
	return lua_yield(NL, 1);
}

// npccutin(name,type,[id])
// Display a cutin picture on the screen
static int buildin_npccutin(lua_State *NL)
{
	struct map_session_data *sd;
	char name[50];
	int type;

	sprintf(name, "%s", lua_tostring(NL,1));
	type = lua_tonumber(NL, 2);
	sd = lua_ea_get_target(NL, 3);

	clif_cutin(sd,name,type);

	return 0;
}

// heal(hp,sp,[id])
// Heal the character by a set amount of HP and SP
static int buildin_heal(lua_State *NL)
{
	struct map_session_data *sd;
	int hp, sp;

	hp = lua_tonumber(NL, 1);
	sp = lua_tonumber(NL, 2);
	sd = lua_ea_get_target(NL, 3);

	pc_heal(sd, hp, sp);

	return 0;
}

// percentheal(hp,sp,[id])
// Heal the character by a percentage of MaxHP and MaxSP
static int buildin_percentheal(lua_State *NL)
{
	struct map_session_data *sd;
	int hp, sp;

	hp = lua_tonumber(NL, 1);
	sp = lua_tonumber(NL, 2);
	sd = lua_ea_get_target(NL, 3);

	pc_percentheal(sd, hp, sp);

	return 0;
}

// itemheal(hp,sp,[id])
// Heal the character by an amount of HP and SP that increases with VIT/INT, skills, etc
static int buildin_itemheal(lua_State *NL)
{
	struct map_session_data *sd;
	int hp, sp;

	hp = lua_tonumber(NL, 1);
	sp = lua_tonumber(NL, 2);
	sd = lua_ea_get_target(NL, 3);

	pc_itemheal(sd, hp, sp);

	return 0;
}

// warp("map.gat",x,y,[id])
// Warp the character to a set location
static int buildin_warp(lua_State *NL)
{
	struct map_session_data *sd;
	char str[16];
	int x, y;

	sprintf(str, "%s", lua_tostring(NL,1));
	x = lua_tonumber(NL, 2);
	y = lua_tonumber(NL, 3);
	sd = lua_ea_get_target(NL, 4);

	if(strcmp(str,"Random")==0) // Warp to random location
		pc_randomwarp(sd,3);
	else if(strcmp(str,"SavePoint")==0 || strcmp(str,"Save")==0) { // Warp to save point
		if(map[sd->.bl.m.flag.noreturn)
			return 0;
		pc_setpos(sd,sd->status.save_point.map,sd->status.save_point.x,sd->status.save_point.y,3);
	} else // Warp to given location
		pc_setpos(sd,str,x,y,0);

	return 0;
}

// jobchange(job,[id])
// Change the job of the character
static int buildin_jobchange(lua_State *NL)
{
	struct map_session_data *sd;
	int job;

	job = lua_tonumber(NL, 1);
	sd = lua_ea_get_target(NL, 2);

	pc_jobchange(sd,job,0);

	return 0;
}

// setlook(type,val,[id])
// Change the look of the character
static int buildin_setlook(lua_State *NL)
{
	struct map_session_data *sd;
	int type,val;

	type = lua_tonumber(NL, 1);
	val = lua_tonumber(NL, 2);
	sd = lua_ea_get_target(NL, 3);

	pc_changelook(sd,type,val);

	return 0;
}

// List of commands to build into Lua, format : {"function_name_in_lua", C_function_name}
static struct LuaCommandInfo commands[] = {
	
	// Basic functions
	{"scriptend", buildin_scriptend},
	
	// Object creation functions
	{"addnpc", buildin_addnpc},
	{"addareascript", buildin_addareascript},
	{"addwarp", buildin_addwarp},
	{"addspawn", buildin_addspawn},
//	{"addgmcommand", buildin_addgmcommand},
//	{"addtimer", buildin_addtimer},
//  {"addclock", buildin_addclock},
//	{"addevent", buildin_addevent},

	// NPC dialog functions
	{"npcmes", buildin_npcmes},
	{"npcclose", buildin_npcclose},
	{"npcnext", buildin_npcnext},
	{"npcinput", buildin_npcinput},
	{"npcmenu_", buildin_npcmenu},
	{"npcshop_", buildin_npcshop},
	{"npccutin", buildin_npccutin},
	
	// Player related functions
	{"heal", buildin_heal},
	{"percentheal", buildin_percentheal},
	{"itemheal", buildin_itemheal},
	{"warp", buildin_warp},
	{"jobchange", buildin_jobchange},
	{"setlook", buildin_setlook},
	
	// End of build-in functions list
	{"-End of list-", NULL},
	
};

#endif

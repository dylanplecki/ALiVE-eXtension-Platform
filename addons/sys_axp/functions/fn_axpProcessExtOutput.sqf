#include "\x\alive\addons\sys_axp\headers\protocol_def.h"

for "_i" from 0 to 1 step 0 do // For performance
{
	private ["_newPackage"];
	_newPackage = [SF_NONE, []] call alive_fnc_axpCallExtWrapper;
	
	if ((_newPackage select 0) != SF_HANDLE) exitWith {};
	
	private ["_handler"];
	_handler = [_newPackage select 1] call alive_fnc_axpGetHandler;
	
	if (!isNil "_handler")
	{
		([_newPackage select 1] call alive_fnc_axpGetExtOutput) call _handler;
	};
};

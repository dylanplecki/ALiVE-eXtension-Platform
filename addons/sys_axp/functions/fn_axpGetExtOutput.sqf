#include "\x\alive\addons\sys_axp\headers\protocol_def.h"

private ["_pkgAddress", "_outputData", "_outputTemp"];
_pkgAddress = _this select 0;
_outputData = "[";

for "_i" from 0 to 1 step 0 do // For performance
{
	_outputTemp = [SF_CHUNK, _pkgAddress] call alive_fnc_axpCallExt;
	if (_outputTemp isEqualTo "") exitWith {};
	_outputData = _outputData + _outputTemp;
};

(call compile (_outputData + "]")) // Return

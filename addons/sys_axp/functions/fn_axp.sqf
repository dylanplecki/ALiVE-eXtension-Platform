if (isNil "alive_axp" || {!alive_axp}) exitWith {nil};
if (typeName(_this) != "ARRAY") then {_this = [_this]};

private ["_library", "_function", "_arguments", "_async", "_handler"];
_library = _this select 0;
_function = _this select 1;
_arguments = if ((count _this) > 2) then {_this select 2} else {nil};
_async = if ((count _this) > 3) then {_this select 3} else {false};
_handler = if ((count _this) > 4) then {_this select 4} else {{axp_lastCall = _this}};

if (typeName(_arguments) != "ARRAY") then {_arguments = [_arguments]};

private ["_statusFlag", "_data", "_return"];
_statusFlag = if (_async) then {6} else {5};
_data = [_statusFlag, ([_library, _function] + _arguments)] call alive_fnc_axpCallExtWrapper;
_return = _data select 1;

switch (_data select 0) do
{
	case 7: // Async
	{
		[(_data select 1), _code] call alive_fnc_axpSetHandler;
	};
	
	case 4: // Sync (chunked)
	{
		_return = [_data select 1] call alive_fnc_axpGetExtOutput;
	};
};

_return // Return

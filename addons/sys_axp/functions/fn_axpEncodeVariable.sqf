
#define ARRAY_BOUND 30 // Array boundary indicator
#define TYPE_ARRAY ["ARRAY", "BOOL", "CODE", "CONFIG", "CONTROL", \
					"DISPLAY", "GROUP", "LOCATION", "NIL", "OBJECT", \
					"SCALAR", "SCRIPT", "SIDE", "STRING", "TEXT", "NAMESPACE"]

private ["_var", "_type", "_var_t", "_stringize"];
_var = _this select 0;

_type = if (isNil "_var") then {"NIL"} else {typeName(_var)};
_var_t = (TYPE_ARRAY find _type) + 1;
_stringize = "";

if (_var_t <= 0) exitWith {nil};

if (_type != "STRING") then
{
	if (_type == "ARRAY") then
	{
		{ // forEach
			_stringize = _stringize + ([_x] call alive_fnc_axpEncodeVariable);
		} forEach _var;
		
		_stringize = _stringize + toString([ARRAY_BOUND]);
	}
	else
	{
		_stringize = str(_var);
	};
};

toString([_var_t]) + (if (_type == "STRING") then {_var} else {_stringize}) // Return

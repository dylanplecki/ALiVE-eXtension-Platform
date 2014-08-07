
private ["_status", "_args", "_arg_str"];
_status = _this select 0;
_args = if ((count _this) > 1) then {_this select 1} else {nil};

if (!isNil "_args") then
{
	_arg_str = [_args] call alive_fnc_axpEncodeVariable;
}
else {_arg_str = ""};

_result = [_status, _arg_str] call alive_fnc_axpCallExt;

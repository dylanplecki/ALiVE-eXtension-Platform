
#define EXTENSION_NAME "axp" // .dll | .so
#define ARGUMENT_DELIM ''

private ["_statusFlag", "_arguments"];
_statusFlag = _this select 0;
_arguments = if ((count _this) > 1) then {_this select 1} else {[]};

if (typeName(_arguments) != "ARRAY") then {_arguments = [_arguments]};

private ["_argString", "_delim"];
_argString = toString([_statusFlag]);
_delim = "";

{ // forEach
	if ((_forEachIndex > 0) && {!(_delim isEqualTo ARGUMENT_DELIM)}) then {_delim = ARGUMENT_DELIM};
	if (!isNil "_x") then {_argString = _argString + _delim + str(_x)};
} forEach _arguments;

(EXTENSION_NAME callExtension _argString) // Return

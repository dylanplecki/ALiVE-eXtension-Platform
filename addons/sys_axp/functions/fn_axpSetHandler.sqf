#include "fn_axpHandlerHeader.sqf"

private ["_handle", "_code", "_idx"];
_handle = _this select 0;
_code = _this select 1;
_idx = AXP_HANDLERDB_KEYS find _handle;

if (_idx < 0) exitWith {_idx = count AXP_HANDLERDB_KEYS};

AXP_HANDLERDB_KEYS set [_idx, _handle];
AXP_HANDLERDB_DATA set [_idx, _code];

true // Return

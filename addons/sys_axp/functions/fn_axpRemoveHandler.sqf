#include "fn_axpHandlerHeader.sqf"

private ["_handle", "_idx", "_last"];
_handle = _this select 0;
_idx = AXP_HANDLERDB_KEYS find _handle;
_last = (count AXP_HANDLERDB_DATA) - 1;

if (_idx < 0) exitWith {false};

AXP_HANDLERDB_KEYS set [_idx, (AXP_HANDLERDB_KEYS select _last)];
AXP_HANDLERDB_DATA set [_idx, (AXP_HANDLERDB_DATA select _last)];

AXP_HANDLERDB_KEYS resize _last;
AXP_HANDLERDB_DATA resize _last;

true // Return

#include "fn_axpHandlerHeader.sqf"

private ["_handle", "_idx"];
_handle = _this select 0;
_idx = AXP_HANDLERDB_KEYS find _handle;

if (_idx < 0) exitWith {nil};

AXP_HANDLERDB_DATA select _idx // Return

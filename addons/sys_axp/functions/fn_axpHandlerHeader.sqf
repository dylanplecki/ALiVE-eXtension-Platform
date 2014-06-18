
#define AXP_HANDLERDB_KEYS (alive_axp_handlerdb select 0)
#define AXP_HANDLERDB_DATA (alive_axp_handlerdb select 1)

if (isNil "alive_axp_handlerdb") then {alive_axp_handlerdb = [[],[]]};


/***********/
/* Defines */
/***********/

#define DEBUG_MODE true
#define EXTENSION_NAME "axp" // .dll | .so

#define TYPE_ARRAY ["ARRAY", "BOOL", "CODE", "CONFIG", "CONTROL", \
					"DISPLAY", "GROUP", "LOCATION", "NIL", "OBJECT", \
					"SCALAR", "SCRIPT", "SIDE", "STRING", "TEXT", "NAMESPACE"]

/**********/
/* Macros */
/**********/

#define DEFAULT_ARGUMENT(idx,dft) (if ((count _this) > idx) then {_this select idx} else {dft})

#ifdef DEBUG_MODE
	#define TRACE_POINT (diag_log text format["ALiVE_SYS_AXP <%1>: TRACE POINT #%2", round(diag_tickTime * 100) / 100, __LINE__])
#else
	#define TRACE_POINT
#endif

/*************/
/* Variables */
/*************/

alive_sys_axp = false;
alive_sys_axp_handlers = [[],[]]; // Hash table

/*********************/
/* General Functions */
/*********************/

alive_fnc_axpEncodeVariable = {
	private ["_var", "_type", "_var_t", "_stringize"];
	_var = _this select 0;
	
	_type = if (isNil "_var") then {"NIL"} else {typeName(_var)};
	_var_t = (TYPE_ARRAY find _type) + 1;
	
	if (_var_t <= 0) exitWith {nil};
	
	if (_type != "STRING") then
	{
		if (_type == "ARRAY") then
		{
			_stringize = "";
			
			{ // forEach
				_stringize = _stringize +
					(if (_forEachIndex > 0) then {''} else {""}) + // Unit Separator (UTF+001F &#31)
					([_x] call alive_fnc_axpEncodeVariable);
			} forEach _var;
			
			_stringize = _stringize + ''; // Record Separator (UTF+001E &#30)
		}
		else
		{
			_stringize = if (isNil "_var") then {"nil"} else {str(_var)};
		};
	};
	
	toString([_var_t]) + (if (_type == "STRING") then {_var} else {_stringize}) // Return
};

/***********************/
/* Extension Functions */
/***********************/

alive_fnc_axpCallExt = {
	(EXTENSION_NAME callExtension (toString([_this select 0]) + DEFAULT_ARGUMENT(1,""))) // Return
};

alive_fnc_axpCallExtWrapper = {
	private ["_status", "_library", "_function", "_args", "_arg_str"];
	_status = _this select 0;
	_library = _this select 1;
	_function = _this select 2;
	_args = DEFAULT_ARGUMENT(3,nil);
	
	// Unit Separator (UTF+001F &#31)
	_arg_str = _library + '' + _function + '' + ([_args] call alive_fnc_axpEncodeVariable);
	
	[[_status, _arg_str] call alive_fnc_axpCallExt] call alive_fnc_axpParseExtOutput // Return
};

alive_fnc_axpGetExtOutput = {
	private ["_handle", "_data", "_int_data"];
	_handle = _this select 0;
	_data = "[";
	
	for "_i" from 0 to 1 step 0 do // For performance
	{
		_int_data = [4, _handle] call alive_fnc_axpCallExt;
		if ((_int_data isEqualTo "") || {_int_data isEqualTo ""}) exitWith {}; // SF_NONE
		_data = _data + _int_data;
	};
	
	//if (_int_data isEqualTo "") exitWith {nil}; // Return
	
	call compile (_data + "]") // Return
};

alive_fnc_axpParseExtOutput = {
	if ((_this select 0) isEqualTo "") exitWith {[-1,nil]}; // Return
	
	private ["_input_arr", "_status", "_return"];
	_input_arr = toArray(_this select 0);
	_status = _input_arr select 0;
	_input_arr set [0, 32]; // 32 == space
	_return = call compile toString(_input_arr);
	
	if (_status == 3) then
	{
		diag_log text format["[%1] AXP <ERROR>: %2", round(diag_tickTime), _return];
	};
	
	[_status, (if (isNil "_return") then {nil} else {_return})] // Return
};

alive_fnc_axpExtInfo = {
	[([14] call alive_fnc_axpCallExt)] call alive_fnc_axpParseExtOutput;
};

/********************************/
/* Handler Management Functions */
/********************************/

alive_fnc_axpGetHandlers = {
	private ["_handle_id", "_idx"];
	_handle_id = _this select 0;
	
	_idx = (alive_sys_axp_handlers select 0) find _handle_id;
	
	if (_idx < 0) then {nil} else // Return
	{
		(alive_sys_axp_handlers select 1) select _idx // Return
	};
};

alive_fnc_axpSetHandlers = {
	private ["_handle_id", "_handler_arr", "_idx"];
	_handle_id = _this select 0;
	_handler_arr = _this select 1;
	
	_idx = (alive_sys_axp_handlers select 0) find _handle_id;
	
	if (_idx < 0) then {false} else // Return
	{
		(alive_sys_axp_handlers select 1) set [_idx, _handler_arr];
		true // Return
	};
};

alive_fnc_axpAddHandler = {
	private ["_handle_id", "_handler", "_idx"];
	_handle_id = _this select 0;
	_handler = _this select 1;
	
	_idx = (alive_sys_axp_handlers select 0) find _handle_id;
	
	if (_idx < 0) then
	{
		_idx = count (alive_sys_axp_handlers select 0);
		(alive_sys_axp_handlers select 0) set [_idx, _handle_id];
		(alive_sys_axp_handlers select 1) set [_idx, [_handler]];
	}
	else
	{
		private ["_handler_arr"];
		_handler_arr = (alive_sys_axp_handlers select 1) select _idx;
		_handler_arr set [(count _handler_arr), _handler];
	};
	
	true // Return
};

alive_fnc_axpClearHandler = {
	private ["_handle_id"];
	_handle_id = _this select 0;
	
	[_handle_id, []] call alive_fnc_axpSetHandlers;
};

alive_fnc_axpCleanHandler = {
	private ["_handle_id", "_idx"];
	_handle_id = _this select 0;
	
	_idx = (alive_sys_axp_handlers select 0) find _handle_id;
	
	if (_idx < 0) then {false} else // Return
	{
		(alive_sys_axp_handlers select 0) set [_idx, objnull];
		alive_sys_axp_handlers set [0, ((alive_sys_axp_handlers select 0) - [objNull])];
		
		(alive_sys_axp_handlers select 1) set [_idx, objnull];
		alive_sys_axp_handlers set [1, ((alive_sys_axp_handlers select 1) - [objNull])];
		
		true // Return
	};
};

alive_fnc_axpCallHandlers = {
	private ["_handle_id", "_args", "_idx"];
	_handle_id = _this select 0;
	_args = DEFAULT_ARGUMENT(1,[]);
	
	_idx = (alive_sys_axp_handlers select 0) find _handle_id;
	
	if (_idx < 0) then {false} else // Return
	{
		{ // forEach
			_args call _x;
		} forEach ((alive_sys_axp_handlers select 1) select _idx);
		
		true // Return
	};
};

alive_fnc_axpSpawnHandlers = {
	private ["_handle_id", "_args", "_idx"];
	_handle_id = _this select 0;
	_args = DEFAULT_ARGUMENT(1,[]);
	
	_idx = (alive_sys_axp_handlers select 0) find _handle_id;
	
	if (_idx < 0) then {false} else // Return
	{
		{ // forEach
			_args spawn _x;
		} forEach ((alive_sys_axp_handlers select 1) select _idx);
		
		true // Return
	};
};

/***********************/
/* Start Extension FSM */
/***********************/

// Get version
private ["_version"];
_version = [[10] call alive_fnc_axpCallExt] call alive_fnc_axpParseExtOutput;

if ((_version select 0) == 2) then // SF_GOOD
{
	// Add session end handler
	addMissionEventHandler ["Ended", {[9] call alive_fnc_axpCallExt}]; // SF_DEL_SESSION
	
	// Start new session
	[8] call alive_fnc_axpCallExt; // SF_NEW_SESSION
	
	alive_sys_axp_ext_version = _version select 1;
	alive_sys_axp = true;
	
	// Run extension handler loop
	alive_sys_axp_extHandlerFSM = [] execFSM "\x\alive\addons\sys_axp\fsm\axpExtHandler.fsm";
};

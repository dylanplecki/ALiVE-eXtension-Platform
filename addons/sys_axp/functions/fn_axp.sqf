
/*
	Function: alive_fnc_axp
	Author: Naught
	
	Description:
	Calls an extension (library) function to be loaded and run by AXP.
	
	Parameter(s):
		0: STRING - library name (without .dll or .so)
		1: STRING - function name (without trailing parameters)
		2 (Optional): ANY - arguments to pass to function (default: nil)
		3 (Optional): BOOL - execution type flag (default: 0)
		4 (Optional): CODE - return handler (default: {})
	
	Returns:
		- ANY on success
		- NIL on failure
	
	Notes:
		1. Available execution type flags:
			0: Synchronous execution
			1: Asynchronous execution (requires a return handler)
			2: Synchronous execution with scheduled return (requires a return handler)
		2. The return handler is passed (via _this) an array of each return call from the function.
*/

#define EXEC_TYPE_FLAGS [5,6,15] // Status flags relate to P@3 index-based value

private ["_library", "_function", "_arguments", "_async", "_handler"];
_library = _this select 0;
_function = _this select 1;
_arguments = if ((count _this) > 2) then {_this select 2} else {nil};
_async = if ((count _this) > 3) then {_this select 3} else {0};
_handler = if ((count _this) > 4) then {_this select 4} else {{}};

private ["_status", "_return"];
_status = EXEC_TYPE_FLAGS select _async;
_return = [_status, _library, _function, _arguments] call alive_fnc_axpCallExtWrapper;

switch (_return select 0) do // Return status
{
case 2: // SF_GOOD
	{
		_return select 1 // Return (data)
	};
	
case 4: // SF_CHUNK
	{
		[_return select 1] call alive_fnc_axpGetExtOutput // Return (data)
	};
	
case 7: // SF_HANDLE
	{
		[(_return select 1), _handler] call alive_fnc_axpAddHandler;
		_return select 1 // Return (handle)
	};
	
	default {nil}; // Possible return
};

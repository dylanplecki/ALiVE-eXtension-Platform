
/*
	Function: alive_fnc_axp
	Author: Naught

	Description:
	Calls an extension (library) function to be loaded and run by AXP.

	Parameter(s):
		0: STRING - library name (without .dll or .so)
		1: STRING - function name (without trailing parameters)
		2 (Optional): ANY - arguments to pass to function (default: nil)
		3 (Optional): BOOL - use asynchronous execution (default: false)
		4 (Optional): CODE - return handler for asynchonrous execution (default: {})

	Returns:
	ANY on success
	NIL on failure
*/

private ["_library", "_function", "_arguments", "_async", "_handler"];
_library = _this select 0;
_function = _this select 1;
_arguments = if ((count _this) > 2) then {_this select 2} else {nil};
_async = if ((count _this) > 3) then {_this select 3} else {false};
_handler = if ((count _this) > 4) then {_this select 4} else {{}};

private ["_status", "_return"];
_status = if (_async) then {6} else {5};
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

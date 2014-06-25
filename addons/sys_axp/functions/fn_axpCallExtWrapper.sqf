private ["_input"];
_input = _this call alive_fnc_axpCallExt;

if (_input isEqualTo "") exitWith {[1, nil]};

private ["_inputArr", "_status", "_return"];
_inputArr = toArray(_input);
_status = _inputArr select 0;
_inputArr set [0, 32]; // 32 == space
_return = call compile toString(_inputArr);

if (_status == 3)
{
	diag_log text format["[%1] AXP <ERROR>: %2", round(diag_tickTime), _return];
};

[_status, _return] // Return

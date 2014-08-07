
// Defines
#define EXTENSION_NAME "axp" // .dll | .so

// Macros
#define DEFAULT_ARGUMENT(idx,dft) (if ((count _this) > idx) then {_this select idx} else {dft})

// Script
(EXTENSION_NAME callExtension (toString([_this select 0]) + DEFAULT_ARGUMENT(1,""))) // Return

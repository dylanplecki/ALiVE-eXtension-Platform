
#define FNC_RECOMPILE 1

class cfgFunctions
{
	class alive
	{
		class sys_axp
		{
			file = "\x\alive\addons\sys_axp\functions";
			
			class axpPreInit
			{
				preInit = 1;
				recompile = FNC_RECOMPILE;
			};
			
			class axp {recompile = FNC_RECOMPILE};
		};
	};
};

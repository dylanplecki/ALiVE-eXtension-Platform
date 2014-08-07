
#define FNC_RECOMPILE 1

class cfgFunctions
{
	class alive
	{
		class sys_axp
		{
			file = "\x\alive\addons\sys_axp\functions";
			
			class axpExtHandler
			{
				preInit = 1;
				ext = ".fsm";
				recompile = FNC_RECOMPILE;
			};
			
			class axp {recompile = FNC_RECOMPILE;};
			class axpCallExt {recompile = FNC_RECOMPILE;};
			class axpCallExtWrapper {recompile = FNC_RECOMPILE;};
			class axpGetExtOutput {recompile = FNC_RECOMPILE;};
			class axpGetHandler {recompile = FNC_RECOMPILE;};
			class axpProcessExtOutput {recompile = FNC_RECOMPILE;};
			class axpSetHandler {recompile = FNC_RECOMPILE;};
		};
	};
};

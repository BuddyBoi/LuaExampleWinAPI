#include "include.hpp"

namespace scriptAPI
{
	static bool isDebug = true;
	static int cachedProcessID = 0;
	static HWND cachedWindowHandle = nullptr;

	int dbgPrint( lua_State* L )
	{
		const char* paramOne = luaL_checkstring( L, 1 );
		if ( !isDebug )
			return 0;
		printf( "%s", paramOne );
		return 1;
	}

	int dbgPrintLine( lua_State* L )
	{
		const char* paramOne = luaL_checkstring( L, 1 );
		if ( !isDebug )
			return 0;
		printf( "%s\n", paramOne );
		return 1;
	}
	int print( lua_State* L )
	{
		const char* paramOne = luaL_checkstring( L, 1 );
		printf( "%s", paramOne );
		return 1;
	}

	int printLine( lua_State* L )
	{
		const char* paramOne = luaL_checkstring( L, 1 );
		printf( "%s\n", paramOne );
		return 1;
	}
	int messageBox( lua_State* L )
	{
		const char* paramMessage = luaL_checkstring( L, 1 );
		const char* paramTitle = luaL_checkstring( L, 2 );

		MessageBoxA( 0, paramMessage, paramTitle, 0 );
		return 1;
	}
	int getSum( lua_State* L )
	{
		int paramOne = luaL_checkinteger( L, 1 );
		int paramTwo = luaL_checkinteger( L, 2 );
		int retSum = paramOne + paramTwo;
		
		//return var to LUA
		lua_pushinteger( L, retSum );
		return 1;
	}
	int getProcessID( lua_State* L )
	{
		const char* paramOne = luaL_checkstring( L, 1 );
		DWORD dwProcessID = 0;
		if ( !sizeof( paramOne ) )
		{
			lua_pushinteger( L, (int)dwProcessID );
			return 0;
		}

		HWND window = FindWindowA( NULL, paramOne );
		if ( !window )
		{
			printf( "FAILED TO GET WINDOW\n" );
			lua_pushinteger( L, (int)dwProcessID );
			return 0;
		}
		cachedWindowHandle = window;

		if ( !GetWindowThreadProcessId( window, &dwProcessID ) )
		{
			lua_pushinteger( L, (int)dwProcessID );
			return 0;
		}

		cachedProcessID = dwProcessID;
		lua_pushinteger( L, (int)dwProcessID );
		return 1;
	}
	int terminateProcess( lua_State* L )
	{
		int paramOne = luaL_checkinteger( L, 1 );
		HANDLE processHandle = OpenProcess( FILE_ALL_ACCESS, false, paramOne );
		bool isTerminated = TerminateProcess( processHandle, 0x7 );
		lua_pushinteger( L, isTerminated );
		return 1;
	}
}

class CLua
{
public:
	lua_State* luaState;
	bool Setup()
	{
		luaState = luaL_newstate();
		luaL_openlibs( luaState );
		return true;
	}
	bool LoadScript( std::string file )
	{
		if ( luaL_loadfile( luaState, file.c_str() ) || lua_pcall( luaState, 0, 0, 0 ) )
		{
			std::cerr << "Error: " << lua_tostring( luaState, -1 ) << std::endl;
		}
		return true;
	}
	bool LoadScripts( std::string dir )
	{

	}
	bool Exit()
	{
		lua_close( luaState );
		return true;
	}
	bool RegisterAPI()
	{		
		lua_register( luaState, "DbgPrint", scriptAPI::dbgPrint );
		lua_register( luaState, "DbgPrintLn", scriptAPI::dbgPrintLine );
		lua_register( luaState, "DbgPrintLine", scriptAPI::dbgPrintLine );
		lua_register( luaState, "Print", scriptAPI::print );
		lua_register( luaState, "PrintLn", scriptAPI::printLine );
		lua_register( luaState, "PrintLine", scriptAPI::printLine );
		lua_register( luaState, "MessageBox", scriptAPI::messageBox);
		lua_register( luaState, "GetSum", scriptAPI::getSum);
		lua_register( luaState, "GetProcessID", scriptAPI::getProcessID);
		lua_register( luaState, "TerminateProcess", scriptAPI::terminateProcess);
		return true;
	}
};

int main()
{
	CLua luaEngine;
	luaEngine.Setup();
	luaEngine.RegisterAPI();
	luaEngine.LoadScript( "test.lua" );
	luaEngine.Exit();
	while ( true )
	{
		Sleep( 100 );
	}
}
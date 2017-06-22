#include "DEBUG_NEW_LEAK_DETECT.h"
#define USE_CONSOLE_LOG true

#include "ApplicationDemo.h"

int main()
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ApplicationBase* app = new ApplicationDemo();
	app->Run();
	delete app;

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}

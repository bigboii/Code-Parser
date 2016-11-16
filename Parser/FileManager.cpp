#include "FileManager.h"

//#define TEST_FILE
#ifdef TEST_FILE
int main()
{
	FileMgr filemgr;
	std::string testpath = "C:/Users/YQ/Desktop/CSE687 OOD/Project2/Project2HelperCode/TestFiles";
	filemgr.findFiles(testpath);

	testpath = "C:/Users/YQ/Desktop/CSE687 OOD/Project2/Project2HelperCode";
	filemgr.findDirectories(testpath);

	return 0;
}
#endif
/*
如果要进行性能测试，就在头文件前加上
#define PERFORMANCE_TEST_ON
*/

#define PERFORMANCE_TEST_ON

#include"my_stl_test.h"


int main()
{	
	//内存泄露检测，仅用于MSVC的Debug模式
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#endif	

	mystl::test::test_string::test();
	mystl::test::test_vector::test();
	mystl::test::test_list::test();
	mystl::test::test_deque::test();
	mystl::test::test_stack::test();
	mystl::test::test_queue::test();
	mystl::test::test_priority_queue::test();
	mystl::test::test_set::test();
	mystl::test::test_multiset::test();
	mystl::test::test_map::test();
	mystl::test::test_multimap::test();
	mystl::test::test_unordered_set::test();
	mystl::test::test_unordered_multiset::test();
	mystl::test::test_unordered_map::test();
	mystl::test::test_unordered_multimap::test();
	
	//内存泄露检测，仅用于MSVC的Debug模式
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif	

}



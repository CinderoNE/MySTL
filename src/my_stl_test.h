#ifndef _MY_STL_TEST_H_
#define _MY_STL_TEST_H_
#include<iostream>
#include<chrono>

#include<string>
#include<vector>
#include<list>
#include<deque>
#include<stack>
#include<queue>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>

#include"my_string.h"
#include"my_vector.h"
#include"my_list.h"
#include"my_deque.h"
#include"my_queue.h"
#include"my_stack.h"
#include"my_set.h"
#include"my_map.h"
#include"my_unordered_set.h"
#include"my_unordered_map.h"



using std::cout;
using std::endl;

#define COUT_STR(str)\
do\
{\
	cout << #str << " : " << str << endl;\
}while(0)

#define TEST_OPERATE_AND_COUT_STR(str,operate)\
do										   \
{										   \
	operate;							   \
	cout << "AFTER : " << #operate << endl;\
	COUT_STR(str);				   \
}while(0)


//遍历容器
#define FOR_EACH(container)					   \
do											   \
{                                              \
	cout <<#container << " for_each :";\
	for(auto &elements:container){			   \
		cout << " " << elements;			   \
	}										   \
	cout << endl;                              \
}while(0)

//执行操作，然后遍历容器
#define TEST_OPERATE_AND_FOR_EACH(container,operate)\
do										   \
{										   \
	operate;							   \
	cout << "AFTER : " << #operate << endl;\
	FOR_EACH(container);				   \
}while(0)

//执行操作，输出返回值
#define TEST_OPERATE_AND_CHECK_RETURN(operate)\
do											  \
{											  \
	cout << #operate <<  " : " << operate << endl;\
}while(0)

//用于map测试
#define PAIR    std::pair<int, int>

// map的遍历输出
#define MAP_FOR_EACH(map) \
do \
{ \
	cout <<#map << " for_each :";\
	for (auto& p : map) {\
		cout << " <" << p.first << "," << p.second << ">"; \
	}\
    cout << endl; \
} while(0)

//执行map的操作，然后遍历容器
#define MAP_TEST_OPERATE_AND_FOR_EACH(map, operate) \
do \
{ \
	operate;\
    cout << "After : " << #operate  << endl; \
    MAP_FOR_EACH(map); \
} while(0)

//执行map的操作，输出返回值
#define MAP_TEST_OPERATE_AND_CHECK_RETURN(operate) \
do { \
    auto& p = operate; \
	cout << #operate  << " : <" << p.first << "," << p.second << ">\n"; \
} while(0)




#ifdef PERFORMANCE_TEST_ON

#define PERFORMANCE_TEST_LEN1 200000
#define PERFORMANCE_TEST_LEN2 2000000
#define PERFORMANCE_TEST_LEN3 5000000

#define PERFORMANCE_TEST_LEN4 100000
#define PERFORMANCE_TEST_LEN5 500000
#define PERFORMANCE_TEST_LEN6 1000000

#define PERFORMANCE_TEST_TYPE_INT(container,len,func)       \
do												   \
{												   \
	srand((unsigned)std::chrono::system_clock::now().time_since_epoch().count());\
	container c;                                         \
	auto start = std::chrono::system_clock::now(); \
	for (size_t i = 0; i < len; ++i)               \
		c.func(rand());                             \
	auto end = std::chrono::system_clock::now();   \
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); \
	cout << #container << '.' << #func << ' '<< len  << " : "<< ms << "ms" << endl;\
} while (0)

//for string
#define PERFORMANCE_TEST_TYPE_CHAR(container,ch,len,func)       \
do												   \
{												   \
	srand((unsigned)std::chrono::system_clock::now().time_since_epoch().count());\
	container c;                                         \
	auto start = std::chrono::system_clock::now(); \
	for (size_t i = 0; i < len; ++i)               \
		c.func(ch);                             \
	auto end = std::chrono::system_clock::now();   \
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); \
	cout << #container << '.' << #func << ' '<< len  << " : "<< ms << "ms" << endl;\
} while (0)

//for map
//type只能为整数，因为构建的值使用的是rand()
#define MAP_PERFORMANCE_TEST(map,type,len,func)       \
do												   \
{												   \
	srand((unsigned)std::chrono::system_clock::now().time_since_epoch().count());\
	map<type,type> m;                                         \
	auto start = std::chrono::system_clock::now(); \
	for (size_t i = 0; i < len; ++i)               \
		m.func(std::make_pair(rand(),rand()));                             \
	auto end = std::chrono::system_clock::now();   \
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); \
	cout << #map << '.' << #func << ' '<< len  << " : "<< ms << "ms" << endl;\
} while (0)

#endif // !PERFORMANCE_TEST_ON


namespace mystl {
	
	namespace test {
		namespace test_string {
			void test() {
				cout << "===========================================" << endl;
				cout << "              test string start            " << endl;
				cout << "===========================================" << endl;
				const char* s = "abcdefg";
				mystl::string str;
				mystl::string str1(5, 'a');
				mystl::string str2(str1, 3);
				mystl::string str3(str1, 0, 3);
				mystl::string str4("abc");
				mystl::string str5("abcde", 3);
				mystl::string str6(s, s + 5);
				mystl::string str7(str1);
				mystl::string str8(std::move(str1));
				mystl::string str9;
				str9 = str2;
				mystl::string str10;
				str10 = std::move(str2);
				mystl::string str11;
				str11 = "123";
				mystl::string str12;
				str12 = 'A';

				TEST_OPERATE_AND_COUT_STR(str, str = 'a');
				TEST_OPERATE_AND_COUT_STR(str, str = "string");
				TEST_OPERATE_AND_CHECK_RETURN(*str.begin());
				TEST_OPERATE_AND_CHECK_RETURN(*str.rbegin());
				TEST_OPERATE_AND_CHECK_RETURN(*(str.end() - 1));
				TEST_OPERATE_AND_CHECK_RETURN(*(str.rend() - 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.front());
				TEST_OPERATE_AND_CHECK_RETURN(str.back());
				TEST_OPERATE_AND_CHECK_RETURN(str[1]);
				TEST_OPERATE_AND_CHECK_RETURN(str.at(2));
				COUT_STR(str.data());
				COUT_STR(str.c_str());
				std::cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(str.empty());
				std::cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(str.size());
				TEST_OPERATE_AND_CHECK_RETURN(str.length());
				TEST_OPERATE_AND_CHECK_RETURN(str.capacity());
				TEST_OPERATE_AND_COUT_STR(str, str.shrink_to_fit());
				TEST_OPERATE_AND_CHECK_RETURN(str.capacity());

				TEST_OPERATE_AND_COUT_STR(str, str.insert(str.begin(), 'a'));
				TEST_OPERATE_AND_COUT_STR(str, str.insert(str.end(), 3, 'x'));
				TEST_OPERATE_AND_COUT_STR(str, str.insert(str.end(), s, s + 3));
				TEST_OPERATE_AND_COUT_STR(str, str.erase(str.begin()));
				TEST_OPERATE_AND_COUT_STR(str, str.erase(str.begin(), str.begin() + 3));
				TEST_OPERATE_AND_COUT_STR(str, str.clear());
				TEST_OPERATE_AND_COUT_STR(str, str.push_back('s'));
				TEST_OPERATE_AND_COUT_STR(str, str.push_back('t'));
				TEST_OPERATE_AND_COUT_STR(str, str.pop_back());
				TEST_OPERATE_AND_COUT_STR(str, str.append(1, 't'));
				TEST_OPERATE_AND_COUT_STR(str, str.append(str4));
				TEST_OPERATE_AND_COUT_STR(str, str.append(str4, 1));
				TEST_OPERATE_AND_COUT_STR(str, str.append(str4, 2, 1));
				TEST_OPERATE_AND_COUT_STR(str, str.append("str"));
				TEST_OPERATE_AND_COUT_STR(str, str.append("inging", 3));
				TEST_OPERATE_AND_COUT_STR(str, str.append(s, s + 3));
				TEST_OPERATE_AND_COUT_STR(str, str.resize(10));
				TEST_OPERATE_AND_CHECK_RETURN(str.size());
				TEST_OPERATE_AND_COUT_STR(str, str.resize(20, 'x'));
				TEST_OPERATE_AND_CHECK_RETURN(str.size());
				TEST_OPERATE_AND_COUT_STR(str, str.clear());

				TEST_OPERATE_AND_COUT_STR(str, str = "string");
				TEST_OPERATE_AND_COUT_STR(str3, str3 = "astrings");
				TEST_OPERATE_AND_CHECK_RETURN(str.compare(str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare(0, 6, str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare(0, 6, str3, 1, 6));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare("atringgg"));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare("zzz"));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare(0, 3, "str"));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare(0, 3, "stri", 4));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare(0, 3, "s", 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.compare(0, 9, "stringabc", 9));
				TEST_OPERATE_AND_CHECK_RETURN(str.substr(0));
				TEST_OPERATE_AND_CHECK_RETURN(str.substr(3));
				TEST_OPERATE_AND_CHECK_RETURN(str.substr(0, 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.substr(0, 10));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(0, 6, str3));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(str.end() - 1, str.end(), str3));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(0, 1, "my "));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(str.end() - 8, str.end(), " test"));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(10, 4, "replace"));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(str.end(), str.end(), " test"));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(0, 2, 3, '6'));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(str.begin(), str.begin() + 3, 6, '6'));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(0, 3, str3, 1, 3));
				TEST_OPERATE_AND_COUT_STR(str, str.replace(str.begin(), str.begin() + 6, s, s + 3));

				TEST_OPERATE_AND_COUT_STR(str, str = "abcabc stringgg");
				TEST_OPERATE_AND_COUT_STR(str3, str3 = "abc");
				TEST_OPERATE_AND_CHECK_RETURN(str.find('a'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find('a', 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find('a', 4));
				TEST_OPERATE_AND_CHECK_RETURN(str.find("abc"));
				TEST_OPERATE_AND_CHECK_RETURN(str.find("abc", 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.find("abc", 1, 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.find(str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find(str3, 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.rfind('g'));
				TEST_OPERATE_AND_CHECK_RETURN(str.rfind('g', 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.rfind("gg"));
				TEST_OPERATE_AND_CHECK_RETURN(str.rfind("bc", 10));
				TEST_OPERATE_AND_CHECK_RETURN(str.rfind(str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.rfind(str3, 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of('g'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of('k'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of("bca"));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of("defg", 10));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of("gnirts"));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of("abc", 6));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of("abcdf", 2, 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of(str3, 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_of(str3, 10));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_not_of('a'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_not_of('d'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_not_of('g', 14));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_not_of("abc"));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_not_of("ggggg", 14, 4));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_not_of(str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_first_not_of(str3, 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of('a'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of('a', 4));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of('g'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of("gg"));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of("gg", 14));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of("ggg", 14, 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of(str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_of(str3, 3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of('g'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of('a'));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of('a', 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of("ggg"));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of("ggg", 14));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of("abc", 3, 1));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of(str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.find_last_not_of(str3, 2));

				TEST_OPERATE_AND_COUT_STR(str, str.swap(str3));
				TEST_OPERATE_AND_CHECK_RETURN(str.size());
				TEST_OPERATE_AND_CHECK_RETURN(str.length());
				TEST_OPERATE_AND_CHECK_RETURN(str.capacity());
				TEST_OPERATE_AND_COUT_STR(str, str += str);
				TEST_OPERATE_AND_COUT_STR(str, str += 'a');
				TEST_OPERATE_AND_COUT_STR(str, str += "bc");
				TEST_OPERATE_AND_CHECK_RETURN(str.size());
				TEST_OPERATE_AND_CHECK_RETURN(str.length());
				TEST_OPERATE_AND_CHECK_RETURN(str.capacity());
				TEST_OPERATE_AND_COUT_STR(str, str.shrink_to_fit());
				TEST_OPERATE_AND_CHECK_RETURN(str.capacity());
				TEST_OPERATE_AND_COUT_STR(str, str.reserve(50));
				TEST_OPERATE_AND_CHECK_RETURN(str.capacity());
				TEST_OPERATE_AND_COUT_STR(str3, str3 = "test");
				TEST_OPERATE_AND_COUT_STR(str4, str4 = " ok!");
				std::cout << " str3 + '!' : " << str3 + '!' << std::endl;
				std::cout << " '#' + str3 : " << '#' + str3 << std::endl;
				std::cout << " str3 + \" success\" : " << str3 + " success" << std::endl;
				std::cout << " \"My \" + str3 : " << "My " + str3 << std::endl;
				std::cout << " str3 + str4 : " << str3 + str4 << std::endl;
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_CHAR(std::string, 'x', PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_CHAR(std::string, 'x', PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_CHAR(std::string, 'x', PERFORMANCE_TEST_LEN3, push_back);
				PERFORMANCE_TEST_TYPE_CHAR(mystl::string, 'x', PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_CHAR(mystl::string, 'x', PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_CHAR(mystl::string, 'x', PERFORMANCE_TEST_LEN3, push_back);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "              test string start            " << endl;
				cout << "===========================================" << endl;
			}
		}


		namespace test_vector {
			void test() {
				cout << "===========================================" << endl;
				cout << "              test vector start            " << endl;
				cout << "===========================================" << endl;
				int a[] = { 1,2,3,4,5 };
				mystl::vector<int> v1;
				mystl::vector<int> v2(10);
				mystl::vector<int> v3(10);
				mystl::vector<int> v4(a, a + 5);
				mystl::vector<int> v5(v2);
				mystl::vector<int> v6(std::move(v2));
				mystl::vector<int> v7{ 1,2,3,4,5,6,7,8,9 };
				mystl::vector<int> v8, v9, v10;
				v8 = v3;
				v9 = std::move(v3);
				v10 = { 1,2,3,4,5,6,7,8,9 };
				TEST_OPERATE_AND_FOR_EACH(v1, v1.assign(8, 8));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.assign(a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.emplace(v1.begin(), 0));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.emplace_back(6));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.push_back(6));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.insert(v1.end(), 7));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.insert(v1.begin() + 3, 2, 3));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.insert(v1.begin(), a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.pop_back());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.erase(v1.begin()));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.erase(v1.begin(), v1.begin() + 2));
				TEST_OPERATE_AND_FOR_EACH(v1, v1.swap(v4));
				TEST_OPERATE_AND_CHECK_RETURN(*v1.begin());
				TEST_OPERATE_AND_CHECK_RETURN(*(v1.end() - 1));
				TEST_OPERATE_AND_CHECK_RETURN(*v1.rbegin());
				TEST_OPERATE_AND_CHECK_RETURN(*(v1.rend() - 1));
				TEST_OPERATE_AND_CHECK_RETURN(v1.front());
				TEST_OPERATE_AND_CHECK_RETURN(v1.back());
				TEST_OPERATE_AND_CHECK_RETURN(v1[0]);
				TEST_OPERATE_AND_CHECK_RETURN(v1.at(1));
				int* p = v1.data();
				*p = 10;
				*++p = 20;
				p[1] = 30;
				cout << " After change v1.data() :" << "\n";
				FOR_EACH(v1);
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(v1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.resize(10));
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.shrink_to_fit());
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.resize(6, 6));
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.shrink_to_fit());
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.clear());
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.reserve(5));
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.reserve(20));
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				TEST_OPERATE_AND_FOR_EACH(v1, v1.shrink_to_fit());
				TEST_OPERATE_AND_CHECK_RETURN(v1.size());
				TEST_OPERATE_AND_CHECK_RETURN(v1.capacity());
				cout << endl << endl;
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::vector<int>, PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_INT(std::vector<int>, PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_INT(std::vector<int>, PERFORMANCE_TEST_LEN3, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::vector<int>, PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::vector<int>, PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::vector<int>, PERFORMANCE_TEST_LEN3, push_back);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "              test vector end              " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_list {
			void test() {
				cout << "===========================================" << endl;
				cout << "              test list start              " << endl;
				cout << "===========================================" << endl;
				int a[] = { 1,2,3,4,5 };
				mystl::list<int> l1;
				mystl::list<int> l2(5);
				mystl::list<int> l3(5, 1);
				mystl::list<int> l4(a, a + 5);
				mystl::list<int> l5(l2);
				mystl::list<int> l6(std::move(l2));
				mystl::list<int> l7{ 1,2,3,4,5,6,7,8,9 };
				mystl::list<int> l8;
				l8 = l3;
				mystl::list<int> l9;
				l9 = std::move(l3);
				mystl::list<int> l10;
				l10 = { 1, 2, 2, 3, 5, 6, 7, 8, 9 };
				TEST_OPERATE_AND_FOR_EACH(l1, l1.assign(8, 8));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.assign(a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.assign({ 1,2,3,4,5,6 }));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.insert(l1.end(), 6));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.insert(l1.end(), 2, 7));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.insert(l1.begin(), a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.push_back(2));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.push_front(1));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.emplace(l1.begin(), 1));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.emplace_front(0));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.emplace_back(10));
				TEST_OPERATE_AND_CHECK_RETURN(l1.size());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.pop_front());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.pop_back());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.erase(l1.begin()));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.erase(l1.begin(), l1.end()));
				TEST_OPERATE_AND_CHECK_RETURN(l1.size());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.resize(10));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.resize(5, 1));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.resize(8, 2));
				TEST_OPERATE_AND_CHECK_RETURN(l1.size());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.splice(l1.end(), l4));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.splice(l1.begin(), l5, l5.begin()));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.splice(l1.end(), l6, l6.begin(), ++l6.begin()));
				TEST_OPERATE_AND_CHECK_RETURN(l1.size());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.remove(0));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.remove_if([](const int& val) {return val % 2 == 0; }));
				TEST_OPERATE_AND_CHECK_RETURN(l1.size());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.assign({ 9,5,3,3,7,1,3,2,2,0,10 }));
				TEST_OPERATE_AND_CHECK_RETURN(l1.size());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.sort());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.unique());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.unique([&](int a, int b) {return b == a + 1; }));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.merge(l7));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.sort(std::greater<int>()));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.merge(l8, std::greater<int>()));
				TEST_OPERATE_AND_FOR_EACH(l1, l1.reverse());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.clear());
				TEST_OPERATE_AND_FOR_EACH(l1, l1.swap(l9));
				TEST_OPERATE_AND_CHECK_RETURN(*l1.begin());
				TEST_OPERATE_AND_CHECK_RETURN(*l1.rbegin());
				TEST_OPERATE_AND_CHECK_RETURN(l1.front());
				TEST_OPERATE_AND_CHECK_RETURN(l1.back());
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(l1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(l1.size());
				cout << endl << endl;
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::list<int>, PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_INT(std::list<int>, PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_INT(std::list<int>, PERFORMANCE_TEST_LEN3, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::list<int>, PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::list<int>, PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::list<int>, PERFORMANCE_TEST_LEN3, push_back);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "              test list end              "   << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}
		
		namespace test_deque{
			void test() {
				cout << "===========================================" << endl;
				cout << "              test deque start              "<< endl;
				cout << "===========================================" << endl;
				int a[] = { 1,2,3,4,5 };
				mystl::deque<int> d1;
				mystl::deque<int> d2(5);
				mystl::deque<int> d3(5, 1);
				mystl::deque<int> d4(a, a + 5);
				mystl::deque<int> d5(d2);
				mystl::deque<int> d6(std::move(d2));
				mystl::deque<int> d7;
				d7 = d3;
				mystl::deque<int> d8;
				d8 = std::move(d3);
				mystl::deque<int> d9{ 1,2,3,4,5,6,7,8,9 };
				mystl::deque<int> d10;
				d10 = { 1,2,3,4,5,6,7,8,9 };

				TEST_OPERATE_AND_FOR_EACH(d1, d1.assign(5, 1));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.assign(8, 8));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.assign(a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.assign({ 1,2,3,4,5 }));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.insert(d1.end(), 6));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.insert(d1.end() - 1, 2, 7));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.insert(d1.begin(), a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.erase(d1.begin()));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.erase(d1.begin(), d1.begin() + 4));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.emplace_back(8));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.emplace_front(8));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.emplace(d1.begin() + 1, 9));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.push_front(1));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.push_back(2));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.pop_back());
				TEST_OPERATE_AND_FOR_EACH(d1, d1.pop_front());
				TEST_OPERATE_AND_FOR_EACH(d1, d1.shrink_to_fit());
				TEST_OPERATE_AND_FOR_EACH(d1, d1.resize(5));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.resize(8, 8));
				TEST_OPERATE_AND_FOR_EACH(d1, d1.clear());
				TEST_OPERATE_AND_FOR_EACH(d1, d1.shrink_to_fit());
				TEST_OPERATE_AND_FOR_EACH(d1, d1.swap(d4));
				TEST_OPERATE_AND_CHECK_RETURN(*(d1.begin()));
				TEST_OPERATE_AND_CHECK_RETURN(*(d1.end() - 1));
				TEST_OPERATE_AND_CHECK_RETURN(*(d1.rbegin()));
				TEST_OPERATE_AND_CHECK_RETURN(*(d1.rend() - 1));
				TEST_OPERATE_AND_CHECK_RETURN(d1.front());
				TEST_OPERATE_AND_CHECK_RETURN(d1.back());
				TEST_OPERATE_AND_CHECK_RETURN(d1.at(1));
				TEST_OPERATE_AND_CHECK_RETURN(d1[2]);
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(d1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(d1.size());
				cout << endl << endl;
#ifdef PERFORMANCE_TEST_ON
				//push_back
				PERFORMANCE_TEST_TYPE_INT(std::deque<int>, PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_INT(std::deque<int>, PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_INT(std::deque<int>, PERFORMANCE_TEST_LEN3, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::deque<int>, PERFORMANCE_TEST_LEN1, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::deque<int>, PERFORMANCE_TEST_LEN2, push_back);
				PERFORMANCE_TEST_TYPE_INT(mystl::deque<int>, PERFORMANCE_TEST_LEN3, push_back);
				//push_front
				PERFORMANCE_TEST_TYPE_INT(std::deque<int>, PERFORMANCE_TEST_LEN1, push_front);
				PERFORMANCE_TEST_TYPE_INT(std::deque<int>, PERFORMANCE_TEST_LEN2, push_front);
				PERFORMANCE_TEST_TYPE_INT(std::deque<int>, PERFORMANCE_TEST_LEN3, push_front);
				PERFORMANCE_TEST_TYPE_INT(mystl::deque<int>, PERFORMANCE_TEST_LEN1, push_front);
				PERFORMANCE_TEST_TYPE_INT(mystl::deque<int>, PERFORMANCE_TEST_LEN2, push_front);
				PERFORMANCE_TEST_TYPE_INT(mystl::deque<int>, PERFORMANCE_TEST_LEN3, push_front);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "              test deque end              " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_stack{
			void test() {
				cout << "===========================================" << endl;
				cout << "              test stack start             " << endl;
				cout << "===========================================" << endl;
				mystl::stack<int> s;
				mystl::stack<int> s2;
				s.push(1); s2.push(1);
				s.push(3); s2.push(3);
				s.push(5); s2.push(5);
				s.push(7); s2.push(7);
				mystl::stack<int> s3 = std::move(s2);

				TEST_OPERATE_AND_CHECK_RETURN(s.size());
				TEST_OPERATE_AND_CHECK_RETURN(s.top()); s.pop();
				TEST_OPERATE_AND_CHECK_RETURN(s.top()); s.pop();
				TEST_OPERATE_AND_CHECK_RETURN(s.top());
				s.emplace(2);
				s.emplace(4);
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(s.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(s.size());
				cout << "get top and pop until empty" << endl;
				while (!s.empty()) {
					cout << s.top() << " ";
					s.pop();
				}
				cout  << endl;
				TEST_OPERATE_AND_CHECK_RETURN(s.size());
				s.swap(s3);
				cout << "After swap s3" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(s.size());
				TEST_OPERATE_AND_CHECK_RETURN(s.top());
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::stack<int>, PERFORMANCE_TEST_LEN1, push);
				PERFORMANCE_TEST_TYPE_INT(std::stack<int>, PERFORMANCE_TEST_LEN2, push);
				PERFORMANCE_TEST_TYPE_INT(std::stack<int>, PERFORMANCE_TEST_LEN3, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::stack<int>, PERFORMANCE_TEST_LEN1, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::stack<int>, PERFORMANCE_TEST_LEN2, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::stack<int>, PERFORMANCE_TEST_LEN3, push);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "              test stack end               " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_queue {
			void test() {
				cout << "===========================================" << endl;
				cout << "              test queue start             " << endl;
				cout << "===========================================" << endl;
				mystl::queue<int> q;
				mystl::queue<int> q2;
				q.push(1); q2.push(1);
				q.push(3); q2.push(3);
				q.push(5); q2.push(5);
				q.push(7); q2.push(7);
				mystl::queue<int> q3 = std::move(q2);
				

				TEST_OPERATE_AND_CHECK_RETURN(q.size());
				TEST_OPERATE_AND_CHECK_RETURN(q.front()); q.pop();
				cout << "After pop" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(q.front()); q.pop();
				cout << "After pop" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(q.back());
				q.emplace(2);				  
				q.emplace(4);
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(q.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(q.size());
				cout << "get front and pop until empty" << endl;
				while (!q.empty()) {
					cout << q.front() << " ";
					q.pop();
				}
				cout << endl;
				TEST_OPERATE_AND_CHECK_RETURN(q.size());
				q.swap(q3);
				cout << "After swap q3" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(q.size());
				TEST_OPERATE_AND_CHECK_RETURN(q.front());
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::queue<int>, PERFORMANCE_TEST_LEN1, push);
				PERFORMANCE_TEST_TYPE_INT(std::queue<int>, PERFORMANCE_TEST_LEN2, push);
				PERFORMANCE_TEST_TYPE_INT(std::queue<int>, PERFORMANCE_TEST_LEN3, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::queue<int>, PERFORMANCE_TEST_LEN1, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::queue<int>, PERFORMANCE_TEST_LEN2, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::queue<int>, PERFORMANCE_TEST_LEN3, push);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "              test queue end               " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_priority_queue{
			void test() {
				cout << "===========================================" << endl;
				cout << "         test priority_queue start         " << endl;
				cout << "===========================================" << endl;
				mystl::priority_queue<int> pq;
				mystl::priority_queue<int> pq2;
				pq.push(1); pq2.push(1);
				pq.push(3); pq2.push(3);
				pq.push(5); pq2.push(5);
				pq.push(7); pq2.push(7);
				mystl::priority_queue<int> pq3 = std::move(pq2);


				TEST_OPERATE_AND_CHECK_RETURN(pq.size());
				TEST_OPERATE_AND_CHECK_RETURN(pq.top()); pq.pop();
				cout << "After pop" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(pq.top()); pq.pop();
				cout << "After pop" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(pq.top());
				pq.emplace(2);
				pq.emplace(4);
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(pq.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(pq.size());
				cout << "get top and pop until empty" << endl;
				while (!pq.empty()) {
					cout << pq.top() << " ";
					pq.pop();
				}
				cout << endl;
				TEST_OPERATE_AND_CHECK_RETURN(pq.size());
				pq.swap(pq3);
				cout << "After swap pq3" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(pq.size());
				TEST_OPERATE_AND_CHECK_RETURN(pq.top());
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::stack<int>, PERFORMANCE_TEST_LEN1, push);
				PERFORMANCE_TEST_TYPE_INT(std::stack<int>, PERFORMANCE_TEST_LEN2, push);
				PERFORMANCE_TEST_TYPE_INT(std::stack<int>, PERFORMANCE_TEST_LEN3, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::stack<int>, PERFORMANCE_TEST_LEN1, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::stack<int>, PERFORMANCE_TEST_LEN2, push);
				PERFORMANCE_TEST_TYPE_INT(mystl::stack<int>, PERFORMANCE_TEST_LEN3, push);
#endif // PERFORMANCE_TEST_ON

				cout << "===========================================" << endl;
				cout << "         test priority_queue end           " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}


		namespace test_set {
			void test() {
				cout << "===========================================" << endl;
				cout << "              test set start               " << endl;
				cout << "===========================================" << endl;
				int a[] = { 5,4,3,2,1 };
				mystl::set<int> s1;
				mystl::set<int, std::greater<int>> s2;
				mystl::set<int> s3(a, a + 5);
				mystl::set<int> s4(a, a + 5);
				mystl::set<int> s5(s3);
				mystl::set<int> s6(std::move(s3));
				mystl::set<int> s7;
				s7 = s4;
				mystl::set<int> s8;
				s8 = std::move(s4);
				mystl::set<int> s9{ 1,2,3,4,5 };
				mystl::set<int> s10;
				s10 = { 1,2,3,4,5 };

				for (int i = 5; i > 0; --i)
				{
					TEST_OPERATE_AND_FOR_EACH(s1, s1.emplace(i));
				}
				TEST_OPERATE_AND_FOR_EACH(s1, s1.emplace_hint(s1.begin(), 0));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin()));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(0));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(1));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin(), s1.end()));
				for (int i = 0; i < 5; ++i)
				{
					TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(i));
				}
				TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(5));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(s1.end(), 5));
				TEST_OPERATE_AND_CHECK_RETURN(s1.count(5));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.find(3));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.lower_bound(3));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.upper_bound(3));
				auto& first = *s1.equal_range(3).first;
				auto& second = *s1.equal_range(3).second;
				cout << "s1.equal_range(3) : from " << first << " to " << second << endl;
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin()));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(1));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin(), s1.find(3)));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.clear());
				TEST_OPERATE_AND_FOR_EACH(s1, s1.swap(s5));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.begin());
				TEST_OPERATE_AND_CHECK_RETURN(*s1.rbegin());
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(s1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(s1.size());
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::set<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(std::set<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(std::set<int>, PERFORMANCE_TEST_LEN6, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::set<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::set<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::set<int>, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON

				cout << "===========================================" << endl;
				cout << "              test set end               " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}


		namespace test_multiset {
			void test() {
				cout << "===========================================" << endl;
				cout << "             test multiset start           " << endl;
				cout << "===========================================" << endl;
				int a[] = { 5,4,3,2,1 };
				mystl::multiset<int> s1;
				mystl::multiset<int, std::greater<int>> s2;
				mystl::multiset<int> s3(a, a + 5);
				mystl::multiset<int> s4(a, a + 5);
				mystl::multiset<int> s5(s3);
				mystl::multiset<int> s6(std::move(s3));
				mystl::multiset<int> s7;
				s7 = s4;
				mystl::multiset<int> s8;
				s8 = std::move(s4);
				mystl::multiset<int> s9{ 1,2,3,4,5 };
				mystl::multiset<int> s10;
				s10 = { 1,2,3,4,5 };

				for (int i = 5; i > 0; --i)
				{
					TEST_OPERATE_AND_FOR_EACH(s1, s1.emplace(i));
				}
				TEST_OPERATE_AND_FOR_EACH(s1, s1.emplace_hint(s1.begin(), 0));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin()));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(0));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(1));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin(), s1.end()));
				for (int i = 0; i < 5; ++i)
				{
					TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(i));
				}
				TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(5));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.insert(s1.end(), 5));
				TEST_OPERATE_AND_CHECK_RETURN(s1.count(5));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.find(3));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.lower_bound(3));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.upper_bound(3));
				auto& first = *s1.equal_range(3).first;
				auto& second = *s1.equal_range(3).second;
				cout << "s1.equal_range(3) : from " << first << " to " << second << endl;
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin()));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(1));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.erase(s1.begin(), s1.find(3)));
				TEST_OPERATE_AND_FOR_EACH(s1, s1.clear());
				TEST_OPERATE_AND_FOR_EACH(s1, s1.swap(s5));
				TEST_OPERATE_AND_CHECK_RETURN(*s1.begin());
				TEST_OPERATE_AND_CHECK_RETURN(*s1.rbegin());
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(s1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(s1.size());
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::multiset<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(std::multiset<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(std::multiset<int>, PERFORMANCE_TEST_LEN6, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::multiset<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::multiset<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::multiset<int>, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON

				cout << "===========================================" << endl;
				cout << "            test multiset end              " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_map {
			void test() {
				cout << "===========================================" << endl;
				cout << "              test map start               " << endl;
				cout << "===========================================" << endl;
				mystl::vector<PAIR> v;
				for (int i = 0; i < 5; ++i)
					v.push_back(PAIR(i, i));
				mystl::map<int, int> m1;
				mystl::map<int, int, std::greater<int>> m2;
				mystl::map<int, int> m3(v.begin(), v.end());
				mystl::map<int, int> m4(v.begin(), v.end());
				mystl::map<int, int> m5(m3);
				mystl::map<int, int> m6(std::move(m3));
				mystl::map<int, int> m7;
				m7 = m4;
				mystl::map<int, int> m8;
				m8 = std::move(m4);
				mystl::map<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
				mystl::map<int, int> m10;
				m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };


				for (int i = 5; i > 0; --i)
				{
					MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.emplace(i, i));
				}
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.emplace_hint(m1.begin(), 0, 0));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin()));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(0));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(1));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin(), m1.end()));
				for (int i = 0; i < 5; ++i)
				{
					MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(PAIR(i, i)));
				}
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(v.begin(), v.end()));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(m1.end(), PAIR(5, 5)));
				TEST_OPERATE_AND_CHECK_RETURN(m1.count(1));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.find(3));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.lower_bound(3));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.upper_bound(2));
				auto first = *m1.equal_range(2).first;
				auto second = *m1.equal_range(2).second;
				cout << "m1.equal_range(2) : from <" << first.first << ", " << first.second
					<< "> to <" << second.first << ", " << second.second << ">" << endl;
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin()));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(1));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin(), m1.find(3)));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.clear());
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.swap(m9));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.begin());
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.rbegin());
				TEST_OPERATE_AND_CHECK_RETURN(m1[1]);
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1[1] = 3);
				TEST_OPERATE_AND_CHECK_RETURN(m1.at(1));
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(m1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(m1.size());
#ifdef PERFORMANCE_TEST_ON
				MAP_PERFORMANCE_TEST(std::map,int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(std::map,int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(std::map,int, PERFORMANCE_TEST_LEN6, insert);
				MAP_PERFORMANCE_TEST(mystl::map,int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(mystl::map,int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(mystl::map,int, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON

				cout << "===========================================" << endl;
				cout << "              test map end                 " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_multimap {
			void test() {
				cout << "===========================================" << endl;
				cout << "            test multimap start            " << endl;
				cout << "===========================================" << endl;
				mystl::vector<PAIR> v;
				for (int i = 0; i < 5; ++i)
					v.push_back(PAIR(i, i));
				mystl::multimap<int, int> m1;
				mystl::multimap<int, int, std::greater<int>> m2;
				mystl::multimap<int, int> m3(v.begin(), v.end());
				mystl::multimap<int, int> m4(v.begin(), v.end());
				mystl::multimap<int, int> m5(m3);
				mystl::multimap<int, int> m6(std::move(m3));
				mystl::multimap<int, int> m7;
				m7 = m4;
				mystl::multimap<int, int> m8;
				m8 = std::move(m4);
				mystl::multimap<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
				mystl::multimap<int, int> m10;
				m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };

				for (int i = 5; i > 0; --i)
				{
					MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.emplace(i, i));
				}
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.emplace_hint(m1.begin(), 0, 0));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin()));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(0));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(1));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin(), m1.end()));
				for (int i = 0; i < 5; ++i)
				{
					MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(std::make_pair(i, i)));
				}
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(v.begin(), v.end()));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(PAIR(5, 5)));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(m1.end(), PAIR(5, 5)));
				TEST_OPERATE_AND_CHECK_RETURN(m1.count(3));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.find(3));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.lower_bound(3));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.upper_bound(2));
				auto first = *m1.equal_range(2).first;
				auto second = *m1.equal_range(2).second;
				cout << "m1.equal_range(2) : from <" << first.first << ", " << first.second
					<< "> to <" << second.first << ", " << second.second << ">" << endl;
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin()));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(1));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.erase(m1.begin(), m1.find(3)));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.clear());
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.swap(m9));
				MAP_TEST_OPERATE_AND_FOR_EACH(m1, m1.insert(PAIR(3, 3)));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.begin());
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*m1.rbegin());
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(m1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(m1.size());
#ifdef PERFORMANCE_TEST_ON
				MAP_PERFORMANCE_TEST(std::multimap, int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(std::multimap, int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(std::multimap, int, PERFORMANCE_TEST_LEN6, insert);
				MAP_PERFORMANCE_TEST(mystl::multimap, int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(mystl::multimap, int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(mystl::multimap, int, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "            test multimap end              " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_unordered_set {
			void test() {
				cout << "===========================================" << endl;
				cout << "         test unordered_set start          " << endl;
				cout << "===========================================" << endl;
				int a[] = { 5,4,3,2,1 };
				mystl::unordered_set<int> us1;
				mystl::unordered_set<int> us2(520);
				mystl::unordered_set<int> us3(520, mystl::hash<int>());
				mystl::unordered_set<int> us4(520, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_set<int> us5(a, a + 5);
				mystl::unordered_set<int> us6(a, a + 5, 100);
				mystl::unordered_set<int> us7(a, a + 5, 100, mystl::hash<int>());
				mystl::unordered_set<int> us8(a, a + 5, 100, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_set<int> us9(us5);
				mystl::unordered_set<int> us10(std::move(us5));
				mystl::unordered_set<int> us11;
				us11 = us6;
				mystl::unordered_set<int> us12;
				us12 = std::move(us6);
				mystl::unordered_set<int> us13{ 1,2,3,4,5 };
				mystl::unordered_set<int> us14;
				us13 = { 1,2,3,4,5 };

				TEST_OPERATE_AND_FOR_EACH(us1, us1.emplace(1));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.emplace_hint(us1.end(), 2));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.insert(5));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.insert(us1.begin(), 5));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.insert(a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.erase(us1.begin()));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.erase(us1.begin(), us1.find(3)));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.erase(1));
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket(1));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(us1.bucket(5)));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.clear());
				TEST_OPERATE_AND_FOR_EACH(us1, us1.swap(us7));
				TEST_OPERATE_AND_CHECK_RETURN(*us1.begin());
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_FOR_EACH(us1, us1.reserve(1000));
				TEST_OPERATE_AND_CHECK_RETURN(*us1.begin(us1.bucket(1)));
				TEST_OPERATE_AND_CHECK_RETURN(us1.size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(1));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(2));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(3));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.rehash(150));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.count(1));
				TEST_OPERATE_AND_CHECK_RETURN(*us1.find(3));
				auto first = *us1.equal_range(3).first;
				auto second = *us1.equal_range(3).second;
				cout << "us1.equal_range(3) : from " << first << " to " << second << endl;
				TEST_OPERATE_AND_CHECK_RETURN(us1.load_factor());
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_load_factor());
				TEST_OPERATE_AND_FOR_EACH(us1, us1.max_load_factor(1.5f));
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_load_factor());
#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::unordered_set<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(std::unordered_set<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(std::unordered_set<int>, PERFORMANCE_TEST_LEN6, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::unordered_set<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::unordered_set<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::unordered_set<int>, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "          test unordered_set end           " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;

			}
		}

		namespace test_unordered_multiset{
			void test() {
				cout << "===========================================" << endl;
				cout << "       test unordered_multiset start       " << endl;
				cout << "===========================================" << endl;
				int a[] = { 5,4,3,2,1 };
				mystl::unordered_multiset<int> us1;
				mystl::unordered_multiset<int> us2(520);
				mystl::unordered_multiset<int> us3(520, mystl::hash<int>());
				mystl::unordered_multiset<int> us4(520, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_multiset<int> us5(a, a + 5);
				mystl::unordered_multiset<int> us6(a, a + 5, 100);
				mystl::unordered_multiset<int> us7(a, a + 5, 100, mystl::hash<int>());
				mystl::unordered_multiset<int> us8(a, a + 5, 100, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_multiset<int> us9(us5);
				mystl::unordered_multiset<int> us10(std::move(us5));
				mystl::unordered_multiset<int> us11;
				us11 = us6;
				mystl::unordered_multiset<int> us12;
				us12 = std::move(us6);
				mystl::unordered_multiset<int> us13{ 1,2,3,4,5 };
				mystl::unordered_multiset<int> us14;
				us14 = { 1,2,3,4,5 };

				TEST_OPERATE_AND_FOR_EACH(us1, us1.emplace(1));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.emplace_hint(us1.end(), 2));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.insert(5));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.insert(us1.begin(), 5));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.insert(a, a + 5));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.erase(us1.begin()));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.erase(us1.begin(), us1.find(3)));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.erase(1));
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket(1));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(us1.bucket(5)));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.clear());
				TEST_OPERATE_AND_FOR_EACH(us1, us1.swap(us7));
				TEST_OPERATE_AND_CHECK_RETURN(*us1.begin());
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(us1.size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_FOR_EACH(us1, us1.reserve(1000));
				TEST_OPERATE_AND_CHECK_RETURN(*us1.begin(us1.bucket(1)));
				TEST_OPERATE_AND_CHECK_RETURN(us1.size());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(1));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(2));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_size(3));
				TEST_OPERATE_AND_FOR_EACH(us1, us1.rehash(150));
				TEST_OPERATE_AND_CHECK_RETURN(us1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(us1.count(1));
				TEST_OPERATE_AND_CHECK_RETURN(*us1.find(3));
				auto first = *us1.equal_range(3).first;
				auto second = *us1.equal_range(3).second;
				cout << "us1.equal_range(3) : from " << first << " to " << second << endl;
				TEST_OPERATE_AND_CHECK_RETURN(us1.load_factor());
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_load_factor());
				TEST_OPERATE_AND_FOR_EACH(us1, us1.max_load_factor(1.5f));
				TEST_OPERATE_AND_CHECK_RETURN(us1.max_load_factor());

#ifdef PERFORMANCE_TEST_ON
				PERFORMANCE_TEST_TYPE_INT(std::unordered_multiset<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(std::unordered_multiset<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(std::unordered_multiset<int>, PERFORMANCE_TEST_LEN6, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::unordered_multiset<int>, PERFORMANCE_TEST_LEN4, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::unordered_multiset<int>, PERFORMANCE_TEST_LEN5, insert);
				PERFORMANCE_TEST_TYPE_INT(mystl::unordered_multiset<int>, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON

				cout << "===========================================" << endl;
				cout << "       test unordered_multiset end         " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_unordered_map{
			void test() {
				cout << "===========================================" << endl;
				cout << "         test unordered_map start          " << endl;
				cout << "===========================================" << endl;
				mystl::vector<PAIR> v;
				for (int i = 0; i < 5; ++i)
					v.push_back(PAIR(5 - i, 5 - i));
				mystl::unordered_map<int, int> um1;
				mystl::unordered_map<int, int> um2(520);
				mystl::unordered_map<int, int> um3(520, mystl::hash<int>());
				mystl::unordered_map<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_map<int, int> um5(v.begin(), v.end());
				mystl::unordered_map<int, int> um6(v.begin(), v.end(), 100);
				mystl::unordered_map<int, int> um7(v.begin(), v.end(), 100, mystl::hash<int>());
				mystl::unordered_map<int, int> um8(v.begin(), v.end(), 100, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_map<int, int> um9(um5);
				mystl::unordered_map<int, int> um10(std::move(um5));
				mystl::unordered_map<int, int> um11;
				um11 = um6;
				mystl::unordered_map<int, int> um12;
				um12 = std::move(um6);
				mystl::unordered_map<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
				mystl::unordered_map<int, int> um14;
				um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.emplace(1, 1));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.emplace_hint(um1.begin(), 1, 2));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.insert(PAIR(2, 2)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.insert(um1.end(), PAIR(3, 3)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.insert(v.begin(), v.end()));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.erase(um1.begin()));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.erase(um1.begin(), um1.find(3)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.erase(1));
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket(1));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(um1.bucket(5)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.clear());
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.swap(um7));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*um1.begin());
				TEST_OPERATE_AND_CHECK_RETURN(um1.at(1));
				TEST_OPERATE_AND_CHECK_RETURN(um1[1]);
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket(1));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(um1.bucket(1)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.reserve(1000));
				TEST_OPERATE_AND_CHECK_RETURN(um1.size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(1));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(2));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(3));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.rehash(150));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.count(1));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*um1.find(3));
				auto first = *um1.equal_range(3).first;
				auto second = *um1.equal_range(3).second;
				cout << "um1.equal_range(3) : from <" << first.first << ", " << first.second
					<< "> to <" << second.first << ", " << second.second << ">" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(um1.load_factor());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_load_factor());
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.max_load_factor(1.5f));
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_load_factor());
#ifdef PERFORMANCE_TEST_ON
				MAP_PERFORMANCE_TEST(std::unordered_map, int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(std::unordered_map, int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(std::unordered_map, int, PERFORMANCE_TEST_LEN6, insert);
				MAP_PERFORMANCE_TEST(mystl::unordered_map, int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(mystl::unordered_map, int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(mystl::unordered_map, int, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON

				cout << "===========================================" << endl;
				cout << "           test unordered_map end          " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}

		namespace test_unordered_multimap {
			void test() {
				cout << "===========================================" << endl;
				cout << "        test unordered_multimap start      " << endl;
				cout << "===========================================" << endl;
				mystl::vector<PAIR> v;
				for (int i = 0; i < 5; ++i)
					v.push_back(PAIR(5 - i, 5 - i));
				mystl::unordered_multimap<int, int> um1;
				mystl::unordered_multimap<int, int> um2(520);
				mystl::unordered_multimap<int, int> um3(520, mystl::hash<int>());
				mystl::unordered_multimap<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_multimap<int, int> um5(v.begin(), v.end());
				mystl::unordered_multimap<int, int> um6(v.begin(), v.end(), 100);
				mystl::unordered_multimap<int, int> um7(v.begin(), v.end(), 100, mystl::hash<int>());
				mystl::unordered_multimap<int, int> um8(v.begin(), v.end(), 100, mystl::hash<int>(), mystl::equal_to<int>());
				mystl::unordered_multimap<int, int> um9(um5);
				mystl::unordered_multimap<int, int> um10(std::move(um5));
				mystl::unordered_multimap<int, int> um11;
				um11 = um6;
				mystl::unordered_multimap<int, int> um12;
				um12 = std::move(um6);
				mystl::unordered_multimap<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
				mystl::unordered_multimap<int, int> um14;
				um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.emplace(1, 1));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.emplace_hint(um1.begin(), 1, 2));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.insert(PAIR(2, 2)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.insert(um1.end(), PAIR(3, 3)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.insert(v.begin(), v.end()));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.erase(um1.begin()));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.erase(um1.begin(), um1.find(3)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.erase(1));
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket(1));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(um1.bucket(5)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.clear());
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.swap(um7));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*um1.begin());
				cout << std::boolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.empty());
				cout << std::noboolalpha;
				TEST_OPERATE_AND_CHECK_RETURN(um1.size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket(1));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(um1.bucket(1)));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.reserve(1000));
				TEST_OPERATE_AND_CHECK_RETURN(um1.size());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(1));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(2));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_size(3));
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.rehash(150));
				TEST_OPERATE_AND_CHECK_RETURN(um1.bucket_count());
				TEST_OPERATE_AND_CHECK_RETURN(um1.count(1));
				MAP_TEST_OPERATE_AND_CHECK_RETURN(*um1.find(3));
				auto first = *um1.equal_range(3).first;
				auto second = *um1.equal_range(3).second;
				cout << "um1.equal_range(3) : from <" << first.first << ", " << first.second
					<< "> to <" << second.first << ", " << second.second << ">" << endl;
				TEST_OPERATE_AND_CHECK_RETURN(um1.load_factor());
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_load_factor());
				MAP_TEST_OPERATE_AND_FOR_EACH(um1, um1.max_load_factor(1.5f));
				TEST_OPERATE_AND_CHECK_RETURN(um1.max_load_factor());
#ifdef PERFORMANCE_TEST_ON
				MAP_PERFORMANCE_TEST(std::unordered_multimap, int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(std::unordered_multimap, int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(std::unordered_multimap, int, PERFORMANCE_TEST_LEN6, insert);
				MAP_PERFORMANCE_TEST(mystl::unordered_multimap, int, PERFORMANCE_TEST_LEN4, insert);
				MAP_PERFORMANCE_TEST(mystl::unordered_multimap, int, PERFORMANCE_TEST_LEN5, insert);
				MAP_PERFORMANCE_TEST(mystl::unordered_multimap, int, PERFORMANCE_TEST_LEN6, insert);
#endif // PERFORMANCE_TEST_ON
				cout << "===========================================" << endl;
				cout << "        test unordered_multimap end        " << endl;
				cout << "===========================================" << endl;
				cout << endl << endl;
			}
		}


	} //end of namespace test
} //end of namespace mystl

#undef TEST_PERFORMANCE 
#endif // !_MY_STL_TEST_H_

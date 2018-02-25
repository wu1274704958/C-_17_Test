#include <iostream>
#include <tuple>
#include <string_view>
using namespace std;

template<typename T,int ...I>
void print_tuple(T tu,index_sequence<I...> is)
{
	((cout << get<I>(tu) << " "), ...);
	cout << "\n";
}

int main()
{
	auto tup = make_tuple(2018, 2.25f, string_view("HelloWorld"));
	print_tuple(tup, make_index_sequence<tuple_size<decltype(tup)>::value>());

	system("pause");
	return 0;
}
#include <iostream>
#include <tuple>
#include <string_view>
using namespace std;

namespace n1 {

	template<typename T, int ...I>
	void print_tuple(T tu, index_sequence<I...> is)
	{
		((cout << get<I>(tu) << " "), ...);
		cout << "\n";
	}

	void test()
	{
		int a = 0;
		if (a = pow(2, 2); a > 8)
		{
			cout << a << endl;
		}

		auto tup = make_tuple(2018, 2.25f, string_view("HelloWorld"));
		print_tuple(tup, make_index_sequence<tuple_size<decltype(tup)>::value>());
		system("pause");
	}
}


int main()
{
	n1::test();

	return 0;
}
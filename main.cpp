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
		auto tup = make_tuple(2018, 2.25f, string_view("HelloWorld"));
		print_tuple(tup, make_index_sequence<tuple_size<decltype(tup)>::value>());
		
	}
}

namespace n3 {
	template<size_t ...Idx>
	struct Index_seq {

	};

	template<size_t ...M, size_t ...T>
	auto merge(Index_seq<M...> &&i, Index_seq<T...> &&ii) -> Index_seq<M..., T...>
	{
		Index_seq<M..., T...> v;
		return v;
	}

	template<size_t N>
	struct Make_index_seq
	{
		using type = decltype(merge(std::declval< typename Make_index_seq<N - 2>::type >(), std::declval< Index_seq<N - 2, N - 1>>()));
	};


	template<>
	struct Make_index_seq<3>
	{
		using type = Index_seq<1, 2>;
	};

	template<>
	struct Make_index_seq<2>
	{
		using type = Index_seq<1>;
	};

	template<size_t N>
	auto make_index_seq() ->typename Make_index_seq<N>::type
	{
		typename Make_index_seq<N>::type v;
		return v;
	}
}

namespace n2 {
	template<size_t N>
	struct Fibonacci {
		static const long long value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
	};

	template<>
	struct Fibonacci<1> {
		static const long long value = 1;
	};

	template<>
	struct Fibonacci<2> {
		static const long long value = 1;
	};

	template<size_t ...Idx>
	long long fibonacci(int num, n3::Index_seq<Idx ...> is)
	{
		long long res = 0;
		((Idx == num && (res = Fibonacci<Idx>::value),false), ...);
		return res;
	}

	void test()
	{
		while (true)
		{
			int num = 0;
			cin >> num;
			if (num < 1)
			{
				break;
			}

			cout << fibonacci(num, n3::make_index_seq<60>()) << endl;
		}
	}
}

namespace n4 {

	template<size_t ...Idx>
	struct Test
	{

	};

	template<size_t N,size_t ...Idx>
	int get_wws(Test<Idx...> t)
	{
		int v = 0;
		int i = 0;
		((N == i && (v = Idx), i++, false), ...);
		return v;
	}

	void test()
	{
		Test<6,7,8,4,2,1> b;
		cout << get_wws<2>(b)  << endl;
	}
}

int main()
{
	n2::test();
	system("pause");
	return 0;
}
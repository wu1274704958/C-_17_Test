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


	template<size_t ...T, size_t ...M>
	auto merge(Index_seq<T...> &&i, Index_seq<M...> &&ii) -> Index_seq<M..., T...>
	{
		Index_seq<M..., T...> v;
		return v;
	}

	template<size_t N>
	struct Make_index_seq
	{
		using type = decltype( merge( declval<typename Make_index_seq<N - 2>::type>(), declval< Index_seq<N - 2, N - 1> >() ) );
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
		cout << get_wws<4>(b)  << endl;
	}
}


namespace n5 {
	template<typename ...T>
	struct w_tup;

	template<>
	struct w_tup<> {
		typedef w_tup<> My_t;
	};

	template<typename _This,typename ...Afters>
	struct w_tup<_This, Afters...> : private w_tup<Afters ...>
	{
		typedef _This this_type;
		typedef w_tup<_This, Afters...> My_t;
		typedef w_tup<Afters...> Base_t;
		
		static const size_t _Mysize = 1 + sizeof...(Afters);

		_This _Myfirst;
		
		constexpr w_tup() : Base_t(),_Myfirst()
		{

		}

		constexpr w_tup(const _This &th,const Afters& ...Afters_arg) : Base_t(Afters_arg...),_Myfirst(th){
		
		}

		Base_t& getBase()
		{
			return (*this);
		}
	};

	template<typename ...L>
	struct sub;

	template<typename T,typename ...L>
	struct sub <w_tup<T,L...> >{
			using type = w_tup<L...>;
	};

	template<size_t N,typename T>
	struct hhh {
		using type = typename hhh<N - 1, typename sub<T>::type >::type;
	};

	template<typename T>
	struct hhh<0,T> {
		using type = T;
	};

	template<size_t N,typename ...T>
	auto w_get(w_tup<T...> &t) 
	{
		typedef typename hhh<N, w_tup<T...> >::type TYPE;
		auto a = reinterpret_cast<TYPE&>(t.getBase());
		return a._Myfirst;
	} 

	template<typename ...T>
	auto make_w_tup(T...arg) -> w_tup<T...>
	{
		w_tup<T...> t (arg...) ;
		return t;
	}
	
	void test()
	{
		auto a = make_w_tup( 78,'a',"sl",90.1,78,'j');
		auto b = w_get<2>(a);
		cout << b << endl;
	}

}


int main()
{
	n5::test();

	system("pause");
	return 0;
}
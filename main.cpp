#include <iostream>
#include <tuple>
#include <string_view>
#include <string>

/*

###                     #               #      #      #                  #       #         #
##  #  #   #  #  #   #               #                     #    #   #               #         #
###                             #         #       #                       #
###                           #                        #              #               #
# #                      #             #      #           #    #            #          #
## #               #   #             #                                  #       #
# ##   #  #   #                           #       #    #         #                      #
##                            #                            #            #       #
###                       #          #       #
###                #   #          #               #     #    #    #       #       #
###   #   #   #                         #
# #                          #                #                    #       #      #
#####                    #           #             #    #     #
#               #   #           #       #                           #      #
# ###   #   #                                 #               #
##                       #        #             #     #            #      #
#  #                   #
# #          #   #           #        #                      #      #
####   #                 #                 #     #    #
# #                #            #                           #
## #          #             #        #     #          #            #
## #    #                                     #
# #                 #        #                           #
###           #                   #    #     #     #
#    #             #                                #
# # #            #         #                      #
##        #                   #     #    #
# # #              #
###        #         #                      #
#   #                    #     #    #
###         #
#  #          #               #
##               #     #
###
###    #
#      #

*/

namespace n1 {


	template<typename T, int ...I>
	void print_tuple(T tu, std::index_sequence<I...> is)
	{
		((std::cout << std::get<I>(tu) << " "), ...);
		std::cout << "\n";
	}

	void test()
	{
		auto tup = std::make_tuple(2018, 2.25f, std::string_view("HelloWorld"));
		print_tuple(tup, std::make_index_sequence<std::tuple_size<decltype(tup)>::value>());
		
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
		using type = decltype( merge( std::declval<typename Make_index_seq<N - 2>::type>(), std::declval< Index_seq<N - 2, N - 1> >() ) );
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
	//using namespace std;

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
			std::cin >> num;
			if (num < 1)
			{
				break;
			}

			std::cout << fibonacci(num, n3::make_index_seq<60>()) << std::endl;
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
		std::cout << get_wws<4>(b)  << std::endl;
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
		this_type& getFirst()
		{
			return _Myfirst;
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
	auto w_get(w_tup<T...> &t)-> decltype( std::declval<typename hhh<N, w_tup<T...> >::type>().getFirst())
	{
		static_assert(N >= 0);
		static_assert(N < w_tup<T...>::_Mysize);
		typedef typename hhh<N, w_tup<T...> >::type TYPE;
		return reinterpret_cast<TYPE&>(t.getBase())._Myfirst;
	} 

	template<typename ...T>
	auto make_w_tup(T...arg) -> w_tup<T...>
	{
		w_tup<T...> t (arg...) ;
		return t;
	}
	
	void test()
	{
		auto a = make_w_tup( 78,'a',"sl",90.1,78,'j',std::string("aaaaaa"));
		auto &b = w_get<6>(a);
		w_get<6>(a).append("sajdak");
		
		std::cout << b << std::endl;
	}

}

#include <thread>


namespace n6 {
	void f1()
	{
		for (int i = 0; i < 5; i++)
		{
			std::cout << std::this_thread::get_id() <<  "  " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::cout << "结束" << std::endl;
	}

	struct Test {
		~Test()
		{
			std::cout << "~Test()" << std::endl;
		}
	};

	void test2()
	{
		std::shared_ptr<Test[]>ptr(new Test[10]);
		std::unique_ptr<Test[]>ptr2(new Test[10]);
		//std::unique_ptr<Test, void(*)(Test *)> ptr(new Test[10], [](Test *p) {delete[] p; });
	}

	void test()
	{
		std::thread t1(f1);
		t1.detach();
	}
}

#include <cstddef>
namespace n7 {
	struct Test {
		int a;
		char b;
	};
	struct X {};
	struct Y{};
	struct A {
		A(int = (A(5), 0)) noexcept {};
		A(const A&) noexcept {};
		A(A&&) noexcept{};
		~A() noexcept(false) {};
	};
	struct B {
		B() noexcept {};
		B(const B&) = default; // 异常规定是“ noexcept(true) ”
		B(B&&, int = (throw Y(), 0)) noexcept {};
		~B() noexcept(false) {};
	};

	struct D : public A, public B {
		int * p = new (std::nothrow) int[6];
		// D 拥有下列隐式声明的成员：
		// D::D() noexcept(false);
		// D::D(const D&) noexcept(true);
		// D::D(D&&) noexcept(false);
		// D::~D() noexcept(false);
	};

	void g(void pfa() noexcept(false))
	{
		try{
			pfa();
		}
		catch (int e)
		{
			printf("sss %d , %llx\n",e,(unsigned long long)&e);
		}
	}

	void test()
	{
		std::cout << sizeof(Test) << "\n";
		std::cout << alignof(Test) << '\n';

		D d;

		g([] {
			int a = 0;
			printf("sss %llx\n", (unsigned long long)&a);
			throw 1;
			printf("end %llx\n", (unsigned long long)&a);
		});
	}
}


namespace n8 {
	struct Test {
		char *p;
		Test(const char* s)
		{
			int len = strlen(s) + 1;
			p = new char[len];
			strcpy_s(p, len, s);
			std::cout << "Test() " << p << std::endl;
		}
		Test(const Test &t)
		{
			int len = strlen(t.p) + 1;
			p = new char[len];
			strcpy_s(p, len, t.p);
			std::cout << "Test(const Test &) " << p << std::endl;
		}
		Test(Test &&t)
		{
			p = t.p;
			t.p = nullptr;
			std::cout << "Test(Test &&) " <<p << std::endl;
			//std::cout << "Test(Test &&) " << p << std::endl;
		}
		~Test()
		{
			
			if (p)
			{
				std::cout << "~Test() " << p << std::endl;
				delete[] p;
			}
			else
			{
				std::cout << "~Test() "<< std::endl;
			}
		}
	};

	Test g()
	{
		Test t = Test("jkjk");
		return t;
	}

	void test()
	{
		Test t = g();
	}
}

#include <functional>

namespace n9 {
	struct Test {
		int a;
		int b;
		double c;
	};

	std::ostream& operator<<(std::ostream &out, const Test& t) {
		return out << t.a <<" " << t.b << "  "<< t.c ;
	}

	template<typename T>
	struct Test2;

#define HHH \
	template<typename T,size_t N> \
	struct Test2 < T [N]  > \
	{ \
		using type = T; \
		const static size_t value = N; \
	};

HHH
#undef HHH

	template<typename T>
	void func(T& a)
	{
		std::cout << a << std::endl;
	}

	void test()
	{
		Test t{ 1,2,3.2 };
		int c = 1;
		auto f = [c](auto &a) ->void {
			std::cout << a  << "  " << c << std::endl;
		};
		int a = 20;

		std::function<int (int)> s;

		f(a);
		func(a);
		f(t); 

		std::cout << typeid(Test2<int [9]>::type).name() << "  "<< Test2<int[9]>::value << std::endl;
	}
}

#include <math.h>
#include <conio.h>
#include <Windows.h>
#define M_PI 3.14159265358979323846

namespace n10 {

	void gotoxy(int x, int y)//定位光标，x为行坐标,y为列坐标
	{
		COORD pos = { x,y };
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(hOut, pos);
		
	}

	void cls()//定位光标，x为行坐标,y为列坐标
	{
		COORD pos = { 0,0 };
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		unsigned long k;
		FillConsoleOutputCharacterA(hOut, ' ', 300 * 300, pos,&k);
	}

	
	float x0 = 45.f, y0 = 45.f;
	
	

	void test_me(float ang,float _rx,float _ry)
	{
		float angle =  (360.0 - ang) / 2.0f;
		float ang_zl = ang  / 40.f;
		
	
		for (int i = 0; i < 40; i++)
		{
			float x = x0 + _rx * cos(angle *  M_PI / 180);
			float y = y0 + _ry * sin(angle * M_PI / 180);
			
			gotoxy(x, y);
			printf("%c", '#');

			angle += ang_zl;
		}
	}

	void test()
	{
		float a = 360.f;
		float rx = 20.f;
		float ry = 20.f;
		float o = 100.0f / 360.0f;
		while (true)
		{
			test_me(a,rx,ry);
			
			Sleep(30);
			cls();
			
			a -= 10.f;
			
			{
				rx += o;
				ry += o;
			}

			if (a <= 0.0f)
				break;
		}
	}

}

int main()
{
	n10::test();

	system("pause");
	return 0;
}
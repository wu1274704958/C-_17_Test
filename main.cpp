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

		constexpr w_tup(const My_t &tup) : Base_t(tup.getBase()), _Myfirst(tup.getFirst()) {

		}
		constexpr w_tup(My_t &&tup) : Base_t(std::move(tup.getBase())), _Myfirst(std::move(tup.getFirst())) {
			
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
		w_get<6>(a).append("sajdak");
		auto b = std::move(a);
		std::cout << w_get<6>(b) << std::endl;
		std::cout << w_get<1>(a) << std::endl;
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
		Test& operator=(const Test& t)
		{
			if (this == &t)
			{
				std::cout << "same" << "\n";
				return *this;
			}
				
			if (!t.p)
			{
				return *this;
			}
			if (p)
			{
				delete[] p;
			}
			int len = strlen(t.p) + 1;
			p = new char[len];
			strcpy_s(p, len, t.p);
			std::cout << "operator=(const Test& t) " << p << std::endl;

			return *this;
		}

		Test& operator=(Test&& t)
		{
			if (this == &t)
				return *this;
			if (!t.p)
			{
				return *this;
			}
			if (p)
			{
				delete[] p;
			}
			p = t.p;
			t.p = nullptr;
			std::cout << "operator=(Test&& t) " << p << std::endl;

			return *this;
		}

		bool operator>(Test& t)
		{
			return strcmp(this->p, t.p) > 0;
		}

		bool operator<(Test& t)
		{
			return strcmp(this->p, t.p) < 0;
		}

		operator const char *()
		{
			return this->p;
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
		std::vector<Test> v;
		Test t = Test("jkjk");
		v.push_back(std::move(t));
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
#define Len(arr) n11::ARR_LEN<decltype(arr)>::value

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

namespace n11 {
	template<typename T>
	void swap(T* t1, T* t2)
	{
		T temp = *t1;
		*t1 = *t2;
		*t2 = std::move(temp);
	}

	template<typename T>
	void adjustHeap(int now_i,int len,T* arr)
	{
		T temp = arr[now_i];
		int k = now_i * 2 + 1;
		for (; k < len; k = k * 2 + 1)
		{
			if (k + 1< len && arr[k] < arr[k + 1])
			{
				k = k + 1;
			}
			if (arr[k]>temp)
			{
				arr[now_i] = arr[k];
				now_i = k;
			}
			else {
				break;
			}
		}
		arr[now_i] = std::move(temp);
	}
	
	template<typename T>
	void heapSort(T* arr, int len)
	{
		int i = len / 2 - 1;
		for (; i >= 0; --i)
		{
			adjustHeap(i, len, arr);
		}
		
		int j = len - 1;
		for (; j > 0; --j)
		{
			swap(&(arr[0]), &(arr[j]));
			adjustHeap(0, j, arr);
		}
	}
	int getCharLen(int n)
	{
		int len = 0;
		while (n > 0)
		{
			++len;
			n /= 10;
		}
		return len;
	}
	int getCharLen(const char *n)
	{
		return strlen(n);
	}
	template<typename T>
	struct print_center_support
	{
		static const bool value = std::is_same<int, T>::value || std::is_same<const char *, T>::value || std::is_same<const char *, decltype((const char *)std::declval<T&>())>::value;
	};
	template<size_t N,char SP,typename T >
	void print_center(T& n)
	{
		static_assert(print_center_support<T>::value,"Not Support Type!!!");
		int len = getCharLen(n);
		int q = (N - len) / 2;
		int w = N - len - q;
		for (int i = 0; i < q; ++i)
		{
			printf("%c", SP);
		}
		std::cout << n;
		for (int i = 0; i < w; ++i)
		{
			printf("%c", SP);
		}
	}
	template<typename T>
	void printHeap(T *arr, int len)
	{
		int j = 1;
		int ls = 0;
		int lines = 0;
		while( ls <= len){
			ls = ls + j;
			lines = lines + 1;
			j = j * 2;
		}
		int spaces = ((ls - 1) / 2);
		int mid_spaces = 0;
		int ns = 1;
		int now = 0;
		for (int m = 0; m < lines;++m) {
			for (int n = 0; n < spaces; ++n){
				printf("     ");
			}
			for (int n = 0; n < ns; ++n){
				if (now >= len) {
					break;
				}
				print_center<5,' '>(arr[now]);

				for (int s = 0; s < mid_spaces;++s) {
					printf("     ");
				}
				now = now + 1;
			}
			printf("\n");
			ns = ns * 2;
			mid_spaces = spaces;
			spaces = (spaces - 1) / 2;
		}
	}
	
	template<typename T>
	struct ARR_LEN;

	template<typename T,size_t N>
	struct ARR_LEN<T[N]> {
		const static int value = N;
	};

	void test()
	{
		//int arr[] = { 7,9,4,1,0,5,65,90,12,100,2,1,11,111,765,121,56,123,87,12,32 };
		//float arr[] = { 1.8f,2.54f,0.9f,0.01f,122.f,89.11f };
		n8::Test arr[] = { "klk","asd","opop","u87","oopo","ll","abcd","mnb","lol","dota2","啊","wws","pop","menu","upup"};
	
		printHeap(arr, Len(arr));
		heapSort(arr, Len(arr));
		printHeap(arr, Len(arr));
	}
}

namespace n12 {

	template<typename T,typename ...Arg>
	void dec(T t,Arg ...arg)
	{
		std::cout << t << "\n";
		if constexpr(sizeof...(Arg) > 0)
		{
			dec<Arg...>(arg...);
		}
	}

	template<typename ...T>
	void kk(T ...k)
	{
		dec<T...>(k...);
	}
	void test()
	{
		kk(90, 89.90f, 8999, "2klkl");
	}
}

#include <mutex>
#include <memory>
#include <utility>

namespace n13 {
	inline std::mutex m;
	template<size_t N>
	void func(int &a)
	{
		for (int i = 0; i < 10; i++)
		{
			{
				std::lock_guard<std::mutex> lock(m);
				if constexpr(N == 0)
				{
					std::cout << "+ " << a++ << "\n";
				}
				else
				{
					std::cout << "- " << a-- << "\n";
				}
			}
		}
	}
	static int a = 50;
	
	void test()
	{
		for (int i = 0; i < 10; ++i)
		{
			system("cls");
			std::thread t1(func<0>, std::ref(a));
			std::thread t2(func<1>, std::ref(a));

			t1.detach();
			t2.detach();
			std::this_thread::sleep_for(std::chrono::seconds::duration(1));
			std::cout << a << "\n";
		}
		
	}
}

namespace n14{
	template<class T>
	void kk(T&& t)
	{
		if constexpr(std::is_same<decltype(std::forward<T>(t)), n8::Test>::value)
		{
			std::cout << "n8::Test" << std::endl;
		}else if constexpr(std::is_same<decltype(std::forward<T>(t)), n8::Test&>::value)
		{
			std::cout << "n8::Test&" << std::endl;
		}
		else if constexpr(std::is_same<decltype(std::forward<T>(t)), n8::Test&&>::value)
		{
			std::cout << "n8::Test&&" << std::endl;
		}
	}
	void test()
	{
		n8::Test t("jkk");
		std::tuple<n8::Test&> tup(t);

		t.p[0] = 'p';

		std::cout << std::get<0>(tup) << std::endl;

		//kk(t);
	}
}

int main()
{
	
	n14::test();
	system("pause");
	return 0;
}
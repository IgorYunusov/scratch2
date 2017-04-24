#define S2_IMPL

#include <cstdint>

#include "s2string.h"
#include "s2list.h"
#include "s2file.h"

#include "s2test.h"

static int _numFooInstances = 0;

class Foo
{
public:
	uint32_t num;

public:
	Foo()
	{
		_numFooInstances++;
		num = 0;
	}

	Foo(const Foo &copy)
	{
		_numFooInstances++;
		num = copy.num;
	}

	~Foo()
	{
		_numFooInstances--;
	}
};

static int _numBarInstances = 0;

class Bar
{
public:
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;

public:
	Bar()
	{
		_numBarInstances++;
	}

	Bar(const Bar &copy)
	{
		_numBarInstances++;
		a = copy.a;
		b = copy.b;
		c = copy.c;
		d = copy.d;
	}

	~Bar()
	{
		_numBarInstances--;
	}
};

int main()
{
	s2::test_begin();

#ifdef S2_USING_STRING
	s2::test_group("string");
	{
		s2::string str = "Hello";
		S2_TEST(str == "Hello");
		S2_TEST(str[0] == 'H');
		str[4] = '!';
		S2_TEST(str == "Hell!");
		S2_TEST(str.len() == 5);

		str = "Hello";
		S2_TEST(str == "Hello");
		str += " world";
		S2_TEST(str == "Hello world");

		str.setf("Test");
		S2_TEST(str == "Test");
		str.setf("Test %d", 10);
		S2_TEST(str == "Test 10");
		str.appendf(", and the other test is %f", 3.14159f);
		S2_TEST(str == "Test 10, and the other test is 3.141590");
	}
#endif

#ifdef S2_USING_LIST
	s2::test_group("list");
	{
		s2::list<int> arr;
		arr.add() = 10;
		arr.add() = 20;
		arr.add(30);
		S2_TEST(arr.len() == 3);
		S2_TEST(arr[1] == 20);
		arr[2] = 50;
		S2_TEST(arr[2] == 50);

		arr.clear();
		S2_TEST(arr.len() == 0);

		s2::list<Foo> foo_arr;
		Foo &f = foo_arr.add();
		f.num = 10;
		S2_TEST(_numFooInstances == 1);
		foo_arr.remove(0);
		S2_TEST(_numFooInstances == 0);

		Foo f2;
		f2.num = 20;
		S2_TEST(_numFooInstances == 1);
		foo_arr.add(f2);
		S2_TEST(_numFooInstances == 2);
		for (Foo &f : foo_arr) {
			S2_TEST(f.num == 20);
		}
		S2_TEST(foo_arr.begin() != foo_arr.end());
		foo_arr.clear();
		S2_TEST(_numFooInstances == 1);
		S2_TEST(foo_arr.begin() == foo_arr.end());
	}
#endif

#ifdef S2_USING_FILE
	s2::test_group("file");
	{
		s2::file file("test.bin");

		file.open(s2::filemode::write);
		S2_TEST(file.get_mode() == s2::filemode::write);
		uint32_t num = 0xC0DECA77;
		file.write(&num, sizeof(uint32_t));
		S2_TEST(file.pos() == 4);
		S2_TEST(file.eof());
		file.close();
		S2_TEST(file.get_mode() == s2::filemode::none);

		num = 0;
		S2_TEST(num == 0);

		file.open(s2::filemode::read);
		S2_TEST(file.get_mode() == s2::filemode::read);
		S2_TEST(file.size() == sizeof(uint32_t));
		S2_TEST(!file.eof());
		file.read(&num, sizeof(uint32_t));
		S2_TEST(num == 0xC0DECA77);
		S2_TEST(file.eof());
		file.close();
		S2_TEST(file.get_mode() == s2::filemode::none);

		file.open(s2::filemode::write);
		S2_TEST(file.eof());
		S2_TEST(file.get_mode() == s2::filemode::write);
		S2_TEST(file.size() == 0);
		Bar bar;
		S2_TEST(_numBarInstances == 1);
		bar.a = 1;
		bar.b = 2;
		bar.c = 3;
		bar.d = 4;
		file.write(bar);
		S2_TEST(file.eof());
		S2_TEST(file.pos() == sizeof(Bar));
		S2_TEST(file.size() == sizeof(Bar));
		file.close();
		S2_TEST(file.get_mode() == s2::filemode::none);

		file.open(s2::filemode::read);
		S2_TEST(file.get_mode() == s2::filemode::read);
		S2_TEST(file.size() == sizeof(Bar));
		S2_TEST(!file.eof());
		Bar bar2 = file.read<Bar>();
		S2_TEST(file.eof());
		S2_TEST(_numBarInstances == 2);
		S2_TEST(bar2.a == bar.a);
		S2_TEST(bar2.b == bar.b);
		S2_TEST(bar2.c == bar.c);
		S2_TEST(bar2.d == bar.d);
		file.close();
		S2_TEST(file.get_mode() == s2::filemode::none);
	}
#endif

	s2::test_end();

	getchar();
	return s2::test_retval();
}

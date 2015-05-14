#include "../source/Core/Delegate.h"

using namespace std;

class Foo
{
public:
	int mf(int x)
	{
		return x*=2;
	}
	static int sf(int x)
	{
		return x*=4;
	}
};

int ff(int x)
{
	return x*=3;
}

int g = 0;

int main()
{
	Foo foo;
	clock_t start;
	double duration;

	start = clock();
	for(long long i=0;i!=1e13;i++)
	{
		g = foo.mf(1);
		ff(1);
		Foo::sf(1);
	}
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout<<"printf: "<< duration <<'\n';

	start = clock();
	Delegate<int(int)> d;
	for(int i=0;i!=1000000000;i++)
	{
		d.Bind(&foo,&Foo::mf);
		//Delegate<void(int)> d2(&bar);
		//<void(int)> d3(&Foo::Bar2);
		g = d(1);	
		//d2(1);
		//d3(1);
	}
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout<<"printf: "<< duration <<'\n';

	cout << g << endl;
	
	return 0;
}
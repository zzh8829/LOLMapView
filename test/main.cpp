#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <set>
using namespace std;

class Class
{
public:
	virtual int func1()
	{
		return 0;
	}
	virtual int func2()
	{
		return 1;
	}
	int var1;
	float var2;
};

class Der : public Class
{
public:
	int func1()
	{
		return 2;
	}
};

int main()
{
	Class* obj = new Der;
	cout << Class << endl;

	return 0;
}

#if 0

typedef long double ld;

ld poly[101];
ld newPoly[101];

ld newton(ld x)
{
	int cnt = 0;
	while (true)
	{
		ld fx = poly[0];
		ld dfx = 0.0;
		for(int i=1;i<=100;i++)
		{
			dfx += i*poly[i]*pow(x,i-1);
			fx += poly[i]*pow(x,i);
		}
		x = x - fx/dfx;
		cnt ++ ;
		if(cnt > 100000) return 0xdeadbeef;
		ld res = poly[0];
		for(int i=1;i<=100;i++)
			if(poly[i])
				res += poly[i]*pow(x,i);
		if(res < 1e-5) break;
	}
	cout << x << endl;
	return x;
}


void divide(ld divisor)
{
	memset(newPoly,0,sizeof(newPoly));

	ld next = poly[100];
	for (int i=99;i>=0;i--)
	{
		newPoly[i] = next;
		next = next*divisor + poly[i];
	}

	memcpy(poly,newPoly,sizeof(poly));
}

int main()
{
	int n;
	cin >> n;
	int co,po;
	int tm = 0;
	for(int i=0;i!=n;i++)
	{
		cin >> co >> po;
		poly[po] = co;
		if(!tm) tm = po;
	}
	set<ld> ans;
	for(int i=0;i!=tm;i++)
	{
		ld root = newton(1227);
		if(root==0xdeadbeef)
			break;
		ans.insert((ld)((((long long)(root))*100000)/100000));
		divide(root);
	}
	if(ans.size()==0)
	{
		cout << "NO REAL ROOTS" << endl;
	} else {
		for(set<ld>::iterator it = ans.begin();it!=ans.end();it++)
			cout << *it << endl;
	}
	return 0;
}

#endif
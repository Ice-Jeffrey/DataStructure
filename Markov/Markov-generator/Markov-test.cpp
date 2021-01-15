#include<string>
#include<cstdlib>
#include<fstream>
#include<iostream>
using namespace std;
typedef pair<std::string, std::string> Pretype;

struct Sufnode
{
	std::string s;
	Sufnode* next;
};

struct Markovnode
{
	Pretype pre;
	Sufnode* Suf;
	Markovnode* next;
};
typedef Markovnode* Markovchain;

int main()
{
	//初始化马尔可夫链
	Markovchain m;
	m = NULL;
	//初始化需要读入的文件 
	char name[20];
	cout << "Please input the name of file which you're about to let the program learn." << endl;
	cin >> name;
	ifstream file(name,ios::in);
	ofstream fileout(name,ios::app);
	//输入并建立前缀表 
	int flag = 0;
	std::string input, s1, s2;
	//建立第一个前缀
	//从文件中读入字符串
	file >> s1 >> s2; 
	Pretype first;
	first = make_pair(s1, s2);
	//建立第一个前缀表并将其插入马尔可夫链当中
	Markovnode *p;
	p = new Markovnode;
	p->pre = first;
	p->Suf = NULL;
	p->next = m;
	m = p;
	
	while (file >> input)
	{	
		//输入的input为后缀，首先要判断该后缀在对应前缀中是否有过重复
		Sufnode *q;
		int flag = 0;
		for (q = p->Suf; q != NULL; q = q->next)
		{
			if (q->s == input)
			{
				flag = 1;
				break;
			}
		}

		//该后缀未出现过，将该后缀插入前缀对应的后缀中
		if (!flag)
		{
			q = new Sufnode;
			q->s = input;
			q->next = p->Suf;
			p->Suf = q;
		}

		//建立下一个前缀 
		Pretype pre;
		pre = make_pair(s2, input);
		
		//判断该前缀是否已有过重复
		flag = 0;
		Markovnode *n;
		for (n = m; n != NULL; n = n->next)
		{
			if (n->pre == pre)
			{
				//如果有过重复，则用p指向该重复元素的Markovnode,方便下次输入时直接插入后缀
				flag = 1;
				p = n;
				break;
			}
		}

		//若该前缀没有重复，则直接建立一个新的Markovnode
		if (!flag)
		{
			Markovnode *temp,*state;
			//为保证前缀具有一定的顺序，采用尾插法
			for (state = m; state->next != NULL; state = state->next);
			temp = new Markovnode;
			temp->pre = pre;
			temp->Suf = NULL;
			temp->next = state->next;
			state->next = temp;
			p = state->next;                //将新建立的前缀指向p，方便下一个后缀的插入
		}

		//进行迭代过程
		s2 = input;
	}
	//将生成的文章打印在屏幕上并同时写入读入文件的末尾。 
	cout << endl << "New essay generated is as follows." << endl;
	fileout << endl << "New essay generated is as follows." << endl;
		
	//遍历前缀表以及后缀表进行检验，并统计前缀表个数
	Markovnode* x;
	int cnt = 0;
	for (x = m; x != NULL; x = x->next)
	{
		cnt++;
		/*
		cout << x->pre.first << " " << x->pre.second << endl;
		for (Sufnode* y = x->Suf; y != NULL; y = y->next)
		{
			cout << y->s << " ";
		}
		cout << endl;*/
	}

	//随机进行输出
	int random,i;
	random = rand() % cnt;
	//随机输出第一个后缀
	Markovnode* r;
	Sufnode *t,*o;
	Pretype Pre;
	for (i = 0, r = m; i < random && r != NULL; i++, r = r->next)
	{
		o = NULL;
		//随机对后缀进行输出
		int c = 1;
		for (t = r->Suf; t != NULL; t = t->next)
		{
			int res;
			res = rand() % c;
			if (!res)
			{
				o = t;
			}
			c++;
		}
		cout << o->s << " ";
		fileout << o->s << " ";
		Pre = make_pair(Pre.second, o->s);
	}
	//循环输出之后的后缀	
	while (cnt>0)
	{
		//在前缀表中查找此前缀
		for (r = m; r != NULL; r = r->next)
		{
			//找到该前缀
			if (r->pre == Pre)
			{
				//随机对后缀进行输出
				Sufnode *t,*o;
				o = NULL;
				int c = 1;
				for (t = r->Suf; t != NULL; t = t->next)
				{
					
					int res;
					res = rand() % c;
					if (!res)
					{
						o = t;
					}
					c++;
				}
				cout << o->s << " ";
				fileout << o->s << " "; 
				Pre = make_pair(Pre.second, o->s);
				cnt--;
				break;
			}	
		}
	}
	file.close();
	fileout.close();
	return 0;
}

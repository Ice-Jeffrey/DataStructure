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
	//��ʼ������ɷ���
	Markovchain m;
	m = NULL;
	//��ʼ����Ҫ������ļ� 
	char name[20];
	cout << "Please input the name of file which you're about to let the program learn." << endl;
	cin >> name;
	ifstream file(name,ios::in);
	ofstream fileout(name,ios::app);
	//���벢����ǰ׺�� 
	int flag = 0;
	std::string input, s1, s2;
	//������һ��ǰ׺
	//���ļ��ж����ַ���
	file >> s1 >> s2; 
	Pretype first;
	first = make_pair(s1, s2);
	//������һ��ǰ׺�������������ɷ�������
	Markovnode *p;
	p = new Markovnode;
	p->pre = first;
	p->Suf = NULL;
	p->next = m;
	m = p;
	
	while (file >> input)
	{	
		//�����inputΪ��׺������Ҫ�жϸú�׺�ڶ�Ӧǰ׺���Ƿ��й��ظ�
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

		//�ú�׺δ���ֹ������ú�׺����ǰ׺��Ӧ�ĺ�׺��
		if (!flag)
		{
			q = new Sufnode;
			q->s = input;
			q->next = p->Suf;
			p->Suf = q;
		}

		//������һ��ǰ׺ 
		Pretype pre;
		pre = make_pair(s2, input);
		
		//�жϸ�ǰ׺�Ƿ����й��ظ�
		flag = 0;
		Markovnode *n;
		for (n = m; n != NULL; n = n->next)
		{
			if (n->pre == pre)
			{
				//����й��ظ�������pָ����ظ�Ԫ�ص�Markovnode,�����´�����ʱֱ�Ӳ����׺
				flag = 1;
				p = n;
				break;
			}
		}

		//����ǰ׺û���ظ�����ֱ�ӽ���һ���µ�Markovnode
		if (!flag)
		{
			Markovnode *temp,*state;
			//Ϊ��֤ǰ׺����һ����˳�򣬲���β�巨
			for (state = m; state->next != NULL; state = state->next);
			temp = new Markovnode;
			temp->pre = pre;
			temp->Suf = NULL;
			temp->next = state->next;
			state->next = temp;
			p = state->next;                //���½�����ǰ׺ָ��p��������һ����׺�Ĳ���
		}

		//���е�������
		s2 = input;
	}
	//�����ɵ����´�ӡ����Ļ�ϲ�ͬʱд������ļ���ĩβ�� 
	cout << endl << "New essay generated is as follows." << endl;
	fileout << endl << "New essay generated is as follows." << endl;
		
	//����ǰ׺���Լ���׺����м��飬��ͳ��ǰ׺�����
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

	//����������
	int random,i;
	random = rand() % cnt;
	//��������һ����׺
	Markovnode* r;
	Sufnode *t,*o;
	Pretype Pre;
	for (i = 0, r = m; i < random && r != NULL; i++, r = r->next)
	{
		o = NULL;
		//����Ժ�׺�������
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
	//ѭ�����֮��ĺ�׺	
	while (cnt>0)
	{
		//��ǰ׺���в��Ҵ�ǰ׺
		for (r = m; r != NULL; r = r->next)
		{
			//�ҵ���ǰ׺
			if (r->pre == Pre)
			{
				//����Ժ�׺�������
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

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "huffman.hpp"
#include "bitstream.hpp"
using namespace std;

//�����ѹ������Ĵ�����ʾ 
int Showhelp()
{
	cout << "Wrong instruction!" << endl;
	cout << "Usage test : a file name, zip or unzip" << endl;
	cout << "Example : test.txt zip" << endl;
	return 0;
} 

//�ڽ�ѹʱ���а�λ�������֮���ļ�д�� 
inline string change(char c)
{
	string data;
	for(int i=0;i<8;i++)
	{
		if( ((c>>i) & 0x01) == 1 )
			data += '1';
		else
			data += '0';	
	} 
	string s(data.rbegin(),data.rend());
	return s;
}

int main(int argc,char* argv[])
{
	//����ʱ���費ʹ�������в������е��� 
	/*char chara[100];
	int flag;
	cin >> chara >> flag;
	argc = 3;
	argv[1] = chara;*/

	//�������֮��ʼ��ʽ��ʱ 
	clock_t start,finish;
    double totaltime;
    start=clock();
    
	//ʹ�������в��� 
	if(argc != 3)
	{
		Showhelp();
		return 1; 
	} 
	
	int flag;                                                  //��ʾ��������ı��� ,0Ϊѹ����1Ϊ��ѹ 
	if(!strcmp(argv[2],"zip"))
		flag = 0;	
	else if(!strcmp(argv[2],"unzip"))
		flag = 1;	
	
	
	//����һЩ����ʱ�ı�Ҫ��ȫ�ֱ���	
	unsigned char charin,charout;                              //��ʾ��Ҫ������������ֽ� 
	int count;                                                 //��ʾ�ļ����ֽ��� 
	int length;                                                //������ʾ�ļ����ĳ��� 	 
	HuffmanTree t;                                             //�������� 
	
	//����ѹ������ 
	if(!flag)                                                       
	{
		//flag = 0, ����ʱ��Ҫ��Ŀ���ļ�����ѹ��
		int cnt;                                                   //��ʾ����Ԫ�ĸ��� 
		int Array[256];                                            //������Ÿ��ַ�Ȩֵ������
		int CByte;	  											   //ת�����м����
			
		//��array������г�ʼ��
		for(int i=0;i<256;i++)
			Array[i] = 0;
			  
		/*��һ�������ļ����ж����ƶ���������ͳ�Ƹ����ֽڵ�Ȩֵ*/
		//���ļ�������	
		fstream ZipFile1(argv[1],ios::in | ios::binary);
		cout << "Loading, please wait for a few seconds. " << endl;
		if(!ZipFile1)                                                 
		{
			//�������ļ�ʧ�ܣ������������Ϣ
			cerr << "Error!" << endl;
			Showhelp();
			return 1;
		} 
		//���ļ��ж����ַ���ֱ�������ļ�β�ٽ��� ,��ͳ���ַ�����
		while(ZipFile1.read((char*)&charin,sizeof(char)))
		{
			//�����ַ���ASCII�룬�����䰴��ASCII������Ӧ��Ȩֵ�����ڲ�  
			CByte = (int)charin;            					
			Array[CByte] += 1;
		} 
		ZipFile1.close(); 
		//ͳ���ַ��ĸ���
		cnt = 0; 
		count = 0;
		for(int i=0;i<256;i++)
		{
			count += Array[i]; 
			if(Array[i])
				cnt++;
		} 
		cout << "The number of non-zero elements is:" << cnt << endl ;
		cout << "The number of bytes is:" << count << endl;
				
		/*�ڶ���,���������������������������*/ 
		//������������,�������������� 
		CreateHuffmanTree(t,256,Array);
		HCode *hc;
		hc = new HCode[256];
		HuffmanCoding(t,hc); 
		
		//���������Ƚ���Ҫ�õ����ļ�����д��һЩ��Ҫ���� 
		char str[100];
		strcpy(str,"zipresultof");
		strcat(str,argv[1]);
		strcat(str,".txt");
		fstream ZipResult(str, ios::out | ios::binary);
		if(!ZipResult)
		{
			//���д����ʧ�ܣ����������Ϣ 
			cerr << "Error!" << endl;
			return 1; 
		}	
		//������һЩѹ����Ҫ�Ĳ���,��ԭ�ļ����ֽڸ����Լ�ԭ�ļ������� 
		length = strlen(argv[1]);
		ZipResult.write((char*)&count,sizeof(int));
		//length�����ر�󣬹ʿ���charд���ļ� 
		ZipResult.write((char*)&length,sizeof(unsigned char)); 
		for(int i=0;i<length;i++)
		{
			ZipResult.write(argv[1]+i,1); 
		}  
		//��������������,ֻ��Ҫ�����ļ��ĸ����±꼴��,�Ҹ��ڵ��޸��ڵ㣬��ֱ�Ӳ�д���ڵ���±� 
		for(int i=0;i<2*t.leafnumber-2;i++)
		{ 
			int temp = t.nodes[i].parent - 256;
			//cout << temp << endl;
			ZipResult.write((char*)&temp,sizeof(unsigned char));
		}
		
		/*���Ĳ����ٴζ����ļ��������ݶ�����ֽڽ������Ӧ�Ĺ���������д�뵽һ���µ��ļ���*/
		//�ٴζ����ļ�
		fstream ZipFile2(argv[1], ios::in | ios::binary);
		if(!ZipFile2)                                                 
		{
			//�������ļ�ʧ�ܣ������������Ϣ
			cerr << "Error!" << endl;
			Showhelp();
			return 1;
		} 
		//�����ַ���ͬʱ���õ���ѹ�������λ������ļ���ȥ 
    	BitOStream bstr;
    	Init(bstr, &ZipResult);   
		for(int i=0;i<count;i++)
		{
			ZipFile2.read((char*)&charin,sizeof(char));
			CByte = (int)charin;
			//����Ӧ���������밴���ֽ�д���ļ��в��滻ԭ�ֽ� 
    		std::string st;
    		st = hc[CByte];
    		for(char ch:st)
    		{
        		Put(bstr, ch == '1');
    		}
		} 
		//������� 
    	Over(bstr);		
						
		//ѹ���ɹ�����������Դ�ļ������ļ�ͬʱ�ر� 
		delete t.nodes;
		delete hc;
		ZipResult.close();
		ZipFile2.close();
		cout << "Zip succeeded!" << endl;		
	} 

	//���н�ѹ���� 
	else
	{
		std::string s;                                                // ��ѹ��������Ҫ�õ����ַ��� 
		//flag=1,����ʱ��Ҫ���ļ����н�ѹ 
		cout << "Loading, please wait for a few seconds. " << endl;
		
		/*��һ�����������ֽڷ�ʽ������Ҫ��ѹ���ļ�*/ 
		fstream Infile(argv[1],ios::in | ios::binary);
		//�����ȡʧ�ܣ���ôֱ������Ļ�����������ϢError 
		if(!Infile)
		{
			cerr << "Error!" << endl;
			return 1;
		}
		
		/*�ڶ�������ȡ������Ϣ*/	
		//��ȡ�����������ַ����� 
		Infile.read((char*)&count,sizeof(int));
		cout << "The number of bytes from sourcefile is:" << count << endl; 
		//���ݶ�����ļ�ͷ�����ļ����ļ���
		char *Name;
		length = 0; 
		Infile.read((char*)&length,sizeof(unsigned char));
		Name = new char[length+1];
		for(int i=0;i<length;i++)
			Infile.read(Name+i,sizeof(char));
		Name[length] = '\0';	
		cout << "Name length is " << length << ", Name is " << Name << endl;
			
		/*��������������������*/	
		//�߶��߽�����������
		t.leafnumber = 256;	
		t.nodes = new HNode[511];
		//�ȶԹ����������г�ʼ�� 
		for(int i=0;i<2*t.leafnumber-1;i++)
		{
			t.nodes[i].info = -1;
			t.nodes[i].weight = -1;
			t.nodes[i].lchild = -1;
			t.nodes[i].rchild = -1;
			t.nodes[i].parent = -1;
		}
		for(int i=0;i<2*t.leafnumber-2;i++)
		{
			int par = 0;
			Infile.read((char*)&par,sizeof(unsigned char));
			t.nodes[i].info = i;
			//����ѹ��ʱ�����ʽ�����ļ�ͷ����ԭ�������� 
			t.nodes[i].parent = par+256;
			par += 256;
			//cout << par << endl;
			
			// ֤���ѵ�����ڵ㣬ֹͣ�������� 
			if(par < 0)
			continue;
			
			if(t.nodes[par].lchild == -1)
				t.nodes[par].lchild = i;
			else
				t.nodes[par].rchild = i;	 
		}
		//�����ù�������֮�󣬽�����������
		 
		//���Ĳ��������ļ��Ľ�ѹ"
		cout << "��ʼ������ļ�:" << Name << endl;
		fstream Unzip(Name,ios::out | ios::binary);
		if(!Unzip)
		{
			cerr << "Error!" << endl;
			return 1;
		} 
		//��ʼqָ����ڵ㣬��Ȩֵ���Ľڵ�
		int q;
		q = 2*t.leafnumber - 2;
		int cnt = count; 
		//����ֽڶԱ�����ж���, ͬʱ�Թ����������б�������������Ҷ�ڵ�ʱ�������ӦȨֵ�����ظ��ڵ�
		cout << "ѹ�����̵Ķ���ʱ�䣺" << count << endl;
		cout << "��ѹ���̣�" << endl;  
		while(cnt > 0) 
		{
			Infile.read((char*)&charin,sizeof(char));
			std::string s;
			s = change(charin);
			//cout << s << endl;
			cout << count-cnt << "\r";
			for(int j=0;j<s.length();)
			{
				if(t.nodes[q].rchild == -1 && t.nodes[q].lchild == -1)
				{
				// ��ʾ��ʱ�ѵ���Ҷ�ڵ�,���ѻ�ȡ��һ���ֽڵ��������������룬�����ֽ��������ѹ���ļ���
					char charout; 
					charout = (char)t.nodes[q].info;
					Unzip.write((char*)&charout,sizeof(char));
					cnt--;        //д��һ���ַ�����������1 
					if(cnt == 0)
					break;                                   
					q = 2*t.leafnumber - 2;					
					continue;
				}	
			 
				if(s[j] == '0')
					q = t.nodes[q].lchild;
				else if(s[j] == '1')
					q = t.nodes[q].rchild;
				
				j++;		
			}
		} 
		Unzip.close(); 
		//�����Ϣ��ʾ��ѹ�ɹ�	 
		cout << endl << "Unzip succeeded!" << endl;
	}
	
	finish=clock();
    totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
   	// cout<<"The running time of the program is "<<totaltime<<" seconds��"<<endl;
	return 0;
}

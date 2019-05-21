#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "Huffman.h"
#include "Bitstream.h"
using namespace std;

//输出本压缩程序的错误提示 
int Showhelp()
{
	cout << "Wrong instruction!" << endl;
	cout << "Usage test : a file name, zip or unzip" << endl;
	cout << "Example : test.txt zip" << endl;
	return 0;
} 

//在解压时进行按位与操作，之后将文件写入 
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
	//测试时假设不使用命令行参数进行调试 
	/*char chara[100];
	int flag;
	cin >> chara >> flag;
	argc = 3;
	argv[1] = chara;*/

	//输入结束之后开始正式计时 
	clock_t start,finish;
    double totaltime;
    start=clock();
    
	//使用命令行参数 
	if(argc != 3)
	{
		Showhelp();
		return 1; 
	} 
	
	int flag;                                                  //表示操作种类的变量 ,0为压缩，1为解压 
	if(!strcmp(argv[2],"zip"))
		flag = 0;	
	else if(!strcmp(argv[2],"unzip"))
		flag = 1;	
	
	
	//设置一些操作时的必要的全局变量	
	unsigned char charin,charout;                              //表示需要读入与读出的字节 
	int count;                                                 //表示文件的字节数 
	int length;                                                //用来表示文件名的长度 	 
	HuffmanTree t;                                             //哈夫曼树 
	
	//进行压缩操作 
	if(!flag)                                                       
	{
		//flag = 0, 即此时需要对目标文件进行压缩
		int cnt;                                                   //表示非零元的个数 
		int Array[256];                                            //用来存放各字符权值的数组
		int CByte;	  											   //转换的中间变量
			
		//对array数组进行初始化
		for(int i=0;i<256;i++)
			Array[i] = 0;
			  
		/*第一步，对文件进行二进制读操作，并统计各个字节的权值*/
		//打开文件并读入	
		fstream ZipFile1(argv[1],ios::in | ios::binary);
		cout << "Loading, please wait for a few seconds. " << endl;
		if(!ZipFile1)                                                 
		{
			//若读入文件失败，则输出错误信息
			cerr << "Error!" << endl;
			Showhelp();
			return 1;
		} 
		//从文件中读入字符，直到读到文件尾再结束 ,并统计字符个数
		while(ZipFile1.read((char*)&charin,sizeof(char)))
		{
			//读入字符的ASCII码，并将其按照ASCII码存进对应的权值数组内部  
			CByte = (int)charin;            					
			Array[CByte] += 1;
		} 
		ZipFile1.close(); 
		//统计字符的个数
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
				
		/*第二步,建立哈夫曼树并求出哈夫曼编码*/ 
		//创建哈夫曼树,并求解哈夫曼编码 
		CreateHuffmanTree(t,256,Array);
		HCode *hc;
		hc = new HCode[256];
		HuffmanCoding(t,hc); 
		
		//第三步，先建立要得到的文件，并写入一些必要参数 
		char str[100];
		strcpy(str,"zipresultof");
		strcat(str,argv[1]);
		strcat(str,".txt");
		fstream ZipResult(str, ios::out | ios::binary);
		if(!ZipResult)
		{
			//如果写操作失败，输出错误信息 
			cerr << "Error!" << endl;
			return 1; 
		}	
		//先输入一些压缩必要的参数,如原文件的字节个数以及原文件的名称 
		length = strlen(argv[1]);
		ZipResult.write((char*)&count,sizeof(int));
		//length不会特别大，故可用char写入文件 
		ZipResult.write((char*)&length,sizeof(unsigned char)); 
		for(int i=0;i<length;i++)
		{
			ZipResult.write(argv[1]+i,1); 
		}  
		//将哈夫曼树输入,只需要输入文件的父链下标即可,且父节点无根节点，可直接不写父节点的下标 
		for(int i=0;i<2*t.leafnumber-2;i++)
		{ 
			int temp = t.nodes[i].parent - 256;
			//cout << temp << endl;
			ZipResult.write((char*)&temp,sizeof(unsigned char));
		}
		
		/*第四步，再次读入文件，并根据读入的字节将得其对应的哈夫曼编码写入到一个新的文件中*/
		//再次读入文件
		fstream ZipFile2(argv[1], ios::in | ios::binary);
		if(!ZipFile2)                                                 
		{
			//若读入文件失败，则输出错误信息
			cerr << "Error!" << endl;
			Showhelp();
			return 1;
		} 
		//读入字符，同时将得到的压缩结果按位输出到文件中去 
    	BitOStream bstr;
    	Init(bstr, &ZipResult);   
		for(int i=0;i<count;i++)
		{
			ZipFile2.read((char*)&charin,sizeof(char));
			CByte = (int)charin;
			//将对应哈夫曼编码按照字节写入文件中并替换原字节 
    		std::string st;
    		st = hc[CByte];
    		for(char ch:st)
    		{
        		Put(bstr, ch == '1');
    		}
		} 
		//输入结束 
    	Over(bstr);		
						
		//压缩成功，将操作的源文件与结果文件同时关闭 
		delete t.nodes;
		delete hc;
		ZipResult.close();
		ZipFile2.close();
		cout << "Zip succeeded!" << endl;		
	} 

	//进行解压操作 
	else
	{
		std::string s;                                                // 解压过程中需要用到的字符串 
		//flag=1,即此时需要对文件进行解压 
		cout << "Loading, please wait for a few seconds. " << endl;
		
		/*第一步，按照逐字节方式读入需要解压的文件*/ 
		fstream Infile(argv[1],ios::in | ios::binary);
		//如果读取失败，那么直接在屏幕上输出错误信息Error 
		if(!Infile)
		{
			cerr << "Error!" << endl;
			return 1;
		}
		
		/*第二步，读取配置信息*/	
		//读取哈夫曼树的字符个数 
		Infile.read((char*)&count,sizeof(int));
		cout << "The number of bytes from sourcefile is:" << count << endl; 
		//根据读入的文件头读入文件的文件名
		char *Name;
		length = 0; 
		Infile.read((char*)&length,sizeof(unsigned char));
		Name = new char[length+1];
		for(int i=0;i<length;i++)
			Infile.read(Name+i,sizeof(char));
		Name[length] = '\0';	
		cout << "Name length is " << length << ", Name is " << Name << endl;
			
		/*第三步，建立哈夫曼树*/	
		//边读边建立哈夫曼树
		t.leafnumber = 256;	
		t.nodes = new HNode[511];
		//先对哈夫曼树进行初始化 
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
			//按照压缩时候的形式读入文件头并还原哈夫曼树 
			t.nodes[i].parent = par+256;
			par += 256;
			//cout << par << endl;
			
			// 证明已到达根节点，停止后续操作 
			if(par < 0)
			continue;
			
			if(t.nodes[par].lchild == -1)
				t.nodes[par].lchild = i;
			else
				t.nodes[par].rchild = i;	 
		}
		//建立好哈夫曼树之后，进行其他操作
		 
		//第四步，进行文件的解压"
		cout << "初始化输出文件:" << Name << endl;
		fstream Unzip(Name,ios::out | ios::binary);
		if(!Unzip)
		{
			cerr << "Error!" << endl;
			return 1;
		} 
		//开始q指向根节点，即权值最大的节点
		int q;
		q = 2*t.leafnumber - 2;
		int cnt = count; 
		//逐个字节对编码进行读入, 同时对哈夫曼树进行遍历，当遍历至叶节点时，输出对应权值，返回根节点
		cout << "压缩过程的对照时间：" << count << endl;
		cout << "解压进程：" << endl;  
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
				// 表示此时已到达叶节点,即已获取了一个字节的完整哈夫曼编码，将该字节输出到解压的文件中
					char charout; 
					charout = (char)t.nodes[q].info;
					Unzip.write((char*)&charout,sizeof(char));
					cnt--;        //写入一个字符，计数器减1 
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
		//输出信息表示解压成功	 
		cout << endl << "Unzip succeeded!" << endl;
	}
	
	finish=clock();
    totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
   	cout<<"The running time of the program is "<<totaltime<<" seconds！"<<endl;
	return 0;
}

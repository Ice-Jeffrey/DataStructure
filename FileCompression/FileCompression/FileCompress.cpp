#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "huffman.hpp"
#include "bitstream.hpp"
using namespace std;

// 显示帮助告诉用户如何使用
int Showhelp() {
	cout << "Wrong instruction!" << endl;
	cout << "Usage test: a file name, zip or unzip" << endl;
	cout << "Example: test.txt zip" << endl;
	return 0;
} 

// 将一个字符按位转化成字符串
string change(char c) {
	string data;
	for(int i=0;i<8;i++) {
		if( ((c>>i) & 0x01) == 1 )
			data += '1';
		else
			data += '0';	
	} 

	// 将得到结果逆序即为原char对应字符串
	string s(data.rbegin(), data.rend());
	return s;
}

// 主函数
int main(int argc, char* argv[]) {
	// 对命令行参数进行校验
	if(argc != 3) {
		Showhelp();
		return 1; 
	} 
	
	// 判断是压缩还是解压
	int flag;                                     
	if(!strcmp(argv[2], "zip"))
		flag = 0;	
	else if(!strcmp(argv[2], "unzip"))
		flag = 1;
	else
		Showhelp();	

	// 获取运行时间
	clock_t start, finish;
    double totaltime;
    start = clock();	

	// 文件压缩
	if(!flag) {
		unsigned char charin, charout;
		int count;	// 文件中所有字符的数量
		int length;
		HuffmanTree t;

		int Array[256];	// 对256个字符的频率进行统计
			
		for(int i=0;i<256;i++)
			Array[i] = 0;
			  
		fstream ZipFile1(argv[1], ios::in | ios::binary);
		cout << "Loading......" << endl;
		if(!ZipFile1) {
			cerr << "Error! Target file not found!" << endl;
			return 1;
		} 

		// 按字节读入数据
		count = 0;
		while(ZipFile1.read((char*)&charin, sizeof(char))) {
			int CByte;
			CByte = (int)charin;        					
			Array[CByte] += 1;
			count++;
		} 

		// 读入结束后关闭文件流
		ZipFile1.close(); 
		// cout << "The number of bytes is: " << count << endl;
				
		// 建立哈夫曼树
		CreateHuffmanTree(t, 256, Array);
		// 得到各个字符的哈夫曼编码
		HCode *hc;
		hc = new HCode[256];
		HuffmanCoding(t, hc); 
		
		char str[100];
		strcpy(str, argv[1]);
		strcat(str,".huf");
		fstream ZipResult(str, ios::out | ios::binary);
		if(!ZipResult) {
			cerr << "Error!" << endl;
			return 1; 
		}

		// 在压缩文件头之前写入文件头
		ZipResult.write((char*)&count, sizeof(int));
		length = strlen(argv[1]);
		ZipResult.write((char*)&length, sizeof(unsigned char)); 
		for(int i = 0; i < length; i++)
			ZipResult.write(argv[1] + i, sizeof(char));
		// 将每个字符对应的父节点写入文件 
		for(int i = 0; i < 2 * t.leafnumber - 2; i++) { 
			int temp = t.nodes[i].parent - 256;
			ZipResult.write((char*)&temp, sizeof(unsigned char));
		}
		
		// 再将要压缩的目标文件读入
		fstream ZipFile2(argv[1], ios::in | ios::binary);
		// 从目标文件中逐字节读入字符，将读入字符的哈夫曼编码转化为bit写入到文件中
    	BitOStream bstr;
    	Init(bstr, &ZipResult);   
		for(int i = 0; i < count; i++) {
			ZipFile2.read((char*)&charin, sizeof(char));
			int CByte = (int)charin;
			// 得到哈夫曼编码
    		std::string st;
    		st = hc[CByte];
    		for(char ch: st) {
        		Put(bstr, ch == '1');
    		}
		} 
    	Over(bstr);		
						
		delete t.nodes;
		delete hc;
		ZipResult.close();
		ZipFile2.close();
		cout << "Zip succeeded!" << endl;		
	} 

	else {
		unsigned char charin, charout;
		int count;	// 文件中所有字符的数量
		int length;
		HuffmanTree t;

		std::string s;
		cout << "Loading......" << endl;
		
		fstream Infile(argv[1], ios::in | ios::binary);
		if(!Infile) {
			cerr << "Error! Target file not found!" << endl;
			return 1;
		}

		// 读入文件中字符的数目
		Infile.read((char*)&count, sizeof(int));	
		// 读入文件名 
		char *Name;
		length = 0; 
		Infile.read((char*)&length, sizeof(unsigned char));
		Name = new char[length+1];
		for(int i = 0; i < length; i++)
			Infile.read(Name+i, sizeof(char));
		Name[length] = '\0';	
		cout << "Source File: " << Name << endl;
			
		// 根据压缩文件中各个字符的父节点还原哈夫曼树
		t.leafnumber = 256;	
		t.nodes = new HNode[2 * t.leafnumber - 1];
		for(int i = 0; i < 2 * t.leafnumber - 1; i++) {
			t.nodes[i].info = -1;
			t.nodes[i].weight = -1;
			t.nodes[i].lchild = -1;
			t.nodes[i].rchild = -1;
			t.nodes[i].parent = -1;
		}

		for(int i = 0; i < 2 * t.leafnumber - 2; i++) {
			int par = 0;
			Infile.read((char*)&par, sizeof(unsigned char));
			t.nodes[i].info = i;
			t.nodes[i].parent = par + 256;
			par += 256;
			
			if(par < 0)
				continue;
			
			if(t.nodes[par].lchild == -1)
				t.nodes[par].lchild = i;
			else
				t.nodes[par].rchild = i;	 
		}
		 
		fstream Unzip(Name, ios::out | ios::binary);
		int q;
		q = 2 * t.leafnumber - 2;
		int cnt = count; 
		while(cnt > 0) {
			Infile.read((char*)&charin, sizeof(unsigned char));
			std::string s;
			s = change(charin);
			int j = 0;
			do {
				if(t.nodes[q].rchild == -1 && t.nodes[q].lchild == -1) {
					char charout; 
					charout = (char)t.nodes[q].info;
					Unzip.write((char*)&charout, sizeof(char));
					cnt--;
					if(cnt == 0)
						break;                                   
					q = 2 * t.leafnumber - 2;					
					continue;
				}	
			 
				if(s[j] == '0')
					q = t.nodes[q].lchild;
				else if(s[j] == '1')
					q = t.nodes[q].rchild;

				j++;	
			} while(j < s.length());
		} 
		Unzip.close(); 
		cout << endl << "Unzip succeeded!" << endl;

		delete t.nodes;
	}
	
	finish = clock();
    totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
   	cout << "The running time of the program is " << totaltime << " seconds." << endl;
	return 0;
}

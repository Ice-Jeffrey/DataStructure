#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include "Huffman.h"
using namespace std;

bool cmp1(HuffmanTree::HNode &node1, HuffmanTree::HNode &node2) {
    return node1.weight < node2.weight;
}

bool cmp2(HuffmanTree::HNode &node1, HuffmanTree::HNode &node2) {
    return node1.info < node2.info;
}

HuffmanTree::HuffmanTree(int leafnumber, int* weights) {
    //申请内存空间来存放哈夫曼树的各个根节点以及叶节点
	nodes = new HuffmanTree::HNode[2 * leafnumber - 1];
	leafnumber = leafnumber;

    //初始化各个叶节点,读入其权值并初始化其父节点、左子节点及右子节点，设初始化的初始值为-1
	for(int i = 0; i < leafnumber; i++) {
        nodes[i].weight = weights[i];
		nodes[i].parent = -1;
        nodes[i].lchild = -1;
        nodes[i].rchild = -1;
        nodes[i].info = i;                    //info为其在原数组中的位置信息 
    }
    
    //对后续未出现但需要的节点进行初始化 
    for(int i = leafnumber; i < 2 * leafnumber - 1; i++) {
    	nodes[i].parent = -1;
    	nodes[i].lchild = -1;
    	nodes[i].rchild = -1;
    	nodes[i].info = -1;
	}

    // 按照权值对结点进行排序
    sort(nodes, nodes+leafnumber, cmp1);

    /*
        在已知节点中寻找最小的两个节点合并，
        新的哈夫曼树的根节点的权值为两叶节点权值之和。
        由于由n棵树合并为1棵树，则共需要合并leafnumber-1次
    */
    int m = 0, n = leafnumber;
    for(int i = 0; i < leafnumber - 1; i++) {
        int min, k1 = 0, k2 = 0;

        //找到两个parent仍为-1且权值最小的叶节点 
		if (m < leafnumber && (n >= leafnumber + i || nodes[m].weight <= nodes[n].weight))
	        k1 = m++;
        else
	        k1 = n++;

	    if (m < leafnumber && (n >= leafnumber + i || nodes[m].weight <= nodes[n].weight))
	        k2 = m++;
        else
	        k2 = n++;
        //cout << k1 << " " << k2 << " " << t.nodes[k1].info << " " << t.nodes[k2].info << endl;
        //在测试时输出模拟整个过程

        nodes[i+leafnumber].info = i+leafnumber;
        nodes[i+leafnumber].weight = nodes[k1].weight + nodes[k2].weight;
        nodes[i+leafnumber].parent = -1;
        nodes[k1].parent = i+leafnumber;
        nodes[k2].parent = i+leafnumber;
        
        // 构造子树，约定下标小的为左子，下标大的为右子 
        if(nodes[k1].info < nodes[k2].info) {
        	nodes[i+leafnumber].lchild = nodes[k1].info;
            nodes[i+leafnumber].rchild = nodes[k2].info;
		}
        else {
        	nodes[i+leafnumber].lchild = nodes[k2].info;
        	nodes[i+leafnumber].rchild = nodes[k1].info;
		}
	}
}

HCode* HuffmanTree::getHuffmanCodes() {
    // 设从根节点开始，向左为0，向右为1
    // 为每个节点代表的字符串申请对应的编码空间
	char *cd;
	cd = new char[100];
    HuffmanCodes = new HCode[leafnumber];

    // 根据输入顺序对哈夫曼树各结点进行排序
    sort(nodes, nodes + leafnumber, cmp2);

    // 开始求各个节点的哈夫曼编码 
    int m = 0; 
	for(int i = 0; i < leafnumber; i++) {
		int start = leafnumber - 1;
		cd[start] = '\0';
		HuffmanCodes[i] = new char[100];
		
		int c,flag;
		int f;
		//从叶节点到根节点逆向求哈夫曼编码
		for(c = i, f = nodes[i].parent; f != -1; c = f, f = nodes[f].parent)
		{
			if (nodes[f].rchild == c)
			    cd[--start] = '1';
			else
			    cd[--start] = '0';
		}

		//将得到的哈夫曼编码存到结构体中 
		int len = leafnumber - start;
		strcpy(HuffmanCodes[i], &cd[start]);
	}
    
    return HuffmanCodes;
}

int HuffmanTree::getLeafnumber() {
    return leafnumber;
}
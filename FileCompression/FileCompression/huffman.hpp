#include <cstring>
#include <string>
#include <algorithm>
typedef int Elemtype;
typedef char* HCode; 

// 对哈夫曼树结点的定义
struct HuffmanNode {
	Elemtype weight;	// 当前结点的权值
	int parent, lchild, rchild, info;	// 当前结点的父节点、子节点等其他信息
};
typedef HuffmanNode HNode;

// 使用静态链表实现哈夫曼树
struct HuffmanTree {
	HNode *nodes;
	int leafnumber;
};
typedef HuffmanTree HTree;

// 重载sort函数的函数
bool cmp1(HNode a, HNode b) {
	return a.weight < b.weight;
}

bool cmp2(HNode a, HNode b) {
	return a.info < b.info;
}

// 构建哈夫曼树
int CreateHuffmanTree(HTree &t, int num, int* weights) {
	// 进行错误检查
	if(num <= 0 || weights == nullptr)
		return 0;

	int leafnumber = num;
	// 进行初始化
	t.nodes = new HNode[2 * leafnumber - 1];
	t.leafnumber = leafnumber;

	for(int i = 0; i < leafnumber; i++) {
		t.nodes[i].weight = weights[i];
		t.nodes[i].parent = -1;
		t.nodes[i].lchild = -1;
		t.nodes[i].rchild = -1;
		t.nodes[i].info = i;	// info与当前结点代表的字符相对应
    }
    
    for(int i = leafnumber; i < 2 * leafnumber - 1; i++) {
    	t.nodes[i].parent = -1;
    	t.nodes[i].lchild = -1;
    	t.nodes[i].rchild = -1;
    	t.nodes[i].info = -1;
	}

    // 根据权值对叶结点进行排序
	std::sort(t.nodes, t.nodes + leafnumber, cmp1);

    // 开始建立哈夫曼树
    int m = 0, n = leafnumber;	// m, n为指向未遍历结点的两个指针
    for(int i = 0; i < leafnumber - 1; i++) {
        int min, k1 = 0, k2 = 0;	// k1, k2代表两个权值最小孤立结点的下标

		if (m < leafnumber && (n >= leafnumber + i || t.nodes[m].weight <= t.nodes[n].weight))
	        k1 = m++;
        else
	        k1 = n++;

	    if (m < leafnumber && (n >= leafnumber + i || t.nodes[m].weight <= t.nodes[n].weight))
	        k2 = m++;
        else
	        k2 = n++;

        t.nodes[i + leafnumber].info = i + leafnumber;
        t.nodes[i + leafnumber].weight = t.nodes[k1].weight + t.nodes[k2].weight;
        t.nodes[i + leafnumber].parent = -1;
        t.nodes[k1].parent = i + leafnumber;
        t.nodes[k2].parent = i + leafnumber;

        // 规定左子结点序号小于右子结点序号
        if(t.nodes[k1].info < t.nodes[k2].info) {
        	t.nodes[i+leafnumber].lchild = t.nodes[k1].info;
            t.nodes[i+leafnumber].rchild = t.nodes[k2].info;
		}
        else {
        	t.nodes[i+leafnumber].lchild = t.nodes[k2].info;
        	t.nodes[i+leafnumber].rchild = t.nodes[k1].info;
		}
	}
    return 1;
}

// 根据哈夫曼树自底向上得到各个结点哈夫曼编码
int HuffmanCoding(HTree &t, HCode *hc) {
	int leafnumber = t.leafnumber;
	char *cd;
	cd = new char[1000];

	// 根据info进行排序
    std::sort(t.nodes, t.nodes + leafnumber, cmp2);

    // 自底向上得到哈夫曼编码
    int m=0; 
	for(int i = 0; i < leafnumber; i++) {
		int start = leafnumber - 1;
		cd[start] = '\0';
		hc[i] = new char[100];
		
		int c, flag;
		int f;
		for(c = i, f = t.nodes[i].parent; f != -1; c = f, f = t.nodes[f].parent) {
			if(t.nodes[f].rchild == c)
				cd[--start] = '1';
			else
				cd[--start] = '0';
		}
 
		strcpy(hc[i], &cd[start]);
	}
	return 0;
}


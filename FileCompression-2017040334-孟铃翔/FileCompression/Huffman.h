#include<cstring>
#include<string>
typedef int Elemtype;
typedef char* HCode; 

struct HuffmanNode
{
	Elemtype weight;                    //节点的权值
	int parent,lchild,rchild,info;      //节点的序号，父节点、左子及右子的序号
};
typedef HuffmanNode HNode;

struct HuffmanTree
{
	HNode *nodes;
	int leafnumber;
};
typedef HuffmanTree HTree;

//用于构造哈夫曼树的函数
int CreateHuffmanTree(HTree &t,int leafnumber,int* weights)
{
	//申请内存空间来存放哈夫曼树的各个根节点以及叶节点
	t.nodes = new HNode[2*leafnumber-1];
	t.leafnumber = leafnumber;

    //初始化各个叶节点,读入其权值并初始化其父节点、左子节点及右子节点，设初始化的初始值为-1
	for(int i=0;i<leafnumber;i++)
    {
        t.nodes[i].weight = weights[i];
		t.nodes[i].parent = -1;
        t.nodes[i].lchild = -1;
        t.nodes[i].rchild = -1;
        t.nodes[i].info = i;                    //info为其在原数组中的位置信息 
    }
    
    //对后续未出现但需要的节点进行初始化 
    for(int i=leafnumber;i<2*leafnumber-1;i++)
    {
    	t.nodes[i].parent = -1;
    	t.nodes[i].lchild = -1;
    	t.nodes[i].rchild = -1;
    	t.nodes[i].info = -1;
	}

    //对t代表的数据进行冒泡排序，但不改变原来的weights数组 
	for(int i=0;i<leafnumber-1;i++)
	{
		for(int j=0;j<leafnumber-1-i;j++)
		{
			HNode temp;                                         //temp为一临时的中间变量 
			if(t.nodes[j].weight > t.nodes[j+1].weight)
			{
				temp.info = t.nodes[j].info;
				temp.weight = t.nodes[j].weight;
				t.nodes[j].info = t.nodes[j+1].info;
				t.nodes[j].weight = t.nodes[j+1].weight;
				t.nodes[j+1].info = temp.info;
				t.nodes[j+1].weight = temp.weight;
			}
		}
	}

    /*在已知节点中寻找最小的两个节点合并，
    新的哈夫曼树的根节点的权值为两叶节点权值之和。
    由于由n棵树合并为1棵树，则共需要合并leafnumber-1次*/
    int m=0,n=leafnumber;
    for(int i=0;i<leafnumber-1;i++)
    {
        int min,k1=0,k2=0;

        //找到两个parent仍为-1且权值最小的叶节点 
		if (m < leafnumber && (n >= leafnumber + i || t.nodes[m].weight <= t.nodes[n].weight))
	        k1 = m++;
        else
	        k1 = n++;

	    if (m < leafnumber && (n >= leafnumber + i || t.nodes[m].weight <= t.nodes[n].weight))
	        k2 = m++;
        else
	        k2 = n++;
        //cout << k1 << " " << k2 << " " << t.nodes[k1].info << " " << t.nodes[k2].info << endl;
        //在测试时输出模拟整个过程

        t.nodes[i+leafnumber].info = i+leafnumber;
        t.nodes[i+leafnumber].weight = t.nodes[k1].weight + t.nodes[k2].weight;
        t.nodes[i+leafnumber].parent = -1;
        t.nodes[k1].parent = i+leafnumber;
        t.nodes[k2].parent = i+leafnumber;
        //约定下标小的为左子，下标大的为右子 
        if(t.nodes[k1].info < t.nodes[k2].info)
        {
        	t.nodes[i+leafnumber].lchild = t.nodes[k1].info;
            t.nodes[i+leafnumber].rchild = t.nodes[k2].info;
		}
        else
        {
        	t.nodes[i+leafnumber].lchild = t.nodes[k2].info;
        	t.nodes[i+leafnumber].rchild = t.nodes[k1].info;
		}
	}
    return 1;
}

//通过二叉树构造哈夫曼编码，即不等长编码，设向左为0，向右为1，ht为每个节点哈夫曼编码字符串的指向向量 
int HuffmanCoding(HTree &t,HCode *hc)
{
	int leafnumber = t.leafnumber;
	
	//为每个节点代表的字符串申请对应的编码空间
	char *cd;
	cd = new char[100];

    //按照输入的顺序对哈夫曼树的储存空间的各结点进行排序，依次与weights数组相对应 
	for(int i=0;i<leafnumber;i++)
	{
		for(int j=0;j<leafnumber-i-1;j++)
		{
			HNode temp;
			if(t.nodes[j].info > t.nodes[j+1].info)
			{
				temp.info = t.nodes[j].info;
				temp.weight = t.nodes[j].weight;
				temp.parent = t.nodes[j].parent;
				temp.lchild = t.nodes[j].lchild;
				temp.rchild = t.nodes[j].rchild;
				t.nodes[j].info = t.nodes[j+1].info;
				t.nodes[j].weight = t.nodes[j+1].weight;
				t.nodes[j].rchild = t.nodes[j+1].rchild;
				t.nodes[j].parent = t.nodes[j+1].parent;
				t.nodes[j].lchild = t.nodes[j+1].rchild;
				t.nodes[j+1].info = temp.info;
				t.nodes[j+1].weight = temp.weight;
				t.nodes[j+1].parent = temp.parent;
				t.nodes[j+1].lchild = temp.lchild;
				t.nodes[j+1].rchild = temp.rchild;
			}
		}
	}

    //开始求各个节点的哈夫曼编码 
    int m=0; 
	for(int i=0;i<leafnumber;i++)
	{
		int start = leafnumber-1;
		cd[start] = '\0';
		hc[i] = new char[100];
		
		int c,flag;
		int f;
		//从叶节点到根节点逆向求哈夫曼编码
		for(c=i,f=t.nodes[i].parent;f!=-1;c=f,f=t.nodes[f].parent)
		{
			if(t.nodes[f].rchild == c)
			 cd[--start] = '1';
			else
			 cd[--start] = '0';
		}

		//将得到的哈夫曼编码存到结构体中 
		int len = leafnumber - start;
		strcpy(hc[i],&cd[start]);
	}
	return 0;
}


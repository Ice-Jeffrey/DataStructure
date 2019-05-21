#include<cstring>
#include<string>
typedef int Elemtype;
typedef char* HCode; 

struct HuffmanNode
{
	Elemtype weight;                    //�ڵ��Ȩֵ
	int parent,lchild,rchild,info;      //�ڵ����ţ����ڵ㡢���Ӽ����ӵ����
};
typedef HuffmanNode HNode;

struct HuffmanTree
{
	HNode *nodes;
	int leafnumber;
};
typedef HuffmanTree HTree;

//���ڹ�����������ĺ���
int CreateHuffmanTree(HTree &t,int leafnumber,int* weights)
{
	//�����ڴ�ռ�����Ź��������ĸ������ڵ��Լ�Ҷ�ڵ�
	t.nodes = new HNode[2*leafnumber-1];
	t.leafnumber = leafnumber;

    //��ʼ������Ҷ�ڵ�,������Ȩֵ����ʼ���丸�ڵ㡢���ӽڵ㼰���ӽڵ㣬���ʼ���ĳ�ʼֵΪ-1
	for(int i=0;i<leafnumber;i++)
    {
        t.nodes[i].weight = weights[i];
		t.nodes[i].parent = -1;
        t.nodes[i].lchild = -1;
        t.nodes[i].rchild = -1;
        t.nodes[i].info = i;                    //infoΪ����ԭ�����е�λ����Ϣ 
    }
    
    //�Ժ���δ���ֵ���Ҫ�Ľڵ���г�ʼ�� 
    for(int i=leafnumber;i<2*leafnumber-1;i++)
    {
    	t.nodes[i].parent = -1;
    	t.nodes[i].lchild = -1;
    	t.nodes[i].rchild = -1;
    	t.nodes[i].info = -1;
	}

    //��t��������ݽ���ð�����򣬵����ı�ԭ����weights���� 
	for(int i=0;i<leafnumber-1;i++)
	{
		for(int j=0;j<leafnumber-1-i;j++)
		{
			HNode temp;                                         //tempΪһ��ʱ���м���� 
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

    /*����֪�ڵ���Ѱ����С�������ڵ�ϲ���
    �µĹ��������ĸ��ڵ��ȨֵΪ��Ҷ�ڵ�Ȩֵ֮�͡�
    ������n�����ϲ�Ϊ1����������Ҫ�ϲ�leafnumber-1��*/
    int m=0,n=leafnumber;
    for(int i=0;i<leafnumber-1;i++)
    {
        int min,k1=0,k2=0;

        //�ҵ�����parent��Ϊ-1��Ȩֵ��С��Ҷ�ڵ� 
		if (m < leafnumber && (n >= leafnumber + i || t.nodes[m].weight <= t.nodes[n].weight))
	        k1 = m++;
        else
	        k1 = n++;

	    if (m < leafnumber && (n >= leafnumber + i || t.nodes[m].weight <= t.nodes[n].weight))
	        k2 = m++;
        else
	        k2 = n++;
        //cout << k1 << " " << k2 << " " << t.nodes[k1].info << " " << t.nodes[k2].info << endl;
        //�ڲ���ʱ���ģ����������

        t.nodes[i+leafnumber].info = i+leafnumber;
        t.nodes[i+leafnumber].weight = t.nodes[k1].weight + t.nodes[k2].weight;
        t.nodes[i+leafnumber].parent = -1;
        t.nodes[k1].parent = i+leafnumber;
        t.nodes[k2].parent = i+leafnumber;
        //Լ���±�С��Ϊ���ӣ��±���Ϊ���� 
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

//ͨ��������������������룬�����ȳ����룬������Ϊ0������Ϊ1��htΪÿ���ڵ�����������ַ�����ָ������ 
int HuffmanCoding(HTree &t,HCode *hc)
{
	int leafnumber = t.leafnumber;
	
	//Ϊÿ���ڵ������ַ��������Ӧ�ı���ռ�
	char *cd;
	cd = new char[100];

    //���������˳��Թ��������Ĵ���ռ�ĸ�����������������weights�������Ӧ 
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

    //��ʼ������ڵ�Ĺ��������� 
    int m=0; 
	for(int i=0;i<leafnumber;i++)
	{
		int start = leafnumber-1;
		cd[start] = '\0';
		hc[i] = new char[100];
		
		int c,flag;
		int f;
		//��Ҷ�ڵ㵽���ڵ����������������
		for(c=i,f=t.nodes[i].parent;f!=-1;c=f,f=t.nodes[f].parent)
		{
			if(t.nodes[f].rchild == c)
			 cd[--start] = '1';
			else
			 cd[--start] = '0';
		}

		//���õ��Ĺ���������浽�ṹ���� 
		int len = leafnumber - start;
		strcpy(hc[i],&cd[start]);
	}
	return 0;
}


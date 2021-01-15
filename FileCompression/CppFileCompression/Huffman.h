#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED

typedef int Elemtype;
typedef char* HCode;

class HuffmanTree {
	public:
		struct HuffmanNode {
			Elemtype weight;
			int parent,lchild,rchild,info;
		};
		typedef HuffmanNode HNode;
		HNode* nodes;
		HuffmanTree(int leafnumber, int* weights);
		HCode* getHuffmanCodes();
		int getLeafnumber();
	private:
		int leafnumber;
		HCode* HuffmanCodes;
};

#endif // HUFFMAN_H_INCLUDED
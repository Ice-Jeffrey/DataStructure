// ��ʾ������������д�������λ
//
#include <iostream>
#include <string>
using namespace std;

struct BitOStream
{
    ostream *ostr;      // ָ��Ŀ�������ʵ����Ϊ�˱���C++�﷨�Ĺ���ʹ�ã���������ָ�롣������������Ӧ���������ã��ڹ��캯�����г�ʼ����
    int bytebuffer;     // �ֽڻ��壬׼��д������λҪ��д���ֽڻ����У����ֽڻ�������8��bit�󣬲Ž�һ���ֽ�д��Ŀ������
    int bitmask;        // λд�������֣�׼��д��bytebuffer�Ķ�ӦλΪ1������λΪ0
};

// ��ʼ��ָ����λ�����
int Init(BitOStream &bstr, ostream *ostr)
{
    bstr.ostr = ostr;
    bstr.bytebuffer = 0;
    bstr.bitmask = 0x80; // �Ӹ�λ���λд�룬�����ֳ�ʼʱ�ֽ����λ��1
    return 0;
}

// ��λ�������д��һ��bit
int Put(BitOStream &bstr, int bit)
{
    if (bit)
        bstr.bytebuffer |= bstr.bitmask; // ��1д��bytebuffer
    bstr.bitmask >>= 1;  // ����λ���Ƶ���һλ
    if (bstr.bitmask == 0)
    {
        // ������һ���ֽڣ����ֽ�д��Ŀ�����У������ֽڻ����������
        bstr.ostr->put(bstr.bytebuffer);
        bstr.bytebuffer = 0;
        bstr.bitmask = 0x80;
    }
    return 0;
}

// ������λ�������д��
int Over(BitOStream &bstr)
{
    if (bstr.bitmask != 0x80)
    {
        // ��δ���������һ���ֽ�д��Ŀ������
        bstr.ostr->put(bstr.bytebuffer);
    }
    return 0;
}

/*��ʾ���ʹ��������ָ�ĺ���������� 
int main()
{
    BitOStream bstr;
    Init(bstr, &cout);   // �����ļ�������ʱ����cout�滻Ϊ�ļ���ʵ�����ɡ�
    
    // ��ʾ��λ�����д��һ��bit�Ĺ���
    string aBitString{"010000000100000101000010010000110100"};
    for(char ch:aBitString)
    {
        Put(bstr, ch == '1');
    }
    Over(bstr);
    
    return 0;
}*/

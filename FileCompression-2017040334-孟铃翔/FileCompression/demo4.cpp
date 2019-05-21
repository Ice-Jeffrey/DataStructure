// ��ʾ��β鿴�������ļ��е��ֽ�����
//
#include <iostream>
#include <iomanip>                //�õ�һЩ���������� 
#include <fstream>
#include <string>
#include <cstdlib> 
using namespace std;
// ��16�ֽ�Ϊһ�У����ָ�����ֽڿ�
int ShowBytes(char *aBytes, int aByteCount, int aOffset)
{
    // ����������
    cout << "         00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F" << endl;
    // ÿ16���ֽ����һ��
    for (int i = 0; i < aByteCount; i += 16)
    {
        // �������ʼƫ����
        cout << hex << setw(8) << setfill('0') << (aOffset + i) << ' '; 
		//setw()�����������Ĳ�����hex������ʮ�����Ʒ�ʽ�����setfill��ʾ�������λ������8λʱ���á�0������� 
		 
        // ���16���ֽ�
        for (int j = 0; j < 16 && i + j < aByteCount; j++)
        {
            cout << hex << setw(2) << setfill('0') << (unsigned int)(unsigned char)aBytes[i + j] << ' ';
        }
        cout << endl;
    }
    return 0;
}
int ShowHelp()
{
    cout << "Usage: demo04 filename [offset]" << endl;
    cout << "  Example: demo04 test.dat" << endl;
    cout << "  Example: demo04 test.dat 1024" << endl;
    return 0;
}
int main(int argc, char *argv[])
{
    // ���������в���
    if (argc != 2 && argc != 3)
    {
        ShowHelp();
        return 1;
    }
    // ��ȡ�����в�����Ŀ���ļ�������ȡλ�ã�
    char* aFileName = argv[1];
    int aOffset = argc == 3 ? atoi(argv[2]) : 0;
    // ���ļ�
    ifstream istr(aFileName, ios::in | ios::binary);
    if (!istr)
    {
        cerr << "Open file " << aFileName << " failure." << endl;
        return 2;
    }
    // ��λ��Ҫ��ȡ��λ�ã���Ѱ�Ҷ�ָ�� 
    istr.seekg(aOffset);
    // ��ȡ�ļ��е�ָ����С�ֽڿ�
    const int BUFFERSIZE = 320; // 20��*16��
    char aBytes[BUFFERSIZE];
    istr.read(aBytes, BUFFERSIZE);
    // ��ѯʵ�ʶ����ֽ���
    int aByteCount = istr.gcount();
    // �ļ���ʹ�����Ҫ�ر�
    istr.close();
    // ��ʾ������ֽڿ�
    ShowBytes(aBytes, aByteCount, aOffset);
    return 0;
} 

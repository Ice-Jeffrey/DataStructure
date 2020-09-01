#ifndef BITSTREAM_H_INCLUDED
#define BITSTREAM_H_INCLUDED

#include <iostream>
using namespace std;

class BitOStream
{
private:
    ostream *ostr;      // 指向目标输出流实例，为了避免C++语法的过多使用，这里用了指针。更合理的做法应该是用引用，在构造函数进行初始化。
    int bytebuffer;     // 字节缓冲，准备写入流的位要先写到字节缓冲中，当字节缓冲填满8个bit后，才将一个字节写入目标流中
    int bitmask;        // 位写入屏蔽字，准备写入bytebuffer的对应位为1，其他位为0
public:
    BitOStream(ostream *ostr);
    void Put(int bit);
    void Over();
};

#endif
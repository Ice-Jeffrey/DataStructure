// 实现哈夫曼编码按位写入的程序
#include <iostream>
#include <string>
using namespace std;

// 设置一个按位写入的结构体
struct BitOStream {
    ostream *ostr;
    int bytebuffer;
    int bitmask;
};

int Init(BitOStream &bstr, ostream *ostr) {
    bstr.ostr = ostr;
    bstr.bytebuffer = 0;
    bstr.bitmask = 0x80;
    return 0;
}

int Put(BitOStream &bstr, int bit) {
    if (bit)
        bstr.bytebuffer |= bstr.bitmask;
    bstr.bitmask >>= 1;
    if (bstr.bitmask == 0) {
        bstr.ostr->put(bstr.bytebuffer);
        bstr.bytebuffer = 0;
        bstr.bitmask = 0x80;
    }
    return 0;
}

int Over(BitOStream &bstr) {
    if (bstr.bitmask != 0x80) {
        bstr.ostr->put(bstr.bytebuffer);
    }
    return 0;
}
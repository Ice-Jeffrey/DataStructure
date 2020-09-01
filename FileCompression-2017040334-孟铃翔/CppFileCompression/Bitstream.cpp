#include <iostream>
#include "Bitstream.h"
using namespace std;

BitOStream::BitOStream(ostream *ostr) {
    ostr = ostr;
    bytebuffer = 0;
    bitmask = 0x80; // 从高位向低位写入，屏蔽字初始时字节最高位置1
}

void BitOStream::Put(int bit) {
    if (bit)
        bytebuffer |= bitmask; // 将1写入bytebuffer
    bitmask >>= 1;  // 屏蔽位右移到下一位
    
    if (bitmask == 0) {
        // 已填满一个字节，将字节写入目标流中，重置字节缓冲和屏蔽字
        ostr->put(bytebuffer);
        bytebuffer = 0;
        bitmask = 0x80;
    }
    return ;
}

void BitOStream::Over() {
    if (bitmask != 0x80) {
        // 将未填满的最后一个字节写入目标流中
        ostr->put(bytebuffer);
    }
    return ;
}
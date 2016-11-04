#pragma once
class sha256 {
public:
	unsigned int m_h[8];
	const static unsigned int sha256_k[];
	unsigned int gesamtlaenge;
	vector<unsigned char*> chunks;
	unsigned char output[64];  //64*8 = 512
	sha256();
	~sha256();
	void setMessage(string ctx);
	unsigned char * getHash();
private:
	void padding();
	void hash();
};

#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_BSIG0(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_BSIG1(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_SSIG0(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
#define SHA256_SSIG1(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)						\
{													\
    *((str) + 3) = (char) ((x)      );				\
    *((str) + 2) = (char) ((x) >>  8);				\
    *((str) + 1) = (char) ((x) >> 16);				\
    *((str) + 0) = (char) ((x) >> 24);				\
}
#define SHA2_PACK32(str, x)							\
{													\
    *(x) =   ((unsigned int) *((str) + 3)      )    \
           | ((unsigned int) *((str) + 2) <<  8)    \
           | ((unsigned int) *((str) + 1) << 16)    \
           | ((unsigned int) *((str) + 0) << 24);   \
}

/*
* AES.h
*
*  Created on: 13.07.2016
*      Author: Ben
*/

using namespace std;

#ifndef AES_H_
#define AES_H_

typedef unsigned char uint8;
typedef unsigned short uint16;

union wort {
	int dasWort;
	struct {
#ifdef ARCG_BIG_ENDIAN
		uint8 char3;
		uint8 char2;
		uint8 char1;
		uint8 char0;
#else
		uint8 char0;
		uint8 char1;
		uint8 char2;
		uint8 char3;
#endif
	};
	struct {
#ifdef ARCG_BIG_ENDIAN
		uint8 byte3;
		uint8 byte2;
		uint8 byte1;
		uint8 byte0;
#else
		uint8 byte0;
		uint8 byte1;
		uint8 byte2;
		uint8 byte3;
#endif
	};
};


class AES {
public:
	AES();
	AES(string & _key);
	virtual ~AES();
	void verschluesseln();
	void entschluesseln();
	int getS(unsigned int);
	void setState(int, int, int, int);
	void setKey(string & _key);
	char gmulb(uint8, uint8);

private:
	wort key[4];
	unsigned int nr;
	unsigned int nk;
	wort Roundkey[44];
	wort s[4];
	void Round(wort, char[]);
	void SubBytes();
	void invSubBytes();
	void ShiftRows();
	void invShiftRows();
	void RotWord(char[]);
	void mixColumns();

	void invMixColumns();
	void AddRoundKey(unsigned int);
	void FinalRound(wort, char[]);
	void expandKey();


	static const int rcon[15];    // Rcon
	static const char sbox[256];    // s-box
	static const char sboxinv[256]; // inverse s-box
	static const char charBase[16];    // characters for hex value conversion
};

#endif /* AES_H_ */

/*
** Name: Eashan Adhikarla
** Project: ClockChain
** Class: CSE411 Advanced Programming Techniques
*/

#include <cstdint>
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;

class Block {
public:
    string sPrevHash;

    Block(uint32_t nIndexIn, const string &sDataIn);

    string GetHash();
    string GetData() const;

    string AsCSV() const;

    void MineBlock(uint32_t nDifficulty);

private:
    uint32_t _nIndex;
    int64_t _nNonce;
    string _sData;
    string _sHash;
    time_t _tTime;

    string _CalculateHash() const;
};

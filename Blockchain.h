/*
** Name: Eashan Adhikarla
** Project: ClockChain
** Class: CSE411 Advanced Programming Techniques
*/

#include <cstdint>
#include <sstream>
#include <fstream>
#include <vector>
#include "Block.h"

using namespace std;

class Blockchain {
public:
    Blockchain();

    void AddBlock(Block bNew);
    void AddBlock(const string &sDataIn);

    string AsCSV() const;
    void ToCSV(string filename) const;

    unsigned GetSize() const;

    Block GetBlock(unsigned index) const;

private:
    uint32_t _nDifficulty;
    vector<Block> _vChain;

    Block _GetLastBlock() const;
};

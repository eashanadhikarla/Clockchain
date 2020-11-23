/*
** Name: Eashan Adhikarla
** Project: ClockChain
** Class: CSE411 Advanced Programming Techniques
*/

#include "Blockchain.h"

Blockchain::Blockchain() {
    _vChain.emplace_back(Block(0, "Genesis Block"));
    _nDifficulty = 2;
}

void Blockchain::AddBlock(Block bNew) {
    bNew.sPrevHash = _GetLastBlock().GetHash();
    bNew.MineBlock(_nDifficulty);
    _vChain.push_back(bNew);
}

void Blockchain::AddBlock(const string &sDataIn) {
    const unsigned index = _vChain.size();
    Block bNew = Block(index, sDataIn);
    AddBlock(bNew);
}

Block Blockchain::_GetLastBlock() const {
    return _vChain.back();
}

Block Blockchain::GetBlock(unsigned index) const {
    if (index < _vChain.size())
        return _vChain[index];
    else {
        stringstream ss("Invalid block index: ");
        ss << index << ", current size: " << _vChain.size();
        return Block(0, ss.str());
    }
}

unsigned Blockchain::GetSize() const {
    return _vChain.size();
}

string Blockchain::AsCSV() const {
    stringstream ss;
    unsigned const len = _vChain.size();
    for (unsigned i=0; i < len; ++i)
        ss << _vChain[i].AsCSV();
    return ss.str();
}

void Blockchain::ToCSV(string filename) const {
    ofstream file(filename);
    file << AsCSV();
}

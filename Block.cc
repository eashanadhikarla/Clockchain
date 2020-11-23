/*
** Name: Eashan Adhikarla
** Project: ClockChain
** Class: CSE411 Advanced Programming Techniques
*/

#include "Block.h"
#include "sha256.h"

Block::Block(uint32_t nIndexIn, const string &sDataIn) : _nIndex(nIndexIn), _sData(sDataIn) {
    _nNonce = -1;
    _tTime = time(nullptr);
}

string Block::GetHash() {
    return _sHash;
}

string Block::GetData() const {
    return _sData;
}

void Block::MineBlock(uint32_t nDifficulty) {
    char cstr[nDifficulty + 1];
    for (uint32_t i = 0; i < nDifficulty; ++i) {
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';

    string str(cstr);

    do {
        _nNonce++;
        _sHash = _CalculateHash();
    } while (_sHash.substr(0, nDifficulty) != str);

    cout << "Block mined: " << _sHash << endl;
}

inline string Block::_CalculateHash() const {
    stringstream ss;
    ss << _nIndex << _tTime << _sData << _nNonce << sPrevHash;

    return sha256(ss.str());
}

/**
 * Returns a CSV-like string representation of this block
 */
string Block::AsCSV() const {
    stringstream ss;

    ss << _nIndex << "," << _nNonce << ",";

    //ss << _sData << ",";
    // commas need to be escaped and I don't want to use C++ regex ...
    bool esc_flag = false;
    for (auto it = _sData.begin(); it != _sData.end(); ++it) {
        if (*it == '\\') {
            esc_flag = true;
        } else {
            if (*it == ',') {
                if (!esc_flag)
                    ss << "\\";
            }
            esc_flag = false;
        }
        ss << *it;
    }

    ss << "," << _sHash << "," << _tTime << "\n";
    return ss.str();
}

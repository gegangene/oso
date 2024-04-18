//
// Created by Bartłomiej Pruszczyk on 18/04/2024.
//

#ifndef COMPRWITHCRYPTO_COMPRESSION_H
#define COMPRWITHCRYPTO_COMPRESSION_H

#include <cstdint>

//saves to ch char value from toAdd char, adding optionally fromPrevious char firstly on top
//structure of len table: [chIterator : toAddLength : fromPreviousLength]
//return false: char ch has ability to write-in more; otherwise: char ch is full and ready to write in file
bool saver(uint8_t &ch, uint8_t &toAdd, uint8_t &fromPrevious, uint8_t len[3]);

//saves from ch char to gottenChar char, adding optionally fromPrevious char firstly on top
//structure of len table: [chIterator : gottenChar (u can define here how long gottenChar has to be) : fromPreviousLength]
//return false: char gottenChar is complete; otherwise: char ch doesn't exist
bool unsaver(uint8_t &ch, uint8_t &gottenChar, uint8_t &fromPrevious, uint8_t len[3]);

uint8_t bitsMax(uint8_t &maxCharValInDic);

void compress(std::string &inputFilename, std::string &outputFilename);
void decompress(std::string &inputFilename, std::string &outputFilename);

#endif //COMPRWITHCRYPTO_COMPRESSION_H

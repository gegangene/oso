//
// Created by Bartłomiej Pruszczyk on 18/04/2024.
//

#include <cstdint>
#include <fstream>
#include "encryption.h"
//#include <iostream>

void de_vigeneration(std::string &textFilename, std::string &keyFilename, std::string &outFilename, bool en_de)
{
	int8_t signer=en_de?1:-1;
	std::ifstream textFile(textFilename);
	std::ifstream keyFile(keyFilename);
	std::ofstream outFile(outFilename);
	key Key;
	if(true)
	{
		std::string temp;
		std::getline(keyFile,temp);
		Key.setKey(temp);
	}
	uint8_t tempChar[2];
	while(textFile>>std::noskipws>>tempChar[0])
	{
//		std::cout<<'\t'<<tempChar[0];
		tempChar[1]=tempChar[0]+signer*static_cast<int16_t>(Key.getChar());
		outFile<<tempChar[1];
	}
	textFile.close();
	keyFile.close();
	outFile.close();
}

void key::setKey(std::string& got)
{
	this->key=got;
}

uint8_t key::getChar()
{
	auto axa=static_cast<uint8_t>(this->key[iterator]);
	generateNewChar();
	nextPos();
	return axa;
}

void key::generateNewChar()
{
	srand(static_cast<unsigned int>(key[iterator]));
	this->key[iterator]=static_cast<char>(static_cast<uint8_t>(rand()%256));
}

void key::nextPos()
{
	iterator=iterator+1<this->key.length()?++iterator:0;
}


//std::string key::getKey()
//{
//	return this->key;
//}
//
//int key::currentIter()
//{
//	return iterator;
//}
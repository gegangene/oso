//
// Created by Bartłomiej Pruszczyk on 18/04/2024.
//

#ifndef COMPRWITHCRYPTO_ENCRYPTION_H
#define COMPRWITHCRYPTO_ENCRYPTION_H

#include <cstdint>

void de_vigeneration(std::string&, std::string&, std::string&, bool);
class key
{
	public:
	//		std::string getKey();
		void setKey(std::string&);
		uint8_t getChar();
	//		int currentIter();

	private:
		void generateNewChar();
		void nextPos();
		size_t iterator=0;
		std::string key;
};


#endif //COMPRWITHCRYPTO_ENCRYPTION_H

#include <cstdint>
#include <iostream>
#include <cstdio>
#include <fstream>
#include "compression.h"
#include "encryption.h"

int main()
{
	std::string keyFilename;
	std::string textFilename;
	std::string outFilename;
	std::string tempFile=".progTemp.txt";
	char choice;

	std::cout<<"\t(c)ompression+encryption/(d)ecryption+decompression:\n>> ";
	choice=getchar();
	std::cout<<"\tText file path:\n>> ";
	std::cin>>textFilename;
	std::cout<<"\tKey file path:\n>> ";
	std::cin>>keyFilename;
	std::cout<<"\tOutput file path:\n>> ";
	std::cin>>outFilename;
	if(!(std::ifstream(keyFilename).good() && std::ifstream(textFilename).good() && std::ofstream(outFilename).good()))
	{
		std::cout<<"file-related error (check filenames/permissions)";
		return 1;
	}
	if(keyFilename==".progTemp.txt"||textFilename==".progTemp.txt"||outFilename==".progTemp.txt")
	{
		std::cout<<"none of files can have filename \".progTemp.txt\"";
		return 1;
	}

	switch(choice)
	{
		case 'C':
		case 'c':
			compress(textFilename,tempFile);
			de_vigeneration(tempFile,keyFilename,outFilename,true);
			break;
		case 'D':
		case 'd':
			de_vigeneration(textFilename,keyFilename,tempFile,false);
			decompress(tempFile,outFilename);
			break;
		default:
			std::cout<<"No such a choice.";
			return 1;

	}
	remove(".progTemp.txt");
	return 0;
}

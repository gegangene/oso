#include <cstdint>
#include <map>
#include <iostream>
#include <fstream>

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

int main()
{
	std::cout<<"this is an ascii-based txt compression program.\n\n\tChoose mode:\n";
	std::cout<<"\t(c)ompression/(d)ecompression\n>> ";
	uint8_t userChoose = std::getchar();

	std::cout<<"\n\tfilename of input file\n>> ";
	std::string inputFilename;
	std::cin>>inputFilename;

	std::cout<<"\n\tfilename of output file\n>> ";
	std::string outputFilename;
	std::cin>>outputFilename;

	if(!std::ifstream(inputFilename).good())
	{
		std::cout<<"input file error";
		return(-1);
	}
	if(!std::ofstream(outputFilename).good())
	{
		std::cout<<"output file error";
		return(-1);
	}

	switch(userChoose)
	{
		case 'c':
		case 'C':
			compress(inputFilename,outputFilename);
			break;
		case 'd':
		case 'D':
			decompress(inputFilename,outputFilename);
			break;
		default:
			std::cout<<"invalid option";
	}
	return 0;
}

bool saver(uint8_t &ch, uint8_t &toAdd, uint8_t &fromPrevious, uint8_t len[3])
{
	if(len[2]>0)
	{
		ch=fromPrevious;
		len[0]+=len[2];
		fromPrevious=0;
		len[2]=0;
	}
	if(((ch<<len[1])>>len[1])==ch && len[0]+len[1]<9)
	{
		ch<<=len[1];
		ch^=toAdd;
		len[0]+=len[1];
	}
	else
	{
		ch<<=(8-len[0]);
		fromPrevious=toAdd<<(8-len[1]+8-len[0]);
		fromPrevious>>=(8-len[1]+8-len[0]);
		ch^=(toAdd>>(len[1]-(8-len[0])));
		len[2]=len[1]-(8-len[0]);
		len[0]=len[0]+(8-len[0]);
	}
	if(len[0]==8)
	{
		return true;
	}
	return false;
}

bool unsaver(uint8_t &ch, uint8_t &gottenChar, uint8_t &fromPrevious, uint8_t len[3])
{
	if(len[2]>0)
	{
		gottenChar=fromPrevious;
		len[1]-=len[2];
		fromPrevious=0;
		len[2]=0;
	}
	if(len[0]-len[1]>=0)
	{
		uint8_t len1copy=len[1];
		uint8_t charcopy=ch;
		gottenChar<<=len[1];
		charcopy>>=(len[0]-len[1]);
		gottenChar^=(charcopy);
		len[1]=0;
		ch<<=((8-len[0])+len1copy);
		ch>>=((8-len[0])+len1copy);
		len[0]-=len1copy;
	}
	else
	{
		fromPrevious=ch;
		len[2]=len[0];
		len[0]=0;
	}
	if(len[1]==0)
	{
		return true;
	}
	return false;
}

uint8_t bitsMax(uint8_t &maxCharValInDic)
{
	uint8_t bitsForChar=0;
	while(maxCharValInDic>0)
	{
		maxCharValInDic>>=1;
		bitsForChar++;
	}
	return bitsForChar;
}

void compress(std::string &inputFilename, std::string &outputFilename)
{
	std::ifstream inputFile(inputFilename);
	auto onFirst=inputFile.tellg();
	std::map<uint8_t,uint8_t> dictionary;
	uint8_t universalCharacter;
	uint8_t charCounter=0;
	long elementCounter=0;
	while(inputFile >> std::noskipws >> universalCharacter)
	{
		if(dictionary.find(universalCharacter) == dictionary.end())
        {
            dictionary[universalCharacter]=charCounter;
            charCounter++;
        }
		elementCounter++;
	}

	std::ofstream outputFile(outputFilename);
	outputFile<<charCounter;
	charCounter=0;
	for(auto &[key, num]: dictionary)
	{
		outputFile<<key;
		dictionary[key]=charCounter++;
	}
	uint8_t bitsForChar=bitsMax(--charCounter);
	uint8_t noOfAddedBits=static_cast<char>((8-(3+static_cast<long>(bitsForChar)*elementCounter)%8)%8);

	inputFile.close();
	inputFile.open(inputFilename);

	uint8_t ch=0;
	uint8_t pre=noOfAddedBits;
	uint8_t len[3]={0,bitsForChar,3};

	while(inputFile >> std::noskipws >> universalCharacter)
	{
		if(saver(ch, dictionary[universalCharacter], pre, len))
		{
			outputFile<<ch;
			ch=0;
			len[0]=0;
		}
	}
	len[1]=noOfAddedBits;
	uint8_t a=0;
	saver(ch,a,pre,len);
	outputFile<<ch;

	inputFile.close();
	outputFile.close();
}

void decompress(std::string &inputFilename, std::string &outputFilename)
{
	std::ifstream inputFile(inputFilename);
	uint8_t universalCharacter;
	inputFile>>std::noskipws>>universalCharacter;

	std::map<uint8_t, uint8_t> dictionary;
	uint8_t charCounter=0;
	for(int i=0; i<universalCharacter; i++)
	{
		uint8_t temp;
		inputFile>>std::noskipws>>temp;
		dictionary[charCounter]=temp;
//		std::cout<<"\nkey: "<<(int)charCounter<<"\tval:"<<temp;
		charCounter++;
	}

	for(auto &[key,val]: dictionary)
	{
//		std::cout<<'\n'<<(int)key<<"\t|\t"<<(int)val;
	}

	inputFile>>std::noskipws>>universalCharacter;
	uint8_t padding=universalCharacter>>5;
	universalCharacter<<=3;
	universalCharacter>>=3;

//	std::cout<<'\n'<<(int)padding<<'\n';
	uint8_t bitsForChar=bitsMax(--charCounter);

	uint8_t ch=universalCharacter;
	uint8_t pre=0;
	uint8_t a=0;
	uint8_t len[3]={5,bitsForChar,0};

	std::ofstream outputFile(outputFilename);
	while(len[0]>0)
	{
		if(unsaver(ch,a,pre,len))
		{
			outputFile<<dictionary[a];
			a=0;
			len[1]=bitsForChar;
		}
//		std::cout<<(int)a<<'\t';
	}
//	std::cout<<"pre:"<<(int)pre;
	while(inputFile>>std::noskipws>>universalCharacter)
	{
		ch=universalCharacter;
		len[0]=8;
		auto beforeIf=inputFile.tellg();
		uint8_t temp;
		if(!(inputFile>>std::noskipws>>temp))
		{
			while(len[0]>padding)
			{
				if(unsaver(ch,a,pre,len))
				{
					outputFile<<dictionary[a];
					a=0;
					len[1]=bitsForChar;
				}
//			std::cout<<(int)a<<'\t';
			}
		}
		else
		{
			while(len[0]>0)
			{
				if(unsaver(ch,a,pre,len))
				{
					outputFile<<dictionary[a];
					a=0;
					len[1]=bitsForChar;
				}
//			std::cout<<(int)a<<'\t';
			}
		}
		inputFile.seekg(beforeIf);
	}

	inputFile.close();
	outputFile.close();
}

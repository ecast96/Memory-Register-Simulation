/*
Erick Castro
CSCI 113 - Program 2

 Notes:
 Program takes instructions from input.txt and runs operation.
 I didn't get to finish it so it might not work correctly.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
using namespace std;

/*
 BLOCK
 Contains data such as valid bit, tag and data.

 */
struct Block {
	int valid_bit = 0;
	string tag = "0000";
	bool lru = 0;           //History Bit;
	bitset <32> data;       //Data represented by bitset string
};

/*
 SET
 Contains an array of two blocks.
 */
struct Set { Block block[2]; };

/*
CACHE
Contains an array of 8 sets.
 */
struct Cache { Set set[8]; };

/*
Initializes Memory and registers.
*/
void initializeVectors(vector<int> &regFile, vector<int> &mem) {
	for (int i = 0; i < mem.size(); i++)
	{
		if (i < 8)
			regFile[i] = 0;
		mem[i] = i + 5;
	}
}

//Prints Register and Memory
void printVectors(vector<int> regFile, vector<int> mem)
{
    for (int i = 0; i < mem.size(); i++)
	{
		if (i < 1) {
			for (int i = 0; i < 8; i++) {
				cout << regFile[i] << ", ";
			}
			cout << endl;
		}
		cout << mem[i] << ", ";
	}
	cout << endl;
}

//Prints out any vector passed in.
void printVector(vector<bool> v)
{
	for (long i = v.size(); i >= 0; i--)
	{
		cout << v[i];
	}
	cout << endl;
}

//Converts offset of string to Decimal - Only for 32bit Strings
int binToDec(string str) {
	int offset = 0;
	for (long i = str.size() - 1; i > 18; i--) {
		if (str[i] == '1')
			offset += pow(2, str.size() - 2 - i);
	}
	return offset;
}

//Converts any string of any size from binary to decimal.
int anyBinToDec(string str) {
	int offset = 0;
	for (long i = str.size() - 1; i >= 0; i--) {
		if (str[i] == '1')
			offset += pow(2, str.size() - 1 - i);
	}
	return offset;
}
/*
 Main Operation Function
 */
void logic(string str, Cache &cache, vector<int> &reg, vector<int> &mem) {
	bool op = 0;
	if (stoi(str.substr(0, 6), nullptr, 2) == 35) {		//Prints out if operation is Load, Store or Unknown.
		op = 0;
		cout << "LOAD - ";
	}
	else if (stoi(str.substr(0, 6), nullptr, 2) == 43) {
		op = 1;
		cout << "STORE - ";
	}
	else {
		cout << "Operation Unknown" << endl;
		return;
	}
    cout << str << endl;
	int wordAddress = binToDec(str) / 4;				//Word Address from instruction offset
	int rt = anyBinToDec(str.substr(13, 5)) - 16;		//RT value from instruction

	int setNum = (wordAddress) % 8;   //Calculated Set # from word address.
	int tagNum = (wordAddress) / 8;   //Calculated Tag from word address.

	for (int i = 0; i <= 1; i++) {																							//Loop that goes through both blocks in calculated set

		if (cache.set[setNum].block[i].valid_bit == 1 && anyBinToDec(cache.set[setNum].block[i].tag) == tagNum) {		//Checks valid bit

			cache.set[setNum].block[i].lru = 1;				//Sets history bit to 1
			cache.set[setNum].block[!i].lru = 0;			//Sets other block history bit to 0

			if (op == 0) {																									//Read/Load Hit
				//cout << "Load Hit" << endl;
				reg[rt] = (int)(cache.set[setNum].block[i].data.to_ulong());	//converts cache data into a decimal which is stored in Register[RT].
				return;
			}
			else if (op == 1) {																								//Write/Store Hit
				//cout << "Store Hit" << endl;
				cache.set[setNum].block[i].data = bitset <32>(reg[rt]);			//Converts Decimal Register value into bitset and is stored in Cache[Data].
				return;
			}
		}
		else if (i > 0) {	//If both blocks don't match	
			if (op == 0) {																									//Read/Load Miss
				//cout << "Load Miss" << endl;
				if (cache.set[setNum].block[i].lru == 0) {
					if (cache.set[setNum].block[i].valid_bit == 1)
						mem[tagNum * 8 + setNum] = (int)(cache.set[setNum].block[i].data.to_ulong());   //takes copy from cache to memory if valid bit == 1
					cache.set[setNum].block[i].data = bitset <32>(mem[wordAddress]);                    //Updates Cache data from memory[wordaddress]
					cache.set[setNum].block[i].valid_bit = 1;                                           //Sets valid bit to 1
					cache.set[setNum].block[i].tag = tagNum;
					reg[rt] = (int)(cache.set[setNum].block[i].data.to_ulong());                        //Updates register[rt] with cache data
					return;
				}
				else continue;
			}
			else if (op == 1) {																								//Write/Store Miss
				//cout << "Store Miss" << endl;
				mem[wordAddress] = reg[rt];
				return;
			}
		}
	}
}

int main()
{
	vector<int> register_file(8);
	vector<int> memory(128);
	initializeVectors(register_file, memory);           //Initializes Values for Register and Memory
	Cache cache;

	string line;
	ifstream myfile("input.txt");
	if (myfile.is_open()) {                             //Reads input.txt to get instructions
		while (getline (myfile, line)) {
			logic(line, cache, register_file, memory);
		}
		myfile.close();
	}

	printVectors(register_file, memory);                //Prints out Register and Memory
    
    for(int i = 0; i < 8; i++){
        cout << "         Valid Bit   Tag      Data" << endl;
        cout << "Block 0: " << cache.set[i].block[0].valid_bit << "            " << cache.set[i].block[0].tag << "    " << cache.set[i].block[0].data << endl;
        cout << "Block 1: " << cache.set[i].block[1].valid_bit << "            " << cache.set[i].block[1].tag << "    " << cache.set[i].block[1].data << endl;
    }

	system("pause");
	return 0;
}

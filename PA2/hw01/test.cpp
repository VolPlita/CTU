#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */
#define FS 30 

int FIBS[FS] = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610,
				987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368,
				75025, 121393, 196418, 317811, 514229, 832040, 1346269};

void dec_to_utf(int n, ofstream & tof)
{
	vector<bool> v_bin;
	while (n > 0)
	{
		v_bin.insert(v_bin.begin(), (bool)(n % 2));
		n /= 2;
	}

	int l = (int)v_bin.size();
	if (l < 8)
	{
		//Add zeros
		for(int i = 0, k = 8 - l; i < k; ++i)
		{
			v_bin.insert(v_bin.begin(), false);
		}
		//Insert to file
		char a = ( (v_bin[0] << 7) | (v_bin[1] << 6) | (v_bin[2] << 5) | (v_bin[3] << 4) |
				   (v_bin[4] << 3) | (v_bin[5] << 2) | (v_bin[6] << 1) | (v_bin[7]) );
		tof.write((char *) & a, 1);
	}
	else if (l < 12)
	{
		//Add zeros
		for(int i = 0, k = 11 - l; i < k; ++i)
		{
			v_bin.insert(v_bin.begin(), false);
		}
		//Insert first prefix
		v_bin.insert(v_bin.begin(), false);
		v_bin.insert(v_bin.begin(), true);
		v_bin.insert(v_bin.begin(), true);
		//Insert second prefix
		v_bin.insert(v_bin.begin()+8, true);
		v_bin.insert(v_bin.begin()+9, false);
		//Insert to file
		char a = ( (v_bin[0] << 7) | (v_bin[1] << 6) | (v_bin[2] << 5) | (v_bin[3] << 4) |
				   (v_bin[4] << 3) | (v_bin[5] << 2) | (v_bin[6] << 1) | (v_bin[7]) );
		char b = ( (v_bin[8] << 7) | (v_bin[9] << 6) | (v_bin[10] << 5) | (v_bin[11] << 4) |
				   (v_bin[12] << 3) | (v_bin[13] << 2) | (v_bin[14] << 1) | (v_bin[15]) );
		tof.write((char *) & a, 1);
		tof.write((char *) & b, 1);
	}
	else if (l < 17)
	{
		//Add zeros
		for(int i = 0, k = 16 - l; i < k; ++i)
		{
			v_bin.insert(v_bin.begin(), false);
		}
		//Insert first prefix
		v_bin.insert(v_bin.begin(), false);
		v_bin.insert(v_bin.begin(), true);
		v_bin.insert(v_bin.begin(), true);
		v_bin.insert(v_bin.begin(), true);
		//Insert second prefix
		v_bin.insert(v_bin.begin()+8, true);
		v_bin.insert(v_bin.begin()+9, false);
		//Insert third prefix
		v_bin.insert(v_bin.begin()+16, true);
		v_bin.insert(v_bin.begin()+17, false);
		//Insert to file
		char a = ( (v_bin[0] << 7) | (v_bin[1] << 6) | (v_bin[2] << 5) | (v_bin[3] << 4) |
				   (v_bin[4] << 3) | (v_bin[5] << 2) | (v_bin[6] << 1) | (v_bin[7]) );
		char b = ( (v_bin[8] << 7) | (v_bin[9] << 6) | (v_bin[10] << 5) | (v_bin[11] << 4) |
				   (v_bin[12] << 3) | (v_bin[13] << 2) | (v_bin[14] << 1) | (v_bin[15]) );
		char c = ( (v_bin[16] << 7) | (v_bin[17] << 6) | (v_bin[18] << 5) | (v_bin[19] << 4) |
				   (v_bin[20] << 3) | (v_bin[21] << 2) | (v_bin[22] << 1) | (v_bin[23]) );
		tof.write((char *) & a, 1);
		tof.write((char *) & b, 1);
		tof.write((char *) & c, 1);
	}
	else // Look here
	{
		//Add zeros
		for(int i = 0, k = 21 - l; i < k; ++i)
		{
			v_bin.insert(v_bin.begin(), false);
		}
		//Insert first prefix
		v_bin.insert(v_bin.begin(), false);
		v_bin.insert(v_bin.begin(), true);
		v_bin.insert(v_bin.begin(), true);
		v_bin.insert(v_bin.begin(), true);
		v_bin.insert(v_bin.begin(), true);
		//Insert second prefix
		v_bin.insert(v_bin.begin()+8, true);
		v_bin.insert(v_bin.begin()+9, false);
		//Insert third prefix
		v_bin.insert(v_bin.begin()+16, true);
		v_bin.insert(v_bin.begin()+17, false);
		//Insert fourth prefix
		v_bin.insert(v_bin.begin()+24, true);
		v_bin.insert(v_bin.begin()+25, false);
		//Insert to file
		char a = ( (v_bin[0] << 7) | (v_bin[1] << 6) | (v_bin[2] << 5) | (v_bin[3] << 4) |
				   (v_bin[4] << 3) | (v_bin[5] << 2) | (v_bin[6] << 1) | (v_bin[7]) );
		char b = ( (v_bin[8] << 7) | (v_bin[9] << 6) | (v_bin[10] << 5) | (v_bin[11] << 4) |
				   (v_bin[12] << 3) | (v_bin[13] << 2) | (v_bin[14] << 1) | (v_bin[15]) );
		char c = ( (v_bin[16] << 7) | (v_bin[17] << 6) | (v_bin[18] << 5) | (v_bin[19] << 4) |
				   (v_bin[20] << 3) | (v_bin[21] << 2) | (v_bin[22] << 1) | (v_bin[23]) );
		char d = ( (v_bin[24] << 7) | (v_bin[25] << 6) | (v_bin[26] << 5) | (v_bin[27] << 4) |
				   (v_bin[28] << 3) | (v_bin[29] << 2) | (v_bin[30] << 1) | (v_bin[31]) );
		tof.write((char *) & a, 1);
		tof.write((char *) & b, 1);
		tof.write((char *) & c, 1);
		tof.write((char *) & d, 1);
	}
	for (size_t i = 0; i < v_bin.size(); i++)
	{
		if (i % 8 == 0) cout << " . ";
		cout << v_bin[i];
	}
	cout << endl;
	
}

int get_dec(vector<bool> & sym)
{
	size_t l = sym.size();
	int res = 0;
	for (size_t i = 0; i < l; ++i)
	{
		res += FIBS[i] * (int)sym[i];
	}
	return (res - 1);
}

int readFib(ifstream &fstr, vector<bool> & vec, int n_bytes)
{
	char sym;
	for (int i = 0; i < n_bytes; i++)
	{
		fstr.read((char *)&sym, 1);
		if (fstr.bad() || fstr.eof()) return i;
		vec.push_back((bool)(sym & 0x1));
		vec.push_back((bool)((sym >> 1) & 0x1));
		vec.push_back((bool)((sym >> 2) & 0x1));
		vec.push_back((bool)((sym >> 3) & 0x1));
		vec.push_back((bool)((sym >> 4) & 0x1));
		vec.push_back((bool)((sym >> 5) & 0x1));
		vec.push_back((bool)((sym >> 6) & 0x1));
		vec.push_back((bool)(sym >> 7));
	}
	return n_bytes;	
}

void move_to_file(vector<bool> & vec, ofstream & tof)
{
	size_t len = vec.size();
	size_t pres = len % 8;
	
	//Preserve last bytes
	vector<bool> tmp;
	for (size_t i = 0; i < pres; ++i)
	{
		tmp.insert(tmp.begin(), vec.back());
		vec.pop_back();
	}
	

	char a;
	for(size_t i = 0, k = (len - pres); i < k; i += 8)
	{
		a = ( (vec[i+7] << 7) | (vec[i+6] << 6) | (vec[i+5] << 5) | (vec[i+4] << 4) | 
			  (vec[i+3] << 3) | (vec[i+2] << 2) | (vec[i+1] << 1) | (vec[i]) );

		tof.write((char *) & a, 1);
	}

	vec.clear();
	vec = tmp;
}

void dec_to_fib(int dec, vector<bool>& fib_s)
{
	dec++;
	fib_s.insert(fib_s.begin(), true);
	fib_s.insert(fib_s.begin(), true);

	// Get the biggest fib_seq number <= dec
	int i = FS - 1;
	while (i >= 0)
	{
		if (FIBS[i--] <= dec) break;
	}
	dec -= FIBS[i+1];

	//Fill the rest with 0/1
	while (i >= 0)
	{
		if (FIBS[i] > dec) 
		{
			fib_s.insert(fib_s.begin(), false);
		}
		else 
		{
			fib_s.insert(fib_s.begin(), true);
			dec -= FIBS[i];
		}
		i--;
	}
}

int readSymbol(ifstream &fstr, unsigned char * bits)
{
	fstr.read((char *)bits, 1);
	if (bits[0] >> 7 == 0) return bits[0];
	else if((bits[0] >> 5) == 6)
	{
		fstr.read((char *)(bits+1), 1);
		if (bits[1] >> 6 != 2) return -1;
		return (((bits[0] & 0x1f) << 6) | (bits[1] & 0x3f));
	}
	else if(bits[0] >> 4 == 14)
	{
		fstr.read((char *)(bits+1), 1);
		fstr.read((char *)(bits+2), 1);
		if ((bits[1] >> 6 != 2) || (bits[2] >> 6 != 2)) return -2;
		return (((bits[0] & 0xf) << 12) | ((bits[1] & 0x3f) << 6) | (bits[2] & 0x3f));
	}
	else if(bits[0] >> 3 == 30)
	{
		fstr.read((char *)(bits+1), 1);
		fstr.read((char *)(bits+2), 1);
		fstr.read((char *)(bits+3), 1);
		if ((bits[1] >> 6 != 2) || (bits[2] >> 6 != 2) || (bits[3] >> 6 != 2)) return -3;
		return (((bits[0] & 0x7) << 18) | ((bits[1] & 0x3f) << 12) | ((bits[2] & 0x3f) << 6) | (bits[3] & 0x3f));
	}
	else return -4;
}

bool utf8ToFibonacci ( const char * inFile, const char * outFile )
{
    ifstream fromf(inFile, ios::binary);
	ofstream tof(outFile, ios::binary);

	if(!fromf.is_open() || !tof.is_open() || fromf.bad() || tof.bad() || !fromf || !tof)
	{
		fromf.close();
		tof.close();
		return false;
	}
	
	vector<bool> to_write;
	int symbol_val;
	while (true)
	{
		// Read the symbol in utf-8 and get a value
		unsigned char symbol_bit[4];
		symbol_val = readSymbol(fromf, symbol_bit);
		if (symbol_val < 0)
		{
			break;
		} 
		if (fromf.eof()) 
		{
			break;
		}
		//Convert the value to fibonacci
		vector<bool> fib_s;
		fib_s.clear();
		dec_to_fib(symbol_val, fib_s);

		to_write.insert(to_write.end(), fib_s.begin(), fib_s.end());
		
		// If more than 100 bit, write to file
		if (to_write.size() > 100)
		{
			move_to_file(to_write, tof);
			if (tof.bad())
			{
				fromf.close();
				tof.close();
				return false;
			}
		}

	}
	if (symbol_val < 0) 
	{
		fromf.close();
		tof.close();
		return false;
	}
	//Add remaining zeros
	size_t ts = to_write.size() % 8;
	if (ts != 0) ts = (8 - ts);
	for (size_t i = 0; i < ts; i++)
	{
		to_write.push_back(false);
	}

	// Write to a file 
	move_to_file(to_write, tof);
	if (tof.bad())
	{
		fromf.close();
		tof.close();
		return false;
	}

	fromf.close();
	tof.close();

	return true;
}
  
bool fibonacciToUtf8 ( const char * inFile, const char * outFile )
{
	ifstream fromf(inFile, ios::binary);
	ofstream tof(outFile, ios::binary);

	if(!fromf.is_open() || !tof.is_open() || fromf.bad() || tof.bad() || !fromf || !tof)
	{
		//cout << 6;
		fromf.close();
		tof.close();
		return false;
	}

	vector<bool> vec;
	vector<bool> sym;
	int dec;
	int bytes_read;
	bool check = false; //
	//bool checkp = false; //
	while (true)
	{
		bytes_read = readFib(fromf, vec, 10);
		if (bytes_read == 0) break;
		// Special case
		int i = 0;
		if ((check == true) && (vec[0] == true) && (sym.size() != 0) /*&& (checkp == false)*/)
		{
			dec = get_dec(sym);
			cout << "! = " << dec << endl;
			if (dec > 0x10ffff) 
			{
				//cout << 5;
				fromf.close();
				tof.close();
				return false; // Look here
			}
			dec_to_utf(dec, tof);
			if (tof.bad())
			{
				//cout << 2;
				fromf.close();
				tof.close();
				return false;
			}
			sym.clear();
			i++;
		}
		// /Special case

		for (int k = (bytes_read * 8); i < k; i++)
		{
			sym.push_back(vec[i]);
			if (sym.size() > FS) 
			{
				//cout << 4;
				fromf.close();
				tof.close();
				return false;
			}
			if((vec[i] == true) && (vec[i+1] == true))
			{
				dec = get_dec(sym);
				cout << " = " << dec << endl;
				if (dec > 0x10ffff) 
				{
					//cout << 3;
					fromf.close();
					tof.close();
					return false; // Look here
				}
				dec_to_utf(dec, tof);
				if (tof.bad())
				{
					//cout << 2;
					fromf.close();
					tof.close();
					return false;
				}
				sym.clear();
				i++;
			}
		}
		check = vec[vec.size() - 1];//
		//checkp = vec[vec.size() - 2];
		vec.clear();
		if (bytes_read < 10) break;
	}
	for (size_t i = 0, k = sym.size(); i < k; i++)
	{
		if (sym[i] == true)
		{
			//cout << 1;
			fromf.close();
			tof.close();
			return false;
		}
	}

	fromf.close();
	tof.close();

	return true;
}
  
#ifndef __PROGTEST__

bool               identicalFiles                          ( const char      * file1,
                                                             const char      * file2 )
{
	ifstream f1(file1, ios::binary | ios::ate); // to move file pointer to the end of the file
    ifstream f2(file2, ios::binary | ios::ate);
    
    if (f1.fail() || f2.fail() || (f1.tellg() != f2.tellg())) 
	{
        return false;
    }
    
    f1.seekg(0, ios::beg);
    f2.seekg(0, ios::beg);
    
    return equal(istreambuf_iterator<char>(f1.rdbuf()), istreambuf_iterator<char>(), istreambuf_iterator<char>(f2.rdbuf()));
}

int main ( int argc, char * argv [] )
{
 assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
          && identicalFiles ( "output.fib", "example/dst_0.fib" ) ); 	// pass
 assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
          && identicalFiles ( "output.fib", "example/dst_1.fib" ) ); 	// pass
 assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
          && identicalFiles ( "output.fib", "example/dst_2.fib" ) ); 	// pass
 assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
          && identicalFiles ( "output.fib", "example/dst_3.fib" ) ); 	// pass
 assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
          && identicalFiles ( "output.fib", "example/dst_4.fib" ) ); 	// pass
 assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );	// pass
 assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )	 
          && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );	// pass
 assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
          && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );	// pass
 assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
          && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );  // pass
 assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
          && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );  // pass
 assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
          && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) ); // pass
 assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );

cout << fibonacciToUtf8 ( "example/in_5035297.bin", "example/out_5035297.bin" );
cout << fibonacciToUtf8 ( "example/in_5035296.bin", "example/out_5035296.bin" );

//cout << fibonacciToUtf8 ( "huj.bin", "khe.bin" );

// cout << fibonacciToUtf8 ( "example/in_5035151.bin", "output1.utf8" );
// cout << fibonacciToUtf8 ( "example/in_5034915.bin", "output2.utf8" );
//cout << fibonacciToUtf8 ( "example/in_5027134.bin", "output.utf8" );
// cout << utf8ToFibonacci ( "output.utf8", "output.fib" );

//cout << fibonacciToUtf8 ( "huj.bin", "output.utf8" );

//cout << fibonacciToUtf8 ( "smfl.bin", "output.utf8");
//cout << fibonacciToUtf8 ( "example/in_5034580.bin", "output.utf8" );

// cout << utf8ToFibonacci ( "example/in_5028698.bin", "output.fib" );

//  assert(!utf8ToFibonacci ( "example/in_5028698.bin", "output.fib" ));
//  assert(!utf8ToFibonacci ( "example/in_5028706.bin", "output.fib" ));

// fibonacciToUtf8 ( "example/in_5028083.bin", "output.utf8" );
// cout << fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" );
// cout << identicalFiles ( "output.utf8", "example/dst_10.utf8" );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

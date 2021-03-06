#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

using namespace std;

// sample_len is used to ignore separator after sample
vector<size_t> z_function(const vector<uint32_t> &s, int sample_len)
{
	size_t n=s.size();
	vector<size_t> z(n);
	size_t l=0, r=0;
	for(size_t i=1;i<n;i++)
	{
		if(i<r)
			z[i]=min(r-i,z[i-l]);
		while(i+z[i]<n && s[z[i]]==s[i+z[i]] && z[i]!=sample_len)
			z[i]++;
		if(i+z[i]>r)
		{
			l=i;
			r=i+z[i];
		}
	}
	return z;
}

// create data=sample+0+text, calculate z(data)
vector<size_t> z_search(const vector<uint32_t> &sample, const vector<uint32_t> &text)
{
	size_t sample_size=sample.size();
	size_t text_size=text.size();
	vector<uint32_t> data;
	try
	{
		data.resize(sample_size+1+text_size);
	}
	catch(std::bad_alloc const&)
	{
		cout<<"ERROR: z_search(), memory allocation fail!"<<endl;
		exit(0);
	}
	copy(sample.begin(), sample.end(), data.begin());
	data[sample_size]=0; // such as a separator, but isn't used	
	copy(text.begin(), text.end(), data.begin()+sample_size+1);
	vector<size_t> z=z_function(data, sample_size);	
	vector<size_t> indexes;
	for(size_t i=0;i<data.size();i++)
		if(z[i]==sample_size)
			indexes.push_back(i-sample_size-1);
	return indexes;
}

// read numbers from string
vector<uint32_t> parse_string(const string &s)
{
	vector<uint32_t> res;
	uint32_t x;
	istringstream ss(s);
	while(ss>>x)
		res.push_back(x);
	return res;
}

struct Reader
{
	size_t str_pos;
	size_t val_pos;
	vector<size_t> str_len;
	string buf;
	char c;

	Reader()
	{
		str_pos=0;
		val_pos=0;
	}

	bool getnum(uint32_t &num)
	{
		buf.clear();

		// search digits 
		do
		{
			c=cin.get();
			if(c=='\n')
			{
				str_len.push_back(val_pos);
				str_pos++;
				val_pos=0;
			}
		}
		while(!cin.eof() && !isdigit(c)); 

		// read digits
		while(!cin.eof() && isdigit(c))
		{
			buf+=c;
			c=cin.get();
		}

		if(cin.eof() || c=='\n')
		{
			str_len.push_back(val_pos);
			str_pos++;
			val_pos=0;
		}

		num=0;
		if(cin.eof())
			return false;

		try
		{
			num=stoi(buf); // string to integer
			val_pos++;
		}
		catch(invalid_argument &e) {}
		return true;
	}

	bool getvec(vector<uint32_t> &vec, size_t maxsize)
	{
		uint32_t x;
		vec.clear();
		for(int i=0;i<maxsize;i++)
		{
			if(!getnum(x))
				if(i==0)
					return false;
				else
					break;
			vec.push_back(x);
		}
		return true;
	}
}

const int BLOCK_SIZE=10;

int main()
{
	// read sample
	string s;
	getline(cin, s, '\n');
	vector<uint32_t> sample=parse_string(s);
	cout<<"sample readed"<<endl;
	
	// read text and count numbers in lines
	Reader reader;
	vector<uint32_t> block;
	while(reader.getvec(block, BLOCK_SIZE))
	{
		cout<<endl;
		for(auto x:block)
			cout<<x<<' ';
	}

	cout<<endl;
	for(auto i:reader.str_len)
	{
		cout<<i<<' ';
	}

//	vector<size_t> str_count;
//	while(getline(cin, s, '\n'))
//	{
//		vector<uint32_t> str=parse_string(s);
//		text.insert(text.end(), str.begin(), str.end());
//		str_count.push_back(str.size());
//	}
//	
//	// search
//	vector<size_t> indexes=z_search(sample,text);
//
//	// calculate and write line and pos for each found sample
//	for(auto i:indexes)
//	{
//		int bi=0, ei=0;	
//		for(int k=0;k<str_count.size();k++)
//		{
//			ei=bi+str_count[k];
//			if(i>=bi && i<ei)
//				cout<<k+1<<", "<<i-bi+1<<endl;
//			bi=ei;
//		}
//	}
}



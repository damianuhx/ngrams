
#include <fstream.h>
#include <iostream.h>
#include <list>
#include <sstream>
using namespace std;



ofstream output;
ifstream file, input;
char letter;
string word, tokenbuffer;
const int n=3;




//**************************************
//Main Class "Ngram"
//**************************************

class Ngram
{
public:
string tokenbegin[n-1];
list<string> tokenlast;
list<string>::iterator tokenlastit;
list<int> tokenlastcount;
list<int>::iterator tokenlastcountit;

	void printngram()
	{
		for (int i=0; i<n-1; i++)
		{
			cout << tokenbegin[i] << ", ";
		}
		tokenlastcountit=tokenlastcount.begin();
		for (tokenlastit=tokenlast.begin(); tokenlastit!=tokenlast.end(); tokenlastit++)
		{
			cout << *tokenlastit << ":" << *tokenlastcountit << ". ";
			tokenlastcountit++;
		}
		cout << "\n";
	};
	
	void sortlasttoken()
	{
	if (tokenlast.size()>1)
	{
	//	list<string> listbuffer;
		list<string>::iterator stringit=tokenlast.begin();
		list<string>::iterator stringit2=tokenlast.end();
		list<string>::iterator stringit3=tokenlast.begin();
		list<int>::iterator intit=tokenlastcount.begin();
		list<int>::iterator intit2=tokenlastcount.end();
		list<int>::iterator intit3=tokenlastcount.begin();
		int step=0, change =0;
	
		while (stringit2 != tokenlast.begin() && intit2 !=tokenlastcount.begin())
		{
			change = 0;
			stringit=tokenlast.begin();
			intit=tokenlastcount.begin();
			
			intit3=tokenlastcount.begin();
			while (stringit3!=stringit2 && intit3 != intit2)
			{
		//	cout << *intit3 << "/" << *intit << "\n";
		//	cout << *stringit3 << "/" << *stringit << "\n";
				if (*intit3<*intit)
				{
		//			cout << "*\n";
					intit=intit3;
					stringit=stringit3;
				}
				
				stringit3++;
				intit3++;
			}
			
			
			tokenlastcount.push_back(*intit);
			tokenlastcount.erase(intit);
			tokenlast.push_back(*stringit);
			tokenlast.erase(stringit);	
			
			step++;
			
			intit3=tokenlastcount.begin();
			stringit3=tokenlast.begin();
			
			stringit2=tokenlast.end();
			intit2=tokenlastcount.end();
			for (int j = 0; j < step; j++)
				{
					stringit2--;
					intit2--;
				}
			
			
			
		}
	//invert
	stringit=tokenlast.end();
	intit=tokenlastcount.end();
	int steps=tokenlast.size();

	while (stringit!=tokenlast.begin())
	{
		stringit--;
		intit--;
		tokenlast.push_back(*stringit);
		tokenlastcount.push_back(*intit);
	}
	
	stringit=tokenlast.begin(); 
	for (int i =0; i<steps; i++)
	{
		tokenlast.pop_front();
		tokenlastcount.pop_front();
	}
	//printngram();
	
	
	}
	
	
	
	};
};


void printstringlist (list<string> stringlist)
{
	list<string>::iterator stringlistit;
	cout << ">";
	for (stringlistit=stringlist.begin(); stringlistit!=stringlist.end(); stringlistit++)
	{
		cout << " " << (*stringlistit) << ".";
	}
	cout << "\n";
};

void printngramlist (list<Ngram> ngramlist)
{
	list<Ngram>::iterator ngramlistit;
	cout << ">>>>>>>>>>>>>>>";
	for (ngramlistit=ngramlist.begin(); ngramlistit!=ngramlist.end(); ngramlistit++)
	{
		(*ngramlistit).printngram();
	}
};


//************************************
//Procedures
//***********************************

//Definiere folgende Funktionen
Ngram search(string searchtoken[n-1], list<Ngram> ngrams, int nsug);
list<string> advancedsearch(string searchtoken[n], list<Ngram> ngrams, int nsug);
//list<string> test(list<string> argument);
Ngram lasttokenmerge (Ngram ngram, Ngram ngram2);

void writetext(char* path, string text)
{
	ofstream outfile;
	outfile.open(path);
	outfile << text;
};

//***************************************************
//1. returns token from "file";
//***************************************************
list<string> maketokenlist(char* path) 
{
	ifstream infile;
	infile.open(path);
	
	list<string> token;
	string word;
	char letter;
	
	//1. make token-list
	while (infile.good()) 	
	{
		
		letter=infile.get();
		
		if (letter > 34 && letter!= 44 && letter !=46 && letter != 63)
		{
			word=word+letter;
		}
		else
		{
			if (word !="")
			{
				token.push_back(word);
			}
			word ="";
			
		}
	}
	return token;
};





//***********************************************
//2. make ngrams
//***********************************************	
	
	
list<Ngram> makengrams(list<string> token)
{
	Ngram ngrambuffer;
	list<Ngram> ngrams;
	list<Ngram>::iterator ngramsit;
	list<string>::iterator tokenit;
	list<string>::iterator tokenitnext;
	
	tokenit=token.begin();
	
	for (tokenit=token.begin(); tokenit!=token.end(); tokenit++)
	{
		tokenitnext=tokenit;
		Ngram ngrambuffer = Ngram();
		
		for (int i=0; i<n-1; i++)
		{
			if (tokenitnext!=token.end())
			{
				ngrambuffer.tokenbegin[i]=*tokenitnext;
				tokenitnext++;
			}
			else
			{
				ngrambuffer.tokenbegin[i]="$";
			}
			
		}
		if (tokenitnext!=token.end())
		{
			(ngrambuffer.tokenlast).push_back(*tokenitnext);
		}
		else
		{
			(ngrambuffer.tokenlast).push_back("$");
		}
		(ngrambuffer.tokenlastcount).push_back(1);
		ngrams.push_back(ngrambuffer);
	}
	return ngrams;
};





//***********************************************
//3. sort ngrams
//***********************************************	


list<Ngram> sortngrams(list<Ngram> ngrams)
{
	list<Ngram>::iterator ngramsit;
	list<Ngram>::iterator ngramsitcomp;
	list<Ngram>::iterator ngramsitlast;
	//list<string>::iterator xgramit;
	//list<int>::iterator xcountit;
	
	
		for (int i=n-2; i>=0; i--)
		{
			//cout << i;
			int step=0;
			ngramsitcomp=ngrams.begin();
			ngramsitlast=ngrams.end();
			
			while (ngramsitlast!=ngrams.begin())
			{
				for (ngramsit=ngrams.begin(); ngramsit!=ngramsitlast; ngramsit++)
				{
					if ((*ngramsit).tokenbegin[i] < (*ngramsitcomp).tokenbegin[i])
					{
						ngramsitcomp=ngramsit;
					}
					
				}
				ngrams.push_back(*ngramsitcomp);
				ngrams.erase(ngramsitcomp);
				step++;
					
				ngramsitcomp=ngrams.begin();
				ngramsitlast=ngrams.end();
				for (int j = 0; j < step; j++)
				{
					ngramsitlast--;
				}
			}
		
		}
		return ngrams;
};





//***********************************************
//4. merge ngrams
//***********************************************

list<Ngram> mergengrams(list<Ngram> ngramsinput)
{
list<Ngram> ngrams = ngramsinput;
list<Ngram>::iterator ngramsit = ngrams.begin();
list<Ngram>::iterator ngramsitcomp = ngrams.begin();
list<string>::iterator tokenit = (*ngramsit).tokenlast.begin();
list<int>::iterator tokencountit = (*ngramsit).tokenlastcount.begin();

	for (ngramsit=ngrams.begin(); ngramsit!=ngrams.end(); ngramsit++)
	{
	int same = 0;
	int add = 1;
	ngramsitcomp=ngramsit;
	ngramsitcomp++;
		for (int i=0; i<n-1; i++)
		{
		//cout << ",";
			if ((*ngramsit).tokenbegin[i] == (*ngramsitcomp).tokenbegin[i])
			{
				same++;
				//cout << same;
			}
		}
		if (same==n-1)
		{
		//cout << ".";
		tokenit=(*ngramsit).tokenlast.begin();
		tokencountit=(*ngramsit).tokenlastcount.begin();
		while (tokenit != (*ngramsit).tokenlast.end() && tokencountit != (*ngramsit).tokenlastcount.end())
		{
			if (*tokenit==(*ngramsitcomp).tokenlast.front())
			{
				(*tokencountit)++;
				add=0;
			}
		tokenit++;
		tokencountit++;
		}
		if (add == 1)
			{
			(*ngramsit).tokenlast.push_back((*ngramsitcomp).tokenlast.front());
			(*ngramsit).tokenlastcount.push_back(1);
			}
		ngrams.erase(ngramsitcomp);
		ngramsit--;
		}
	}	
	return ngrams;
};

//***********************************************
//5. save ngrams into file output
//***********************************************

void savengrams(list<Ngram> ngrams, char* path)
{
ofstream outfile;
outfile.open(path);

list<Ngram>::iterator ngramsit;
list<Ngram>::iterator ngramsitcomp;
list<Ngram>::iterator ngramsitlast;
list<string>::iterator xgramit;
list<int>::iterator xcountit;
	
for (ngramsit=ngrams.begin(); ngramsit!=ngrams.end(); ngramsit++)
	{
		
		(*ngramsit).sortlasttoken();
		for (int i=0; i<n-1; i++)
		{
			//cout << (*ngramsit).tokenbegin[i] << ", ";
			outfile << "!" << (*ngramsit).tokenbegin[i];
		}
		xcountit = ((*ngramsit).tokenlastcount).begin();
		for (xgramit=((*ngramsit).tokenlast).begin(); xgramit!=(*ngramsit).tokenlast.end(); xgramit++)
			{
			//cout << (*xgramit) << ": " << (*xcountit);
			//cout << ".\n";
			outfile << "?" << (*xgramit)<<"."<<(*xcountit);
			xcountit++;
			}
		outfile <<",\n";
	}
	outfile << "!";
};





//**************************************
//6. LIes Datenfile ein
//**************************************

list<Ngram> readdata(char* path)
{
list<Ngram> ngrams;
list<Ngram>::iterator ngramsit;
int tokentype, tokencount;
word = "";
Ngram ngrambuffer;
string stringbuffer;
int intbuffer;
tokencount=0;
char letter;

ifstream infile;
infile.open(path);
	
	
while (infile.good()) 	
	{
	letter=infile.get();
	//cout << letter; 
		if (letter == 33 || letter == 63 || letter == 46 || letter == 44)
		{
			if (word !="")
			{
				//cout << tokentype << ",";
				if (tokentype==1)
				{
				ngrambuffer.tokenbegin[tokencount]=word;
				//cout << word << ", ";
				tokencount++;
				}
				if (tokentype==2)
				{
				ngrambuffer.tokenlast.push_back(word);
				//cout << word << ", ";
				}
				if (tokentype==3)
				{
				stringstream ss(word); 
				ss >> intbuffer;
				ngrambuffer.tokenlastcount.push_back(intbuffer);
				intbuffer=0;
				}
				if (tokentype==4)
				{
				ngrams.push_back(ngrambuffer);
			//	(ngrams.back()).printngram();
				ngrambuffer.tokenlast.clear();
				ngrambuffer.tokenlastcount.clear();
				Ngram ngrambuffer;
				tokencount=0;
				tokentype == 0;
				}
				word ="";
				//cout << tokentype;
			}
		}
		
		if (letter==33)
		{
		tokentype=1;
		}
		else if (letter==63)
		{
		tokentype = 2;
		}
		else if (letter==46)
		{
		tokentype=3;
		}
		else if (letter==44)
		{
		tokentype=4;
		}
		else
		{
			word=word+letter;
		}
	
	}
	return ngrams;
};



//****************************************
// 7. Teste Vorhersagewahrscheinlichkeit
//****************************************

float test(list<Ngram> ngrams, list<Ngram> ngramscomp, int nsug)
{
	list<Ngram>::iterator ngramsit;
	list<Ngram>::iterator ngramscompit;
	list<Ngram>::iterator ngramslastit;
	list<string> result;
	list<string>::iterator resultit;
	int predicted=0, predictedwords=0, total=0;
	
	ngramslastit=ngramscomp.end();
	//for (int i=0; i<n; i++){ngramslastit=ngramscomp.end();}

	for (ngramscompit = ngramscomp.begin(); ngramscompit != ngramslastit; ngramscompit++)
	{
		//(*ngramscompit).printngram();
		result= (search((*ngramscompit).tokenbegin, ngrams, nsug)).tokenlast;
		predicted =0;
		int i=0;
		for (resultit=result.begin(); resultit!=result.end() && i++<nsug; resultit++)
		{
			if (*resultit==(*ngramscompit).tokenlast.front()) {predicted=1;}
		}
		predictedwords+=predicted;
		total++;
		cout << "predicted words / total words: " << predictedwords << " / " << total << "\n";
	}
	cout << predictedwords << " of " << total << " words (" << ((float)predictedwords/(float)total)*100 << "%) were in the sugestions.\n";
	return (float)predictedwords*100/(float) total;
};


float advancedtest(list<Ngram> ngrams, list<Ngram> ngramscomp, int nsug)
{
	list<Ngram>::iterator ngramsit;
	list<Ngram>::iterator ngramscompit;
	list<Ngram>::iterator ngramslastit;
	list<string> result;
	list<string>::iterator resultit;
	int predicted=0, predictedwords=0, total=0;
	ngramslastit=ngramscomp.end();
	int keystrokes=0, nchars=0;
	
	string searchbuffer[n];
	string::iterator laststringbuffer;

	for (ngramscompit = ngramscomp.begin(); ngramscompit != ngramslastit; ngramscompit++)
	{
		nchars+=((*ngramscompit).tokenlast.front()).size(); //länge des wortes
		//nchars++; keystrokes++;//leerschlag (od zeichen zwischen token)
		
		cout << "search for: ";
		(*ngramscompit).printngram();
		
		predicted =0;
		for (int i=0; i<n-1; i++)
		{
			searchbuffer[i]=(*ngramscompit).tokenbegin[i];
		}
		searchbuffer[n-1]="";
		
			for (laststringbuffer=((*ngramscompit).tokenlast.front()).begin(); laststringbuffer!=((*ngramscompit).tokenlast.front()).end() && predicted==0; laststringbuffer++)
			{
				
		
				//cout << searchbuffer[n-1] << " ";
				
				result= (advancedsearch(searchbuffer, ngrams, nsug));

				int i=0;
				for (resultit=result.begin(); resultit!=result.end() && i++<nsug && predicted==0; resultit++)
				{
					if (*resultit==(*ngramscompit).tokenlast.front()) {predicted=1; cout << "*";}
				}
				searchbuffer[n-1].push_back(*laststringbuffer);
				keystrokes++;
			}
			
		cout << "Keystrokes Needed / Number of Characters: " << keystrokes << "/" << nchars << "\n";
		
		
	}
	cout << "\n" << keystrokes << " keystrokes were needed for " << nchars << " characters. So " << ((float)keystrokes/(float)nchars)*100 << "% of the keystrokes were needed, when proposing " << nsug << " Words each time.\n";
	return (float)predictedwords*100/(float) total;
};




//******************************************************
// 8. Merge
//******************************************************
list<Ngram> merge (list<Ngram> ngrams, list<Ngram> ngrams2)
{
list<Ngram>::iterator ngramsit;
list<Ngram>::iterator ngrams2it;
int equals=0, i=0;

ngramsit=ngrams.begin();
ngrams2it=ngrams2.begin();

while (ngrams2it!=ngrams2.end() && ngramsit!=ngrams.end())
{
	//cout << ".";
	equals=0;
	i=0;
	
	while (i<(n-1) && (*ngramsit).tokenbegin[i]==(*ngrams2it).tokenbegin[i])
	{
	//cout << (*ngramsit).tokenbegin[i] << "/" << (*ngrams2it).tokenbegin[i] << ", ";
	equals++;
	i++;
	}
	//cout << (*ngramsit).tokenbegin[i] << "/" << (*ngrams2it).tokenbegin[i] << ", ";
	
	//cout << ": " << equals << "/" << i <<  "\n";
	if (equals==n-1)
	{
		*ngramsit=lasttokenmerge(*ngramsit, *ngrams2it);
		//cout << "merged: ";
		//(*ngramsit).printngram();
		ngramsit++;
		ngrams2it++;
	}
	else if (equals < n-1)
	{
		if ((*ngramsit).tokenbegin[i] > (*ngrams2it).tokenbegin[i])
		{
			//cout << "inserted: ";
			//(*ngrams2it).printngram();
			ngrams.insert(ngramsit, *ngrams2it);
			ngrams2it++;
		}
		else if ((*ngramsit).tokenbegin[i] < (*ngrams2it).tokenbegin[i])
		{
			//cout << "kept: ";
			//(*ngramsit).printngram();
			ngramsit++;
		}
	}
}



while (ngrams2it!=ngrams2.end())
{
	ngrams.push_back(*ngrams2it);
	ngrams2it++;
}


cout << "merging done\n";
return ngrams;
};


//****************************************************************************************************************
int main(int argc, char* argv[] )
{
//Set Mode
string mode = argv[1];
char* datapath;
char* textpath;
char* savepath;

int part;
if (mode=="create"){part=1;}
if (mode=="predict"){part=2;}
if (mode=="test"){part=3;}
if (mode=="advtest"){part=4;}
if (mode=="merge"){part=5;}

int nsug = 5;
//Set Input/Output stream destinations
if (part==1)
{
textpath = argv[2];
savepath=argv[3];
}

if (part==2)
{
textpath = argv[4];
datapath = argv[3];
}
	
if (part==3 || part==4)
{
textpath = argv[3];
datapath = argv[2];

if (part==4)
{
string nsugstring;
nsugstring = argv[4];
stringstream ss2(nsugstring); 
ss2 >> nsug;
}
}


if (part==5)
{
savepath = argv[4];
textpath = argv[3];
datapath = argv[2];
}



//define main lists
list<string> token;
list<Ngram> ngrams;
list<Ngram> ngramsdata;
Ngram ngramtest;

//Main Programm

/*while (file.good()) 	
	{
	letter=file.get();
	cout << letter; 
	}*/
	
if (part == 1 || part==3 || part==4)
{
	cout << "making token\n";
	token=maketokenlist(textpath); //returns tokenlist from file
	
	cout << "making ngrams\n";
	ngrams=makengrams(token);
}

if (part==2)
{
cout << "reading data\n";
ngramsdata = readdata(datapath);
//printngramlist(ngramsdata);

cout << "predicting next token\n";
list<string>::iterator stringit;
string text = argv[2];
bool adv=true;
string::iterator itbuffer; itbuffer = text.end(); itbuffer--;
if (*itbuffer <= 34 || *itbuffer== 44 || *itbuffer ==46 || *itbuffer == 63)
{adv=false;}
writetext(textpath, text);
token=maketokenlist(textpath);
stringit=token.end();

cout << "searching token matching: ";

if (adv==true)
{
	list<string> result;
	string stringbuffer[n];
	 
	for (int i=0; i<n; i++)
	{
		if (stringit!=token.begin())
		{
			stringit--;
			stringbuffer[n-1-i]=*stringit;
		}
		else
		{
			stringbuffer[n-1-i]="";
		}
	}
	
	for (int i=0; i<n; i++)
	{
		cout << stringbuffer[i] << ". ";
	}
	
	result=advancedsearch(stringbuffer, ngramsdata, nsug);
	cout << "\nSugestions: ";
	printstringlist(result);
}

else if (adv==false)
{
	Ngram result;
	string stringbuffer[n-1];
	for (int i=0; i<n-1; i++)
	{
		if (stringit!=token.begin())
		{
			stringit--;
			cout << *stringit;
			stringbuffer[n-2-i]=*stringit;
		}
		else
		{
			stringbuffer[n-2-i]="";
		}
	}
	
	for (int i=0; i<n-1; i++)
	{
		cout << stringbuffer[i] << ". ";
	}
	
	
	result=search(stringbuffer, ngramsdata, nsug);
	cout << "\nSugestions: ";
	printstringlist(result.tokenlast);
}
}


if (part == 1)
{	
	cout << "sorting ngrams\n";
	ngrams=sortngrams(ngrams);
	//printngramlist(ngrams);
	
	cout << "merging ngrams\n";
	ngrams=mergengrams(ngrams);
	//printngramlist(ngrams);
	
	cout << "saving ngrams\n";
	savengrams(ngrams, savepath);
}	

if (part==3 || part ==4)
{
	cout << "read data\n";
	ngramsdata = readdata(datapath);
	
	cout << "testing predictionrate\n";
	if (part==4){cout << "keystroke-rate\n"; advancedtest (ngramsdata, ngrams, nsug);}
	if (part==3){cout << "word-rate\n"; test (ngramsdata, ngrams, nsug);}
}

if (part==5)
{
	cout << "read data1\n";
	ngramsdata = readdata(datapath);
	printngramlist(ngramsdata);
	cout << "read data2\n";
	ngrams = readdata(textpath);
	cout << "merging data\n";
	ngramsdata = merge(ngramsdata, ngrams);
	cout << "saving data\n";
	savengrams(ngramsdata, savepath);
}

		return 0;
	
}
//********************************************************************************************************************




Ngram search(string searchtoken[n-1], list<Ngram> ngrams, int nsug)
{
	list<Ngram>::iterator searchit;
	int hits, i;
	
	if (ngrams.begin()==ngrams.end()){cout << "data of ngrams is empty";}
	
	for (searchit=ngrams.begin(); searchit!=ngrams.end(); searchit++)
	{
		hits =0;
		for (i = 0; i<n-1; i++)
		{
			if ( (*searchit).tokenbegin[i] == searchtoken[i])
			{
				hits++;
			}
		}
		
		if (hits==n-1)
		{
		//	(*searchit).printngram();
			return (*searchit);
		}
	}
	
	return Ngram();
};

list<string> advancedsearch(string searchtoken[n], list<Ngram> ngrams, int nsug)
{
	list<string> selection;
	list<string> sugestions;
	list<string>::iterator it;
	string::iterator stringit;
	string::iterator compit;
	int hits;
	
	string searcharray[n-1];
	
	//cout << searchtoken[n-1].size();
	
	for (int i=0; i<n-1; i++)
	{
		searcharray[i]=searchtoken[i];
	}
	sugestions=(search(searcharray, ngrams, nsug)).tokenlast;
	
	int resultcount=0;
	for (it=sugestions.begin(); it!=sugestions.end(); it++)
	{
	
		hits=0;
		compit=(*it).begin();
		stringit=searchtoken[n-1].begin();
		
		for(stringit=searchtoken[n-1].begin(); stringit!=searchtoken[n-1].end(); stringit++)
		{
			if (*stringit==*compit)
			{
				hits++;
			}
			compit++; //achtung: hier etwas machen, damit wort kürzer sein darf als zu suchende zeichenkette!
		}
		
		
		if (hits==searchtoken[n-1].size() && resultcount<10)
		{
			//cout << *it << "-";
			selection.push_back(*it);
			resultcount++;
		}
	}
	
	return selection;
};


Ngram lasttokenmerge (Ngram ngram, Ngram ngram2)
{
	//if (ngram.tokenbegin==ngram2.tokenbegin)
	{
		list<string>::iterator tokenit;
		list<int>::iterator tokencountit;
		list<string>::iterator token2it;
		list<int>::iterator tokencount2it;
		int found = 0;
		
		tokenit=ngram.tokenlast.begin();
		tokencountit=ngram.tokenlastcount.begin();
		//cout << "Begin merging lasttoken: \n";
		
		while (tokenit!=ngram.tokenlast.end() && tokencountit!=ngram.tokenlastcount.end())
		{
			token2it=ngram2.tokenlast.begin();
			tokencount2it=ngram2.tokenlastcount.begin();
			found=0;
			cout << "\n" << *tokenit << ": ";
			
			while (token2it!=ngram2.tokenlast.end() && tokencount2it!=ngram2.tokenlastcount.end())
			{
				//cout << *token2it << ", ";
				if (*tokenit==*token2it)
				{
					*tokencountit+=*tokencount2it;
					ngram2.tokenlast.erase(token2it);
					ngram2.tokenlastcount.erase(tokencount2it);
					//cout << " found!";
					found=1;
					break;
					
				}
				
				token2it++;
				tokencount2it++;
			}
			
			tokenit++;
			tokencountit++;
		}
		
		
	}
	while (ngram2.tokenlast.begin()!=ngram2.tokenlast.end())
	{
		
		ngram.tokenlast.push_back(ngram2.tokenlast.front());
		ngram.tokenlastcount.push_back(ngram2.tokenlastcount.front());
		cout << "added: " << (ngram2.tokenlast.front()) << ". ";
		ngram2.tokenlast.pop_front();
		ngram2.tokenlastcount.pop_front();
	}
	ngram.sortlasttoken();
	return ngram;
};
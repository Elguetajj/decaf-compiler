#include"scanner/Scanner.cpp"


// using namespace std;

int main(int argc, char **argv)
{
	int i;
	std::string outfile;
	std::string target;
	std::string options_stage;
	std::string debug_stage;
	Scanner scan;
	bool debug = 0;



    for (i = 1; i < argc; i++)
	{

		if (strcmp(argv[i], "-o")== 0) 
		{	
			outfile = argv[++i];
		}
		else if (strcmp(argv[i], "-target")== 0)
		{	
			target = argv[++i];
		}
		else if (strcmp(argv[i], "-opt")== 0)
		{	
			options_stage = argv[++i];
		}
		if (strcmp(argv[i], "-debug")== 0) 
		{	
			debug_stage = argv[++i];
		}
	}
	std::string source(argv[argc-1]);
	cout<<"target: "+target<<"\n";
	cout<<"source: "+source<<"\n";
	if(debug_stage=="scan"){
		debug = 1;
	}
	if(target=="scan"){
		scan.tokenize(argv[argc-1],debug);
	}
	// State* hola = rreg.compile("(a|b)*");
	// string a = "aabbc";
	// cout<<rreg.eval(hola,a)<<"\n";

	return 0;
}
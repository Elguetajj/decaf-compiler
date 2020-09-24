#include"scanner/Scanner.cpp"


// using namespace std;

int main(int argc, char **argv)
{
	Scanner scan;
    scan.tokenize(argv[1]);
    
	return 0;
}
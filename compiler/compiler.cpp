#include"scanner/Regx.cpp"

int main(int argc, char **argv)
{
	Regx re;
    bool x = re.eval(argv[1],argv[2]);
    cout<<x<<"\n";
	return 0;
}
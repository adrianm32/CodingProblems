#include <iostream>
#include <stack>

using namespace std;


void TowersOfHanoi(int n, char x /*start tower */, char y /*final tower */, char z /*intermediate tower */, int indent)
{
	if (n == 0) return;

	TowersOfHanoi(n - 1, x, z, y, indent);
	cout << "Move topmost disk from Tower " << x << " to " << y << "\n";
	TowersOfHanoi(n - 1, z, y, x, indent);
}

struct ToH
{
	int n = 0; // no. of disks
	char x; //start tower
	char y; //final tower
	char z; //intermediate tower

	ToH(int n, char x, char y, char z) : n(n), x(x), y(y), z(z)
	{}
};

void TowersOfHanoiWithoutRecursion(int n, char x /*start tower */, char y /*final tower */, char z /*intermediate tower */)
{
	stack<ToH> stack;
	ToH toh(0, ' ', ' ', ' ');

	stack.push(ToH(n, x, y, z)); //push current contents on stack


	while (!stack.empty())
	{
		toh = stack.top();
		stack.pop();
		n = toh.n; x = toh.x; y = toh.y; z = toh.z;

		if (n <= 1)
		{
			cout << "Move topmost disk from Tower " << x << " to " << y << "\n";
		}
		else
		{
			stack.push(ToH(n - 1, z, y, x)); // push right on stack on first so that it is pop'd last
			stack.push(ToH(-n, x, y, z));   // visit current & force it to print by setting  n to be less than 1.
			stack.push(ToH(n - 1, x, z, y));  // push left on stack last so that it is pop'd first.
		}
	}
}


int main()
{
	int n;
	cout << "Move all disks from tower X to tower Y using Tower Z as intermediate. ( X Y Z)\n";
	cout << "Please enter number of disks: ";
	cin >> n;

	printf("\n ToH With Recursion\n");
	TowersOfHanoi(n, 'x', 'y', 'z', 1);

	printf("\n ToH Without Recursion\n");
	TowersOfHanoiWithoutRecursion(n, 'x', 'y', 'z');

	cin >> n;

	return 0;

}
#include <iostream>

using namespace std;

void AddIndent(int indent)
{
	for (int i = 0; i < indent; i++)
	{
		printf("-");
	}
}

void Trace(int n, char x /*start tower*/, char y /*final tower */, char z /*intermediate tower*/, int indent)
{
	AddIndent(indent);
	printf("TOI(%i, %c, %c, %c)\n", n, x, y, z);
}


void TowersOfHanoi(int n, char x /*start tower*/, char y /*final tower */, char z /*intermediate tower*/, int indent)
{
	if (n == 0) return;

	Trace(n - 1, x, z, y, indent++);
	TowersOfHanoi(n - 1, x, z, y, indent);

	AddIndent(indent);
	cout << "Move topmost disk from Tower " << x << " to " << y << "\n";

	Trace(n - 1, z, y, x, indent);
	TowersOfHanoi(n - 1, z, y, x, indent);
}



int main()
{
	int n;
	cout << "Move all disks from tower X to tower Y using Tower Z as intermediate. ( X Y Z)\n";
	cout << "Please enter number of disks: ";
	cin >> n;

	Trace(n, 'x', 'y', 'z', 0);
	TowersOfHanoi(n, 'x', 'y', 'z', 1);

	cin >> n;

	return 0;

}
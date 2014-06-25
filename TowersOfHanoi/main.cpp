#include <iostream>

using namespace std;



void TowersOfHanoi(int n, char x /*start tower*/, char y /*final tower */, char z /*intermediate tower*/)
{
	if (n == 0) return;

	TowersOfHanoi(n - 1, x, z, y);
	cout << "Move topmost disk from Tower " << x << " to " << y << "\n";
	TowersOfHanoi(n - 1, z, y, x);
}

int main()
{
	int n;
	cout << "Please enter number of disks: ";
	cin >> n;

	TowersOfHanoi(n, 'x', 'y', 'z');

	cin >> n;

	return 0;

}
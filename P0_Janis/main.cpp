/*****************************************************************/
/* Author: Daniel Janis                                           /
/  Course: CS 4280                                                /
/  File: main.cpp                                                 /
/  Last Edit: 2/22/2020                                           /
/                                                                 /
/  main() checks how the file was invoked and uses tree.cpp and   /
/  traversals.cpp to create a binary tree and to traverse it      /
/  inorder, postorder, and preorder, including error detection.   /                      
/                                                                */
/*****************************************************************/

#include "tree.h"
#include "node.h"

int main(int argc, char *argv[]) {
	string fileName;
	// Creates an empty node, named root
	node *root = NULL;

    // Filename was given
    if (argc == 2) {
        fileName = argv[1];
		// Checks for file extension
		if (checkExtension(fileName) == 0) {
			cout << "There is no file extension.\n";
			fileName.append(".sp2020");
			cout << "Filename: " << fileName << "\n";
		}
		else {
			cout << "There is a file extension.\n";
			cout << "Filename: " << fileName << "\n";
		}
		ifstream infile(fileName.c_str());
		// If the file can't open, noFile takes input from keyb
		if (!infile) {
			cout << "File does not exist.\n";
			noFile(fileName.c_str());
		}
		else {
			cout << "Opening the file -> " << fileName << "\n";
			// seekg finds the "end" position of the file,
			// tellg returns the "end" position
			infile.seekg(0, ios::end);
			// If the file is empty, noFile takes input from keyb:
			if (infile.tellg() == 0) {
				noFile(fileName.c_str());
			}
		}
	}
	// Filename NOT given
	else if (argc == 1) {	
		// No file was provided with invokation,
		// Creates a temporary file to store the words	
		fileName = "output.sp2020";
		noFile(fileName.c_str());
	}
	// If too many argumetns were provided in the function call
	else {
		cout << "\nError, too many arguemnts, terminating...\n";
		return 1;
	}

	root = buildTree(fileName);

	// Tree did not build? Error checked in buildTree()
	if (root == NULL) {
		cout << "\nError, tree did not build, check that your file is not empty.\n";
		return 1;
	}	

	// Prints the traversals (fixing filenames as it goes)
	fileName = removeExtension(fileName);
	fileName.append(".inorder");
	ofstream inorder;
	inorder.open(fileName.c_str());
	cout << "Printing inorder:\n\n";
	printInorder(root, inorder);
	inorder.close();

	fileName = removeExtension(fileName);
	fileName.append(".preorder");
	ofstream preorder;
	preorder.open(fileName.c_str());
	cout << "\n\nPrinting preorder:\n\n";
	printPreorder(root, preorder);
	preorder.close();

	fileName = removeExtension(fileName);
	fileName.append(".postorder");
	ofstream postorder;
	postorder.open(fileName.c_str());
	cout << "\n\nPrinting postorder:\n\n";
	printPostorder(root, postorder);
	postorder.close();
	cout << '\n';
    return 0;
}

// Returns 1 if an extension exists, 0 if no extension exists.
int checkExtension(string fileName) {
	string dot = ".";
	// If the dot is at any position but -1 the extension exists
	if (fileName.find(dot) != -1) {
		return 1;
	}
	// Else the extension doesn't exist (no dot)
	else {
		return 0;
	}
}

// Returns 1 if file is empty,
int checkEmpty(string fileName) {
	ifstream infile(fileName.c_str());
	// seekg finds the "end" position of the file,
	// tellg returns the "end" position
	infile.seekg(0, ios::end);
	// Checks if the file is empty:
	if (infile.tellg() == 0) {
		return 1;
	}	
	return 0;
}

//Removes the file extension
string removeExtension(string fileName) {
	if (checkExtension(fileName) != 0) {
		// findDot is a value representing where in the string the dot is
		size_t findDot = fileName.find_last_of(".");
		return fileName.substr(0, findDot);
	}
	return fileName;
}

// Creates a file with the given fileName, or provides a temporary one
void noFile(string fileName) {
	string keyb_input;
	ofstream tempFile;
	tempFile.open(fileName.c_str());
	if (checkEmpty(fileName) == 1) {
		cout << "\nPlease enter alphanumerical strings, separated by spaces,\n"
			 << "and when you are finished hit CTRL+D to save the file.\n\n";
	}
	while (cin >> keyb_input) {
		tempFile << keyb_input << " ";
	}
	cout << "\n" << fileName << " has been saved.\n";	
	tempFile.close();
}

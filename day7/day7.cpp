// day7.cpp : Defines the entry point for the application.
//

#include "day7.h"

TreeNode preparePuzzleInput(string filename)
{
	TreeNode rootNode, *currentNode = &rootNode;

	rootNode.name = "/";

	ifstream myfile(filename);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			//command
			if (line[0] == '$')
			{
				string cmd = line.substr(2);
				//if command is ls go to next line
				if (cmd == "ls") continue;

				//else command is cd
				//if cd .. go up to parent node
				if (cmd.substr(3) == "..")
				{
					currentNode = currentNode->parentNode;
				}
				//if root go to next line as root is set
				else if (cmd.substr(3) == "/") { continue; }
				//else go down: find directory named and reset currentNode 
				else
				{
					for (int i = 0; i < currentNode->childNodes.size(); i++)
					{
						if (currentNode->childNodes.at(i).name == cmd.substr(3))
						{
							currentNode = &(currentNode->childNodes.at(i));
							break;
						}
					}
				}
			}
			//directory
			else if (isalpha(line[0]))
			{
			
				currentNode->childNodes.push_back(TreeNode(line.substr(4),currentNode));

			}
			//file
			else if (isdigit(line[0]))
			{
				//split size and name of file
				size_t pos = line.find(' ');
				string name = line.substr(pos + 1);
				int size = stoi(line.substr(0, pos));

				currentNode->filesInNode.push_back(tuple(name, size));
			}
		}

		myfile.close();
	}

	return rootNode;
}

int findAllDirectoriesUpToSize(TreeNode& node, int maxSize)
{
	int sumSize = 0;
	int folderSize = 0;

	for (auto& [name, size] : node.filesInNode)
	{
		folderSize += size;
	}

	//get size of all subfolders and check all subfolders recursively
	for (int i = 0; i < node.childNodes.size(); i++)
	{
		folderSize += node.childNodes.at(i).getTotalFolderSize();
		sumSize += findAllDirectoriesUpToSize(node.childNodes.at(i),maxSize);
	}

	//if total folder size (incl. subfolders) is < maxSize, add to summary 
	if (folderSize <= maxSize) sumSize += folderSize;

	//cout << "Folder " << node.name << " Size: " << folderSize << "\n";

	return sumSize;
}

vector<int> findDirectorySizes(TreeNode& node)
{
	int folderSize = 0;
	vector<int> subFolderSizes;

	for (auto& [name, size] : node.filesInNode)
	{
		folderSize += size;
	}

	//get size of all subfolders and check all subfolders recursively
	for (int i = 0; i < node.childNodes.size(); i++)
	{
		folderSize += node.childNodes.at(i).getTotalFolderSize();

		vector<int> dummy= findDirectorySizes(node.childNodes.at(i));
		subFolderSizes.insert(subFolderSizes.end(), dummy.begin(), dummy.end());
	}
	subFolderSizes.push_back(folderSize);

	return subFolderSizes;
}

int findSizeOfDirectoryToDelete(TreeNode& root, int systemSize, int neededSpace)
{
	int currentSize = root.getTotalFolderSize();
	int currentFree = systemSize - currentSize;
	int deletedObjectSize = currentSize;

	if (currentFree >= neededSpace) return 0;

	int extraNeededSpace = neededSpace - currentFree;

	vector<int> folderSizes = findDirectorySizes(root);
	
	for (int size : folderSizes)
	{
		if (size > extraNeededSpace && size < deletedObjectSize)
		{
			deletedObjectSize = size;
		}
	}

	return deletedObjectSize;
}

int main()
{
	TreeNode testFileSystem = preparePuzzleInput("puzzleTest.txt");
	TreeNode puzzleFileSystem = preparePuzzleInput("puzzleInput.txt");

	//testFileSystem.writeFolderContents(0);
	int maxFolderSize = 100000;
	cout << "Part 1:\n";
	cout << "Sum of size of all directories with size < " << maxFolderSize << " (test): ";
	cout << findAllDirectoriesUpToSize(testFileSystem, maxFolderSize) << " .\n";

	cout << "Sum of size of all directories with size < " << maxFolderSize << " (puzzle): ";
	cout << findAllDirectoriesUpToSize(puzzleFileSystem, maxFolderSize) << " .\n";

	int systemSize = 70000000;
	int updateSize = 30000000;
	cout << "\n Part2:\n";
	cout << "Size of directory to delete (test): " << findSizeOfDirectoryToDelete(testFileSystem,systemSize,updateSize) << ".\n";
	cout << "Size of directory to delete (puzzle): " << findSizeOfDirectoryToDelete(puzzleFileSystem, systemSize, updateSize) << ".\n";

	return 0;
}

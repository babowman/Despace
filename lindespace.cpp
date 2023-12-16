#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <cassert>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	// check args for file path
	std::string PATH;
	if (argc > 1)
	{
		PATH = argv[1];
	}else{
		PATH = ".";
	}

	struct stat sb;

	if (stat(PATH.c_str(), &sb) != 0)
	{
		std::cout << "Error: The path specified does not exist." << std::endl;
		exit(1);
			
	}
	std::vector<std::string> dirList; // declare vector for storing contents of directory
	DIR *dir;
	struct dirent *ent;

	// get list of all directory contents
	if ((dir = opendir (PATH.c_str())) != NULL) {
		  while ((ent = readdir (dir)) != NULL) 
		  	{
		  	dirList.push_back(ent->d_name);
			}
		      	closedir (dir);
	} else {
		  /* could not open directory */
		  perror ("");
		    return EXIT_FAILURE;
	}

	// loop through list of contents and rename those that contain spaces in the filename
	int numChanges = 0;
	std::vector<std::string> changes_vct;
	for (auto ele : dirList)
	{
		if (ele.find(" ") != std::string::npos){
			int i = 0;
			int chr_count = 0;
			for (auto chr : ele)
			{
				if (chr == ' '){
					chr_count++;					
					if (chr_count == 1)
					{
						numChanges++;
						changes_vct.push_back(ele);
					}
					std::string oldName = PATH + "/" + ele;
					ele[i] = '_';
					std::string newName = PATH + "/" + ele;
					rename(oldName.c_str(), newName.c_str());
				}
				
				// append all string data into one variable and then c_str()
				i++;
			}

		}
	}
	std::cout << std::endl;
	void assert(numChanges >= 0); // make sure the impossible has not happened
	if (numChanges > 1){
		std::cout << numChanges << " changes have been made:" << std::endl;
		for (auto chg : changes_vct){
			std::cout << "  " << chg << std::endl;
		}
	}else if (numChanges == 1){
		std::cout << "1 change has been made:" << std::endl;	
		std::cout << "  " << changes_vct[0] << std::endl;
	}else{
		std::cout << "No changes to be made in this directory." << std::endl;
	}
	std::cout << std::endl;
	return 0;
}

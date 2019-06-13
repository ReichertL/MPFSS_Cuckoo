#include <vector> 
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iostream>
#include <algorithm>

extern "C"{
    #include <sys/types.h>
    #include <sys/stat.h>
}

using namespace std;



/*  For benchmarking purposes. 
    Creates/ appends to a file the run time of the execution.
    Files are stored in the subfolder "benchmark".
*/
void benchmark_list(string type, int len_of_lists, std::vector<string> list_of_names, std::vector<string> list_of_values){

		string t=list_of_values.at(1);
		string size=list_of_values.at(2);
        std::ostringstream oss;
        oss << "benchmark/" << type << "/results_t:" <<t<<"_size:"<<size;
        std::string filename = oss.str();
        
        std::ostringstream oss2;
        oss2 << "benchmark/" << type;
        std::string path = oss2.str();

        time_t rawtime;
        struct tm * timeinfo;
        char current_time[80];
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        strftime (current_time,80,"%d.%m.%Y-%H:%M",timeinfo);
        std::string str_current_time(current_time);

        //create dir if not exist
        struct stat myStat;
        if (!((stat(path.c_str(), &myStat) == 0) && (((myStat.st_mode) & S_IFMT) == S_IFDIR))) {

            //splits path
            std::replace(path.begin(), path.end(), '/', ' '); 
            vector<string> array;
            stringstream ss(path);
            string temp;
            while (ss >> temp)
                array.push_back(temp);
            string part;
            for (int i = 0; i < (int) array.size(); ++i){
                part.append(array.at(i));
                part.append("/");
                const int dir_err = mkdir(part.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                if (-1 == dir_err){
                    printf("Error creating directory %s!\n", part.c_str());
                    //exit(1);
                }
            }
        }

        fstream file;
        file.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);


      // If file does not exist, Create new file
        if (!file ){
            file.open(filename,  fstream::in | fstream::out | fstream::trunc);
        }

        file.seekg(0, ios_base::end);
        int len = file.tellg();
        if (0 == len) {
            file<<"Current Time";
            for (int i = 0; i < len_of_lists; ++i){
                file <<","<<list_of_names.at(i);
            }
            file <<"\n";
        }

        file<<current_time;
        for(int i=0;i<len_of_lists;i++){
            file<<","<<list_of_values.at(i);
        }
        file <<"\n";
        file.close();

}
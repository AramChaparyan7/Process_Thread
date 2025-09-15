#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <filesystem>
#include <fstream>

void process(const std::string& fn) {
    std::ifstream file(fn);
    std::string line;
    int count = 0;
    int error_500 = 0;
    double time = 0;
    while(getline(file, line)) {
        std::stringstream ss(line);
        std::string error;
        std::string value;
        ss >> error >> value;
        if(stoi(error) == 500) {
            ++error_500;
        }
        ++count;
        time += stod(value);
    }
    std::string fname = fn.substr(0, fn.size() - 4) + "_result.json";;
    std::ofstream out(fname);
    out << "{\n" << "file: " << fn << ",\n";
    out << "total_requests: " << count << ",\n";
    out << "https_500_errors: " << error_500 << ",\n";
    out << "tavg_response_time: " << time/count << ",\n";
    out << "}";
    out.close();
    file.close();
}

int main() {
    std::vector<std::string> files;
    DIR* directory = opendir("./logs");
    std::filesystem::path path{"./logs"};
    for(const auto& entity : std::filesystem::directory_iterator(path)) {
        if(entity.is_regular_file()) {
            std::string fn = entity.path().filename().string();
            if(fn.substr(0, 3) == "Log" && fn.substr(fn.size() - 4, 4) == ".txt") {
                files.push_back(fn);
            }
        }
    }
    for(int i = 0; i < files.size(); ++i) {
        pid_t pid = fork();
        if(pid < 0) {
            return -1;
        }
        if(pid == 0) {
            process("./logs/" + files[i]);
            return 0;
        }
    }
    for(int i = 0; i < files.size(); ++i) {
        wait(nullptr);
    }
    int count_all = 0;
    int error_all = 0;
    double time_all = 0;
    for(int i = 0; i < files.size(); ++i) {
        std::string fname = "./logs/" +files[i].substr(0, files[i].size() - 4) + "_result.json";;
        std::ifstream in(fname);
        std::string line;
        std::string tmp;
        int count = 0;
        int error = 0;
        double time = 0;
        getline(in, line);
        getline(in, line);
        getline(in, line);
        std::stringstream ss1(line);
        ss1 >>tmp >>count;
        getline(in, line);
        std::stringstream ss2(line);
        ss2 >>tmp >>error;
        getline(in, line);
        std::stringstream ss3(line);
        ss3 >>tmp >>time;
        count_all += count;
        error_all += error;
        time_all += time;
        std::ofstream out("./logs/results.txt");
        out << "Aggregated results" << ",\n";
        out << "Total_requests: " << count_all << ",\n";
        out << "Https_500_errors: " << error_all << ",\n";
        out << "Avg_response_time: " << time_all/count_all << ",\n";
    }
    return 0;
}
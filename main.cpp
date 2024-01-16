#include <iostream>
#include <fstream>
#include <cstring>

struct Arguments {
    int lines = -1;
    char delimiter;
    bool tail = false;
    std::string file_name;
    ~Arguments() = default;
};

std::string HelpMessage(){
    return "Usage: PrintFile [OPTION]... [FILE]...\n"
           "Print the content of FILE to standard output.\n\n"
           "  -l, --lines=NUMBER     output the last NUMBER lines, instead of the last 10\n"
           "  -d, --delimiter=CHAR   use CHAR instead of newline as the record delimiter\n"
           "  -t, --tail=BOOL        output the file end\n"
           "  -h, --help             display this help\n";
}

int CountLines(const std::string& filename){
    std::ifstream fin(filename);
    int counter = 0;
    char ch;
    while (fin.get(ch)){
        if (ch == '\n'){
            counter++;
        }
    }
    return counter + 1;
}

Arguments ParsArgs (int &count_args, char* argv[], Arguments &args) {
    args.file_name = argv[count_args - 1];
    if (count_args < 2) {
        std::cerr << HelpMessage();
    } else {
        for (int i = 1; i < count_args; ++i) {
            if (strcmp(argv[i], "-l") == 0 or strcmp(argv[i], "--lines") == 0) {
                if (argv[i + 1] == "-d" or argv[i + 1] == "--delimiter"
                    or argv[i + 1] == "-t" or argv[i + 1] == "--tail" or i == count_args - 1) {
                    std::cerr << "missing value of argument ==> lines";
                    std::cerr << HelpMessage();
                } else {
                    args.lines = std::stoi(argv[i + 1]);
                }
            } else if (strcmp(argv[i], "-d") == 0 or strcmp(argv[i], "--delimiter") == 0) {
                if (argv[i + 1] == "-l" or argv[i + 1] == "--lines"
                    or argv[i + 1] == "-t" or argv[i + 1] == "--tail" or i == count_args - 1) {
                    std::cerr << "missing value of argument ==> delimiter";
                    std::cerr << HelpMessage();
                } else {
                    args.delimiter = argv[i + 1][0];
                }
            } else if (strcmp(argv[i], "-tail") == 0 or strcmp(argv[i], "--tails") == 0) {
                if (argv[i + 1] == "-l" or argv[i + 1] == "--lines"
                    or argv[i + 1] == "-d" or argv[i + 1] == "--delimiter") {
                    std::cerr << "missing value of argument ==> tail";
                    std::cerr << HelpMessage();
                } else {
                    args.tail = true;
                }
            }
        }
    }
    return args;
}

void PrintFile(Arguments &args) {
    std::ifstream fin(args.file_name);
    if (!fin.is_open()) {
        std:: cerr << "File " << args.file_name << " not found." << std::endl;
        return;
    }
    int total_lines = CountLines(args.file_name);
    if (total_lines == args.lines or args.lines == -1) {
        args.tail = false;
        args.lines = total_lines;
    }
    char ch;
    bool n_flag = true;
    bool n_flag_2 = false;
    int lines_counter = 0;
    while (fin.get(ch)) {
        if (ch == '\n'){
            n_flag = true;
            ++lines_counter;
        }
        if (args.tail) {
            if (lines_counter + 2 > total_lines - args.lines){
                if (n_flag){
                    if (n_flag_2){
                        std::cout << ch;}
                    n_flag_2 = true;
                }
                if (ch == args.delimiter) {
                    n_flag = false;
                }
            }
        } else {
            if (n_flag){
                std::cout << ch;
            }
            if (ch == args.delimiter) {
                n_flag = false;
            }
            if (lines_counter == args.lines){
                break;
            }
        }
    }
    fin.close();
}

int main(int argc, char* argv[]) {
    if (strcmp(argv[1], "-h") == 0 or strcmp(argv[1], "--help") == 0) {
        std::cout << HelpMessage();
        return 0;
    }
    Arguments arguments;
    arguments = ParsArgs(argc, argv, arguments);
    PrintFile(arguments);
    return 0;
}

#include <iostream>
#include <string.h>
#include <sys/stat.h>

using namespace std;

string exec(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

string getParentFolder(string path) {
    return path.substr(0,path.find_last_of('/')+1);
}

string getFullName(string path) {
    int pos = path.find_last_of("/");
    return path.substr(pos+1);
}

string getFileName(string fullName) {
    return fullName.substr(0,fullName.find_last_of("."));
}

string getFileExtention(string fullName) {
    int dotPos = fullName.find_last_of(".");
    if (dotPos == string::npos) return "";
    else return fullName.substr(dotPos+1);
}

bool isFile (struct stat info) {
    return S_ISREG(info.st_mode);
}

string isHiddenFile (string fullName) {
    if (fullName[0] == '.') {
        return "hidden";
    }
    else return "not hidden";
}

string readPermission(struct stat info) {
    mode_t tmp = info.st_mode;
    string ow = "(", gr = "(", ot = "(";
    if (tmp & S_IRUSR) ow += " read";
    if (tmp & S_IWUSR) ow += " write";
    if (tmp & S_IXUSR) ow += " execute";
    if (ow == "(") ow += " no permission )";
    else ow += " )";

    if (tmp & S_IRGRP) gr += " read";
    if (tmp & S_IWGRP) gr += " write";
    if (tmp & S_IXGRP) gr += " execute";
    if (gr == "(") gr += " no permission )";
    else gr += " )";

    if (tmp & S_IROTH) ot += " read";
    if (tmp & S_IWOTH) ot += " write";
    if (tmp & S_IXOTH) ot += " execute";
    if (ot == "(") ot += " no permission )";
    else ot += " )";
    return ow + "," + gr + "," + ot;
}

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

void printProperty(short y, string title, string option, string path) {
    gotoxy(0,y); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
    gotoxy(0,y+1); cout << "| " << title;
    gotoxy(40,y+1); cout << "| " << exec(("stat -c '%"+option+"' "+path).c_str());
    gotoxy(140,y+1); cout << "|";
    gotoxy(0,y+2); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
}

void printProperty(short y, string title, string value) {
    gotoxy(0,y); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
    gotoxy(0,y+1); cout << "| " << title;
    gotoxy(40,y+1); cout << "| " << value;
    gotoxy(140,y+1); cout << "|";
    gotoxy(0,y+2); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
}

int run() {
    cout << "\e[8;60;150t";


    system("clear");
    cout << "Enter path: ";
    string path;
    getline(cin, path);


    char pathArr[path.length()+1];
    strcpy(pathArr,path.c_str());


    struct stat info;
    stat(pathArr,&info);


    if (isFile(info)) {
        printProperty(2,"Mount point","m",path);
        printProperty(4,"Parent folder",getParentFolder(path));
        printProperty(6,"Name",getFileName(getFullName(path)));
        printProperty(8,"Type",getFileExtention(getFullName(path)));
        printProperty(10,"Symbolic link","N",path);
        printProperty(12,"Permission in octal","a",path);
        printProperty(14,"Permission in human readable form","A",path);
        printProperty(16,"Permission of owner, group, others",readPermission(info));


        gotoxy(0,18); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(0,19); cout << "| Total size in bytes";
        gotoxy(40,19); cout << "| " << info.st_size << info.st_size;
        gotoxy(140,19); cout << "|";


        gotoxy(0,20); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(0,21); cout << "| Number of blocks allocated";
        gotoxy(40,21); cout << "| " << info.st_blocks;
        gotoxy(140,21); cout << "|";


        gotoxy(0,22); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(0,23); cout << "| The size in bytes of each block";
        gotoxy(40,23); cout << "| " << info.st_blksize;
        gotoxy(140,23); cout << "|";

        
        printProperty(24,"Device number in decimal","d",path);
        printProperty(26,"Device number in hex","D",path);
        printProperty(28,"Raw mode in hex","f",path);


        gotoxy(0,30); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(0,31); cout << "| UID of owner";
        gotoxy(40,31); cout << "| " << info.st_uid;
        gotoxy(140,31); cout << "|";


        printProperty(32,"Username of owner","U",path);


        gotoxy(0,34); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(0,35); cout << "| Group ID";
        gotoxy(40,35); cout << "| " << info.st_gid;
        gotoxy(140,35); cout << "|";


        printProperty(36,"Group name","G",path);
        printProperty(38,"Showed/hidden",isHiddenFile(getFullName(path)));


        gotoxy(0,40); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(0,41); cout << "| Number of hard links";
        gotoxy(40,41); cout << "| " << info.st_nlink;
        gotoxy(140,41); cout << "|";


        gotoxy(0,42); cout << "--------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(0,43); cout << "| Inode number";
        gotoxy(40,43); cout << "| " << info.st_ino;
        gotoxy(140,43); cout << "|";


        printProperty(44,"Optimal I/O transfer size hint","o",path);
        printProperty(46,"Time of file birth","w",path);
        printProperty(48,"Time of last access","x",path);
        printProperty(50,"Time of last modification","y",path);
        printProperty(52,"Time of last status change","z",path);
        return 55;
    }
    else {
        gotoxy(0,2);
        cout << "This path doesn't lead to regular file!" << endl;
        return 3;
    }
}

int main(int argc, char const *argv[])
{
    char c;
    int pos;
    do {
        pos = run();
        gotoxy(0,pos); cout << "Do you want to continue? (y/n): ";
        cin >> c;
        cin.ignore();
    } while(c == 'y');
    return 0;
    run();
}
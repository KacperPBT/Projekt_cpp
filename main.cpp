#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;
//30 odczytow na linijke

vector<string> getData(string);

int strCount(string, char);

bool validateIn(string);

struct mtime{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
};

struct mdata{
    mtime dtime; //(starting) minute = 0 czas(00 ; 05]
    float av_U;
    float av_I;
};

mdata calculateAV();
mtime timePars(string);

int main() {
    //vector<string> data = getData("data.json");
    //calculateAV();
    return 0;
}

mdata calculateAV() {
    vector<string> data = getData("data.json");
    string line;
    string single_data;
    mtime start_time_list[data.size()]
    mtime end_time_list[data.size()]
    mdata secound_data[data.size() * 30]
    for (int i = 0; i < data.size(); i++) {
        line = data[i];
        if (validateIn(line)) {
            line = line.substr(49);
            ParsLine(line) obj;

        }
    }
    mdata x;
    return x;
}
// pobiera informacjie z pliku
vector<string> getData(string file_name) {
    vector<string> content;
    ifstream f(file_name);
    if (!f) {
        cerr << "Blad przy wczytywaniu pliku" << endl;
        return content;
    }
    string line;
    while (getline(f, line)) {
        content.push_back(line);
    }
    f.close();
    return content;
}
// waliduje linie danych z pliku
bool validateIn(string str) {
    int x = strCount(str, '{');
    int y = strCount(str, '}');
    if (x==y&&x==32){
        return true;
    }else{
        cout << "Znaleziono niepelna linie" << endl;
        return false;
    }
}
// zlicza wystapienia <target> w podanym stringu
// używany do walidacji informacji
int strCount(string str, char target) {
    return count(str.begin(), str.end(), target);
}
// przeksztalca dane z lini pliku na informacje zapisane w strukturze mdata
mdata dataPars(string data) {
    mdata outdata;
    outdata.dtime = timePars(data.substr(0,14));
    outdata.av_U = stof(data.substr(21,4));
    outdata.av_I = stof(data.substr(31,4));
    return outdata;
}
// rozdziela stringa "YYYYMMDDHHmmSS" i zapisuje w strukturze mtime
mtime timePars(string data) {
    mtime otime;
    otime.year = stoi(data.substr(0,4));
    otime.month = stoi(data.substr(4,2));
    otime.day = stoi(data.substr(6,2));
    otime.hour = stoi(data.substr(8,2));
    otime.minute = stoi(data.substr(10,2));
    otime.second = stoi(data.substr(12,2));
    return otime;
}

class ParsLine {
    private:
        string line;
    public:
        mtime start_time;
        mtime end_time;
        mdata data[30];

        ParsLine(string line) {
            this->line = line;
            if (validateIn(line)){
                line = line.substr(49);
                string data_list[30];
                SeperateData();
            }
        }

        void SeperateData() {
            string s_data[30];
            for (int i = 0; i <30; i++){
                sepe_data = line.substr(0 + 39*i, 36);
                data[i] = dataPars(sepe_data);
            }
        }
};

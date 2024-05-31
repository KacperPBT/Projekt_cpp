#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
//30 odczytow na linijke NIE PRAWDA
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
//pojedyncze dane
struct mdata{
    mtime dtime;
    float av_U;
    float av_I;
};
//dane z 5 minut
struct m5data{
    mtime m5time;
    float av_U = 0;
    float av_I = 0;
};
//dane z 30 minut
struct m30data{
    mtime m30time;
    m5data data[6];
    float av_U = 0;
    float av_I = 0;
};
//dane z godziny
struct hdata{
    mtime htime;
    m30data data[2];
    float av_U = 0;
    float av_I = 0;
};

class CalculateAV;

bool checkTimeRange (mtime, mtime, mtime);

vector<string> getData(string);

bool validateIn(string);

int strCount(string, char);

mdata dataPars(string);

mtime timePars(string);

//class ParsLine(string);

int main() {
    //vector<string> data = getData("data.json");
    //calculateAV();
    return 0;
}

class CalculateAV {
    public:
        vector<string> data = getData("data.json");
        string line;
        string single_data;
        //mtime start_time_list[data.size()];
        //mtime end_time_list[data.size()];
        vector<mdata> secound_data;
        vector<hdata> av_data;
        CalculateAV() {
            for (int i = 0; i < data.size(); i++) {
                line = data[i];
                if (validateIn(line)) {
                    line = line.substr(49);
                    ParsLine(line) obj;
                    //start_time_list[i] = obj.start_time;
                    //end_time_list[i] = obj.end_time;
                    for (int j = 0; j <30; j++) {
                        secound_data.push_back(obj.data[j]);
                    }
                }
            }
        }
        void calculate() {
            hdata hour;
            hour.htime = start_time_list[0];
            hour.htime.minute = 0;
            hour.htime.second = 0;
            int i = start_time_list[0].minute / 5;
            mtime time_s_range = hour.htime;
            mtime time_e_range;
            vector<float> U;
            vector<float> I;
            m30data half_hour;
            half_hour.m30time = hour.htime;
            float suma_30min_U = 0;
            float suma_30min_I = 0;
            int k = 0;
            for (i<6; i++) {
                time_s_range.minute = 5 * i;
                time_e_rang = time_s_range;
                time_e_range.minute += 4;
                time_e_range.second = 5;
                mdata x = secound_data[0].value;
                while (checkTimeRange(time_s_range, time_e_range, x.dtime)) {
                    U.push_back(x.av_U);
                    I.push_back(x.av_I);
                    secound_data.erase(secound_data.begin());
                    x = secound_data[0].value;
                }
                float suma_U = 0;
                float suma_I = 0;
                for (int j = 0; j < U.size(); j++) {
                    suma_U += U[j].value;
                    suma_I += I[j].value;
                }
                m5data minutes5;
                minutes5.m5time = time_s_range;
                minutes5.av_U = suma_U / U.size();
                minutes5.av_I = suma_I / I.size();
                half_hour.data[i] = minutes5;
                suma_30min_U += minutes5.av_U;
                suma_30min_I += minutes5.av_I;
                k++;
            }
            if (k > 0) {
                half_hour.av_U = suma_30min_U / k;
                half_hour.av_I = suma_30min_I / k;
            }
            hour.data[0] = half_hour;
            for (i<12; i++) {
                time_s_range.minute =
            }
        }
};

bool checkTimeRange (mtime start_range, mtime end_range, mtime to_check) {
    if (to_check.year >= start_range.year && to_check.year <= end_range.year) {
        if (to_check.month >= start_range.month && to_check.month <= end_range.month) {
            if (to_check.day >= start_range.day && to_check.day <= end_range.day) {
                if (to_check.hour >= start_range.hour && to_check.hour <= end_range.hour) {
                    if (to_check.minute >= start_range.minute && to_check.minute <= end_range.minute) {
                        if (to_check.second >= start_range.second && to_check.second <= end_range.second) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
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
    if (x==y){
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
        //mtime start_time;
        //mtime end_time;
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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
//30 odczytow na linijke NIE PRAWDA
//============================================================================== struktury
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
//============================================================================== [2]Pobiera dane z pliku
vector<string> getData(string file_name) {
    vector<string> content;
    ifstream f(file_name);
    if (!f) {
        cerr << "Blad przy wczytywaniu pliku" << endl;
        return content;
    }
    string line;
    while (getline(f, line)) {
        if (line != ""){
            content.push_back(line);
        }
    }
    f.close();
    return content;
}
//============================================================================== [4]Zlicza wystapienia chara w stringu
int strCount(string str, char target) {
    return count(str.begin(), str.end(), target);
}
//============================================================================== [3]Waliduje czy linia jest kapletna
bool validateIn(string str) {
    //cout << str << endl;
    int x = strCount(str, '{');
    int y = strCount(str, '}');
    if (x==y){
        return true;
    }else{
        cout << "Znaleziono niepelna linie" << endl;
        return false;
    }
}
//============================================================================== [7]Parsuje date "YYYYMMDDHHmmSS" w strukture mtime
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
//============================================================================== [6]Parsuje pojedynczy pomiar w strukture mdata
mdata dataPars(string data) {
    mdata outdata;
    outdata.dtime = timePars(data.substr(0,14));
    outdata.av_U = stof(data.substr(21,4));
    outdata.av_I = stof(data.substr(31,4));
    return outdata;
}
//============================================================================== Sprawdza czy czas miesci sie w przedziale
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
//============================================================================== [5]Parsuje linie
class ParsLine {
    private:
        string line;
    public:
        //mtime start_time;
        //mtime end_time;
        int mesurments;
        mdata data[30];
        ParsLine(string line) {
            this->line = line;
            mesurments = strCount(line, '{');
            line = line.substr(49);
            string data_list[30];
            SeperateData();
        }
        void SeperateData() {
            string s_data[30];
            for (int i = 0; i <mesurments; i++){
                s_data[i] = line.substr(0 + 39*i, 36);
                data[i] = dataPars(s_data[i]);
            }
        }
};
//============================================================================== [1]
class CalculateAV {
    public:
        vector<string> data = getData("data.json");                           // Pobrane dane z pliku
        string line;
        string single_data;
        vector<mdata> secound_data;
        vector<hdata> av_data;
        CalculateAV() {
            for (int i = 0; i < data.size(); i++) {                           // Wchodzi w petle w ktorej operuje pojedynczymi liniami
                line = data[i];
                if (validateIn(line)) {                                       // Sprawdza poprawnosc lini
                    line = line.substr(49);
                    ParsLine obj(line);                                       // Parsuje linie (juz bez naglowka)
                    for (int j = 0; j <obj.mesurments; j++) {                 // Pobiera z klasy dane ktore byly w lini tej iteracji
                        secound_data.push_back(obj.data[j]);

                    }
                }
            }
            calculate();
        }
        void calculate() {
            int i = 0;
            int j;
            int k;
            int magic = 0;
            while (true) {                                                    // Petla odpowiadajaca za godziny
                if (i == secound_data.size()) {
                    break;
                }
                mdata x = secound_data[i];
                hdata hour;
                hour.htime = x.dtime;
                hour.htime.minute = 0;
                hour.htime.second = 0;
                k = x.dtime.minute / 5;
                j = k/6;
                int count30 = 0;
                float suma_U = 0;
                float suma_I = 0;
                for (j=j; j < 2; j++){                                          // Petla odpowiadajaca za 30 minut
                    m30data half_hour;
                    half_hour.m30time = x.dtime;
                    half_hour.m30time.minute = j * 30;
                    half_hour.m30time.second = 0;
                    float suma_30m_U = 0;
                    float suma_30m_I = 0;
                    if (k > 5) {
                        k-= 6;
                        magic = 6;
                    }
                    int count5 = 0;
                    for(k=k; k < 6; k++){                                       // Petla odpowiadajaca za 5 minut
                        m5data minutes5;
                        minutes5.av_U = 0;
                        minutes5.av_I = 0;
                        minutes5.m5time = x.dtime;
                        minutes5.m5time.minute = (k + magic) * 5;
                        minutes5.m5time.second = 0;
                        mtime end_time = minutes5.m5time;
                        end_time.minute += 4;
                        end_time.second += 59;
                        float suma_5m_U = 0;
                        float suma_5m_I = 0;
                        int count1 = 0;
                        while(checkTimeRange(minutes5.m5time, end_time, x.dtime)) {
                            suma_5m_U += x.av_U;
                            suma_5m_I += x.av_I;
                            i++;
                            count1++;
                            if (i != secound_data.size()){
                                x = secound_data[i];
                            } else {
                                break;
                            }
                        }
                        if (count1 != 0){
                            minutes5.av_U = suma_5m_U/count1;
                            minutes5.av_I = suma_5m_I/count1;
                            suma_30m_U += minutes5.av_U;
                            suma_30m_I += minutes5.av_I;
                        }

                        half_hour.data[k] = minutes5;
                        if (minutes5.av_U != 0){
                            count5+= 1;
                        }
                    }
                    magic = 0;
                    if (count5 != 0){
                        half_hour.av_U = suma_30m_U/count5;
                        half_hour.av_I = suma_30m_I/count5;
                        suma_U += half_hour.av_U;
                        suma_I += half_hour.av_I;
                    }

                    count30++;
                    hour.data[j] = half_hour;
                }
                if (count30 != 0){
                    hour.av_U = suma_U/count30;
                    hour.av_I = suma_I/count30;
                }
                av_data.push_back(hour);
            }
        }

        void print(string arg) {

            for (int i = 0; i < av_data.size(); i++){
                cout << endl << av_data[i].htime.year << "." << av_data[i].htime.month << "." << av_data[i].htime.day << " " << av_data[i].htime.hour << ":00:00" << endl << endl << "==========" << endl;;
                if (arg == "h"){
                    cout << "U: " << av_data[i].av_U << endl;
                    cout << "I: " << av_data[i].av_I << endl;
                    cout << "==========" << endl;
                } else {
                    for (int j = 0; j < 2; j++) {
                        if (arg == "m30"){
                            if (av_data[i].data[j].av_U != 0) {
                                cout << "U: " << av_data[i].data[j].av_U << endl;
                                cout << "I: " << av_data[i].data[j].av_I << endl;
                                cout << "==========" << endl;
                            }
                        } else {
                            for (int k = 0; k < 6; k++) {
                                if (arg == "m5"){
                                    if (av_data[i].data[j].data[k].av_U != 0) {
                                        cout << "U: " << av_data[i].data[j].data[k].av_U << endl;
                                        cout << "I: " << av_data[i].data[j].data[k].av_I << endl;
                                        cout << "==========" << endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
};
//==============================================================================
int main(int argc, char *argv[]) {
    if (argc = 3) {
        if (argv[1] == "-t") {
            if 
        }
    }
    CalculateAV obj;
    obj.print("m5");
    return 0;
}

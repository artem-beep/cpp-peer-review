#include <vector>
#include <set>
#include <numeric>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class ReadingMotivation
{
public:
    ReadingMotivation() = default;
    void Read(int user_id, int page)
    {
        unique_reader_ids_.insert(user_id);
        if (id_to_page_[user_id] == 0)
        {
            id_to_page_[user_id] = page;
            page_to_numb_of_pers_[page] += 1;
        }
        else
        {
            auto prev_page = id_to_page_[user_id];
            page_to_numb_of_pers_[prev_page] -= 1;

            id_to_page_[user_id] = page;
            page_to_numb_of_pers_[page] += 1;
        }
    }

    double Cheer(int user_id)
    {
        double result;
        auto curr_page = id_to_page_[user_id];
        if (curr_page != 0)
        {
            if (unique_reader_ids_.size() == 1)
            {
                return 1;
            }
            int sum_of_readers = accumulate(page_to_numb_of_pers_.begin(), page_to_numb_of_pers_.begin() + curr_page, 0);
            result = sum_of_readers * 1.0 / (unique_reader_ids_.size() - 1) * 1.0;
            return result;
        }
        else
        {
            return 0;
        }
    }

private:
    vector<int> id_to_page_ = vector(1000000, 0);
    vector<int> page_to_numb_of_pers_ = vector(1000, 0);
    set<int> unique_reader_ids_;
};

int main()
{
    string numb_of_requests;
    getline(cin, numb_of_requests);
    ReadingMotivation readmot;
    for (int counter = 0; counter < stoi(numb_of_requests); counter++)
    {
        string query;
        getline(cin, query);
        if (query[0] == 'R')
        {
            auto args_joined = query.substr(5);
            int index = args_joined.find(' ');
            int user_id = stoi(string{args_joined.begin(), args_joined.begin() + index});
            int page = stoi(string{args_joined.begin() + index, args_joined.end()});
            readmot.Read(user_id, page);
        }
        else
        {
            int user_id = stoi(query.substr(6));
            cout << setprecision(6) << readmot.Cheer(user_id) << endl;
        }
    }
}
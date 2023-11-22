#include <vector>
#include <string_view>
#include <string>

using namespace std;

struct Person
{
    string name;
    int age;
};

// Структура необходимых данных для загрузки. Выбрана из соображений масштабируемости.
struct LoadingInfo
{
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
    int min_age;
    int max_age;
    string_view name_filter;
};

vector<Person> LoadPersons(LoadingInfo &info)
{
    DBConnector connector(info.db_allow_exceptions, info.db_log_level);
    DBHandler db;
    if (info.db_name.starts_with("tmp."s))
    {
        db = connector.ConnectTmp(info.db_name, info.db_connection_timeout);
    }
    else
    {
        db = connector.Connect(info.db_name, info.db_connection_timeout);
    }
    if (!info.db_allow_exceptions && !db.IsOK())
    {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << info.min_age << " and "s << info.max_age << " "s
              << "and Name like '%"s << db.Quote(info.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query))
    {
        persons.push_back({move(name), age});
    }
    return persons;
}
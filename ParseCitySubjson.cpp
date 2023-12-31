#include <vector>
#include <string_view>
using namespace std;
// Структура данных о городе, которая строится по стране
struct CityInfo
{
    string_view country_name;
    string_view country_iso_code;
    string_view country_phone_code;
    string_view country_time_zone;
};
// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City> &cities, const Json &json, const CityInfo &city_info,
                      const vector<Language> &languages)
{
    for (const auto &city_json : json.AsList())
    {
        const auto &city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          city_info.country_phone_code + city_obj["phone_code"s].AsString(), city_info.country_name, city_info.country_iso_code,
                          city_info.country_time_zone, languages});
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country> &countries, vector<City> &cities, const Json &json)
{
    for (const auto &country_json : json.AsList())
    {
        const auto &country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country &country = countries.back();
        for (const auto &lang_obj : country_obj["languages"s].AsList())
        {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(cities, country_obj["cities"s], {country.name, country.iso_code, country.phone_code, country.time_zone}, country.languages);
    }
}
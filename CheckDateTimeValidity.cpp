#include <string>
#include <stdexcept>
#include <array>
using namespace std;
struct DateTime
{
    size_t year;
    size_t month;
    size_t day;
    size_t hour;
    size_t minute;
    size_t second;
};

string VARIBLE_NAME = "NULL";                                       // Глобальная переменная,в которую будет складываться наименование единцы времени
#define SAVE_VARIBLE_NAME(varible) VARIBLE_NAME = string(#varible); // Макрос для построения строки из имени переменной

void ExceptionHandling(size_t min, size_t max, size_t tested) // универсальная функция, использующая макрос для корректного выброса исключений
{
    SAVE_VARIBLE_NAME(tested);
    std::string err_msg = VARIBLE_NAME + " is too";
    if (tested < min)
    {
        throw domain_error(err_msg + " small"s);
    }
    else if (tested > max)
    {
        throw domain_error(err_msg + " big"s);
    }
}

void CheckDateTimeValidity(const DateTime &dt)
{
    ExceptionHandling(1, 9999, dt.year);
    ExceptionHandling(1, 12, dt.month);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    ExceptionHandling(1, month_lengths[dt.month - 1], dt.month);

    ExceptionHandling(0, 23, dt.hour);

    ExceptionHandling(0, 59, dt.minute);
    ExceptionHandling(0, 59, dt.second);
}
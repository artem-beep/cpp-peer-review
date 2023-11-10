#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>
#include <numeric>

using namespace std;

class Domain
{
public:
    // точка в начале и reverse для корректного поиска подстроки
    Domain(string domain)
    {
        domain_ += domain;
        reverse(domain_.begin(), domain_.end());
    }

    bool operator==(const Domain &other) const
    {
        return domain_ == other.domain_;
    }

    bool IsSubdomain(const Domain &other) const
    {
        return domain_.find(other.domain_) == 0;
    }

    string_view GetAddress() const
    {
        return string_view{domain_};
    }

    // Оператор необходим для корректной сортировки
    bool operator<(const Domain &other) const noexcept
    {
        return std::lexicographical_compare(domain_.begin(), domain_.end(), other.domain_.begin(), other.domain_.end());
    }

private:
    string domain_ = ".";
};

class DomainChecker
{
public:
    template <typename It>
    DomainChecker(It begin, It end) : forb_domains_(begin, end)
    {

        std::sort(forb_domains_.begin(), forb_domains_.end());
        // Из отсортированного вектора запрещенных доменов удаляются все поддомены
        forb_domains_.erase(
            std::unique(
                forb_domains_.begin(), forb_domains_.end(),
                [](const Domain &a, const Domain &b)
                {
                    return b.IsSubdomain(a);
                }),
            forb_domains_.end());
    }

    bool IsForbidden(const Domain &domain)
    {
        // Алгоритм upper-bound выбран из соображений скорости

        auto it = upper_bound(forb_domains_.begin(), forb_domains_.end(), domain);
        if (it != forb_domains_.begin())
        {
            auto previous = it - 1;
            return domain.IsSubdomain(*previous);
        }
        else
        {
            return false;
        }
    }

private:
    vector<Domain> forb_domains_;
};

std::vector<Domain> ReadDomains(std::istream &input, size_t numb)
{
    std::vector<Domain> result;
    for (size_t count = 0; count != numb; count++)
    {
        string address;
        getline(input, address);

        result.push_back(Domain(move(address)));
    }

    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream &input)
{
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main()
{
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain &domain : test_domains)
    {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
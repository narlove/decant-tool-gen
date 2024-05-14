#include <cstdio>
#include <string>
#include "csv.hpp"

using namespace csv;
using std::string;

int main()
{
    CSVFormat format;
    format.delimiter(',').no_header();

    CSVReader reader("../target/test-file.csv", format);

    for (CSVRow &row : reader)
    {
        for (CSVField &field : row)
        {
            printf("%s, ", field.get<string>().c_str());
        }

        printf("\n");
    }

    return 0;
}
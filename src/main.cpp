#include <cstdio>
#include <string>
#include "csv.hpp"

using namespace csv;
using std::string;

const int MAX_BARCODE_NUMBER = 10;

template <typename T>
struct keycode_array_wrapper
{
    T data[MAX_BARCODE_NUMBER];
    unsigned int size;

    keycode_array_wrapper(T default_val)
    {
        this->size = 0;

        for (int i = 0; i < size; i++)
        {
            data[i] = default_val;
        }
    }

    void add(T val)
    {
        data[size] = val;
        size++;
    }
};

int main()
{
    keycode_array_wrapper<unsigned int> keycodes(0);

    CSVFormat format;
    format.delimiter(',').no_header(); // so that the file gets data from all rows

    CSVReader read_keycodes("../target/keycodes.csv", format);
    for (CSVRow &row : read_keycodes)
    {
        for (CSVField &field : row)
        {
            keycodes.add(field.get<unsigned int>());
        }
    }

    // printing the array
    for (int i = 0; i < keycodes.size; i++)
    {
        printf("%u\n", keycodes.data[i]);
    }

    return 0;
}
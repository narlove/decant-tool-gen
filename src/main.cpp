#include <cstdio>
#include <string>
#include "csv.hpp"

using namespace csv;
using std::string;

const int MAX_BARCODE_NUMBER = 10;

template <typename T>
struct barcode_array_wrapper
{
    T data[MAX_BARCODE_NUMBER];
    unsigned int size;

    barcode_array_wrapper(T default_val)
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
    barcode_array_wrapper<unsigned int> array(0);

    CSVFormat format;
    format.delimiter(',').no_header(); // so that the file gets data from all rows

    CSVReader barcodes("../target/scanned-barcodes.csv", format);

    for (CSVRow &row : barcodes)
    {
        for (CSVField &field : row)
        {
            array.add(field.get<unsigned int>());
        }
    }

    for (int i = 0; i < array.size; i++)
    {
        printf("%u\n", array.data[i]);
    }

    return 0;
}
#include <cstdio>
#include <string>
#include "csv.hpp"
#include <unordered_map>
#include <stdexcept>

using std::invalid_argument;
using std::unordered_map;
using namespace csv;
using std::string;

const int MAX_BARCODE_NUMBER = 10;

const int PALLETS_ASSOCIATED_DATA_NUMBER = 2;

// can i make this use inheritance to simplify, instead of two of the example same struct?
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

// TODO: tests?
int main()
{
    keycode_array_wrapper<unsigned int> keycodes(0);
    unordered_map<unsigned int, unsigned int> keycode_cartons;

    CSVFormat format;
    format.delimiter(',').no_header(); // so that the file gets data from all rows

    // load keycodes
    CSVReader read_keycodes("../target/keycodes.csv", format);
    for (CSVRow &row : read_keycodes)
    {
        for (CSVField &field : row)
        {
            keycodes.add(field.get<unsigned int>());
        }
    }

    // maybe reading from a csv and returning into a reference to an array is a function?
    // loading the pallets file is basically the SQL database here... actually, maybe i learn sql for the hd project?!
    CSVReader read_pallet_information("../target/pallets.csv", format);
    for (CSVRow &row : read_pallet_information)
    {
        if (row.size() != PALLETS_ASSOCIATED_DATA_NUMBER)
        {
            throw invalid_argument("invalid arguments provided to the pallets.csv file");
        }

        keycode_cartons[row[0].get<unsigned int>()] = row[1].get<unsigned int>();
    }

    // printing the array
    // a function, maybe a CALLBACK?!!?
    // doing it this way because i think thats its easier to read than using "auto" and .first, .second, etc.
    for (int i = 0; i < keycodes.size; i++)
    {
        printf("Pallet with keycode %u has %u cartons on it\n", keycodes.data[i], keycode_cartons[keycodes.data[i]]);
    }

    return 0;
}
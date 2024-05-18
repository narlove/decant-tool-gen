#include <cstdio>
#include <string>
#include "csv.hpp"
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <stdlib.h>

using std::invalid_argument;
using std::malloc;
using std::unordered_map;
using std::vector;
using namespace csv;
using std::string;

const int MAX_PALLET_NUMBER = 10;

// was really long and didn't wanna type it all the time.
// almost over engineered this, but i think i steered it back down to be readable enough.
typedef unordered_map<unsigned int, unsigned int> keycode_size_type;

struct keycodes_wrapper
{
    unsigned int data[MAX_PALLET_NUMBER];
    unsigned int size;
};

struct carton_sizes_wrapper
{
    unsigned int data[MAX_PALLET_NUMBER];
    unsigned int size;
};

// array wrapper function(s)
// just incase i have multiple array wrappers
template <typename T>
void array_add(T array[], unsigned int &size, T val)
{
    array[size] = val;
    size++;
}

void read_pallet_information(keycode_size_type &keycode_size_map, CSVFormat format)
{
    CSVReader pallet_info("../target/pallets.csv", format);
    for (CSVRow &row : pallet_info)
    {
        // 2 = 1 (for the keycode) + 1 (for the size)
        if (row.size() != 2)
        {
            throw invalid_argument("invalid arguments provided to the pallets.csv file");
        }

        keycode_size_map[row[0].get<unsigned int>()] = row[1].get<unsigned int>();
    }
}

void read_keycodes(keycodes_wrapper &keycodes, CSVFormat format)
{
    CSVReader read_keycodes("../target/keycodes.csv", format);
    for (CSVRow &row : read_keycodes)
    {
        for (CSVField &field : row)
        {
            array_add<unsigned int>(keycodes.data, keycodes.size, field.get<unsigned int>());
        }
    }
}

// procedure so that we're not returning a big data type
// using arrays because showing gained knowledge, otherwise would use a vector

// fix so that it uses a new array wrapper
void extract_carton_count(keycode_size_type &keycode_size_map, carton_sizes_wrapper carton_sizes)
{
    for (auto keycode_size : keycode_size_map)
    {
        // adding to the sizes array, the size that we find for each keycode.
        array_add<unsigned int>(carton_sizes.data, carton_sizes.size, keycode_size.second);
    }
}

// fix so that it uses a new array wrapper
unsigned int calculate_total_cartons(carton_sizes_wrapper sizes)
{
    unsigned int sum = 0;
    for (unsigned int size : sizes.data)
    {
        sum += size;
    }

    return sum;
};

// TODO: tests?
int main()
{
    keycodes_wrapper keycodes = {.size = 0};
    keycode_size_type keycode_size_map;

    CSVFormat format;
    format.delimiter(',').no_header(); // so that the file gets data from all rows

    // was going to remove keycodes, but the reason its here is for easy read and write if
    // i get a barcode scanner or similar
    read_keycodes(keycodes, format);

    read_pallet_information(keycode_size_map, format);

    for (int i = 0; i < keycodes.size; i++)
    {
        printf("Imported pallet %u has data (from lookup): CARTON_SIZE: %u;\n", keycodes.data[i], keycode_size_map[keycodes.data[i]]);
    }

    return 0;
}
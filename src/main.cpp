#include <cstdio>
#include <string>
#include "csv.hpp"
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <stdlib.h>
#include <cmath>

using std::invalid_argument;
using std::malloc;
using std::unordered_map;
using std::vector;
using namespace csv;
using std::ceil;
using std::string;

const double AVAILABLE_TIME = 3.0;
const double CLEANUP_HOURS = 6.0;
const double CUTTING_RATE = 110.0;

const int MAX_PALLET_NUMBER = 20;

// was really long and didn't wanna type it all the time.
// almost over engineered this, but i think i steered it back down to be readable enough.
typedef unordered_map<int, int> keycode_size_type;

struct keycodes_wrapper
{
    int data[MAX_PALLET_NUMBER];
    int size;
};

struct carton_sizes_wrapper
{
    int data[MAX_PALLET_NUMBER];
    int size;
};

// array wrapper function(s)
// just incase i have multiple array wrappers
template <typename T>
void array_add(T array[], int &size, T val)
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

        keycode_size_map[row[0].get<int>()] = row[1].get<int>();
    }
}

void read_keycodes(keycodes_wrapper &keycodes, CSVFormat format)
{
    CSVReader read_keycodes("../target/keycodes.csv", format);
    for (CSVRow &row : read_keycodes)
    {
        for (CSVField &field : row)
        {
            array_add<int>(keycodes.data, keycodes.size, field.get<int>());
        }
    }
}

// procedure so that we're not returning a big data type
// using arrays because showing gained knowledge, otherwise would use a vector
void extract_carton_count(const keycode_size_type &keycode_size_map, carton_sizes_wrapper &carton_sizes)
{
    for (std::pair<int, int> keycode_size : keycode_size_map)
    {
        // adding to the sizes array, the size that we find for each keycode.
        array_add<int>(carton_sizes.data, carton_sizes.size, keycode_size.second);
    }
}

int calculate_total_cartons(carton_sizes_wrapper sizes)
{
    int sum = 0;
    for (int size : sizes.data)
    {
        sum += size;
    }

    return sum;
};

double calculate_required_cutting_hours(int cartons)
{
    return (cartons / CUTTING_RATE);
}

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
        printf("Imported pallet %d has data (from lookup): CARTON_SIZE: %d;\n", keycodes.data[i], keycode_size_map[keycodes.data[i]]);
    }

    // total cartons
    carton_sizes_wrapper carton_sizes = {.size = 0};
    extract_carton_count(keycode_size_map, carton_sizes);

    int total_cartons = calculate_total_cartons(carton_sizes);

    // calculate cutter numbers
    double cutting_hours = calculate_required_cutting_hours(total_cartons);
    double sorting_hours = cutting_hours / 2.0;

    double total_hours = CLEANUP_HOURS + cutting_hours + sorting_hours;

    int team_member_number = ceil(total_hours / AVAILABLE_TIME);

    int cutter_number = round(team_member_number * 0.667);
    int sorter_number = round(team_member_number * 0.333);

    // for printing purposes
    double cleanup_time = (CLEANUP_HOURS / team_member_number);
    double finish_time = AVAILABLE_TIME - cleanup_time;

    printf("total cartons: %d\n", total_cartons);
    printf("cutting hours: %lf\n", cutting_hours);
    printf("number of cutters: %d\n", cutter_number);
    printf("number of sorters: %d\n", sorter_number);
    printf("number of team: %d\n", team_member_number);
    printf("load finish time: %lf\n", finish_time);
    printf("cleanup time: %lf\n", cleanup_time);

    return 0;
}
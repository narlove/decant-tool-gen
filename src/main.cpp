/*
    Project name: Decant Tool Generator
    Finish Date: 19/05/2024
    Unit Code: SIT102
    Student name: Nathan Love
    Student ID: 224554748
*/

#include <cstdio> // printf
#include <string> // strings
#include "csv.hpp" // csv interaction
#include <unordered_map> // the map
#include <stdexcept> // invalid_arg
#include <cmath> // for ceil and round
#include <algorithm> // for sort
#include <cstring> // memcpy

using std::invalid_argument;
using std::unordered_map;
using namespace csv;
using std::ceil;
using std::memcpy;
using std::sort;
using std::string;

// these constants can maybe be changed by a config file in future
const double AVAILABLE_TIME = 3.0;
const double CLEANUP_HOURS = 6.0;
const double CUTTING_RATE = 110.0;

const int MAX_ROWS = 5;
const int ROW_SIZE = 10;
const int MAX_PALLET_NUMBER = 20;

// was really long and didn't wanna type it all the time.
// almost over engineered this, but i think i steered it back down to be readable enough.
typedef unordered_map<int, int> keycode_size_type;

// array wrappers for organisation
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

struct rows_array_wrapper
{
    int data[MAX_ROWS][ROW_SIZE];
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

// load the pallet info and save it to an array
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

// identify each keycode and save it.
// i do this separately, so that if i implement a database, we can also have a list of all of the keycodes
// that will be looked up from the DB
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

// basic arithmetic.
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

/**
 * @brief Takes the sizes from carton_sizes and maps them into rows, depending on a few constants
 * defined at the beginning.
 * 
 * @param rows The array wrapper to hold the rows array.
 * @param carton_sizes The array wrapper to hold the sizes.
 */
void sort_into_rows(rows_array_wrapper &rows, const carton_sizes_wrapper &carton_sizes)
{
    int values_assigned = 0;
    int temp_row[ROW_SIZE];

    // should realistically never get to this condition,
    // unless we have an array that is the perfect number of items to perfectly fill each row.
    // usually the break will catch
    for (int j = 0; j < MAX_ROWS; j++)
    {
        for (int i = 0; i < ROW_SIZE; i++)
        {
            // if we've assigned all the values that were stored in carton sizes, we're done.
            // zero the rest while we finish this row.
            if (values_assigned >= carton_sizes.size)
            {
                temp_row[i] = 0;
                continue;
            }

            temp_row[i] = carton_sizes.data[j * ROW_SIZE + i];
            values_assigned += 1;
        }

        // copy value
        int move_size = sizeof(int) * ROW_SIZE;
        memcpy(rows.data[j], temp_row, move_size);

        rows.size++;

        // clear the temp array
        for (int k = 0; k < ROW_SIZE; k++)
        {
            temp_row[k] = 0;
        }

        // checking if we need to keep adding rows - if we have exceeded or are equal to teh number of values
        // we needed to sort into rows, then we're good.
        if (values_assigned >= carton_sizes.size)
        {
            break; // we dont need to do any more.
        }
    }
}

/**
 * @brief Nested for loops that print out the rows array in a neat manner. 
 * Before you use this function, call sort_into_rows().
 * 
 * @param rows The array wrapper that contains the rows array. Rows array should be int [][].
 */
void print_rows(const rows_array_wrapper &rows)
{
    printf("FRONT\n");
    printf("--------------------------------\n");
    for (int i = 0; i < rows.size; i++)
    {
        printf("row %d: ", i);
        for (int j = 0; j < ROW_SIZE; j++)
        {
            printf("%d ", rows.data[i][j]);
        }
        printf("\n");
    }
    printf("--------------------------------\n");
}

/**
 * @brief 
 * 
 * @param carton_sizes 
 */
void handle_calculations(const carton_sizes_wrapper &carton_sizes)
{
    int total_cartons = calculate_total_cartons(carton_sizes);

    // calculate cutter numbers
    double cutting_hours = calculate_required_cutting_hours(total_cartons);
    double sorting_hours = cutting_hours / 2.0;

    double total_hours = CLEANUP_HOURS + cutting_hours + sorting_hours;

    int team_member_number = ceil(total_hours / AVAILABLE_TIME);

    int cutter_number = round(team_member_number * 0.667); // 2 thirds on decanting/cutting
    int sorter_number = round(team_member_number * 0.333); // 1 third running/sorting

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
}

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

    // total cartons
    carton_sizes_wrapper carton_sizes = {.size = 0};
    extract_carton_count(keycode_size_map, carton_sizes);

    handle_calculations(carton_sizes);

    // for the rows, we're going to use cartons_sizes, where each size represents a pallet.
    // for the second argument, we need a pointer to the end of the data. for this, we use pointer arithmetic.
    // we can identify where it ends by adding the size on to it.
    sort(carton_sizes.data, carton_sizes.data + carton_sizes.size, std::greater<int>());
    rows_array_wrapper rows = {.size = 0};

    sort_into_rows(rows, carton_sizes); // procedure with references, dont return anything
    print_rows(rows);

    return 0;
}
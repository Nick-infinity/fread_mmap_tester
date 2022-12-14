#include <chrono>
#include <functional>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <fcntl.h>    // for open
#include <unistd.h>   // for close
#include <sys/mman.h> // for mmap
#include <sys/stat.h> // fd
#include <vector>

using namespace std;
using namespace chrono;

template <typename T>
void vector_printer(std::vector<T> v)
{
    for (auto it : v)
    {
        std::cout << it;
    }
}

template <typename T>
bool vector_validator(std::vector<T> v1, std::vector<T> v2, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (v1[i] != v2[i])
        {
            return false;
        }
    }
    return true;
}

// The function we want to time
void fread_test(std::vector<char> *v)
{
    // Open the file for reading
    FILE *file = fopen("file100.bin", "r");

    // Check if the file was opened successfully
    if (file == nullptr)
    {
        // Handle the error here
        std::cout << "Error";
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate a buffer to store the contents of the file
    char *buffer = new char[size + 1];
    // memset(buffer, 0, size + 1);

    // Read the contents of the file into the buffer
    size_t read = fread(buffer, 1, size, file);

    // Check if the read was successful
    if (read != size)
    {
        // Handle the error here
        std::cout << "Error";
    }

    // push buffer chars in a vector
    for (int i = 0; i < size + 1; ++i)
    {
        v->push_back(buffer[i]);
    }
    // Print the contents of the buffer
    // printf("%s\n", buffer);

    // Clean up
    fclose(file);
    delete[] buffer;
}

void mmap_test(std::vector<char> *v)
{
    // Open the file
    int fd = open("file100.bin", O_RDONLY);
    if (fd == -1)
    {
        std::cerr << "Error opening file\n";
        // return 1;
    }

    // Get the file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        std::cerr << "Error getting file size\n";
        // return 1;
    }

    // Map the file into memory
    void *addr = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
    {
        std::cerr << "Error mapping file into memory\n";
        // return 1;
    }

    // Read the data from the mapped memory
    // Assuming the file contains ASCII text
    std::string data = static_cast<const char *>(addr);
    // push buffer chars in a vector
    for (int i = 0; i < data.size(); ++i)
    {
        v->push_back(data[i]);
    }
    // std::cout << "File contents: " << data << '\n';

    // Unmap the memory and close the file
    munmap(addr, sb.st_size);
    close(fd);
}

// A function that takes another function as an argument and returns the time it takes to execute
template <typename Func, typename... Args>
double time_benchmark(Func f, Args... args)
{
    // Record the start time
    auto start = high_resolution_clock::now();

    // Call the function
    f(args...);

    // Record the end time
    auto end = high_resolution_clock::now();

    // Return the elapsed time in milliseconds
    return duration_cast<milliseconds>(end - start).count();
}

int main()
{
    std::vector<char> fread_vector;
    std::vector<char> mmap_vector;

    double elapsed_fread = time_benchmark(fread_test, &fread_vector);
    double elapsed_mmap = time_benchmark(mmap_test, &mmap_vector);

    std::cout << "fread_vector size: " << fread_vector.size() << "\n";
    std::cout << "mmap_vector size: " << mmap_vector.size() << "\n";
    // last index of both vector with valid data is 99999999

    if (vector_validator(fread_vector, mmap_vector, 100000000))
    {
        std::cout << "Data has been verified correctly\n";
    }
    else
    {
        std::cout << "Data missmatch\n";
    }

    std::cout << "Elapsed time: fread: " << elapsed_fread << "ms" << endl;
    std::cout << "Elapsed time: mmap: " << elapsed_mmap << "ms" << endl;

    return 0;
}

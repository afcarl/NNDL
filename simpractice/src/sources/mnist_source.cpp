#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "debug.h"
#include "signal.h"

#include "mnist_source.h"



MnistSource::MnistSource()
{
    this->load_dataset();
}

MnistSource::~MnistSource()
{
    for (unsigned int i = 0; i < this->nimages; i++)
    {
            delete this->dataset[i];
    }
}

std::ostream& operator<<(std::ostream &outstream, const MnistSource &ms)
{
    outstream << debug_print_header("MNIST Source") << std::endl;
    outstream << "ntimes_given: " << ms.ntimes_given << std::endl;
    outstream << "nimages: " << ms.nimages << std::endl;
    outstream << "imgsz: " << ms.imgsz << std::endl;
    outstream << debug_print_closing("MNIST Source") << std::endl;

    return outstream;
}

std::vector<Signal> MnistSource::get(uint64_t time)
{
    std::vector<Signal> data_vector;
    unsigned char *img = this->dataset[this->ntimes_given++];
    for (unsigned int i = 0; i < this->imgsz; i++)
    {
        unsigned char val = img[i];
        Signal s((fpoint_t)val);
        data_vector.push_back(s);
    }

    return data_vector;
}

bool MnistSource::has_more(uint64_t time)
{
    return this->ntimes_given < this->nimages;
}



void MnistSource::load_dataset()
{
    /*
     * This method shamelessly taken from Stack Overflow:
     * https://stackoverflow.com/questions/8286668/how-to-read-mnist-data-in-c
     */

    auto reverse_int = [](int i)
    {
        unsigned char c1, c2, c3, c4;
        c1 = i & 0xFF;
        c2 = (i >> 8) & 0xFF;
        c3 = (i >> 16) & 0xFF;
        c4 = (i >>24) & 0xFF;
        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    };

    int number_of_images = 0;
    int image_size = 0;
    std::string path("mnist/train-images-idx3-ubyte");//This one is for training
    //std::string path("mnist/t10k-images-idx3-ubyte");//This one is for testing
    std::ifstream file(path, std::ios::binary);

    if (file.is_open())
    {
        int magic_number = 0;
        int n_rows = 0;
        int n_cols = 0;

        //Read the first value into 'magic_number' to see if the file is valid
        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverse_int(magic_number);

        bool magic_valid = (magic_number == 2051) || (magic_number == 2049);
        if (!magic_valid)
            throw std::runtime_error("Invalid MNIST data file.");

        std::cout << "Magic Number: " << magic_number << std::endl;

        file.read((char *)&number_of_images, sizeof(number_of_images));
        number_of_images = reverse_int(number_of_images);
        std::cout << "Number of images: " << number_of_images << std::endl;

        file.read((char *)&n_rows, sizeof(n_rows));
        n_rows = reverse_int(n_rows);
        std::cout << "Number of rows: " << n_rows << std::endl;

        file.read((char *)&n_cols, sizeof(n_cols));
        n_cols = reverse_int(n_cols);
        std::cout << "Number of columns: " << n_cols << std::endl;

        image_size = n_rows * n_cols;
        std::cout << "Image size: " << image_size << std::endl;

        unsigned char **dataset = new unsigned char *[number_of_images];
        for (int i = 0; i < number_of_images; i++)
        {
            dataset[i] = new unsigned char[image_size];
            file.read((char *)dataset[i], image_size);
        }
        this->dataset = dataset;
        this->nimages = number_of_images;
        this->imgsz = image_size;
    }
    else
    {
        throw std::runtime_error("Cannot open file `" + path + "`");
    }
}











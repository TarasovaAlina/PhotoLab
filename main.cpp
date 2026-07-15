#include <iostream>
#include "include/reader_class.h"
#include "include/process.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            throw std::invalid_argument {
                "Arguments: \n\t"
                "input BMP file\n\t"
                "output BMP file\n\t"
            };
        }
        std::string inputBmpFile { argv[1] };
        std::string outputBmpFile { argv[2] };

        Bmp bmp { inputBmpFile };
        CNN cnn {};
        cnn.proccesingImage(bmp.data(), bmp.getHeight(), bmp.getWidth());
        bmp.setData(cnn.getOutputData());


        // makeBW(bmp);
        // keepGreenChannel(bmp);
        // makeNegative(bmp);

        if (!bmp.saveFile(outputBmpFile)) {
            std::cout << "Fail to save file" << std::endl;
        }

    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}
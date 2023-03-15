/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "OpenImageDenoise/oidn.hpp"

#include "src/lib/OpenEXR/ImfArray.h"
#include "src/lib/OpenEXR/ImfChannelList.h"
#include "src/lib/OpenEXR/ImfIO.h"
#include "src/lib/OpenEXR/ImfInputFile.h"
#include "src/lib/OpenEXR/ImfOutputFile.h"
#include "src/lib/OpenEXR/ImfRgbaFile.h"
#include "src/lib/OpenEXR/ImfStringAttribute.h"

using namespace Imf;
using namespace Imath;

#include "boost/program_options.hpp"

#include <cassert>
#include <cfloat>
#include <iostream>

using namespace std;

class Image3f {
public:
    Image3f(const int width, const int height) : width_(width), height_(height) {
        data_ = new float[width_ * height_ * 3];

        for (int x = 0; x < width_; ++x) {
            for (int y = 0; y < height_; ++y) {
                auto r = 0.f;
                auto g = 0.f;
                auto b = 0.f;

                data_[(width_ * y + x) * 3] = r;
                data_[(width_ * y + x) * 3 + 1] = g;
                data_[(width_ * y + x) * 3 + 2] = b;
            }
        }
    }

    void set_pixel(int x, int y, float red, float green, float blue) {
        assert(x >= 0);
        assert(y >= 0);
        assert(x < width_);
        assert(y < height_);

        data_[(width_ * y + x) * 3] = red;
        data_[(width_ * y + x) * 3 + 1] = green;
        data_[(width_ * y + x) * 3 + 2] = blue;
    }

    float *data() const {
        return data_;
    }

    float *data() {
        return data_;
    }

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

private:
    int width_ = 0;
    int height_ = 0;
    float *data_;
};

Image3f load_image_openexr(std::string_view filename) {
    // see https://www.openexr.com/documentation/ReadingAndWritingImageFiles.pdf
    // Heading Reading an Image File
    InputFile file(filename.data());
    const Header &header = file.header();

    Box2i dw = header.dataWindow();
    int width = dw.max.x - dw.min.x + 1;
    int height = dw.max.y - dw.min.y + 1;

    /*
    bool hasRed = false;
    bool hasGreen = false;
    bool hasBlue = false;

    for (ChannelList::ConstIterator it = header.channels().begin(), ite = header.channels().end(); it != ite; it++) {
        if ((strcmp(it.name(), "R") == 0)) { hasRed = true; }
        if ((strcmp(it.name(), "G") == 0)) { hasGreen = true; }
        if ((strcmp(it.name(), "B") == 0)) { hasBlue = true; }
        if (it.channel().type != HALF) {
            //HDR_LOG("Unable to open EXR file \"%s\" (unsupported data type %s)", filename, it.channel().type);
            //return (IEFileCantOpen);
        }
    }
    */

    Imf::Array2D<float> rPixels;
    Imf::Array2D<float> gPixels;
    Imf::Array2D<float> bPixels;

    rPixels.resizeErase(height, width);
    gPixels.resizeErase(height, width);
    bPixels.resizeErase(height, width);

    FrameBuffer frameBuffer;

    frameBuffer.insert("R", // name
                       Slice(FLOAT, // type
                             (char *) (&rPixels[0][0] - // base
                                       dw.min.x -
                                       dw.min.y * width),
                             sizeof(rPixels[0][0]) * 1, // xStride
                             sizeof(rPixels[0][0]) * width,// yStride
                             1, 1, // x/y sampling
                             FLT_MAX)); // fillValue

    frameBuffer.insert("G", // name
                       Slice(FLOAT, // type
                             (char *) (&gPixels[0][0] - // base
                                       dw.min.x -
                                       dw.min.y * width),
                             sizeof(gPixels[0][0]) * 1, // xStride
                             sizeof(gPixels[0][0]) * width,// yStride
                             1, 1, // x/y sampling
                             FLT_MAX)); // fillValue

    frameBuffer.insert("B", // name
                       Slice(FLOAT, // type
                             (char *) (&bPixels[0][0] - // base
                                       dw.min.x -
                                       dw.min.y * width),
                             sizeof(bPixels[0][0]) * 1, // xStride
                             sizeof(bPixels[0][0]) * width,// yStride
                             1, 1, // x/y sampling
                             FLT_MAX)); // fillValue

    file.setFrameBuffer(frameBuffer);
    file.readPixels(dw.min.y, dw.max.y);

    Image3f img{width, height};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            img.set_pixel(x, y, rPixels[y][x], gPixels[y][x], bPixels[y][x]);
        }
    }
    return img;
}

void store_open_exr(const std::string_view &filename, const Image3f &image) {
    Header header(image.width(), image.height());
    header.insert("comments", Imf::StringAttribute("Generated by Okapi"));

    //Imf::Compression& ctype = header.compression();
    //assert(ctype == Imf::Compression::ZIP_COMPRESSION);

    ChannelList &channels = header.channels();
    channels.insert("R", Imf::Channel(Imf::FLOAT));
    channels.insert("G", Imf::Channel(Imf::FLOAT));
    channels.insert("B", Imf::Channel(Imf::FLOAT));

    FrameBuffer frameBuffer;
    size_t compStride = sizeof(float),
            pixelStride = 3 * compStride,
            rowStride = pixelStride * image.width();

    char *data = reinterpret_cast<char *>(image.data());
    frameBuffer.insert("R", Imf::Slice(Imf::FLOAT, data, pixelStride, rowStride));
    data += compStride;
    frameBuffer.insert("G", Imf::Slice(Imf::FLOAT, data, pixelStride, rowStride));
    data += compStride;
    frameBuffer.insert("B", Imf::Slice(Imf::FLOAT, data, pixelStride, rowStride));

    OutputFile file(filename.data(), header);
    file.setFrameBuffer(frameBuffer);
    file.writePixels(image.height());
}

int main(int argc, char ** argv) {
    cout << "Simple denoising example" << endl;

    using namespace boost::program_options;

    variables_map vm;
    options_description desc{"Options"};
    desc.add_options()
        ("filename", value<std::string>()->default_value(std::string("denoised.exr")), "Filename where the denoised image will be stored");

    try {
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
    } catch (const error &ex) {
        cerr << ex.what() << std::endl;
        return -2;
    }

    string str_denoised_output_image_filename = vm["filename"].as<std::string>();;
  
    cout << "Filename for denoised image: " << str_denoised_output_image_filename << endl;

    Image3f color = load_image_openexr("data/cornel_box.naive_diffuse.box_filter.spp128.embree.exr");
    //Image3f color = load_image_openexr("data/noisy_10spp.exr");
    Image3f normal = load_image_openexr("data/normal_10spp.exr");
    Image3f albedo = load_image_openexr("data/albedo_10spp.exr");
    Image3f out{color.width(), color.height()};

    // for debug reasons the color image can be initialized with a const color
    if(false) { 
        for (int x = 0; x < color.width(); ++x) {
            for (int y = 0; y < color.height(); ++y) {
                color.set_pixel(x,y,.5f, .5f, .5f);
            }
        }
    }

    float* colorPtr = color.data();
    float* albedoPtr = albedo.data();
    float* normalPtr = normal.data();
    float* outputPtr = out.data();
    int width = out.width();
    int height = out.height();

    oidn::DeviceRef device = oidn::newDevice();
    device.set("verbose", 1);
    device.commit();

    // Create a filter for denoising a beauty (color) image using optional auxiliary images too
    oidn::FilterRef filter = device.newFilter("RT"); // generic ray tracing filter
    filter.setImage("color",  colorPtr,  oidn::Format::Float3, width, height); // beauty
    //filter.setImage("albedo", albedoPtr, oidn::Format::Float3, width, height); // auxiliary
    //filter.setImage("normal", normalPtr, oidn::Format::Float3, width, height); // auxiliary
    filter.setImage("output", outputPtr, oidn::Format::Float3, width, height); // denoised beauty
    filter.set("hdr", true); // beauty image is HDR
    filter.commit();

    // Filter the image
    filter.execute();

    // Check for errors
    const char* errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None) {
        std::cout << "Error: " << errorMessage << std::endl;
    }

    store_open_exr(str_denoised_output_image_filename, out);

    return 0;
}

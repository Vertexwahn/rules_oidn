/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "OpenImageDenoise/oidn.hpp"

#include "imaging/io/io_openexr.h"
using namespace de_vertexwahn;

#include <iostream>
using namespace std;

int main() {
    cout << "Hello World!" << endl;

    Image3f noisy = load_image_openexr("testbed/oidn/data/noisy_10spp.exr");
    Image3f normal = load_image_openexr("testbed/oidn/data/normal_10spp.exr");
    Image3f albedo = load_image_openexr("testbed/oidn/data/albedo_10spp.exr");
    Image3f out{noisy.size()};

    float* colorPtr = noisy.data();
    float* albedoPtr = albedo.data();
    float* normalPtr = normal.data();
    float* outputPtr = out.data();
    int width = out.width();
    int height = out.height();

    oidn::DeviceRef device = oidn::newDevice();
    device.commit();

    // Create a filter for denoising a beauty (color) image using optional auxiliary images too
    oidn::FilterRef filter = device.newFilter("RT"); // generic ray tracing filter
    filter.setImage("color",  colorPtr,  oidn::Format::Float3, width, height); // beauty
    filter.setImage("albedo", albedoPtr, oidn::Format::Float3, width, height); // auxiliary
    filter.setImage("normal", normalPtr, oidn::Format::Float3, width, height); // auxiliary
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

    return 0;
}

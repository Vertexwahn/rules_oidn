/*
 *  SPDX-FileCopyrightText: Copyright 2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "OpenImageDenoise/oidn.hpp"

#include <iostream>

using namespace std;

int main() {
    cout << "Hello World!" << endl;

    oidn::DeviceRef device = oidn::newDevice();
    device.commit();

    float* colorPtr = nullptr;
    float* albedoPtr = nullptr;
    float* normalPtr = nullptr;
    float* outputPtr = nullptr;
    int width = 0;
    int height = 0;

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
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << "Error: " << errorMessage << std::endl;
}

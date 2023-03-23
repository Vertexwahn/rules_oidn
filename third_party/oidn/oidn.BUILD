"""
    SPDX-FileCopyrightText: 2023 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

load("@bazel_skylib//rules:expand_template.bzl", "expand_template")
load("@rules_ispc//:ispc.bzl", "ispc_cc_library")
load("@rules_oidn//third_party/oidn:oidn_generate_cpp_from_blob.bzl", "generate_cpp_from_blob_cc_library")
load("@rules_oidn//third_party/oidn:deps.bzl", "oidn_deps")

expand_template(
    name = "config",
    out = "include/OpenImageDenoise/config.h",
    substitutions = {
        "#cmakedefine OIDN_STATIC_LIB": "",
        "#cmakedefine OIDN_API_NAMESPACE": "",
        "@OIDN_VERSION_MAJOR@": "1",
        "@OIDN_VERSION_MINOR@": "4",
        "@OIDN_VERSION_PATCH@": "3",
        "@OIDN_VERSION_NUMBER@": "010403",
        "@OIDN_VERSION_NOTE": "",
    },
    template = "include/OpenImageDenoise/config.h.in",
)

'''
COMMON_DEFINES = select({
    "@platforms//os:osx": [
        "OIDN_BNNS",
        "OIDN_STATIC_LIB",
        "OIDN_FILTER_RT",
        "OIDN_FILTER_RTLIGHTMAP",
    ],
    "//conditions:default": [
        "OIDN_DNNL",
        "OIDN_STATIC_LIB",
        "OIDN_FILTER_RT",
        "OIDN_FILTER_RTLIGHTMAP",
    ],
})
'''

COMMON_DEFINES = [
    "OIDN_DNNL",  # change this to OIDN_BNNS for macOS
    "OIDN_STATIC_LIB",
    "OIDN_FILTER_RT",
    "OIDN_FILTER_RTLIGHTMAP",
]

ispc_cc_library(
    name = "input_reorder_ispc",
    srcs = [
        "core/color.isph",
        "core/image.isph",
        "core/input_reorder.ispc",
        "core/math.isph",
        "core/reorder.isph",
        "core/tensor.isph",
        "core/vec.isph",
    ],
    out = "input_reorder_ispc.h",
    defines = COMMON_DEFINES,
    ispc_main_source_file = "core/input_reorder.ispc",
)

ispc_cc_library(
    name = "color_ispc",
    srcs = [
        "core/color.ispc",
        "core/color.isph",
        "core/image.isph",
        "core/math.isph",
        "core/vec.isph",
    ],
    out = "color_ispc.h",
    defines = COMMON_DEFINES,
    ispc_main_source_file = "core/color.ispc",
)

ispc_cc_library(
    name = "output_copy_ispc",
    srcs = [
        "core/color.isph",
        "core/image.isph",
        "core/math.isph",
        "core/output_copy.ispc",
        "core/vec.isph",
    ],
    out = "output_copy_ispc.h",
    defines = COMMON_DEFINES,
    ispc_main_source_file = "core/output_copy.ispc",
)

ispc_cc_library(
    name = "upsample_ispc",
    srcs = [
        "core/color.isph",
        "core/image.isph",
        "core/math.isph",
        "core/tensor.isph",
        "core/upsample.ispc",
        "core/vec.isph",
    ],
    out = "upsample_ispc.h",
    defines = COMMON_DEFINES,
    ispc_main_source_file = "core/upsample.ispc",
)

ispc_cc_library(
    name = "output_reorder_ispc",
    srcs = [
        "core/color.isph",
        "core/image.isph",
        "core/math.isph",
        "core/output_reorder.ispc",
        "core/reorder.isph",
        "core/tensor.isph",
        "core/vec.isph",
    ],
    out = "output_reorder_ispc.h",
    defines = COMMON_DEFINES,
    ispc_main_source_file = "core/output_reorder.ispc",
)

cc_library(
    name = "common",
    srcs = [
        "common/barrier.h",
        "common/exception.h",
        "common/math.h",
        "common/platform.cpp",
        "common/platform.h",
        "common/ref.h",
        "common/tasking.cpp",
        "common/tasking.h",
        "common/thread.cpp",
        "common/thread.h",
        "common/timer.h",
        "include/OpenImageDenoise/config.h",  # generated
        "include/OpenImageDenoise/oidn.h",
        "include/OpenImageDenoise/oidn.hpp",
    ],
    includes = ["include/OpenImageDenoise"],
    deps = [
        "@oneTBB//:tbb",
    ] + select({
        "@platforms//os:osx": [],
        "//conditions:default": ["@onednn//:onednn"],
    }),
)

py_binary(
    name = "blob_to_cpp",
    srcs = ["scripts/blob_to_cpp.py"],
    data = ["@oidn-weights//:weights"],
)

generate_cpp_from_blob_cc_library(name = "rt_alb.tza")

generate_cpp_from_blob_cc_library(name = "rt_hdr.tza")

generate_cpp_from_blob_cc_library(name = "rt_hdr_alb.tza")

generate_cpp_from_blob_cc_library(name = "rt_hdr_alb_nrm.tza")

generate_cpp_from_blob_cc_library(name = "rt_hdr_calb_cnrm.tza")

generate_cpp_from_blob_cc_library(name = "rt_ldr.tza")

generate_cpp_from_blob_cc_library(name = "rt_ldr_alb.tza")

generate_cpp_from_blob_cc_library(name = "rt_ldr_alb_nrm.tza")

generate_cpp_from_blob_cc_library(name = "rt_ldr_calb_cnrm.tza")

generate_cpp_from_blob_cc_library(name = "rt_nrm.tza")

generate_cpp_from_blob_cc_library(name = "rtlightmap_dir.tza")

generate_cpp_from_blob_cc_library(name = "rtlightmap_hdr.tza")

cc_library(
    name = "OpenImageDenoise",
    srcs = [
        "core/api.cpp",
        "core/buffer.h",
        "core/color.cpp",
        "core/color.h",
        "core/common.h",
        "core/conv.h",
        "core/cpu_buffer.h",
        "core/cpu_device.cpp",
        "core/cpu_device.h",
        "core/data.h",
        "core/device.cpp",
        "core/device.h",
        "core/filter.cpp",
        "core/filter.h",
        "core/image.h",
        "core/input_reorder.cpp",
        "core/input_reorder.h",
        "core/network.cpp",
        "core/network.h",
        "core/node.h",
        "core/output_copy.cpp",
        "core/output_copy.h",
        "core/output_reorder.cpp",
        "core/output_reorder.h",
        "core/pool.h",
        "core/progress.h",
        "core/reorder.h",
        "core/scratch.cpp",
        "core/scratch.h",
        "core/tensor.h",
        "core/tza.cpp",
        "core/tza.h",
        "core/unet.cpp",
        "core/unet.h",
        "core/upsample.cpp",
        "core/upsample.h",
        "core/vec.h",
    ],
    hdrs = [
        "include/OpenImageDenoise/config.h",
        "include/OpenImageDenoise/oidn.h",
        "include/OpenImageDenoise/oidn.hpp",
    ],
    defines = COMMON_DEFINES,
    includes = [
        "core",
        "include",
    ],
    linkopts = select({
        "@platforms//os:osx": ["-framework Accelerate"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":color_ispc",
        ":common",
        ":input_reorder_ispc",
        ":output_copy_ispc",
        ":output_reorder_ispc",
        ":rt_alb.tza",
        ":rt_hdr.tza",
        ":rt_hdr_alb.tza",
        ":rt_hdr_alb_nrm.tza",
        ":rt_hdr_calb_cnrm.tza",
        ":rt_ldr.tza",
        ":rt_ldr_alb.tza",
        ":rt_ldr_alb_nrm.tza",
        ":rt_ldr_calb_cnrm.tza",
        ":rt_nrm.tza",
        ":rtlightmap_dir.tza",
        ":rtlightmap_hdr.tza",
        ":upsample_ispc",
    ],
)

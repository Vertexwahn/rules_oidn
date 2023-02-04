"""
    SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

"""External dependencies for odin."""

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def oidn_deps():
    """Fetches dependencies of odin."""

    maybe(
        http_archive,
        name = "bazel_skylib",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
            "https://github.com/bazelbuild/bazel-skylib/releases/download/1.3.0/bazel-skylib-1.3.0.tar.gz",
        ],
        sha256 = "74d544d96f4a5bb630d465ca8bbcfe231e3594e5aae57e1edbf17a6eb3ca2506",
    )

    maybe(
        http_archive,
        name = "mkl_dnn_v1",
        build_file = "@de_vertexwahn_rules_oidn//:mkldnn_v1.BUILD",
        sha256 = "dc2b9bc851cd8d5a6c4622f7dc215bdb6b32349962875f8bf55cceed45a4c449",
        strip_prefix = "oneDNN-2.7.1",
        urls = [
            "https://github.com/oneapi-src/oneDNN/archive/refs/tags/v2.7.1.tar.gz",
        ],
    )

    maybe(
        http_archive,
        name = "oneTBB",
        urls = [
            "https://github.com/oneapi-src/oneTBB/archive/3b161438c9bb4099b41838fda21dbfcb02980662.zip",
        ],
        strip_prefix = "oneTBB-3b161438c9bb4099b41838fda21dbfcb02980662",
        #sha256 = "bed8a9ca62c8b4a20e9d51e9090a06fc6402287ace8ef5e830d0f306af2ddcd7",
    )

    maybe(
        http_archive,
        name = "oidn",
        build_file = "@de_vertexwahn_rules_oidn//:BUILD.oidn",
        strip_prefix = "oidn-1.4.3",
        patches = ["@de_vertexwahn_rules_oidn//:0001-Bazel-support.patch"],
        urls = [
            "https://github.com/OpenImageDenoise/oidn/archive/refs/tags/v1.4.3.tar.gz",
        ],
        sha256 = "37941f260af212183579efb6b1e07e4075e3ab9cbd63120afd007e62367efef6",
    )

    maybe(
        git_repository,
        name = "rules_ispc",
        commit = "ec5063fde457732a3b06e40ad7a3c1a7b6c5d34e",
        #branch = "main",
        remote = "https://github.com/Vertexwahn/rules_ispc",
        shallow_since = "1671698253 +0100",
    )

    maybe(
        git_repository,
        name = "oidn-weights",
        build_file = "@de_vertexwahn_rules_oidn//:BUILD.oidn-weights",
        commit = "a34b7641349c5a79e46a617d61709c35df5d6c28",
        remote = "https://github.com/OpenImageDenoise/oidn-weights",
    )

def generate_cpp_from_blob_cc_library(name, out, **kwargs):
    generted_header_filename = out

    native.genrule(
        name = "%s_weights_gen" % name,
        srcs = [ name ],
        outs = [name + ".cpp", name + ".h",],
        cmd = "python3 $(location @de_vertexwahn_rules_oidn//:blob_to_cpp) $(locations %s) -o $(location %s.cpp) -H $(location %s.h)" % (name, name, name),
        tools = "@de_vertexwahn_rules_oidn//:blob_to_cpp"
    )
    native.cc_library(
        name = name,
        srcs = [name + ".o"],
        hdrs = [name + ".h"],
        **kwargs
    )

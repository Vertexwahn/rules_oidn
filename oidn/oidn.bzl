"""
    SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

"""External dependencies for odin."""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def odin_deps():
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
        build_file = "@de_vertexwahn_oidn_rules//:mkldnn_v1.BUILD",
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
        urls = [
            "https://github.com/OpenImageDenoise/oidn/archive/refs/tags/v1.4.3.tar.gz",
        ],
        #sha256 = "b8c22d275d9128741265537c559d0ea73074adbf2f2b66b0a766ca52c52d665b",
    )

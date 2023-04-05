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
        sha256 = "b8a1527901774180afc798aeb28c4634bdccf19c4d98e7bdd1ce79d1fe9aaad7",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.4.1/bazel-skylib-1.4.1.tar.gz",
            "https://github.com/bazelbuild/bazel-skylib/releases/download/1.4.1/bazel-skylib-1.4.1.tar.gz",
        ],
    )

    maybe(
        http_archive,
        name = "onednn",
        build_file = "@rules_oidn//third_party/onednn:onednn.BUILD",
        sha256 = "a50993aa6265b799b040fe745e0010502f9f7103cc53a9525d59646aef006633",
        strip_prefix = "oneDNN-2.7.3",
        urls = [
            "https://github.com/oneapi-src/oneDNN/archive/refs/tags/v2.7.3.tar.gz",
        ],
    )

    #maybe(
    #    http_archive,
    #    name = "mkl_dnn_acl_compatible",
    #    build_file = "@rules_oidn//third_party/onednn:mkldnn_acl.BUILD",
    #    patches = [
    #        "@rules_oidn//third_party/onednn:onednn_acl_threadcap.patch",
    #        "@rules_oidn//third_party/onednn:onednn_acl_fixed_format_kernels.patch",
    #        "@rules_oidn//third_party/onednn:onednn_acl_depthwise_convolution.patch",
    #        "@rules_oidn//third_party/onednn:onednn_acl_threadpool_scheduler.patch",
    #    ],
    #    sha256 = "a50993aa6265b799b040fe745e0010502f9f7103cc53a9525d59646aef006633",
    #    strip_prefix = "oneDNN-2.7.3",
    #    urls = ["https://github.com/oneapi-src/oneDNN/archive/v2.7.3.tar.gz"],
    #)

    #maybe(
    #    http_archive,
    #    name = "compute_library",
    #    sha256 = "e20a060d3c4f803889d96c2f0b865004ba3ef4e228299a44339ea1c1ba827c85",
    #    strip_prefix = "ComputeLibrary-22.11",
    #    build_file = "@rules_oidn//third_party/compute_library:compute_library.BUILD",
    #    patches = [
    #        "@rules_oidn//third_party/compute_library:compute_library.patch",
    #        "@rules_oidn//third_party/compute_library:acl_fixed_format_kernels_striding.patch",
    #        "@rules_oidn//third_party/compute_library:acl_openmp_fix.patch",
    #    ],
    #    urls = ["https://github.com/ARM-software/ComputeLibrary/archive/v22.11.tar.gz"],
    #)

    #maybe(
    #    git_repository,
    #    name = "compute_library",
    #   remote = "https://github.com/Vertexwahn/ComputeLibrary.git",
    #    commit = "38f6df217c601f0714598bda0efa019439811a0a",
    #)

    maybe(
        http_archive,
        name = "oneTBB",
        urls = [
            "https://github.com/oneapi-src/oneTBB/archive/5b2a49008c32c7ba3df7c9e137a894ff53710b64.zip",
        ],
        strip_prefix = "oneTBB-5b2a49008c32c7ba3df7c9e137a894ff53710b64",
        sha256 = "d9ae2b326b6bac49c5a97ddfd7068a024fc7955f64b1e0811300aca6cff30c94",
    )

    maybe(
        git_repository,
        name = "oidn-weights",
        build_file = "@rules_oidn//third_party/oidn-weights:oidn-weights.BUILD",
        commit = "e9cae1b5e7814b8bc2540ff09e77e7ae874b08df",
        #remote = "https://github.com/OpenImageDenoise/oidn-weights",
        remote = "https://github.com/Vertexwahn/oidn-weights-without-git-lfs",
        shallow_since = "1676196087 +0100",
    )

    maybe(
        http_archive,
        name = "oidn",
        build_file = "@rules_oidn//third_party/oidn:oidn.BUILD",
        strip_prefix = "oidn-1.4.3",
        patches = ["@rules_oidn//third_party/oidn:0001-Bazel-support.patch"],
        urls = [
            "https://github.com/OpenImageDenoise/oidn/archive/refs/tags/v1.4.3.tar.gz",
        ],
        sha256 = "37941f260af212183579efb6b1e07e4075e3ab9cbd63120afd007e62367efef6",
    )

    maybe(
        http_archive,
        name = "rules_ispc",
        sha256 = "2c5563f878e63eaaf498293ca084c84d3e6cb2e6818bf0d4b24b5dfc63570892",
        strip_prefix = "rules_ispc-0.0.2",
        url = "https://github.com/Vertexwahn/rules_ispc/releases/download/v0.0.2/rules_ispc-v0.0.2.tar.gz",
    )

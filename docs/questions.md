Title: How to work around "select is not iterable" in a macro?

Tags: bazel
bazel-rules


I have a macro called `ispc_cc_library`. I call it in a `BUILD` file this way:

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

When I call the Bazel build command, I get this error:

    ERROR: Traceback (most recent call last):
            File "/private/var/tmp/_bazel_vertexwahn/998288db447b21c21cab4093cf36fa19/external/oidn/BUILD.bazel", line 48, column 16, in <toplevel>
                    ispc_cc_library(
            File "/private/var/tmp/_bazel_vertexwahn/998288db447b21c21cab4093cf36fa19/external/rules_ispc/ispc.bzl", line 5, column 11, in ispc_cc_library
                    if len(defines) > 0:
    Error in len: select is not iterable

The implementation of `ispc_cc_library` looks like this:

    def ispc_cc_library(name, out, ispc_main_source_file, srcs, defines = [], target_compatible_with = [], **kwargs):
        generted_header_filename = out
    
        ispc_defines_list = ""
        if len(defines) > 0:
            ispc_defines_list = "-D" + " -D".join(defines)
    
        native.genrule(
            name = "%s_ispc_gen" % name,
            srcs = srcs,
            outs = [name + ".o", generted_header_filename],
            cmd = select({
                "@platforms//os:linux": "$(location @ispc_linux_x86_64//:ispc) %s --target=avx2 --target-os=linux --arch=x86-64 --addressing=64 --pic $(locations %s) --header-outfile=$(location %s) -o $(location %s.o)" % (ispc_defines_list, ispc_main_source_file, generted_header_filename, name),
                "@rules_ispc//:osx_arm64": "$(location @ispc_osx_arm64//:ispc) %s --target=neon --target-os=macos --arch=aarch64 --addressing=64 --pic $(locations %s) --header-outfile=$(location %s) -o $(location %s.o)" % (ispc_defines_list, ispc_main_source_file, generted_header_filename, name),
                "@rules_ispc//:osx_x86_64": "$(location @ispc_osx_x86_64//:ispc) %s --target=sse2 --target-os=macos --arch=x86-64 --addressing=64 --pic $(locations %s) --header-outfile=$(location %s) -o $(location %s.o)" % (ispc_defines_list, ispc_main_source_file, generted_header_filename, name),
                "@platforms//os:windows": "$(location @ispc_windows_x86_64//:ispc) %s --target=avx2 --target-os=windows --arch=x86-64 --addressing=64 $(locations %s) --header-outfile=$(location %s) -o $(location %s.o)" % (ispc_defines_list, ispc_main_source_file, generted_header_filename, name),
            }),
            tools = select({
                "@platforms//os:linux": ["@ispc_linux_x86_64//:ispc"],
                "@rules_ispc//:osx_arm64": ["@ispc_osx_arm64//:ispc"],
                "@rules_ispc//:osx_x86_64": ["@ispc_osx_x86_64//:ispc"],
                "@platforms//os:windows": ["@ispc_windows_x86_64//:ispc"],
            }),
            target_compatible_with = target_compatible_with,
        )
        native.cc_library(
            name = name,
            srcs = [name + ".o"],
            hdrs = [name + ".h"],
            defines = defines,
            target_compatible_with = target_compatible_with,
            **kwargs


When I remove the select from `COMMON_DEFINES`, e.g. on macOS this way:

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
        "OIDN_BNNS",
        "OIDN_STATIC_LIB",
        "OIDN_FILTER_RT",
        "OIDN_FILTER_RTLIGHTMAP",
    ]

It works. As Bazel reports `select is not iterable`. What is the right way to solve this issue? Do I need to convert `ispc_cc_library` macro to a rule? Or is there a easy workaround?

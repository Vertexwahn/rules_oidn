[![Support Ukraine](https://img.shields.io/badge/Support-Ukraine-FFD500?style=flat&labelColor=005BBB)](https://opensource.fb.com/support-ukraine)

# rules_oidn -- Bazel build rules for Intel Open Image Denoise

Rules for using [Intel Open Image Denoise](https://www.openimagedenoise.org/) in your Bazel builds.

## How to test?

```shell
git clone https://github.com/Vertexwahn/rules_oidn.git
cd rules_oidn
cd tests
bazel build --config=gcc11 //... # See test/.bazelrc for other supported configs
```

## License

This work is published under the Apache 2.0 License.

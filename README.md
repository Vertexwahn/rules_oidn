[![Support Ukraine](https://img.shields.io/badge/Support-Ukraine-FFD500?style=flat&labelColor=005BBB)](https://opensource.fb.com/support-ukraine)

# rules_oidn -- Bazel build rules for Intel Open Image Denoise

Rules for using [Intel Open Image Denoise](https://www.openimagedenoise.org/) in your Bazel builds.

## Current status

Currently, these rules compile on Ubuntu 22.04 and Windows.
macOS with M1 processor builds will fail (neon needs to be supported).
If you run the example (see next section) the following output is generated:

![Denoised image](tests/data/denoised_onlyColor.png)

## Quick start

**Prerequisites:**

The following tools should be installed:
- [Git](https://git-scm.com/)
- [Bazel](https://bazel.build/install)
- A C++ compiler (GCC, Visual Studio, Clang, etc.)

**Checkout, build, and run:**

*All platforms:*

```shell
git clone https://github.com/Vertexwahn/rules_oidn.git
cd rules_oidn
cd tests
```

*Run Hello World demo with Ubuntu 22.04:*

```shell
bazel run --config=gcc11 //:example # See test/.bazelrc for other supported configs
```

## More about the example

The example provides noisy rendering of the Cornell Box.

![Noisy](tests/data/noisy_10spp.png)

Additionally there is a corrsponding albedo image:

![Noisy](tests/data/albedo_10spp.png)

And a normal image:

![Noisy](tests/data/normal_10spp.png)

This serves as an input for the example.

## Generating weight manually

```shell
python3 .\scripts\blob_to_cpp.py .\weights\rt_alb.tza -o .\weights\rt_alb.tza.cpp -H .\weights\rt_alb.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_hdr.tza -o .\weights\rt_hdr.tza.cpp -H .\weights\rt_hdr.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_hdr_alb.tza -o .\weights\rt_hdr_alb.tza.cpp -H .\weights\rt_hdr_alb.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_hdr_alb_nrm.tza -o .\weights\rt_hdr_alb_nrm.tza.cpp -H .\weights\rt_hdr_alb_nrm.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_hdr_calb_cnrm.tza -o .\weightsrt_hdr_calb_cnrm.tza.cpp -H .\weights\rt_hdr_calb_cnrm.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_ldr.tza -o .\weights\rt_ldr.tza.cpp -H .\weights\rt_ldr.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_ldr_alb.tza -o .\weights\rt_ldr_alb.tza.cpp -H .\weights\rt_ldr_alb.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_ldr_alb_nrm.tza -o .\weights\rt_ldr_alb_nrm.tza.cpp -H .\weights\rt_ldr_alb_nrm.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_ldr_calb_cnrm.tza -o .\weights\rt_ldr_calb_cnrm.tza.cpp -H .\weights\rt_ldr_calb_cnrm.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rt_nrm.tza -o .\weights\rt_nrm.tza.cpp -H .\weights\rt_nrm.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rtlightmap_dir.tza -o .\weights\rtlightmap_dir.tza.cpp -H .\weights\rtlightmap_dir.tza.h
python3 .\scripts\blob_to_cpp.py .\weights\rtlightmap_hdr.tza -o .\weights\rtlightmap_hdr.tza.cpp -H .\weights\rtlightmap_hdr.tza.h
```

## License

This work is published under the Apache 2.0 License.

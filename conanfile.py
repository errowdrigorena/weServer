from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeDeps, CMakeToolchain


class WeServerConan(ConanFile):
    name = "weserver"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
        "include/*",
        "src/CMakeLists.txt",
        "src/main.cpp",
        "src/store/*",
        "src/protocol/*",
        "src/crud/*",
        "src/net/*",
        "test/*",
    )

    def requirements(self):
        self.requires("boost/1.88.0")
        self.requires("openssl/3.4.0")
        self.test_requires("gtest/1.15.0")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.variables["CMAKE_CXX_STANDARD"] = "20"
        tc.cache_variables["WEB_SERVER_BUILD_TESTS"] = True
        tc.generate()

    def layout(self):
        cmake_layout(self, build_folder="build")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

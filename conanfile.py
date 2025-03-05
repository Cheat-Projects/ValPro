"""Conan package definition for ValPro"""


from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd


class CS2Project(ConanFile):

    name = "ValPro"
    version = "1.0"
    label = "ValPro package"

    settings = "os", "compiler", "build_type"

    exports_sources = "CMakeLists.txt", "cmake/*", "src/*"

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.27.0]")
        self.tool_requires("ccache/[>=4.8.3]")
        self.tool_requires("cppcheck/[>=2.12.1]")

    def validate(self):
        check_min_cppstd(self, "17")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

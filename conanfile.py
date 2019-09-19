from conans import ConanFile

class NamedTypeConan(ConanFile):
    name = "NamedType"
    version = "1.0"
    exports_sources = "include/*"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    description = """"""

    def package(self):
        self.copy("*.h", dst="include", src="include")

    def package_info(self):
        self.cpp_info.includedirs = ["include"]

    def package_id(self):
        self.info.header_only()
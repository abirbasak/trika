#include <azuik/gpu.hpp>
#define CNN_NO_SERIALIZATION
#include <tiny_dnn/tiny_dnn.h>
int main(int argc, char *argv[])
{

    char c = 'a';
    unsigned char uc = 'b';
    signed char sc = 'c';
    std::cout << c << " " << uc << " " << sc << std::endl;

    using namespace azuik;
    std::string kernel_code =
        R"CODE(
                void kernel simple_add(global const int* A, global const int* B, global int* C)
                {                                                                               
                       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];              
                }                                                                                
                )CODE";
    std::cout << kernel_code << std::endl;

    auto platforms = gpu::get_platforms();
    for (auto &platform : platforms) {
        std::cout << "name:" << platform.name() << ",version:" << platform.version() << ",vendor:" << platform.vendor()
                  << ",profile:" << platform.profile() << ",extensions:" << platform.extensions() << "\n";

        std::cout << "devices : \n";
        auto devices = platform.devices();
        for (auto &device : devices) {
            std::cout << "\tname:" << device.name() << ",vendor:" << device.vendor()
                      << ",memory_size: " << device.memory_size() << ",address_bits: " << device.address_bits() << "\n";
        }
    }
    auto &cl = platforms[0];
    auto devices = cl.devices();
    auto gpu_device = devices[1];

    auto ctx = gpu::context{gpu_device};

    auto program = gpu::program{ctx, kernel_code.c_str()};
    program.compile();

    auto mem_a = ctx.allocate<int>(10);
    auto mem_b = ctx.allocate<int>(10);
    auto mem_c = ctx.allocate<int>(10);
}

#include <iostream>
#include "BIOS.h"
#include "Interconnect.h"
#include "Range.h"
#include "CPU.h"

int main() {
//    std::cout << "Hello, World!" << std::endl;
    BIOS bios = BIOS();
    Interconnect inter = Interconnect(bios);
    CPU cpu = CPU(inter);
    while(true){
        try {
            cpu.run_next_instruction();
        }
        catch (UnhandledStore& e){
            std::cout<<e.what()<<std::endl;
            return 0;
        }
        catch (UnhandledInstruction& e){
            std::cout<<e.what()<<std::endl;
            return 0;
        }
    }
    return 0;
}

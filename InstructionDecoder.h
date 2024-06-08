//
// Created by root on 6/8/24.
//

#ifndef GTIRBMIXER_INSTRUCTIONDECODER_H
#define GTIRBMIXER_INSTRUCTIONDECODER_H
#include <capstone/capstone.h>
#include <gtirb/gtirb.hpp>
class InstructionDecoder
{
gtirb::Module &Module;
std::shared_ptr<csh> CsHandle;
public:
    InstructionDecoder(gtirb::Module &M) : Module(M)
    {
        // Create smart Capstone handle.
        CsHandle.reset(new csh(0), [](csh* Handle) {
            cs_close(Handle);
            delete Handle;
        });
        auto Target = std::make_tuple(Module.getFileFormat(), Module.getISA(), Module.getByteOrder());
        // Setup Capstone engine.
        cs_mode Mode;
        cs_arch Arch;
        auto [FileFormat, ISA, ByteOrder] = Target;
        switch (ISA)
        {
            case gtirb::ISA::IA32:
                Arch = CS_ARCH_X86;
                Mode = CS_MODE_32;
                break;
            case gtirb::ISA::X64:
                Arch = CS_ARCH_X86;
                Mode = CS_MODE_64;
                break;
            case gtirb::ISA::ARM:
                Arch = CS_ARCH_ARM;
                Mode = CS_MODE_ARM;
                break;
            case gtirb::ISA::ARM64:
                Arch = CS_ARCH_ARM64;
                Mode = CS_MODE_ARM;
                break;
            case gtirb::ISA::MIPS32:
                Arch = CS_ARCH_MIPS;
                Mode = CS_MODE_MIPS32;
                break;
            case gtirb::ISA::MIPS64:
                Arch = CS_ARCH_MIPS;
                Mode = CS_MODE_MIPS64;
                break;
            case gtirb::ISA::PPC32:
                Arch = CS_ARCH_PPC;
                Mode = CS_MODE_32;
                break;
            case gtirb::ISA::PPC64:
                Arch = CS_ARCH_PPC;
                Mode = CS_MODE_64;
                break;
            case gtirb::ISA::ValidButUnsupported:
            case gtirb::ISA::Undefined:
                throw std::runtime_error("Unsupported ISA");
        }
        [[maybe_unused]] cs_err Err = cs_open(Arch, Mode, CsHandle.get());
        assert(Err == CS_ERR_OK && "Failed to initialize disassembler.");
        cs_option(*CsHandle, CS_OPT_SYNTAX, CS_OPT_ON);
    }
    std::vector<uint64_t> decode(const uint8_t* Bytes, uint64_t Size, uint64_t Addr) {
        // Decode get the offset of the instructions.
        cs_insn* CsInsn;
        size_t Count = cs_disasm(*CsHandle, Bytes, Size, Addr, 0, &CsInsn);
        std::vector<uint64_t> Offsets;
        for (size_t i = 0; i < Count; i++) {
            auto &CsInstruction = CsInsn[i];
            auto Offset = CsInstruction.address - Addr;
            Offsets.push_back(Offset);
        }
        cs_free(CsInsn, Count);
        return Offsets;
    }
};

#endif // GTIRBMIXER_INSTRUCTIONDECODER_H

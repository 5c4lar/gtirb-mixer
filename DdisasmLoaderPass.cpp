//
// Created by root on 4/12/24.
//

#include "DdisasmLoaderPass.h"
#include <gtirb-decoder/target/ElfArm32Loader.h>
#include <gtirb-decoder/target/ElfArm64Loader.h>
#include <gtirb-decoder/target/ElfMips32Loader.h>
#include <gtirb-decoder/target/ElfX64Loader.h>
#include <gtirb-decoder/target/ElfX86Loader.h>
#include <gtirb-decoder/target/PeX64Loader.h>
#include <gtirb-decoder/target/PeX86Loader.h>
#include <gtirb-decoder/target/RawArm32Loader.h>
#include <gtirb-decoder/target/RawArm64Loader.h>
#include <gtirb-decoder/target/RawMips32Loader.h>
#include <gtirb-decoder/target/RawX64Loader.h>
#include <gtirb-decoder/target/RawX86Loader.h>

void removeSectionSymbols(gtirb::Context &Context, gtirb::Module &Module)
{
    auto *SymbolInfo = Module.getAuxData<gtirb::schema::ElfSymbolInfo>();
    auto *SymbolTabIdxInfo = Module.getAuxData<gtirb::schema::ElfSymbolTabIdxInfo>();
    if(!SymbolInfo)
    {
        return;
    }
    std::vector<gtirb::UUID> Remove;
    for(const auto &[Uuid, Info] : *SymbolInfo)
    {
        if(std::get<1>(Info) == "SECTION")
        {
            Remove.push_back(Uuid);
        }
    }
    for(const auto Uuid : Remove)
    {
        gtirb::Node *N = gtirb::Node::getByUUID(Context, Uuid);
        if(auto *Symbol = gtirb::dyn_cast_or_null<gtirb::Symbol>(N))
        {
            Module.removeSymbol(Symbol);
            SymbolInfo->erase(Uuid);
        }

        // Remove auxdata that refer to the symbol.
        if(SymbolTabIdxInfo)
        {
            SymbolTabIdxInfo->erase(Uuid);
        }
    }
}
void removePreviousModuleContent(gtirb::Module &Module)
{
    for(auto &Bi : Module.byte_intervals())
    {
        std::vector<gtirb::CodeBlock *> CodeToRemove;
        for(auto &Block : Bi.code_blocks())
        {
            CodeToRemove.push_back(&Block);
        }
        for(auto Block : CodeToRemove)
        {
            Bi.removeBlock(Block);
        }
        std::vector<gtirb::DataBlock *> DataToRemove;
        for(auto &Block : Bi.data_blocks())
        {
            DataToRemove.push_back(&Block);
        }
        for(auto Block : DataToRemove)
        {
            Bi.removeBlock(Block);
        }
        std::vector<uint64_t> SymExprOffset;
        for(auto SymExpr : Bi.symbolic_expressions())
        {
            SymExprOffset.push_back(SymExpr.getOffset());
        }
        for(auto Offset : SymExprOffset)
        {
            Bi.removeSymbolicExpression(Offset);
        }
    }
}
void removeEntryPoint(gtirb::Module &Module)
{
    // Remove initial entry point.
    // ElfReader and PeReader create a code block with zero size to set the entrypoint.
    // It's no longer needed; we connect it to a real code block once it's created.
    if(gtirb::CodeBlock *Block = Module.getEntryPoint())
    {
        Block->getByteInterval()->removeBlock(Block);
    }
    Module.setEntryPoint(nullptr);
}
std::map<DdisasmLoaderPass::Target, DdisasmLoaderPass::Factory>& DdisasmLoaderPass::loaders()
{
    static std::map<Target, Factory> Loaders;
    return Loaders;
}
void DdisasmLoaderPass::transformImpl(AnalysisPassResult& Result, gtirb::Context& Context,
                                      gtirb::Module& Module)
{
    DatalogAnalysisPass::transformImpl(Result, Context, Module);
    removeEntryPoint(Module);
    removeSectionSymbols(Context, Module);
    removePreviousModuleContent(Module);
}
void DdisasmLoaderPass::loadImpl(AnalysisPassResult& Result, const gtirb::Context& Context,
                                 const gtirb::Module& Module, AnalysisPass* PreviousPass)
{
    auto Target = std::make_tuple(Module.getFileFormat(), Module.getISA(), Module.getByteOrder());
    auto Factories = DdisasmLoaderPass::loaders();
    if(auto It = Factories.find(Target); It != Factories.end())
    {
        auto Loader = (It->second)();
        Program = Loader.load(Module);
    }
    else
    {
        std::stringstream StrBuilder;
        StrBuilder << Module.getName() << ": "
                   << "Unsupported binary target: " << binaryFormat(Module.getFileFormat()) << "-"
                   << binaryISA(Module.getISA()) << "-" << binaryEndianness(Module.getByteOrder())
                   << "\n\nAvailable targets:\n";

        for(auto& [T, L] : Factories)
        {
            auto [FileFormat, Arch, ByteOrder] = T;
            StrBuilder << "\t" << binaryFormat(FileFormat) << "-" << binaryISA(Arch) << "-"
                       << binaryEndianness(ByteOrder) << "\n";
        }
        Result.Errors.push_back(StrBuilder.str());
    }
}

void DdisasmLoaderPass::registerDatalogLoaders()
{
#if defined(DDISASM_ARM_32)
    // Register ELF-ARM32-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::ELF, gtirb::ISA::ARM, gtirb::ByteOrder::Little}, ElfArm32Loader);

    // Register RAW-ARM32-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::RAW, gtirb::ISA::ARM, gtirb::ByteOrder::Little}, RawArm32Loader);
#endif

#if defined(DDISASM_ARM_64)
    // Register ELF-ARM64-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::ELF, gtirb::ISA::ARM64, gtirb::ByteOrder::Little}, ElfArm64Loader);

    // Register RAW-ARM64-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::RAW, gtirb::ISA::ARM64, gtirb::ByteOrder::Little}, RawArm64Loader);
#endif

#if defined(DDISASM_MIPS_32)
    // Register ELF-MIPS32-BE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::ELF, gtirb::ISA::MIPS32, gtirb::ByteOrder::Big}, ElfMips32BELoader);

    // Register ELF-MIPS32-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::ELF, gtirb::ISA::MIPS32, gtirb::ByteOrder::Little}, ElfMips32LELoader);

    // Register RAW-MIPS32-BE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::RAW, gtirb::ISA::MIPS32, gtirb::ByteOrder::Big}, RawMips32BELoader);

    // Register RAW-MIPS32-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::RAW, gtirb::ISA::MIPS32, gtirb::ByteOrder::Little}, RawMips32LELoader);
#endif

#if defined(DDISASM_X86_32)
    // Register ELF-X86-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::ELF, gtirb::ISA::IA32, gtirb::ByteOrder::Little}, ElfX86Loader);

    // Register PE-X86-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::PE, gtirb::ISA::IA32, gtirb::ByteOrder::Little}, PeX86Loader);

    // Register RAW-X86-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::RAW, gtirb::ISA::IA32, gtirb::ByteOrder::Little}, RawX86Loader);
#endif

#if defined(DDISASM_X86_64)
    // Register ELF-X64-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::ELF, gtirb::ISA::X64, gtirb::ByteOrder::Little}, ElfX64Loader);

    // Register PE-X64-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::PE, gtirb::ISA::X64, gtirb::ByteOrder::Little}, PeX64Loader);

    // Register RAW-X64-LE target.
    DdisasmLoaderPass::registerLoader(
        {gtirb::FileFormat::RAW, gtirb::ISA::X64, gtirb::ByteOrder::Little}, RawX64Loader);
#endif
}
//
// Created by root on 4/15/24.
//

#include "SymbolicExpressionLoaderPass.h"
#include "AuxDataSchema.h"
#include "InstructionDecoder.h"
void SymbolicExpressionLoaderPass::loadImpl(AnalysisPassResult &Result, const gtirb::Context &Context,
                                         const gtirb::Module &Module, AnalysisPass *PreviousPass)
{
}
void SymbolicExpressionLoaderPass::analyzeImpl(AnalysisPassResult &Result, const gtirb::Module &Module)
{
}
void SymbolicExpressionLoaderPass::transformImpl(AnalysisPassResult &Result, gtirb::Context &Context,
                                              gtirb::Module &Module)
{
    auto RefSectionIndex = ReferenceModule.getAuxData<gtirb::schema::SectionIndex>();
    auto ModuleSectionIndex = Module.getAuxData<gtirb::schema::SectionIndex>();
    auto &RefSymbolicExpressionInfo = *ReferenceModule.getAuxData<gtirb::schema::SymbolicExpressionInfo>();
    gtirb::schema::SymbolicExpressionInfo::Type ModuleSymbolicExpressionInfo;
    InstructionDecoder Decoder(Module);

    for (auto &[Index, RefUUID]: *RefSectionIndex) {
        auto RefSection = gtirb::dyn_cast<gtirb::Section>(gtirb::Node::getByUUID(Context, RefUUID));
        if (ModuleSectionIndex->count(Index) == 0) {
            continue;
        }
        auto Section = gtirb::dyn_cast<gtirb::Section>(gtirb::Node::getByUUID(Context, (*ModuleSectionIndex)[Index]));
        for (auto SymbolicExpression: RefSection->symbolic_expressions()) {

            auto *RefBI = SymbolicExpression.getByteInterval();
            auto Offset = RefBI->getAddress().value() + SymbolicExpression.getOffset() - RefSection->getAddress().value();
            auto &BI = Section->findByteIntervalsOn(Section->getAddress().value() + Offset).front();
            auto Info = RefSymbolicExpressionInfo[std::make_tuple(RefBI->getUUID(), SymbolicExpression.getOffset())];
            auto Flags = std::get<3>(Info);
            if (Flags & FKF_IsPCRel) {
                auto &CodeBlock = BI.findCodeBlocksOnOffset(Offset).front();
                // decode the instruction at the offset
                uint64_t Address = CodeBlock.getOffset() + uint64_t(RefBI->getAddress().value());
                auto Offsets = Decoder.decode(CodeBlock.rawBytes<uint8_t>(), CodeBlock.getSize(), Address);
                auto SymExprOffset = Offset - CodeBlock.getOffset();
                // Find the first offset that is greater than SymExprOffset
                auto It = std::lower_bound(Offsets.begin(), Offsets.end(), SymExprOffset);
                if (It != Offsets.end()) {
                    auto Delta = *It - SymExprOffset;
                    auto SymExpr = SymbolicExpression.getSymbolicExpression();
                    if (std::holds_alternative<gtirb::SymAddrConst>(SymExpr)) {
                        auto &SymAddr = std::get<gtirb::SymAddrConst>(SymExpr);
                        SymAddr.Offset += Delta;
                        BI.addSymbolicExpression(Offset, SymExpr);
                    } else if (std::holds_alternative<gtirb::SymAddrAddr>(SymExpr)) {
                        auto &SymAddr = std::get<gtirb::SymAddrAddr>(SymExpr);
                        SymAddr.Offset += Delta;
                        BI.addSymbolicExpression(Offset, SymExpr);
                    }
                }
            } else {
                BI.addSymbolicExpression(Offset, SymbolicExpression.getSymbolicExpression());
            }
            ModuleSymbolicExpressionInfo[std::make_tuple(BI.getUUID(), Offset)] = Info;
        }
    }
    Module.addAuxData<gtirb::schema::SymbolicExpressionInfo>(std::move(ModuleSymbolicExpressionInfo));
}

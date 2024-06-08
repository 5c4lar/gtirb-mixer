//
// Created by root on 5/14/24.
//

#ifndef GTIRB_MIXER_AUXDATASCHEMA_H
#define GTIRB_MIXER_AUXDATASCHEMA_H
#include <AuxDataSchema.h>
enum FixupKindFlags {
    /// Is this fixup kind PCrelative? This is used by the assembler backend to
    /// evaluate fixup values in a target independent manner when possible.
    FKF_IsPCRel = (1 << 0),

    /// Should this fixup kind force a 4-byte aligned effective PC value?
    FKF_IsAlignedDownTo32Bits = (1 << 1),

    /// Should this fixup be evaluated in a target dependent manner?
    FKF_IsTarget = (1 << 2),

    /// This fixup kind should be resolved if defined.
    /// FIXME This is a workaround because we don't support certain ARM
    /// relocation types. This flag should eventually be removed.
    FKF_Constant = 1 << 3,
};
namespace auxdata {
    /// SymbolicExpressionInfo is a map from {BI_UUID, Offset} to {FixupKindInfo.Name, FixupKindInfo.TargetOffset, FixupKindInfo.TargetSize, FixupKindInfo.Flags, Variant}.
    using SymbolicExpressionInfo = std::map<std::tuple<gtirb::UUID, uint64_t>, std::tuple<std::string, uint32_t, uint32_t, uint32_t, uint16_t>>;
} // namespace auxdata

namespace gtirb {
    namespace schema {
        // 5c4lar
        /// \brief Auxilary data recording symbolic expression types and variant
        struct SymbolicExpressionInfo
        {
            static constexpr const char* Name = "symbolicExpressionInfo";
            typedef auxdata::SymbolicExpressionInfo Type;
        };
    } // namespace schema
} // namespace gtirb
#endif // DDISASM_AUXDATASCHEMA_H

//
// Created by root on 4/15/24.
//

#ifndef DDISASM_SymbolicExpressionLoaderPass_H
#define DDISASM_SymbolicExpressionLoaderPass_H

#include <gtirb/gtirb.hpp>
#include <passes/AnalysisPass.h>

/**
Compute strongly connected components and store them in a AuxData table SccMap called "SCCs"
*/
class SymbolicExpressionLoaderPass : public AnalysisPass
{
public:
    explicit SymbolicExpressionLoaderPass(gtirb::Module& referenceModule) : ReferenceModule(referenceModule)
    {}
    virtual std::string getName() const override
    {
        return "SymExpr loader";
    }

    virtual bool hasTransform(void) override
    {
        return true;
    }

protected:
    virtual void loadImpl(AnalysisPassResult& Result, const gtirb::Context& Context,
                          const gtirb::Module& Module,
                          AnalysisPass* PreviousPass) override;
    virtual void analyzeImpl(AnalysisPassResult& Result, const gtirb::Module& Module) override;
    virtual void transformImpl(AnalysisPassResult& Result, gtirb::Context& Context,
                               gtirb::Module& Module) override;

private:
    gtirb::Module& ReferenceModule;
};

#endif // DDISASM_SymbolicExpressionLoaderPass_H

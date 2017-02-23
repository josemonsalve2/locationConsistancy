#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      errs() << "I saw a function called " << F.getName() << "!\n";
      for (Function::iterator i = F.begin(); i != F.end(); ++i)
      {
        errs()<< "Basic Block = "<< i->getName() << "\n";
	for (BasicBlock::iterator j = i->begin(); j != i->end(); ++j)
        {
	   if (auto *op = dyn_cast<LoadInst>(j) )
	   {
           	errs()<< *j << " ==//== "<<op->getOperand(0)->getName()<< "\n";
	   }
	   if (auto *op = dyn_cast<StoreInst>(j)) {
           	errs()<< *j << " ==//== "<<op->getOperand(1)->getName() << "\n";
	   }

        }
      }

      return false;
    }
  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);

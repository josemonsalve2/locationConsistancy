#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Module.h"
using namespace llvm;

namespace {
        struct SkeletonPass : public FunctionPass {
                static char ID;
                SkeletonPass() : FunctionPass(ID) {}

                virtual bool runOnFunction(Function &F) {
                        // Get the function to call from our runtime library.
                        LLVMContext &Ctx = F.getContext();
                        // Insert function that returns a void and gets an integer. Type:: is used for this
                        Constant *loadFunc = F.getParent()->getOrInsertFunction(
                                        "printLoadVar", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx), NULL
                                        );
                        Constant *storeFunc = F.getParent()->getOrInsertFunction(
                                        "printStoreVar", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx), NULL
                                        );
                        errs() << "I saw a function called " << F.getName() << "!\n";

                        for (Function::iterator i = F.begin(); i != F.end(); ++i)
                        {
                                errs()<< "Basic Block = "<< i->getName() << "\n";
                                for (BasicBlock::iterator j = i->begin(); j != i->end(); ++j)
                                {
                                        if (auto *op = dyn_cast<LoadInst>(j) )
                                        {
                                                errs()<< *j << " ==//== "<<op->getOperand(0)->getName()<< "\n";
                                                // Insert *after* `op`.
                                                IRBuilder<> builder(op);
                                                builder.SetInsertPoint(&(*i), ++builder.GetInsertPoint());
                                                
						// Take the pointer and get an integer with its address
						Value* loadAddr = builder.CreatePtrToInt(op->getPointerOperand(), builder.getInt32Ty());

                                                builder.CreateCall(loadFunc ,loadAddr);
                                        }
                                        if (auto *op = dyn_cast<StoreInst>(j)) {
                                                errs()<< *j << " ==//== "<<op->getOperand(1)->getName() << "\n";
                                                // Insert *after* `op`.
                                                IRBuilder<> builder(op);
                                                builder.SetInsertPoint(&(*i), ++builder.GetInsertPoint());
                                                
						// Take the pointer and get an integer with its address
						Value* storeAddr = builder.CreatePtrToInt(op->getPointerOperand(), builder.getInt32Ty());

                                                builder.CreateCall(storeFunc,storeAddr);
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

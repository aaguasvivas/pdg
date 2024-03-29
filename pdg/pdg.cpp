#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

#define DEBUG_TYPE "hello"

namespace {
  struct Hello : public FunctionPass {
    static char ID;
    Hello():FunctionPass(ID){}
    virtual bool runOnFunction(Function &F) override {
      errs()<< "digraph "+F.getName() + "{\n";
      errs() << "\n";
      for(auto block = F.getBasicBlockList().begin();block != F.getBasicBlockList().end();block++)
      {
        for (auto inst = block->begin(); inst != block->end(); inst++)
        {
          for(Use &U:inst->operands())
          {
            Value *v = U.get();
            if(dyn_cast<Instruction>(v))
            {
              errs() << "\"" << *dyn_cast<Instruction>(v) << "\"" << " -> " << "\"" << *inst << "\"" << ";\n";
            }
            if (v->getName() != "") {
              errs() << "\"" << v->getName() << "\"" << " -> " << "\"" << *inst << "\"" << ";\n";
              errs() << "\"" << v->getName() << "\"" << " [ color = red ]\n";
            }
          }
        }
      }
      errs() << "\n}\n";
 	   return false;
    }
  };
} 

char Hello::ID = 0;
static RegisterPass<Hello>
X("hello", "PDG Pass");

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerpdgpass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new Hello());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerpdgpass);

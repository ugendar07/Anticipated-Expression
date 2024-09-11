
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/Instructions.h"

#include <set>
#include <map>
#include <sstream>
#include<iostream>
#include<string.h>


using namespace std;
using namespace llvm;




namespace {
struct AnticipatedExpression : public FunctionPass {
    /* *******
        BB: Basic Block
        I: Instruction
        Use_set : Use
        Def_set : Def
        IN_set : IN
        OUT_set : OUT
        Type IN: set<pair<int,pair<llvm::Value*,llvm::Value*>>>

    ******* */


  static char ID;
  AnticipatedExpression() : FunctionPass(ID) {}

  std::map<BasicBlock* , set<pair<int,pair<llvm::Value*,llvm::Value*>>>> IN,OUT

    bool runOnFunction(Function &f) override {

        errs() << "Function To be Evaluated : " <<f.getName()<<" \n\n\n";

        /************** Start Finding the Universal Set ***************/

        set<pair<int,pair<llvm::Value*,llvm::Value*>>> U_set;
        for(auto &BB : f){
            for(auto &I : BB)
            {
                if(I.getNumOperands() > 0)
                {
                    Value* Operand1 = I.getOperand(0);
                    Value* Operand2 = I.getNumOperands() > 1 ? I.getOperand(1)
                    pair<int,pair<llvm::Value*,llvm::Value*>> u_pair = make_pai
                    U_set.insert(u_pair);
                }
            }
        }
        /************** Start Initializing IN[BB] with U *************/
        for(auto &BB : f) IN[&BB] = U_set;


        /************** Main Algorithm (Anticipated Expressions for Each Block)
        /*
            Finding  [ IN, OUT,DEF, USE ]
        */
        bool update = true;
        while (update) {
            update = false;
            for (auto &BB : reverse(f)) {

                /************* Finding DEF for Block **************/
                DEF[&BB] = Find_DEF(BB);

                /************* Finding USE for Block **************/
                USE[&BB] = Find_USE(BB);

                /************* Finding OUT for Block **************/
                for (auto *sc : successors(&BB)) {
                    if (OUT[&BB].empty())
                        OUT[&BB]= IN[sc];
                    else
                        OUT[&BB] = Intersection(OUT[&BB], IN[sc]);
                }

                /************* Finding IN for Block **************/
                set<pair<int,pair<llvm::Value*,llvm::Value*>>> temp_IN;
                temp_IN = Union(USE[&BB], SetDifference(OUT[&BB], DEF[&BB]));
                if (temp_IN != IN[&BB]) {
                    IN[&BB] = temp_IN;
                    update = true;
                }
            }
        }

        /************* Printing [IN, OUT, DEF, USE ] for each block ***********/
        int i=1;

        for(auto &BB : f)
        {
            errs() << "Basic block (name="<< i++ << BB.getName() << ") has "<<
            for (Instruction &I : BB)
                errs() << I << "\n";
            errs()<<"\nUse Set \n";
            for( auto x : USE[&BB])
                errs()<< x.first << "  " << x.second.first << "  " << x.second.
            errs()<<"\n\n\nDEF Set \n";
            for( auto x : DEF[&BB])
                errs()<< x.first << "  " << x.second.first << "  " << x.second.
            errs()<<"\n\n\nIN Set \n";
            for( auto x : IN[&BB])
                errs()<< x.first << "  " << x.second.first << "  " << x.second.
            errs()<<"\n\n\nOUT Set \n";
            for( auto x : OUT[&BB])
                errs()<< x.first << "  " << x.second.first << "  " << x.second.
            errs()<<"\n\n\n";
        }


        return true;
    }

    /************** Finding Intersection for Instructions **************/
    set<pair<int,pair<llvm::Value*,llvm::Value*>>> Intersection(set<pair<int,paint,pair<llvm::Value*,llvm::Value*>>> sp2) {
        set<pair<int,pair<llvm::Value*,llvm::Value*>>> Intersection_set;
        for (auto p : sp1) {
            if (sp2.find(p) != sp2.end()) {
                Intersection_set.insert(p);
            }
        }
        return Intersection_set;
    }
    /************** Finding Union for Instructions **************/
    set<pair<int,pair<llvm::Value*,llvm::Value*>>> Union(set<pair<int,pair<llvmr<llvm::Value*,llvm::Value*>>> sp2) {
        set<pair<int,pair<llvm::Value*,llvm::Value*>>> Union_set = sp1;
        for (auto p : sp2) {
            Union_set.insert(p);
        }
        return Union_set;
    }
    /************** Finding Set_Difference for Instructions **************/
    set<pair<int,pair<llvm::Value*,llvm::Value*>>> SetDifference(set<pair<int,p<int,pair<llvm::Value*,llvm::Value*>>> sp2) {
        set<pair<int,pair<llvm::Value*,llvm::Value*>>> SetDifference_set;
        for (auto p : sp1) {
            if (sp2.find(p) == sp2.end()) {
                SetDifference_set.insert(p);
            }
        }
        return SetDifference_set;
    }

    /************** Finding DEF for Block **************/
    set<pair<int,pair<llvm::Value*,llvm::Value*>>> Find_DEF(BasicBlock &BB)
    {

        set<pair<int,pair<llvm::Value*,llvm::Value*>>> Def_set;
        for (auto &I : BB) {
            if (isa<StoreInst>(&I)) {
                Value *val = I.getOperand(1);
                Value *addr = I.getOperand(0);
                Def_set.insert(make_pair(I.getOpcode(), make_pair(val, addr)));
            } else if (isa<AllocaInst>(&I)) {
                Value *val = &I;
                Value *addr = &I;
                Def_set.insert(make_pair(I.getOpcode(), make_pair(val, addr)));
            }
        }
        return Def_set;
    }


    /************** Finding USE for Block **************/
    std :: set<pair<int,pair<llvm::Value*,llvm::Value*>>> Find_USE(BasicBlock &
    {
        set<pair<int,pair<llvm::Value*,llvm::Value*>>> Use_set;
        for (auto &I : BB) {
            for (Use &U : I.operands()) {
                Value *val = U.get();
                if (isa<Instruction>(val) || isa<Argument>(val) || isa<Constant
                    Use_set.insert(make_pair(I.getOpcode(), make_pair(val, null
                }
            }
        }
        return Use_set;
    }
};
}

char AnticipatedExpression::ID = 0;
static RegisterPass<AnticipatedExpression> X("Anticipated", "Anticipated Expression",false,false);


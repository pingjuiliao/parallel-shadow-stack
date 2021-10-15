#include "X86.h"
#include "X86InstrBuilder.h"
#include "X86InstrInfo.h"
#include "X86MachineFunctionInfo.h"
#include "X86Subtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm ;


namespace { 

class X86ShadowStack : public MachineFunctionPass {
public:
    X86ShadowStack() : MachineFunctionPass(ID) {}
    bool runOnMachineFunction(MachineFunction &MF) override ;
private:   
    // allocate memory for machine function pass
    void initShadowStack(MachineFunction &MF) ;
    // shadow stack prologue
    // void functionPrologue(MachineFunction &MF) ;
    // shadow stack epilogue
    // void functionEpilogue(MachienFunction &MF) ;
    
    // Machine instruction builder requirements
    const X86Subtarget    *STI = nullptr ;
    const TargetInstrInfo *TII = nullptr ;

    // pass registration
    StringRef getPassName() const override { return "X86 Shadow Stack" ; }
    static char ID;
};

char X86ShadowStack::ID = 0 ;

} // anonymous namespace


FunctionPass *llvm::createX86ShadowStack() {
    return new X86ShadowStack() ;
}


bool X86ShadowStack::runOnMachineFunction(MachineFunction &MF) {
    STI = &MF.getSubtarget<X86Subtarget>() ;
    TII = STI->getInstrInfo() ;
    if ( MF.getName() == "main" ) {
        initShadowStack(MF) ;
    }
    return true ;
}

void X86ShadowStack::initShadowStack(MachineFunction &MF) {

    // getFirstBasicBlock
    MachineBasicBlock &firstMBB = MF.front() ;
    MachineBasicBlock::iterator itFirstMBB = firstMBB.begin() ;
    const DebugLoc &DL = itFirstMBB->getDebugLoc() ;

    // assemblies
    for ( int i = 0 ; i < 7 ; i ++ ) {
        BuildMI(firstMBB, itFirstMBB, DL, TII->get(X86::NOOP));
    }   
}

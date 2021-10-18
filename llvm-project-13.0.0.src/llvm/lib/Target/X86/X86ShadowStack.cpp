#include "X86.h"
#include "X86InstrBuilder.h"
#include "X86InstrInfo.h"
#include "X86MachineFunctionInfo.h"
#include "X86Subtarget.h"
#include "llvm/ADT/SmallVector.h"
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


    const size_t STACKSIZE = 0x10000 ;
    const size_t QWORDSIZE = 0x8 ;

    // allocate memory for machine function pass
    void initShadowStack(MachineFunction &MF) ;
    
    // shadow stack prologue
    void writePrologue(MachineFunction &MF) ;
    
    // shadow stack epilogue
    void findAndWriteEpilogue(MachineFunction &MF) ;
    void writeEpilogue(MachineBasicBlock &MBB) ;
    
    // Machine instruction builder requirements
    const X86Subtarget    *STI = nullptr ;
    const TargetInstrInfo *TII = nullptr ;
    MachineRegisterInfo   *MRI = nullptr ;

    // pass registration
    StringRef getPassName() const override { return "X86 Shadow Stack" ; }
    static char ID;
};

char X86ShadowStack::ID = 0 ;

} // anonymous namespace


FunctionPass *
llvm::createX86ShadowStack() {
    return new X86ShadowStack() ;
}


bool 
X86ShadowStack::runOnMachineFunction(MachineFunction &MF) {
    
    // global setup
    MRI = &MF.getRegInfo() ;
    STI = &MF.getSubtarget<X86Subtarget>() ;
    TII = STI->getInstrInfo() ;
    
    // 64-bit only
    if ( !STI->is64Bit() ) {
        return false ;
    }
    // TODO: inline function should not be instrumented 
    // if ( MF.hasInlineAsm() ) return false ; // this changes nothing
    

    if ( MF.getName() == "main" ) {
        initShadowStack(MF) ;
    } else if ( MF.getName() != "swap" ){
        errs() << MF.getName() << "() is protected by shadow stack\n" ;
        writePrologue(MF) ; 
        findAndWriteEpilogue(MF) ;
    }
    return true ;
}

void 
X86ShadowStack::initShadowStack(MachineFunction &MF) {

    // getFirstBasicBlock
    MachineBasicBlock &MBB = MF.front() ;
    MachineBasicBlock::iterator I = MBB.begin() ;
    const DebugLoc &DL = I->getDebugLoc() ;

    // assembly
    /************************
     * 1) get random address: %rand = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) ;
     * 2) set it as %gs     : arch_prctl(SET_GS_BASE, %rand)            ;
     * 3) allocate stack    : %stack_base = mmap(NULL, STACKSIZE, ...)  ; 
     * 4) get stack top     : %stack_top  = %stack_base + STACKSIZE - 8 ;
     * 5) set %gs:108       : mov %stack_top, %gs:108 ;
     ***********************/
    // 1) %rax = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV64ri), X86::RDI)
        .addImm(0x0) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::ESI)
        .addImm(0x1000) ; // alloca a page
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::EDX)
        .addImm(0x3) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::ECX)
        .addImm(0x22);
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::R8D)
        .addImm(0xffffffff) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::R9D)
        .addImm(0x0) ;
    BuildMI(MBB, I, DL, TII->get(X86::CALL64pcrel32))
        .addExternalSymbol("mmap") ;
    
    // 2) arch_prctl(ARCH_SET_GS, %rax) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV64ri32), X86::RDI)
        .addImm(0x1001);
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rr), X86::RSI)
        .addReg(X86::RAX) ; 
    BuildMI(MBB, I, DL, TII->get(X86::CALL64pcrel32))
        .addExternalSymbol("arch_prctl");


    // 3) %rax = mmap(NULL, STACKSIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV64ri), X86::RDI)
        .addImm(0x0) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::ESI)
        .addImm(STACKSIZE) ; // alloca a page
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::EDX)
        .addImm(0x3) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::ECX)
        .addImm(0x22);
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::R8D)
        .addImm(0xffffffff) ;
    BuildMI(MBB, I, DL, TII->get(X86::MOV32ri), X86::R9D)
        .addImm(0x0) ;
    BuildMI(MBB, I, DL, TII->get(X86::CALL64pcrel32))
        .addExternalSymbol("mmap") ;


    // 4) add %rax, STACKSIZE-8  
    BuildMI(MBB, I, DL, TII->get(X86::ADD64ri32), X86::RAX)
        .addImm(STACKSIZE-QWORDSIZE);
    
    // 5) mov %rax, %gs:108
    BuildMI(MBB, I, DL, TII->get(X86::MOV64mr))
        .addReg(0x0)        // Mem: base  register
        .addImm(0x1)        //      scale immediate
        .addReg(0x0)        //      index register 
        .addImm(0x108)      //      disp  immediate      
        .addReg(X86::GS)    //      Segment
        .addReg(X86::RAX);  // Reg
    
    // clean up
    BuildMI(MBB, I, DL, TII->get(X86::XOR64rr), X86::RAX)
        .addReg(X86::RAX)
        .addReg(X86::RAX);
}

void 
X86ShadowStack::writePrologue(MachineFunction &MF) {
    
    // getFirstBasicBlock
    MachineBasicBlock &MBB = MF.front() ;
    MachineBasicBlock::iterator I = MBB.begin() ;
    const DebugLoc &DL = I->getDebugLoc() ;
    
    // assembly
    /****************************
     * 1) mov %rsp, %r1
     * 2) mov %gs:108, %rsp
     * 3) mov 0x8(%rbp), %r0 
     * 4) push %r0
     * 5) mov %rsp, %gs:108
     * 6) mov %r1, %rsp
     ****************************/
    
    Register R0 = MF.getRegInfo().createVirtualRegister(&X86::GR64RegClass) ;
    Register R1 = MF.getRegInfo().createVirtualRegister(&X86::GR64RegClass) ;
    
    
    // 1 )
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rr), R0)
        .addReg(X86::RSP);

    // 2)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rm), X86::RSP)
        .addReg(0)
        .addImm(1)
        .addReg(0)
        .addImm(0x108)
        .addReg(X86::GS) ;

    // 3 )
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rm), R1)
        .addReg(X86::RBP) // base
        .addImm(1)        // scale
        .addReg(0x0)      // index
        .addImm(0x8)
        .addReg(0x0) ;
    

    // 4)
    BuildMI(MBB, I, DL, TII->get(X86::PUSH64r), R1) ;

    // 5)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64mr))
        .addReg(0)
        .addImm(1)
        .addReg(0)
        .addImm(0x108)
        .addReg(X86::GS)
        .addReg(X86::RSP);

    // 6)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rr), X86::RSP)
        .addReg(R0);

}

void
X86ShadowStack::findAndWriteEpilogue(MachineFunction &MF) {
    // TODO: this is weird
    SmallVector<MachineBasicBlock *, 4> retBBs;
    for ( auto &MBB: MF ) {
        // MachineInstr &MI = MBB.instr_back() ;
        for ( auto &MI: MBB ) {
            if ( MI.isReturn() ) {
                // writeEpilogue(MBB);
                retBBs.push_back(&MBB) ;
                break ;
            }   
        }
    }
    for (size_t i =0 ; i < retBBs.size(); ++i ) {
        writeEpilogue(*retBBs[i]);
    } 
}



void 
X86ShadowStack::writeEpilogue(MachineBasicBlock &MBB) {

    // get first instruction
    MachineBasicBlock::iterator I = MBB.begin() ;
    const DebugLoc &DL = I->getDebugLoc() ;
    
    // assembly
    /******************
     * 1) mov %rsp, %r0
     * 2) mov %gs:108, %rsp
     * 3) pop %r1
     * 4) mov %r1, 0x8(%rbp)
     * 5) mov %rsp, %gs:108
     * 6) mov %r0, %rsp
     *********************/
    Register R0 = MBB.getParent()->getRegInfo().createVirtualRegister(&X86::GR64RegClass) ;
    Register R1 = MBB.getParent()->getRegInfo().createVirtualRegister(&X86::GR64RegClass) ;
    
    // 1)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rr), R0)
        .addReg(X86::RSP);
    
    // 2)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rm), X86::RSP)
        .addReg(0)
        .addImm(1)
        .addReg(0)
        .addImm(0x108)
        .addReg(X86::GS);
    
    // 3)
    BuildMI(MBB, I, DL, TII->get(X86::POP64r), R1);

    // 4)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64mr))
        .addReg(X86::RBP)
        .addImm(1)
        .addReg(0)
        .addImm(0x8)
        .addReg(0)
        .addReg(R1);

    // 5)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64mr))
        .addReg(0)
        .addImm(1)
        .addReg(0)
        .addImm(0x108)
        .addReg(X86::GS)
        .addReg(X86::RSP) ;

    // 6)
    BuildMI(MBB, I, DL, TII->get(X86::MOV64rr), X86::RSP)
        .addReg(R0) ;
}

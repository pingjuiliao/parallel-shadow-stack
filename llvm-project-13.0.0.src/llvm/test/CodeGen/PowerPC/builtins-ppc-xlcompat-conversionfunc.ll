; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -verify-machineinstrs -mtriple=powerpc64le-unknown-linux-gnu \
; RUN:   -mcpu=pwr8 < %s | FileCheck %s
; RUN: llc -verify-machineinstrs -mtriple=powerpc64-unknown-linux-gnu \
; RUN:   -mcpu=pwr7 < %s | FileCheck %s
; RUN: llc -verify-machineinstrs -mtriple=powerpc-unknown-aix \
; RUN:   -mcpu=pwr7 < %s | FileCheck %s
; RUN: llc -verify-machineinstrs -mtriple=powerpc64-unknown-aix \
; RUN:   -mcpu=pwr7 < %s | FileCheck %s

declare double @llvm.ppc.fcfid(double %a)
define dso_local double @test_fcfid(double %a) {
; CHECK-LABEL: test_fcfid:
; CHECK:       # %bb.0:
; CHECK-NEXT:    xscvsxddp 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fcfid(double %a)
  ret double %1
}

declare double @llvm.ppc.fcfud(double %a)
define dso_local double @test_fcfud(double %a) {
; CHECK-LABEL: test_fcfud:
; CHECK:       # %bb.0:
; CHECK-NEXT:    xscvuxddp 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fcfud(double %a)
  ret double %1
}

declare double @llvm.ppc.fctid(double %a)
define dso_local double @test_fctid(double %a) {
; CHECK-LABEL: test_fctid:
; CHECK:       # %bb.0:
; CHECK-NEXT:    fctid 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fctid(double %a)
  ret double %1
}

declare double @llvm.ppc.fctidz(double %a)
define dso_local double @test_fctidz(double %a) {
; CHECK-LABEL: test_fctidz:
; CHECK:       # %bb.0:
; CHECK-NEXT:    fctid 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fctid(double %a)
  ret double %1
}

declare double @llvm.ppc.fctiw(double %a)
define dso_local double @test_fctiw(double %a) {
; CHECK-LABEL: test_fctiw:
; CHECK:       # %bb.0:
; CHECK-NEXT:    fctid 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fctid(double %a)
  ret double %1
}

declare double @llvm.ppc.fctiwz(double %a)
define dso_local double @test_fctiwz(double %a) {
; CHECK-LABEL: test_fctiwz:
; CHECK:       # %bb.0:
; CHECK-NEXT:    fctid 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fctid(double %a)
  ret double %1
}

declare double @llvm.ppc.fctudz(double %a)
define dso_local double @test_fctudz(double %a) {
; CHECK-LABEL: test_fctudz:
; CHECK:       # %bb.0:
; CHECK-NEXT:    fctid 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fctid(double %a)
  ret double %1
}

declare double @llvm.ppc.fctuwz(double %a)
define dso_local double @test_fctuwz(double %a) {
; CHECK-LABEL: test_fctuwz:
; CHECK:       # %bb.0:
; CHECK-NEXT:    fctid 1, 1
; CHECK-NEXT:    blr
  %1 = call double @llvm.ppc.fctid(double %a)
  ret double %1
}

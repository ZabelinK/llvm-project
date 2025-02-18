//===- TestAllReduceLowering.cpp - Test gpu.all_reduce lowering -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains test passes for lowering the gpu.all_reduce op.
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/GPU/Passes.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/StandardOps/IR/Ops.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

using namespace mlir;

namespace {
struct TestGpuRewritePass
    : public PassWrapper<TestGpuRewritePass, OperationPass<ModuleOp>> {
  void getDependentDialects(DialectRegistry &registry) const override {
    registry.insert<StandardOpsDialect, memref::MemRefDialect>();
  }
  void runOnOperation() override {
    OwningRewritePatternList patterns(&getContext());
    populateGpuRewritePatterns(patterns);
    (void)applyPatternsAndFoldGreedily(getOperation(), std::move(patterns));
  }
};
} // namespace

namespace mlir {
void registerTestAllReduceLoweringPass() {
  PassRegistration<TestGpuRewritePass> pass(
      "test-gpu-rewrite",
      "Applies all rewrite patterns within the GPU dialect.");
}
} // namespace mlir

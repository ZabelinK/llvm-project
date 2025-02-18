//===- TestPolynomialApproximation.cpp - Test math ops approximations -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains test passes for expanding math operations into
// polynomial approximations.
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/Math/IR/Math.h"
#include "mlir/Dialect/Math/Transforms/Passes.h"
#include "mlir/Dialect/Vector/VectorOps.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

using namespace mlir;

namespace {
struct TestMathPolynomialApproximationPass
    : public PassWrapper<TestMathPolynomialApproximationPass, FunctionPass> {
  void runOnFunction() override;
  void getDependentDialects(DialectRegistry &registry) const override {
    registry
        .insert<vector::VectorDialect, math::MathDialect, LLVM::LLVMDialect>();
  }
};
} // end anonymous namespace

void TestMathPolynomialApproximationPass::runOnFunction() {
  OwningRewritePatternList patterns(&getContext());
  populateMathPolynomialApproximationPatterns(patterns);
  (void)applyPatternsAndFoldGreedily(getOperation(), std::move(patterns));
}

namespace mlir {
namespace test {
void registerTestMathPolynomialApproximationPass() {
  PassRegistration<TestMathPolynomialApproximationPass> pass(
      "test-math-polynomial-approximation",
      "Test math polynomial approximations");
}
} // namespace test
} // namespace mlir

//===- FuncConversions.h - Patterns for converting std.funcs ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This files contains patterns for converting standard functions.
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_STANDARDOPS_TRANSFORMS_FUNCCONVERSIONS_H_
#define MLIR_DIALECT_STANDARDOPS_TRANSFORMS_FUNCCONVERSIONS_H_

namespace mlir {

// Forward declarations.
class ConversionTarget;
class MLIRContext;
class Operation;
class OwningRewritePatternList;
class TypeConverter;

/// Add a pattern to the given pattern list to convert the operand and result
/// types of a CallOp with the given type converter.
void populateCallOpTypeConversionPattern(OwningRewritePatternList &patterns,
                                         TypeConverter &converter);

/// Add a pattern to the given pattern list to rewrite branch operations to use
/// operands that have been legalized by the conversion framework. This can only
/// be done if the branch operation implements the BranchOpInterface. Only
/// needed for partial conversions.
void populateBranchOpInterfaceTypeConversionPattern(
    OwningRewritePatternList &patterns, TypeConverter &converter);

/// Return true if op is a BranchOpInterface op whose operands are all legal
/// according to converter.
bool isLegalForBranchOpInterfaceTypeConversionPattern(Operation *op,
                                                      TypeConverter &converter);

/// Add a pattern to the given pattern list to rewrite `return` ops to use
/// operands that have been legalized by the conversion framework.
void populateReturnOpTypeConversionPattern(OwningRewritePatternList &patterns,
                                           TypeConverter &converter);

/// For ReturnLike ops (except `return`), return True. If op is a `return` &&
/// returnOpAlwaysLegal is false, legalize op according to converter. Otherwise,
/// return false.
bool isLegalForReturnOpTypeConversionPattern(Operation *op,
                                             TypeConverter &converter,
                                             bool returnOpAlwaysLegal = false);

/// Return true if op is neither BranchOpInterface nor ReturnLike.
///
/// TODO Try to get rid of this function and invert the meaning of
/// `isLegalForBranchOpInterfaceTypeConversionPattern` and
/// `isLegalForReturnOpTypeConversionPattern`.
bool isNotBranchOpInterfaceOrReturnLikeOp(Operation *op);
} // end namespace mlir

#endif // MLIR_DIALECT_STANDARDOPS_TRANSFORMS_FUNCCONVERSIONS_H_

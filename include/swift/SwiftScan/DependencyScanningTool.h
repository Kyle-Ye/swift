//===-------------- DependencyScanningTool.h - Swift Compiler -------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2020 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_DEPENDENCY_SCANNING_TOOL_H
#define SWIFT_DEPENDENCY_SCANNING_TOOL_H

#include "swift/AST/ModuleDependencies.h"
#include "swift/Frontend/PrintingDiagnosticConsumer.h"
#include "swift/SwiftScan/ScanDependencies.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/StringSaver.h"

namespace swift {
namespace dependencies {

/// The high-level implementation of the dependency scanner that runs on
/// an individual worker thread.
class DependencyScanningTool {
public:
  /// Construct a dependency scanning tool.
  DependencyScanningTool();

  /// Collect the full module depenedency graph for the input, ignoring any
  /// placeholder modules.
  ///
  /// \returns a \c StringError with the diagnostic output if errors
  /// occurred, \c FullDependencies otherwise.
  llvm::ErrorOr<std::string>
  getDependencies(ArrayRef<const char *> Command,
                  const llvm::StringSet<> &PlaceholderModules);

  /// Collect the full module depenedency graph for the input collection of
  /// module names (batch inputs) and output them to the
  /// BatchScanInput-specified output locations.
  ///
  /// \returns a \c StringError with the diagnostic output if errors
  /// occurred.
  std::error_code getDependencies(ArrayRef<const char *> Command,
                                  const std::vector<BatchScanInput> &BatchInput,
                                  const llvm::StringSet<> &PlaceholderModules);

private:
  /// Using the specified invocation command, instantiate a CompilerInstance
  /// that will be used for this scan.
  llvm::ErrorOr<std::unique_ptr<CompilerInstance>>
  initCompilerInstanceForScan(ArrayRef<const char *> Command);

  /// Shared cache of module dependencies, re-used by individual queries
  /// during the lifetime of this Tool
  std::unique_ptr<ModuleDependenciesCache> SharedCache;

  /// A shared consumer that, for now, just prints the encountered diagnostics
  PrintingDiagnosticConsumer PDC;
  llvm::BumpPtrAllocator Alloc;
  llvm::StringSaver Saver;
};

} // end namespace dependencies
} // end namespace swift

#endif // SWIFT_DEPENDENCY_SCANNING_TOOL_H

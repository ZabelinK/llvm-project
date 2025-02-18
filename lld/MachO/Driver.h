//===- Driver.h -------------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLD_MACHO_DRIVER_H
#define LLD_MACHO_DRIVER_H

#include "lld/Common/LLVM.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Support/MemoryBuffer.h"

#include <set>
#include <type_traits>

namespace llvm {
namespace MachO {
class InterfaceFile;
} // namespace MachO
} // namespace llvm

namespace lld {
namespace macho {

class DylibFile;
class InputFile;

class MachOOptTable : public llvm::opt::OptTable {
public:
  MachOOptTable();
  llvm::opt::InputArgList parse(ArrayRef<const char *> argv);
  void printHelp(const char *argv0, bool showHidden) const;
};

// Create enum with OPT_xxx values for each option in Options.td
enum {
  OPT_INVALID = 0,
#define OPTION(_1, _2, ID, _4, _5, _6, _7, _8, _9, _10, _11, _12) OPT_##ID,
#include "Options.inc"
#undef OPTION
};

void parseLCLinkerOption(InputFile*, unsigned argc, StringRef data);

std::string createResponseFile(const llvm::opt::InputArgList &args);

// Check for both libfoo.dylib and libfoo.tbd (in that order).
llvm::Optional<std::string> resolveDylibPath(llvm::StringRef path);

llvm::Optional<DylibFile *> loadDylib(llvm::MemoryBufferRef mbref,
                                      DylibFile *umbrella = nullptr,
                                      bool isBundleLoader = false);

llvm::Optional<InputFile *> loadArchiveMember(MemoryBufferRef, uint32_t modTime,
                                              StringRef archiveName,
                                              bool objCOnly);

uint32_t getModTime(llvm::StringRef path);

void printArchiveMemberLoad(StringRef reason, const InputFile *);

// Helper class to export dependency info.
class DependencyTracker {
public:
  explicit DependencyTracker(llvm::StringRef path);

  // Adds the given path to the set of not-found files.
  void logFileNotFound(std::string);
  void logFileNotFound(const llvm::Twine &path);

  // Writes the dependencies to specified path.
  // The content is sorted by its Op Code, then within each section,
  // alphabetical order.
  void write(llvm::StringRef version,
             const llvm::SetVector<InputFile *> &inputs,
             llvm::StringRef output);

private:
  enum DepOpCode : char {
    // Denotes the linker version.
    Version = 0x00,
    // Denotes the input files.
    Input = 0x10,
    // Denotes the files that do not exist(?)
    NotFound = 0x11,
    // Denotes the output files.
    Output = 0x40,
  };

  const llvm::StringRef path;
  bool active;

  // The paths need to be alphabetically ordered.
  // We need to own the paths because some of them are temporarily
  // constructed.
  std::set<std::string> notFounds;
};

extern DependencyTracker *depTracker;

} // namespace macho
} // namespace lld

#endif

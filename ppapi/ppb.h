#ifndef ppb_h_
#define ppb_h_

#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_file_io.h"
#include "ppapi/c/ppb_file_ref.h"
#include "ppapi/c/ppb_file_system.h"

struct PPB {
  const PPB_Core* core;
  const PPB_FileIO* file_io;
  const PPB_FileRef* file_ref;
  const PPB_FileSystem* file_system;
};
extern PPB ppb;

bool PPB_Init();

#endif  // ppb_h_

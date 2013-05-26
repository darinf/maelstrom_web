#include "ppb.h"

#include "ppapi_main/ppapi_main.h"

PPB ppb;

bool PPB_Init() {
  ppb.core = reinterpret_cast<const PPB_Core*>(
      PPAPI_GetInterface(PPB_CORE_INTERFACE));
  ppb.file_io = reinterpret_cast<const PPB_FileIO*>(
      PPAPI_GetInterface(PPB_FILEIO_INTERFACE));
  ppb.file_ref = reinterpret_cast<const PPB_FileRef*>(
      PPAPI_GetInterface(PPB_FILEREF_INTERFACE));
  ppb.file_system = reinterpret_cast<const PPB_FileSystem*>(
      PPAPI_GetInterface(PPB_FILESYSTEM_INTERFACE));
  return true;
}

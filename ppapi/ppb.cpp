#include "ppb.h"

#include "ppapi_main/ppapi_main.h"

PPB ppb;

bool PPB_Init() {
  ppb.console = reinterpret_cast<const PPB_Console*>(
      PPAPI_GetInterface(PPB_CONSOLE_INTERFACE));
  ppb.core = reinterpret_cast<const PPB_Core*>(
      PPAPI_GetInterface(PPB_CORE_INTERFACE));
  ppb.file_io = reinterpret_cast<const PPB_FileIO*>(
      PPAPI_GetInterface(PPB_FILEIO_INTERFACE));
  ppb.file_ref = reinterpret_cast<const PPB_FileRef*>(
      PPAPI_GetInterface(PPB_FILEREF_INTERFACE));
  ppb.file_system = reinterpret_cast<const PPB_FileSystem*>(
      PPAPI_GetInterface(PPB_FILESYSTEM_INTERFACE));
  ppb.graphics_2d = reinterpret_cast<const PPB_Graphics2D*>(
      PPAPI_GetInterface(PPB_GRAPHICS_2D_INTERFACE));
  ppb.image_data = reinterpret_cast<const PPB_ImageData*>(
      PPAPI_GetInterface(PPB_IMAGEDATA_INTERFACE));
  ppb.instance = reinterpret_cast<const PPB_Instance*>(
      PPAPI_GetInterface(PPB_INSTANCE_INTERFACE));
  ppb.var = reinterpret_cast<const PPB_Var*>(
      PPAPI_GetInterface(PPB_VAR_INTERFACE));
  return true;
}

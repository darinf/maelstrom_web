#include "ppb.h"

PPB ppb;

bool PPB_Init(PPB_GetInterface get_interface) {
  ppb.audio = reinterpret_cast<const PPB_Audio*>(
      get_interface(PPB_AUDIO_INTERFACE));
  ppb.audio_config = reinterpret_cast<const PPB_AudioConfig*>(
      get_interface(PPB_AUDIO_CONFIG_INTERFACE));
  ppb.console = reinterpret_cast<const PPB_Console*>(
      get_interface(PPB_CONSOLE_INTERFACE));
  ppb.core = reinterpret_cast<const PPB_Core*>(
      get_interface(PPB_CORE_INTERFACE));
  ppb.file_io = reinterpret_cast<const PPB_FileIO*>(
      get_interface(PPB_FILEIO_INTERFACE));
  ppb.file_ref = reinterpret_cast<const PPB_FileRef*>(
      get_interface(PPB_FILEREF_INTERFACE));
  ppb.file_system = reinterpret_cast<const PPB_FileSystem*>(
      get_interface(PPB_FILESYSTEM_INTERFACE));
  ppb.graphics_2d = reinterpret_cast<const PPB_Graphics2D*>(
      get_interface(PPB_GRAPHICS_2D_INTERFACE));
  ppb.image_data = reinterpret_cast<const PPB_ImageData*>(
      get_interface(PPB_IMAGEDATA_INTERFACE));
  ppb.input_event = reinterpret_cast<const PPB_InputEvent*>(
      get_interface(PPB_INPUT_EVENT_INTERFACE));
  ppb.instance = reinterpret_cast<const PPB_Instance*>(
      get_interface(PPB_INSTANCE_INTERFACE));
  ppb.keyboard_input_event = reinterpret_cast<const PPB_KeyboardInputEvent*>(
      get_interface(PPB_KEYBOARD_INPUT_EVENT_INTERFACE));
  ppb.mouse_input_event = reinterpret_cast<const PPB_MouseInputEvent*>(
      get_interface(PPB_MOUSE_INPUT_EVENT_INTERFACE));
  ppb.var = reinterpret_cast<const PPB_Var*>(
      get_interface(PPB_VAR_INTERFACE));
  return true;
}

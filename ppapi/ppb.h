#ifndef ppb_h_
#define ppb_h_

#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_audio.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/c/ppb_console.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_file_io.h"
#include "ppapi/c/ppb_file_ref.h"
#include "ppapi/c/ppb_file_system.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/c/ppb_input_event.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_var.h"

struct PPB {
  const PPB_Audio* audio;
  const PPB_AudioConfig* audio_config;
  const PPB_Console* console;
  const PPB_Core* core;
  const PPB_FileIO* file_io;
  const PPB_FileRef* file_ref;
  const PPB_FileSystem* file_system;
  const PPB_Graphics2D* graphics_2d;
  const PPB_ImageData* image_data;
  const PPB_InputEvent* input_event;
  const PPB_Instance* instance;
  const PPB_KeyboardInputEvent* keyboard_input_event;
  const PPB_MouseInputEvent* mouse_input_event;
  const PPB_Var* var;
};
extern PPB ppb;

bool PPB_Init(PPB_GetInterface get_browser_interface);

#endif  // ppb_h_
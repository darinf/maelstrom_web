#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ppapi/c/pp_errors.h>
#include <ppapi/c/ppp.h>
#include <ppapi/c/ppp_input_event.h>
#include <ppapi/c/ppp_instance.h>
#include "myerror.h"
#include "ppb.h"
#include "pp_resource_queue.h"

PP_Instance g_instance;
PPResourceQueue g_input_queue;
pthread_t g_main_thread;

int Maelstrom_main(int argc, const char* argv[]);

void* MainThreadFunc(void* params) {
  int argc = 1;
  const char* argv[] = { "maelstrom" };

  Maelstrom_main(argc, argv);
  return NULL;
}

PP_Bool Instance_DidCreate(PP_Instance instance,
                           uint32_t argc,
                           const char* argn[],
                           const char* argv[]) {
  g_instance = instance;
  //mesg("Instance_DidCreate");

  ppb.input_event->RequestInputEvents(g_instance, 
                                      PP_INPUTEVENT_CLASS_MOUSE |
                                      PP_INPUTEVENT_CLASS_KEYBOARD);

  
  pthread_create(&g_main_thread, NULL, MainThreadFunc, NULL);
  return PP_TRUE;
}

void Instance_DidDestroy(PP_Instance instance) {
  //mesg("Instance_DidDestroy");
}

void Instance_DidChangeView(PP_Instance instance, PP_Resource view) {
  //mesg("Instance_DidChangeView");
}

void Instance_DidChangeFocus(PP_Instance instance, PP_Bool has_focus) {
  //mesg("Instance_DidChangeFocus");
}

PP_Bool Instance_HandleDocumentLoad(PP_Instance instance, PP_Resource url_loader) {
  //mesg("Instance_HandleDocumentLoad");
  return PP_FALSE;
}

PP_Bool Instance_HandleInputEvent(PP_Instance instance, PP_Resource input_event) {
  //mesg("Instance_HandleInputEvent");
  g_input_queue.Put(input_event);
  return PP_TRUE;
}

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,
                                       PPB_GetInterface get_browser_interface) {
  //mesg("PPP_InitializeModule");

  if (!PPB_Init(get_browser_interface))
    return PP_ERROR_FAILED;

  return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule() {
  //mesg("PPP_ShutdownModule");
}

PP_EXPORT const void* PPP_GetInterface(const char* interface_name) {
  // TODO: report this bug
  //mesg("PPP_GetInterface(%s)", interface_name);

  if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0) {
    static PPP_Instance instance_interface = {
      &Instance_DidCreate, 
      &Instance_DidDestroy,
      &Instance_DidChangeView,
      &Instance_DidChangeFocus,
      &Instance_HandleDocumentLoad
    };
    return &instance_interface;
  }

  if (strcmp(interface_name, PPP_INPUT_EVENT_INTERFACE) == 0) {
    static PPP_InputEvent input_event_interface = {
      &Instance_HandleInputEvent
    };
    return &input_event_interface;
  }

  return NULL;
}

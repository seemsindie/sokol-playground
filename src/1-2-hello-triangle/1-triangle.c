#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "1-triangle.glsl.h"

static struct {
  sg_pipeline pip;
  sg_bindings bind;
  sg_pass_action pass_action;
} state;

void init(void) {
  sg_setup(&(sg_desc){.context = sapp_sgcontext()});

  sg_shader shd = sg_make_shader(simple_shader_desc());

  float vertices[] = {
    // positions
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f, // bottom right
    0.0f, 0.5f, 0.0f, // top
  };

  state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
    .size = sizeof(vertices),
    .content = vertices,
    .label = "triangle-vertices"
  });

  state.pip = sg_make_pipeline(&(sg_pipeline_desc){
    .shader = shd,
    .layout = {
      .attrs = {
        [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3
      }
    },
    .label = "triangle-pipeline"
  });

  state.pass_action =
      (sg_pass_action){.colors[0] = {.action = SG_ACTION_CLEAR,
                                     .val = {0.2f, 0.3f, 0.3f, 1.0f}}};
}

void frame(void) {
  sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);
  sg_draw(0, 3, 1);
  sg_end_pass();
  sg_commit();
}

void cleanup(void) { sg_shutdown(); }

void event(const sapp_event* e) {
  if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
    if (e->key_code == SAPP_KEYCODE_ESCAPE) {
      sapp_request_quit();
    }
  }
}

sapp_desc sokol_main(int argc, char* argv[]) {
  return (sapp_desc){
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
      .width = 800,
      .height = 600,
      .gl_force_gles2 = true,
      .window_title = "Rendering - LearnOpenGL",
  };
}
#include <error.hpp>
#include <framebuffer.hpp>
#include <polygon.hpp>
#include <rectangle.hpp>
#include <circle.hpp>
#include <renderer.hpp>
#include <label.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h> // load after GLFW
#include <numbers>
#include <iostream>

// it's only single file that shows fre2d example window; so it's okay to keep
// it in global scope.
using namespace fre2d;

std::unique_ptr<Renderer> renderer;

void framebuffer_size_callback(GLFWwindow*, int, int);
void window_key_process(GLFWwindow*);
void window_scroll_callback(GLFWwindow*, double, double);

constexpr unsigned Width { 800 };
constexpr unsigned Height { 600 };

float delta_time { 0.f };

int main() {
  glfwInit();
  // use OpenGL 4.5
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  // we use core profile; so for glad generator;
  // don't forget to select core profile.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

  GLFWwindow* window = glfwCreateWindow(Width, Height, "fre2d example", NULL, NULL);

  if (window == NULL) {
    std::cout << "error: failed to create window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetScrollCallback(window, window_scroll_callback);


  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "error: failed to initialize GLAD\n";
    glfwTerminate();
    return -1;
  }


  // we need to enable blending for text rendering.
  // TODO: this might be moved into Renderer initialize() function
  glEnable(GL_BLEND);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(error_callback, 0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // we will draw everything on to this framebuffer.
  Framebuffer custom_framebuffer(Width, Height, false);
  Rectangle custom_framebuffer_quad(Width, Height, detail::drawable::default_position, detail::drawable::default_color);


  constexpr auto main_framebuffer_vertex_shader = detail::framebuffer::default_vertex;
  constexpr auto main_framebuffer_fragment_shader = R"(
#version 450 core
in vec2 TexCoords;
in vec4 Color;
out vec4 FragColor;
uniform sampler2D ScreenTexture; // main framebuffer
uniform sampler2D SecondaryTexture; // secondary framebuffer (custom) pass
void main() {
FragColor = texture(SecondaryTexture, TexCoords) * texture(ScreenTexture, TexCoords) * Color;
}
  )";

  renderer = std::make_unique<Renderer>();
  
  // create custom framebuffer with depth and stencil buffers attached to it.
  // TODO: do it optional since in 2D depth and stencil buffers are not used
  // frequently. also, do not use Framebuffer and Camera separately;
  // otherwise you have to sync window size changes yourself.
  renderer->attach_framebuffer(std::make_unique<Framebuffer>(Width, Height, false, main_framebuffer_vertex_shader, main_framebuffer_fragment_shader));
  renderer->attach_camera(std::make_unique<Camera>(Width, Height));
  renderer->attach_light_manager(std::make_unique<LightManager>());

  renderer->get_light_manager()->initialize();

  renderer->get_light_manager()->push_point_light(
      fre2d::PointLight(
          glm::vec2(-200.f, 200.f),
          glm::vec3 { 1.f, 1.0f, 1.0f },
          glm::vec3 { 0.f, 0.f, 0.9f},
          0.3f,
          0.09f,
          0.0f
      )
  );

  renderer->get_light_manager()->push_point_light(
    fre2d::PointLight(
      glm::vec2(0.f, 200.f),
      glm::vec3 { 1.0f, 1.0f, 1.0f },
      glm::vec3 { 0.f, 0.f, 0.f},
      0.0f,
      0.009f,
      0.00032f
    )
  );

  renderer->get_light_manager()->get_ambient_light_mutable().set_color(
      glm::vec4(0.f, 0.f, 0.f, 0.f)
  );


  renderer->get_light_manager()->update_buffers();

  // we attach SecondaryTexture uniform texture into main framebuffer.
  renderer->get_framebuffer()->set_additional_textures(std::vector {
      AdditionalTexturesInfo{1, "SecondaryTexture", custom_framebuffer.get_color_buffer()}
  });

  // create shader program using default shaders
  Shader default_shader(detail::shader::default_vertex, detail::shader::default_fragment);

  // framebuffer and camera stored as unique_ptr within Renderer class.
  const auto& fb = renderer->get_framebuffer();

  // load texture
  Texture tex("../../example/gechland.apartment.png");
  Texture tex1("../../example/gechland.icon.png");

  Rectangle rect(
    500,
    500,
    glm::vec2(-200.f, 200.f),
    detail::drawable::default_color,
    tex
  );

  // create polygon with given vertices.
  Polygon pol(
      20.f,
      20.f,
      std::vector {
          Vertex2{{-1.5f, -1.5f}, {1.f, 1.f, 1.f, 1.f}}, // left bottom
          Vertex2{{1.5f, -1.5f}, {1.f, 1.f, 1.f, 1.f}}, // right bottom
          Vertex2{{2.f, 1.5f}, {1.f, 0.f, 0.f, 1.f}}, // right top
          Vertex2{{-2.f, 1.5f}, {1.f, 0.f, 0.f, 1.f}} // left top
      },
      {200.f, 100.f},
      tex
  );

  // create already rotated rectangle
  Circle x(
      300,
      300,
      glm::vec2(0.f, 0.f),
      {1.f, 1.f, 1.f, 1.f},
      detail::circle::default_thickness,
      tex,
      std::numbers::pi_v<float> / 3.f
  );

  Circle ring(
      300,
      250,
      glm::vec2{-150.f, -150.f},
      {1.f, 0.f, 1.f, 0.5f},
      0.3f
  );

  // initialize font backend
  FontManager font_manager(true);

  // load font
  Font font(font_manager, "../../example/JetBrainsMono-Regular.ttf", 96);

  // load default text shaders
  Shader text_shader(detail::label::default_vertex, detail::label::default_fragment);
  Shader circle_shader(detail::circle::default_vertex, detail::circle::default_fragment);

  // create label; you can change colors of each vertex.
  // or just set one vec4. for more advanced things; you can use custom shaders.
  // but remember that: fields should stay (i mean uniform variables).
  // we might do custom shading language that compiles to SPIR-V, GLSL and maybe
  // HLSL. but for now since we only support OpenGL it's okay to keep it raw GLSL.
  Label label(
      font,
      "Hello world",
      {250.f, 0.f},
      std::array<glm::vec4, 6> {
          glm::vec4{ 1.f, 0.f, 0.f, 0.5f},
          glm::vec4{1.f, 0.f, 0.f, 1.f},
          glm::vec4{1.f, 1.f, 1.f, 1.f},
          glm::vec4{1.f, 1.f, 1.f, 1.f},
          glm::vec4{1.f, 1.f, 1.f, 1.f},
          glm::vec4{1.f, 1.f, 1.f, 1.f},
      }
  );

  label.set_ignore_zoom(true);

  // used for delta time calculation
  float last_frame { 0.0f };

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // delta time calculation
    auto current_frame { static_cast<float>(glfwGetTime()) };
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    float velocity { 2000.f * delta_time };

    // move label with I    keys
    //                J K L
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
      x.set_position(x.get_position() + glm::vec2{0.f, velocity});

    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
      x.set_position(x.get_position() - glm::vec2{0.f, velocity});

    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
      x.set_position(x.get_position() - glm::vec2{velocity, 0.f});

    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
      x.set_position(x.get_position() + glm::vec2{velocity, 0.f});

    window_key_process(window);

    custom_framebuffer.call([&] {
      custom_framebuffer.clear_color(0.f, 0.f, 0.f, 1.f);
      x.set_rotation(static_cast<GLfloat>(glfwGetTime()));

      // change rotation of Polygon by clockwise winding
      pol.set_rotation(-static_cast<GLfloat>(glfwGetTime()));

      // change rotation of Label by counter-clockwise winding
      // label.set_rotation(static_cast<GLfloat>(glfwGetTime()));

      label.initialize_label(
          font,
          std::to_string(glfwGetTime()).data(),
          label.get_position(),
          detail::drawable::default_color
      );
      // draw objects (TODO: no batch calls for now)
      x.draw(circle_shader, renderer);
      ring.draw(circle_shader, renderer);
      pol.draw(default_shader, renderer);
      rect.draw(default_shader, renderer);
      label.draw(text_shader, renderer);
    });

    custom_framebuffer.render_texture();

    // automatically bind and unbind framebuffer; this can be added to
    // other classes too; which guarantees to unbind in the end.
    fb->call([&] {
      // clear current framebuffer (might be default too)
      // this will apply Mexican filter used in movies.
      // fb->clear_color(1.0f, 0.95f, 0.4f, 1.0f);
      fb->clear_color(1.f, 1.f, 1.f, 1.f);
    });

    // with default framebuffer, this instruction won't affect current one.
    // if you are using custom framebuffers and not default, this call will not do anything
    fb->clear_color(1.0f, 1.0f, 1.0f, 1.0f);

    // render the screen quad with the framebuffer's texture
    fb->render_texture();

    // swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void window_key_process(GLFWwindow* window) {
  // camera movement
  float velocity { 2000.f * delta_time };
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    renderer->get_camera()->set_position(renderer->get_camera()->get_camera_position() + glm::vec3(0.0f, velocity, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    renderer->get_camera()->set_position(renderer->get_camera()->get_camera_position() - glm::vec3(0.0f, velocity, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    renderer->get_camera()->set_position(renderer->get_camera()->get_camera_position() - glm::vec3(velocity, 0.f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    renderer->get_camera()->set_position(renderer->get_camera()->get_camera_position() + glm::vec3(velocity, 0.f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    renderer->get_camera()->set_position(renderer->get_camera()->get_camera_position() + glm::vec3(0.0f, 0.f, velocity));
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    renderer->get_camera()->set_position(renderer->get_camera()->get_camera_position() - glm::vec3(0.0f, 0.f, velocity));

  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    renderer->get_light_manager()->get_point_light_mutable(
                                    renderer->get_light_manager()->get_point_lights().size() - 1
                                    ).get_position_mutable() += glm::vec2(0.0f, velocity);
  if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    renderer->get_light_manager()->get_point_light_mutable(
                                    renderer->get_light_manager()->get_point_lights().size() - 1
                                    ).get_position_mutable() -= glm::vec2(0.0f, velocity);
  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    renderer->get_light_manager()->get_point_light_mutable(
                                    renderer->get_light_manager()->get_point_lights().size() - 1
                                    ).get_position_mutable() -= glm::vec2(velocity, 0.f);
  if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    renderer->get_light_manager()->get_point_light_mutable(
                                    renderer->get_light_manager()->get_point_lights().size() - 1
                                    ).get_position_mutable() += glm::vec2(velocity, 0.f);

  if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    renderer->get_light_manager()->get_ambient_light_mutable().set_color(
        glm::vec4(1.f, 0.f, 0.f, 1.f)
    );

  if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    renderer->get_light_manager()->get_ambient_light_mutable().set_color(
        glm::vec4(0.f, 0.f, 0.f, 1.f)
    );

  if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
    renderer->get_light_manager()->get_point_light_mutable(
        renderer->get_light_manager()->get_point_lights().size() - 1
        ).set_disabled(true);
  }

  if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
    renderer->get_light_manager()->get_point_light_mutable(
                                     renderer->get_light_manager()->get_point_lights().size() - 1
                                     ).set_disabled(false);
  }
}

void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  // there might be exceptions of this but we don't need xoffset for mouse wheel.
  if(yoffset > 0.f) {
    renderer->get_camera()->set_zoom_factor(renderer->get_camera()->get_zoom_factor() + 0.10f);
  } else if(yoffset < 0.f) {
    renderer->get_camera()->set_zoom_factor(renderer->get_camera()->get_zoom_factor() - 0.10f);
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // automatically resize both camera and framebuffer (default too)
  renderer->get_framebuffer()->resize(width, height);
}
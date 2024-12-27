// MIT License
//
// Copyright (c) 2024 Ferhat Geçdoğan All Rights Reserved.
// Distributed under the terms of the MIT License.
//
#pragma once

#include <glad/glad.h>
#include <array>
#include <iostream>
#include <string_view>
#include <source_location>

namespace fre2d {
namespace detail::error {
static constexpr std::array<std::string_view, 9> errors {
  "no error", // GL_NO_ERROR
  "invalid enum", // GL_INVALID_ENUM
  "invalid value", // GL_INVALID_VALUE
  "invalid operation", // GL_INVALID_OPERATION
  "stack overflow", // GL_STACK_OVERFLOW
  "stack underflow", // GL_STACK_UNDERFLOW
  "out of memory", // GL_OUT_OF_MEMORY
  "invalid framebuffer operation", // GL_INVALID_FRAMEBUFFER_OPERATION
  "context lost" // GL_CONTEXT_LOST
};

static constexpr std::array<std::string_view, 6> sources {
  "api", // GL_DEBUG_SOURCE_API
  "window system", // GL_DEBUG_SOURCE_WINDOW_SYSTEM
  "shader compiler", // GL_DEBUG_SOURCE_SHADER_COMPILER
  "third party", // GL_DEBUG_SOURCE_THIRD_PARTY
  "application", // GL_DEBUG_SOURCE_APPLICATION
  "other" // GL_DEBUG_SOURCE_OTHER
};

static constexpr std::array<std::string_view, 9> message_types {
  "error", // GL_DEBUG_TYPE_ERROR
  "deprecated behavior", // GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR
  "undefined behavior", // GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR
  "portability", // GL_DEBUG_TYPE_PORTABILITY
  "performance", // GL_DEBUG_TYPE_PERFORMANCE
  "marker", // GL_DEBUG_TYPE_MARKER
  "push group", // GL_DEBUG_TYPE_PUSH_GROUP
  "pop group", // GL_DEBUG_TYPE_POP_GROUP
  "other" // GL_DEBUG_TYPE_OTHER
};

static constexpr std::array<std::string_view, 4> severity {
  "high", // GL_DEBUG_SEVERITY_HIGH
  "medium", // GL_DEBUG_SEVERITY_MEDIUM
  "low", // GL_DEBUG_SEVERITY_LOW
  "notification" // GL_DEBUG_SEVERITY_NOTIFICATION
};

// maps error codes to array indexes
[[nodiscard]] static constexpr std::uint8_t map_gl_error(const GLenum& error_code) noexcept {
  switch(error_code) {
    // might be better to subtract error_code by 0x0500? but GL_NO_ERROR starts from 0,
    // so probably no need for exceptional case here since it does not differ.
    case GL_NO_ERROR: { return 0; }
    case GL_INVALID_ENUM: { return 1; }
    case GL_INVALID_VALUE: { return 2; }
    case GL_INVALID_OPERATION: { return 3; }
    case GL_STACK_OVERFLOW: { return 4; }
    case GL_STACK_UNDERFLOW: { return 5; }
    case GL_OUT_OF_MEMORY: { return 6; }
    case GL_INVALID_FRAMEBUFFER_OPERATION: { return 7; }
    case GL_CONTEXT_LOST: { return 8; }
    default: { return 0; }
  }
}

// maps message source codes to array indexes
[[nodiscard]] static constexpr std::uint8_t map_gl_message_source(const GLenum& msg_src_code) noexcept {
  switch(msg_src_code) {
    case GL_DEBUG_SOURCE_API: { return 0; }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: { return 1; }
    case GL_DEBUG_SOURCE_SHADER_COMPILER: { return 2; }
    case GL_DEBUG_SOURCE_THIRD_PARTY: { return 3; }
    case GL_DEBUG_SOURCE_APPLICATION: { return 4; }
    case GL_DEBUG_SOURCE_OTHER:
    default: { return 5; }
  }
}

// maps message type codes to array indexes
[[nodiscard]] static constexpr std::uint8_t map_gl_message_type(const GLenum& msg_type_code) noexcept {
  switch(msg_type_code) {
    case GL_DEBUG_TYPE_ERROR: { return 0; }
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: { return 1; }
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: { return 2; }
    case GL_DEBUG_TYPE_PORTABILITY: { return 3; }
    case GL_DEBUG_TYPE_PERFORMANCE: { return 4; }
    case GL_DEBUG_TYPE_MARKER: { return 5; }
    case GL_DEBUG_TYPE_PUSH_GROUP: { return 6; }
    case GL_DEBUG_TYPE_POP_GROUP: { return 7; }
    case GL_DEBUG_TYPE_OTHER:
    default: { return 8; }
  }
}

// maps severity codes to array indexes
[[nodiscard]] static constexpr std::uint8_t map_gl_severity(const GLenum& severity_code) noexcept {
  switch(severity_code) {
    case GL_DEBUG_SEVERITY_HIGH: { return 0; }
    case GL_DEBUG_SEVERITY_MEDIUM: { return 1; }
    case GL_DEBUG_SEVERITY_LOW: { return 2; }
    case GL_DEBUG_SEVERITY_NOTIFICATION:
    default: { return 3; }
  }
}
} // namespace fre2d::detail::error

// can be used with legacy glGetError()
static void check(const GLenum& code, std::source_location loc = std::source_location::current()) noexcept {
  if(code == GL_NO_ERROR) {
    return;
  }
  // TODO: use colorized library here with custom logging impl.
  std::cerr << "OpenGL error: " << detail::error::errors[detail::error::map_gl_error(code)] << '\n'
            << " -- details --\n"
            << ' ' << loc.line() << ":" << loc.column() << '\n'
            << " in " << loc.file_name() << '\n'
            << ' ' << loc.function_name() << '\n';
}

static void GLAPIENTRY error_callback(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam
) {
  std::cerr << "OpenGL error:\n"
               " -- details --\n"
            << "source: " << detail::error::sources[detail::error::map_gl_message_source(source)] << '\n'
            << "type: " << detail::error::message_types[detail::error::map_gl_message_type(type)] << '\n'
            << "id: " << id << '\n'
            << "severity: " << detail::error::severity[detail::error::map_gl_severity(severity)] << '\n'
            << "message: \"" << message << "\"\n"
               " -- end --\n";
}
} // namespace fre2d
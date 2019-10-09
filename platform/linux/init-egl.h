#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <EGL/egl.h>

using namespace std;

void error_fatal(const char* format, ...) {
    printf("error: ");

    va_list va;
    va_start(va, format);
    vprintf(format, va);
    va_end(va);

    printf("\n");
    exit(1);
}

# define ERROR "ERROR"

# define OK(expr) (!(expr) ? (throw "ERROR##expr"), false : true)

void setup_egl(
	EGLNativeWindowType native_window,
	EGLDisplay* out_display,
	EGLConfig* out_config,
	EGLContext* out_context,
	EGLSurface* out_window_surface) {

	OK(eglBindAPI(EGL_OPENGL_ES_API));

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	OK(display != EGL_NO_DISPLAY);

	EGLint ignore = 0;
	OK(eglInitialize(display, &ignore, &ignore));

	EGLint configs_size = 256;
	EGLConfig* configs = new EGLConfig[configs_size];
	EGLint num_configs = 0;

	const EGLint egl_config_attribs[] = {
		EGL_COLOR_BUFFER_TYPE,     EGL_RGB_BUFFER,
		EGL_BUFFER_SIZE,           32,
		EGL_RED_SIZE,              8,
		EGL_GREEN_SIZE,            8,
		EGL_BLUE_SIZE,             8,
		EGL_ALPHA_SIZE,            8,

		EGL_DEPTH_SIZE,            24,
		EGL_STENCIL_SIZE,          8,

		EGL_SAMPLE_BUFFERS,        0,
		EGL_SAMPLES,               0,

		EGL_SURFACE_TYPE,          EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,       EGL_OPENGL_ES2_BIT,

		EGL_NONE,
	};
	OK(eglChooseConfig(
		display,
		egl_config_attribs,
		configs,
		configs_size, // num requested configs
		&num_configs)); // num returned configs
	OK(num_configs);
	EGLConfig config = configs[0];
	delete [] configs;

	const EGLint egl_context_attribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE,
	};
	EGLContext context = eglCreateContext(
		display,
		config,
		EGL_NO_CONTEXT,
		egl_context_attribs);
	OK(context);

	const EGLint egl_surface_attribs[] = {
		EGL_RENDER_BUFFER,
		EGL_BACK_BUFFER,
		EGL_NONE,
	};
	EGLSurface surface = eglCreateWindowSurface(
		display,
		config,
		native_window,
		egl_surface_attribs);
	OK(surface);

	OK(eglMakeCurrent(display, surface, surface, context));

	// Check if surface is double buffered.
	EGLint render_buffer;
	OK(eglQueryContext(
		display,
		context,
		EGL_RENDER_BUFFER,
		&render_buffer));

	if (render_buffer == EGL_SINGLE_BUFFER) {
		printf("warn: EGL surface is single buffered\n");
	}

	*out_display = display;
	*out_config = config;
	*out_context = context;
	*out_window_surface = surface;
}


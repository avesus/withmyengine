# include <array>

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
 
# include <xcb/xcb.h>

# include "./init-egl.h"
# include "./runApp.h"



# include <sys/epoll.h>
# include <fcntl.h>

// # include <sys/socket.h>
# include <unistd.h>

# define ALEN(arr) (sizeof(arr)/sizeof(arr[0]))

int bar () {
	// Create epoll file descriptor
	// int epfd = epoll_create1(EPOLL_CLOEXEC);
	int epfd = epoll_create1(0);

	return epfd;
}

void baz (int epfd, int sfd) {

	// Enable level triggering of arrived data on the fd
	epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.ptr = 0;
	event.data.fd = sfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
}

void bazz (int epfd, int fdDns) {

	fcntl(fdDns, F_SETFL, fcntl(fdDns, F_GETFL) | O_NONBLOCK);

	epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.ptr = 0;
	event.data.fd = fdDns;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fdDns, &event);
}

int events_pump (int epfd, int fdDns, int sfd) {

	epoll_event ready_events[ 1024 ] = { 0 };
	int num_fd_ready = -1;
	do {
		// num_fd_ready = epoll_pwait(epfd, ready_events, ALEN(ready_events), -1, &mask);
		num_fd_ready = epoll_wait(epfd, ready_events, ALEN(ready_events), -1);
		printf("Got an event from the pump\n");


		if (num_fd_ready < 0) {
			break;
		}

		for (int i = 0; i < num_fd_ready; ++i) {
			int fdSource = ready_events[i].data.fd;
			if (fdSource == sfd) {
				printf("Got second FD event from the pump\n");
				return 0;

			} else if (fdSource == fdDns) {

			} else {
				printf("STDIN %d %d\n", num_fd_ready, ready_events[i].data.fd);
				int byte_result = 0;
				read(ready_events[i].data.fd, &byte_result, 1);
			}
		}
	} while (num_fd_ready > 0);

	return 0;
}





xcb_generic_event_t* WaitForEvent(xcb_connection_t *XConnection)
{
    xcb_generic_event_t *Event = nullptr;

    int XCBFileDescriptor = xcb_get_file_descriptor(XConnection);
    //fd_set FileDescriptors;

    //struct timespec Timeout = { 0, 250000000 }; // Check for interruptions every 0.25 seconds

    int wait_on_fd = bar();
    bazz(wait_on_fd, XCBFileDescriptor);

    while (true) {

        // interruptible<std::thread>::check();

        //FD_ZERO(&FileDescriptors);
        //FD_SET(XCBFileDescriptor, &FileDescriptors);

        events_pump(wait_on_fd, 777, XCBFileDescriptor);
        printf("Events pump done\n");

        //if (pselect(XCBFileDescriptor + 1, &FileDescriptors, nullptr, nullptr, &Timeout, nullptr) > 0)
        //{
            if ((Event = xcb_poll_for_event(XConnection)))
                break;
        //}
    }

    // interruptible<std::thread>::check();

    return Event;
}

# include "./c_arguments_parser.h"


/*
See https://unix.stackexchange.com/a/254747/77499 for exit code meanings.
Exit codes 1, 2, 126 to 165, and 255 have special meanings, and should therefore be avoided for user-specified exit parameters:

Exit Code Number Meaning Example Comments
1 Catchall for general errors let "var1 = 1/0" Miscellaneous errors, such as "divide by zero" and other impermissible operations

2 Misuse of shell builtins (according to Bash documentation) empty_function() {} Missing keyword or command, or permission problem (and diff return code on a failed binary file comparison).

126 Command invoked cannot execute /dev/null Permission problem or command is not an executable

127 "command not found" illegal_command Possible problem with $PATH or a typo

128 Invalid argument to exit exit 3.14159 exit takes only integer args in the range 0 - 255 (see first footnote)

128+n Fatal error signal "n" kill -9 $PPID of script $? returns 137 (128 + 9)

130 Script terminated by Control-C Ctl-C Control-C is fatal error signal 2, (130 = 128 + 2, see above)

255* Exit status out of range exit -1 exit takes only integer args in the range 0 - 255

*/


uint8_t std_app (const c_arguments_parser& args) {


	char string[] = "Hello, XCB!";
	uint8_t string_len = strlen(string);
 
	xcb_rectangle_t rectangles[] = {
		{40, 40, 20, 20},
	};
 
	xcb_connection_t* c = xcb_connect(NULL, NULL);
 
	// get the first screen
	xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;
 
	// create black (foreground) graphic context
	/*
	xcb_gcontext_t foreground = xcb_generate_id(c);
	uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	uint32_t values[2] = { screen->black_pixel, 0 };
	xcb_create_gc(c, foreground, screen->root, mask, values);
 
	// create white (background) graphic context
	xcb_gcontext_t background = xcb_generate_id(c);
	mask = XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	values[0] = screen->white_pixel;
	values[1] = 0;
	xcb_create_gc(c, background, screen->root, mask, values);
	*/

 
	xcb_drawable_t win = xcb_generate_id(c);
	// uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t mask = XCB_CW_EVENT_MASK;
	uint32_t values[1] = { //screen->white_pixel,
		XCB_EVENT_MASK_EXPOSURE
		| XCB_EVENT_MASK_KEY_PRESS
		| XCB_EVENT_MASK_KEY_RELEASE
		| XCB_EVENT_MASK_RESIZE_REDIRECT
		| XCB_EVENT_MASK_FOCUS_CHANGE
		| XCB_EVENT_MASK_VISIBILITY_CHANGE
		| XCB_EVENT_MASK_POINTER_MOTION
		| XCB_EVENT_MASK_BUTTON_PRESS
		| XCB_EVENT_MASK_BUTTON_RELEASE };

	int border_width = 0;
	int launch_width = 64;
	int launch_height = 64;
	xcb_create_window(c,
		XCB_COPY_FROM_PARENT,
		win,
		screen->root,
		0, 0,
		launch_width, launch_height,
		border_width,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		screen->root_visual,
		mask, values);
 
	// map the window on the screen
	xcb_map_window(c, win);
 
	xcb_flush(c);
  
	EGLDisplay egl_display;
	EGLConfig egl_config;
	EGLContext egl_context;
	EGLSurface egl_surface;
	setup_egl(
              win,
              &egl_display,
              &egl_config,
              &egl_context,
              &egl_surface);

	// while (xcb_generic_event_t* e = xcb_wait_for_event(c)) {
	while (xcb_generic_event_t* e = WaitForEvent(c)) {

		switch (e->response_type & ~0x80) {

			case XCB_EXPOSE: {
				printf("Exposure\n");
				runApp();
				eglSwapBuffers(egl_display, egl_surface);
				break;
			}

			case XCB_KEY_PRESS:
				goto endloop;

			case XCB_RESIZE_REQUEST: {
				const xcb_resize_request_event_t* cfgEvent
					= (const xcb_resize_request_event_t*)e;

				printf("%dx%d\n", cfgEvent->width, cfgEvent->height);
				break;
			}

			case XCB_CONFIGURE_NOTIFY: {

				const xcb_configure_notify_event_t* cfgEvent = (const xcb_configure_notify_event_t *)e;
				printf("%dx%d\n", cfgEvent->width, cfgEvent->height);
				/*
				if (((cfgEvent->width != width) || (cfgEvent->height != height))) {

					destWidth = cfgEvent->width;
					destHeight = cfgEvent->height;
					if ((destWidth > 0) && (destHeight > 0))
					{
						// Swap chain recreation ins done in this function              
						windowResize();                 
					}
				}*/
			}
		} // switch

		printf("XCB %d\n", e->response_type);

		free(e);
	} // while

  endloop:
 
 	// Exit code can be 0 to 125
	return 0;
}

// Exit codes:
// 0: success
// 1-125: error. Meanings are custom per app. 1 is recommended to be a catch-all
// 126: bash can't run your program because of permission denied "Permission denied"
// 127: bash can't find your program "No such file or directory"
// 128: exit got wrong argument
// 129-192: standard signals (also crashes of program - we can intercept any of these!)
// 129: signal 1
// ...
// 192: signal 64

/*
// Outputs: uint8_t0..255 exit code
class c_app_output_interface {
public:
	virtual void exit_code (uint8_t exit_code) = 0;
	virtual void stdout (const c_stream_block& block) = 0;
};

// Base app can call a callback (delegate functor) each time signal is emitted,
// block of new input data is available from stdin.
// State of arguments is immutable and always available for reading.
// 
// Delegate functors are optional, app does some default processing of inputs.
//
// Question is, can a web app access the same set of arguments, signals,
// and stdin??? If not, don't make these a part of standard interface.

class c_app_input_interface {
public:
	virtual const c_arguments& arguments (void) = 0;
	virtual const c_stream_block& stdin (void) = 0;
};
*/

int main (int args_array_size, const char** args_array) {

	printf("withmyengine %s\n", args_array[0]);

	c_arguments_parser args(args_array_size, args_array);

	return (int)std_app(args);
}


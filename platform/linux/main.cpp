#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
#include <xcb/xcb.h>

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

int eventsPump (int epfd, int fdDns, int sfd) {

  epoll_event ready_events[ 1024 ] = { 0 };
  int num_fd_ready = -1;
  do {
    // num_fd_ready = epoll_pwait(epfd, ready_events, ALEN(ready_events), -1, &mask);
    num_fd_ready = epoll_wait(epfd, ready_events, ALEN(ready_events), -1);


    if (num_fd_ready < 0) {
      break;
    }

    for (int i = 0; i < num_fd_ready; ++i) {
      int fdSource = ready_events[i].data.fd;
      if (fdSource == sfd) {

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













xcb_generic_event_t *WaitForEvent(xcb_connection_t *XConnection)
{
    xcb_generic_event_t *Event = nullptr;

    int XCBFileDescriptor = xcb_get_file_descriptor(XConnection);
    fd_set FileDescriptors;

    struct timespec Timeout = { 0, 250000000 }; // Check for interruptions every 0.25 seconds

    while (true) {

        // interruptible<std::thread>::check();

        FD_ZERO(&FileDescriptors);
        FD_SET(XCBFileDescriptor, &FileDescriptors);

        if (pselect(XCBFileDescriptor + 1, &FileDescriptors, nullptr, nullptr, &Timeout, nullptr) > 0)
        {
            if ((Event = xcb_poll_for_event(XConnection)))
                break;
        }
    }

    // interruptible<std::thread>::check();

    return Event;
}


 
int main () {

 
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

	xcb_create_window(c,
                     XCB_COPY_FROM_PARENT,          /* depth         */
                     win,                           /* window Id     */
                     screen->root,                  /* parent window */
                     0, 0,                          /* x, y          */
                     64, 64,                      /* width, height */
                     0,                            /* border_width  */
                     XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class         */
                     screen->root_visual,           /* visual        */
                     mask, values);                 /* masks         */
 
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
		}

		printf("XCB %d\n", e->response_type);

		free(e);
	}

  endloop:
 
  return 0;
}


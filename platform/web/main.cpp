# include "./browser.h"

int web_impl ();

extern "C" int bind_ref_by_name (int parent_ref_id, const char* key);

extern "C" int create_array (void);

extern "C" int create_string (const char* cstring);

extern "C" void assign_by_string_key_property (int object_ref_id, const char* key, int value_ref);
extern "C" void assign_by_int_key_property (int object_ref_id, int numeric_key, int value_ref);

extern "C" int call_fn (int function_ref_id, int this_ref_id, int arguments_array_ref_id);

const int WINDOW_REF = 1;


/*
 window.addEventListener('load', function () {
    const scriptElt = window.document.createElement('script');
    scriptElt.setAttribute('async', '');
    // scriptElt.setAttribute('type', 'module');
    scriptElt.setAttribute('src', 'data:text/javascript;base64,${ delayedScriptToLoadPageInstantlyBase64 }');
    window.document.head.appendChild(scriptElt);
  });
*/


void consoleLog (const char* msg, ...) {
	int console = bind_ref_by_name(WINDOW_REF, "console");
	int log = bind_ref_by_name(console, "log");
	int args = create_array();
	int message = create_string(msg);
	assign_by_int_key_property(args, 0, message);

	//va_list vargs;
	//va_start(vargs, msg);

	call_fn(log, console, args);

	//va_end(vargs);
}

struct i_window_DOM_event {
	virtual void load () = 0;
};

class c_window_DOM_events_handler : public i_window_DOM_event {
public:
	void load () {
		consoleLog("load");
	}
};

typedef void (i_window_DOM_event::*type_method_evt) ();

c_window_DOM_events_handler o_window_DOM_events_handler;

typedef void (*type_fn_callback) ();

void add_event_listener (int ref_object, const char* event,
	type_fn_callback fn_callback) {

	fn_callback();
}

int init_gpu () {
	/*int Object = bind_ref_by_name(window, "Object");
	int keys = bind_ref_by_name(Object, "keys");
	int args2 = create_array();
	assign_by_int_key_property(args2, 0, window);
	int all_top_level_DOM_object_names = call_fn(keys, Object, args2);*/

	return 0;
}

extern "C" void assign_by_int_key_property_fn (int object_ref_id, int numeric_key,
	type_method_evt value_ref);

__attribute__((visibility("default"))) extern "C"
int init () {
	return init_gpu();
	
	// void* p_handler = (void*)&o_window_DOM_events_handler;
	
	// Abstract interface
	//i_window_DOM_event* p_handler = &o_window_DOM_events_handler;
	// Abstract method
	type_method_evt method_load = &i_window_DOM_event::load;

	//add_event_listener(WINDOW_REF, "load", [=]() {
	//	// Test method ptr:
	//	(p_handler->*method_load)();
	//});

	int window = WINDOW_REF;
	int document = bind_ref_by_name(window, "document");

	int getElementById = bind_ref_by_name(document, "getElementById");
	int args = create_array();
	int canvasElementId = create_string("root");
	assign_by_int_key_property(args, 0, canvasElementId);
	int canvas_DOM_id = call_fn(getElementById, document, args);

	// 1. Find 'console' on 'window':
	int console = bind_ref_by_name(window, "console");
	// 2. Find 'log' on 'console':
	int log = bind_ref_by_name(console, "log");
	// 3. Create a temporary array of arguments for this function call:
	int arguments_array = create_array();
	// 4. Create a string
	int message = create_string("Hello there");
	// 5. Assign a property
	assign_by_int_key_property(arguments_array, 0, message);
	assign_by_int_key_property(arguments_array, 1, canvas_DOM_id);
	assign_by_int_key_property_fn(arguments_array, 2, method_load);
	// 6. Call it
	call_fn(log, console, arguments_array);

	int documentElement = bind_ref_by_name(document, "documentElement");

	assign_by_int_key_property(arguments_array, 0, documentElement);
	call_fn(log, console, arguments_array);

	int Object = bind_ref_by_name(window, "Object");
	int keys = bind_ref_by_name(Object, "keys");
	int args2 = create_array();
	assign_by_int_key_property(args2, 0, window);
	int all_top_level_DOM_object_names = call_fn(keys, Object, args2);

	assign_by_int_key_property(arguments_array, 0, all_top_level_DOM_object_names);
	call_fn(log, console, arguments_array);



	return 0;
}

__attribute__((visibility("default")))
// [[gnu::visibility("default")]]
extern "C" int call_me_2 () {
	return 8888;
}

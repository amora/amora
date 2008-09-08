/**
 * @file   amora.h
 * @author Adenilson Cavalcanti
 * @date   Wed Aug 13 14:21:30 2008
 *
 * @brief  Some upper level amora functions.
 *
 * I should include fundamental upper level functions here.
 */

#ifndef __AMORA_LIB__
#define __AMORA_LIB__

#include <X11/Xlib.h>

/* Foward declaration */
struct service_description;

/** Amora global struct that holds the main resources */
struct amora_s {
	/* Bluetooth dongle used (its required for dbus disconnection code) */
	char hci_str[6];
	/** Log resource */
	struct log_resource *log;
	/** X11 display */
	Display *display;
	/** Service description structure */
	struct service_description *sd;
	/** Server socket */
	int server_socket;
	/** Client socket */
	int client_socket;
	/** Default channel used by amora */
	int channel;
	/** Connection function callback */
	void (*conn_callback) (const char *device_name);
	/** Disconnectoin function callback */
	void (*disconn_callback) (const char *device_name);
};


/** Client file descriptor callback
 *
 * @param client_socket the file descriptor itself
 *
 * @return 0 on success, -1 otherwise
 *
 */
int client_socket_cb(void *context, int client_socket);


/** Server file descriptor callback
 *
 * @param server_socket the file descriptor itself
 *
 * @return 0 on success, -1 otherwise
 *
 */
int server_socket_cb(void *context, int server_socket);


/** Check for protocol commands in buffer, used by \ref process_events
 *
 *
 * @param buffer A string buffer with commands (i.e. CONN_CLOSE), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 * @param client_socket Client socket connection descriptor. This function will
 * take screenshots and write data back to client.
 * display connection to grab screenshots.
 *
 * @return Number of bytes read on success, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
int treat_command(struct amora_s *amora, char *buffer, int length, int client_socket);



/** Check for protocol commands, handle input events (mouse
 * and keyboard).
 *
 *
 * @param buffer A string buffer with commands (i.e. UP, DOWN), see
 * all_codes in \ref protocol.h
 * @param length Buffer length
 * @param client_socket Client socket connection descriptor (really used by
 * \ref treat_command).
 *
 * @return Number of bytes read on success, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
int treat_events(struct amora_s *amora, char *buffer, int length,
		 int client_socket);


/** Process event stream. Reads what new commands are being received
 * in socket and send them to X session.
 *
 * @param fd Socket file descriptor.
 * @param clean_up Free local allocated resources.
 *
 * @return Number of bytes read on success, -1 on error, CONN_CLOSE on exit
 * (see \ref codes).
 */
int process_events(struct amora_s *amora, int fd, int clean_up);


/** Creates a new amora context.
 *
 * It creates the resources (service description, server socket, etc) for
 * running amora server.
 *
 * @param logfile Log file file name + path (pass NULL if you dont need logging).
 *
 * @param channel Channel number to be used (suggests 16).
 *
 * @return The newly created structure or NULL in error case.
 */
struct amora_s *amora_context_new(char *logfile, int channel, int hci_device);


/** Starts amora server listing for connections. Pay attention that this
 * function will not return (will block, so it must be forked or run in
 * another thread if used in a UI app).
 *
 * TODO: add a way to ask loop to exit.
 *
 * @param context An server amora context, created with \ref amora_context_new.
 *
 */
void amora_start(struct amora_s *context);

/** Deletes amora context resources (should be called when app is exiting).
 *
 *
 * @param context A context created with \ref amora_context_new.
 */
void amora_context_delete(struct amora_s *context);


/** Sets a callback function that will be called each time a new client
 * connects with amora server.
 *
 * \todo: return human readable device name.
 *
 * @param context The amora server context.
 *
 * @param conn_cb A callback function pointer with signature:
 * void foo(char *). The name of device just connected will be passed as
 * the string in the callback function.
 */
void amora_connection_callback(struct amora_s *context,
			       void (*conn_cb) (const char *device_name));


#endif

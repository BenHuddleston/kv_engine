/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#pragma once

#include <event.h>
#include <memcached/engine_error.h>
#include <memcached/types.h>
#include <platform/socket.h>
#include <subdoc/operations.h>
#include <gsl/gsl>

#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>

/** \file
 * The main memcached header holding commonly used data
 * structures and function prototypes.
 */

/** Maximum length of a key. */
#define KEY_MAX_LENGTH 250

#define DATA_BUFFER_SIZE 2048
#define MAX_SENDBUF_SIZE (256 * 1024 * 1024)

/* Maximum length of config which can be validated */
#define CONFIG_VALIDATE_MAX_LENGTH (64 * 1024)

/* Maximum IOCTL get/set key and payload (body) length */
#define IOCTL_KEY_LENGTH 128
#define IOCTL_VAL_LENGTH 128

#define MAX_VERBOSITY_LEVEL 2

extern struct stats stats;

// Forward decl
namespace cb {
class Pipe;
}
class Cookie;
class Connection;
struct thread_stats;

void initialize_buckets();
void cleanup_buckets();

void associate_initial_bucket(Connection& connection);


class ListeningPort;
void dispatch_conn_new(SOCKET sfd, std::shared_ptr<ListeningPort>& interface);

void threadlocal_stats_reset(std::vector<thread_stats>& thread_stats);

void notify_io_complete(gsl::not_null<const void*> cookie,
                        ENGINE_ERROR_CODE status);
void safe_close(SOCKET sfd);
int add_conn_to_pending_io_list(Connection* c,
                                Cookie* cookie,
                                ENGINE_ERROR_CODE status);
void listen_event_handler(evutil_socket_t, short, void *);

const char* get_server_version(void);

/**
 * Connection-related functions
 */

/**
 * Increments topkeys count for the key specified within the command context
 * provided by the cookie.
 */
void update_topkeys(const Cookie& cookie);

SERVER_HANDLE_V1* get_server_api();

void shutdown_server();
bool associate_bucket(Connection& connection, const char* name);
void disassociate_bucket(Connection& connection);

/**
 * The executor pool used to pick up the result for requests spawn by the
 * client io threads and dispatched over to a background thread (in order
 * to allow for out of order replies).
 */
namespace cb {
class ExecutorPool;
}
extern std::unique_ptr<cb::ExecutorPool> executorPool;

void iterate_all_connections(std::function<void(Connection&)> callback);

void start_stdin_listener(std::function<void()> function);

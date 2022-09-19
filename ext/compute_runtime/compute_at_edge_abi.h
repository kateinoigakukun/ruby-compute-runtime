#ifndef COMPUTE_RUNTIME_COMPUTE_AT_EDGE_ABI_H
#define COMPUTE_RUNTIME_COMPUTE_AT_EDGE_ABI_H

#include <stddef.h>
#include <stdint.h>

#define WASM_IMPORT(module, name) __attribute__((import_module(module), import_name(name)))

typedef int32_t fastly_status_t;
typedef int32_t response_handle_t;
typedef int32_t body_handle_t;
typedef int32_t body_write_end_t;
typedef int32_t object_store_handle_t;

#define BODY_WRITE_END_BACK 0
#define BODY_WRITE_END_FRONT 1

// Module fastly_http_body
WASM_IMPORT("fastly_http_body", "append")
fastly_status_t fastly_http_body_append(body_handle_t dest, body_handle_t src);

WASM_IMPORT("fastly_http_body", "new")
fastly_status_t fastly_http_body_new(body_handle_t *body_handle);

WASM_IMPORT("fastly_http_body", "close")
fastly_status_t fastly_http_body_close(body_handle_t handle);

WASM_IMPORT("fastly_http_body", "write")
fastly_status_t fastly_http_body_write(body_handle_t body_handle, const uint8_t *buf, size_t buf_len, body_write_end_t end, size_t *nwritten_out);

WASM_IMPORT("fastly_http_body", "read")
fastly_status_t fastly_http_body_read(body_handle_t handle, uint8_t *data, size_t data_max_len, size_t* nwritten);

// Module fastly_http_req
WASM_IMPORT("fastly_http_req", "downstream_client_ip_addr")
int fastly_http_req_downstream_client_ip_addr(uint8_t* octets, size_t* nwritten);

// Module fastly_http_resp
WASM_IMPORT("fastly_http_resp", "new")
fastly_status_t fastly_http_resp_new(response_handle_t *resp_handle);

WASM_IMPORT("fastly_http_resp", "send_downstream")
fastly_status_t fastly_http_resp_send_downstream(response_handle_t resp_handle, body_handle_t body_handle, int32_t streaming);

// Module fastly_object_store
WASM_IMPORT("fastly_object_store", "open")
fastly_status_t fastly_object_store_open(const char* name, size_t name_len, object_store_handle_t* object_store_handle);

WASM_IMPORT("fastly_object_store", "lookup")
fastly_status_t fastly_object_store_lookup(object_store_handle_t object_store_handle, const char* key, size_t key_len, body_handle_t* body_handle);

WASM_IMPORT("fastly_object_store", "insert")
fastly_status_t fastly_object_store_insert(object_store_handle_t object_store_handle, const char* key, size_t key_len, body_handle_t body_handle);

// Module fastly_geo
WASM_IMPORT("fastly_geo", "lookup")
int fastly_geo_lookup(const char *ip, size_t ip_len, char *value, size_t value_max_len, size_t* nwritten);

#endif
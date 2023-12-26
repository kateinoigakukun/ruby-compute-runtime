#ifndef COMPUTE_RUNTIME_COMPUTE_AT_EDGE_ABI_H
#define COMPUTE_RUNTIME_COMPUTE_AT_EDGE_ABI_H

#include <stddef.h>
#include <stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-attributes"

#define WASM_IMPORT(module, name)                                              \
  __attribute__((import_module(module), import_name(name)))

typedef int32_t fastly_status_t;
typedef int32_t response_handle_t;
typedef int32_t request_handle_t;
typedef int32_t pending_request_handle_t;
typedef int32_t body_handle_t;
typedef int32_t body_write_end_t;
typedef int32_t object_store_handle_t;
typedef int32_t log_endpoint_handle_t;
typedef int32_t dictionary_handle_t;

#define BODY_WRITE_END_BACK 0
#define BODY_WRITE_END_FRONT 1

// Module fastly_abi
WASM_IMPORT("fastly_abi", "init")
fastly_status_t fastly_abi_init(uint64_t abi_version);

// Module fastly_uap
WASM_IMPORT("fastly_uap", "parse")
fastly_status_t fastly_uap_parse(const char *user_agent, size_t user_agent_len,
                                 char *family, size_t family_max_len,
                                 size_t *family_nwritten, char *major,
                                 size_t major_max_len, size_t *major_nwritten,
                                 char *minor, size_t minor_max_len,
                                 size_t *minor_nwritten, char *patch,
                                 size_t patch_max_len, size_t *patch_nwritten);

// Module fastly_http_body
WASM_IMPORT("fastly_http_body", "append")
fastly_status_t fastly_http_body_append(body_handle_t dest, body_handle_t src);

WASM_IMPORT("fastly_http_body", "new")
fastly_status_t fastly_http_body_new(body_handle_t *body_handle);

WASM_IMPORT("fastly_http_body", "read")
fastly_status_t fastly_http_body_read(body_handle_t handle, uint8_t *data,
                                      size_t data_max_len, size_t *nwritten);

WASM_IMPORT("fastly_http_body", "write")
fastly_status_t fastly_http_body_write(body_handle_t body_handle,
                                       const uint8_t *buf, size_t buf_len,
                                       body_write_end_t end,
                                       size_t *nwritten_out);

WASM_IMPORT("fastly_http_body", "close")
fastly_status_t fastly_http_body_close(body_handle_t handle);

// Module fastly_log
WASM_IMPORT("fastly_log", "endpoint_get")
fastly_status_t fastly_log_endpoint_get(const char *name, size_t name_len,
                                        log_endpoint_handle_t *endpoint_handle);

WASM_IMPORT("fastly_log", "write")
fastly_status_t fastly_log_write(log_endpoint_handle_t endpoint_handle,
                                 const char *msg, size_t msg_len,
                                 size_t *nwritten);

// Module fastly_http_req
WASM_IMPORT("fastly_http_req", "body_downstream_get")
fastly_status_t fastly_http_req_body_downstream_get(request_handle_t *req_handle_out,
                                               body_handle_t *body_handle_out);

WASM_IMPORT("fastly_http_req", "cache_override_set")
fastly_status_t fastly_http_req_cache_override_set(request_handle_t req_handle,
                                              int tag, uint32_t ttl,
                                              uint32_t stale_while_revalidate);

WASM_IMPORT("fastly_http_req", "cache_override_v2_set")
fastly_status_t
fastly_http_req_cache_override_v2_set(request_handle_t req_handle, int tag,
                                 uint32_t ttl, uint32_t stale_while_revalidate,
                                 const char *surrogate_key,
                                 size_t surrogate_key_len);

WASM_IMPORT("fastly_http_req", "downstream_client_ip_addr")
fastly_status_t fastly_http_req_downstream_client_ip_addr(uint8_t *octets,
                                                          size_t *nwritten);
WASM_IMPORT("fastly_http_req", "new")
fastly_status_t fastly_http_req_new(request_handle_t *req_handle_out);

WASM_IMPORT("fastly_http_req", "header_names_get")
fastly_status_t fastly_http_req_header_names_get(request_handle_t req_handle,
                                            char *buf, size_t buf_len,
                                            uint32_t cursor,
                                            int64_t *ending_cursor,
                                            size_t *nwritten);

WASM_IMPORT("fastly_http_req", "original_header_names_get")
fastly_status_t fastly_http_req_original_header_names_get(char *buf, size_t buf_len,
                                                     uint32_t cursor,
                                                     int64_t *ending_cursor,
                                                     size_t *nwritten);

WASM_IMPORT("fastly_http_req", "original_header_count")
fastly_status_t fastly_http_req_original_header_count(uint32_t *count);

WASM_IMPORT("fastly_http_req", "header_value_get")
fastly_status_t fastly_http_req_header_value_get(request_handle_t req_handle,
                                            const char *name, size_t name_len,
                                            char *value, size_t value_max_len,
                                            size_t *nwritten);

WASM_IMPORT("fastly_http_req", "header_values_get")
fastly_status_t fastly_http_req_header_values_get(
    request_handle_t req_handle, const char *name, size_t name_len, char *buf,
    size_t buf_len, uint32_t cursor, int64_t *ending_cursor, size_t *nwritten);

WASM_IMPORT("fastly_http_req", "header_values_set")
fastly_status_t fastly_http_req_header_values_set(request_handle_t req_handle,
                                             const char *name, size_t name_len,
                                             const char *values,
                                             size_t values_len);

WASM_IMPORT("fastly_http_req", "header_insert")
fastly_status_t fastly_http_req_header_insert(request_handle_t req_handle,
                                         const char *name, size_t name_len,
                                         const char *value, size_t value_len);

WASM_IMPORT("fastly_http_req", "header_append")
fastly_status_t fastly_http_req_header_append(request_handle_t req_handle,
                                         const char *name, size_t name_len,
                                         const char *value, size_t value_len);

WASM_IMPORT("fastly_http_req", "header_remove")
fastly_status_t fastly_http_req_header_remove(request_handle_t req_handle,
                                         const char *name, size_t name_len);

WASM_IMPORT("fastly_http_req", "method_get")
fastly_status_t fastly_http_req_method_get(request_handle_t req_handle, char *method,
                                      size_t method_max_len, size_t *nwritten);

WASM_IMPORT("fastly_http_req", "method_set")
fastly_status_t fastly_http_req_method_set(request_handle_t req_handle,
                                      const char *method, size_t method_len);

WASM_IMPORT("fastly_http_req", "uri_get")
fastly_status_t fastly_http_req_uri_get(request_handle_t req_handle, char *uri,
                                   size_t uri_max_len, size_t *nwritten);

WASM_IMPORT("fastly_http_req", "uri_set")
fastly_status_t fastly_http_req_uri_set(request_handle_t req_handle, const char *uri,
                                   size_t uri_len);

WASM_IMPORT("fastly_http_req", "version_get")
fastly_status_t fastly_http_req_version_get(request_handle_t req_handle,
                                       uint32_t *version);

WASM_IMPORT("fastly_http_req", "version_set")
fastly_status_t fastly_http_req_version_set(request_handle_t req_handle,
                                       uint32_t version);

WASM_IMPORT("fastly_http_req", "send")
fastly_status_t fastly_http_req_send(request_handle_t req_handle,
                                body_handle_t body_handle, const char *backend,
                                size_t backend_len,
                                response_handle_t *resp_handle_out,
                                body_handle_t *resp_body_handle_out);

WASM_IMPORT("fastly_http_req", "send_async")
fastly_status_t
fastly_http_req_send_async(request_handle_t req_handle, body_handle_t body_handle,
                      const char *backend, size_t backend_len,
                      pending_request_handle_t *pending_req_out);

WASM_IMPORT("fastly_http_req", "send_async_streaming")
fastly_status_t fastly_http_req_send_async_streaming(
    request_handle_t req_handle, body_handle_t body_handle, const char *backend,
    size_t backend_len, pending_request_handle_t *pending_req_out);

WASM_IMPORT("fastly_http_req", "pending_req_poll")
fastly_status_t fastly_http_req_pending_req_poll(
    pending_request_handle_t req_handle, uint32_t *is_done_out,
    response_handle_t *resp_handle_out, body_handle_t *resp_body_handle_out);

WASM_IMPORT("fastly_http_req", "pending_req_wait")
fastly_status_t
fastly_http_req_pending_req_wait(pending_request_handle_t req_handle,
                            response_handle_t *resp_handle_out,
                            body_handle_t *resp_body_handle_out);

WASM_IMPORT("fastly_http_req", "pending_req_select")
fastly_status_t
fastly_http_req_pending_req_select(pending_request_handle_t req_handles[],
                              size_t req_handles_len, uint32_t *done_idx_out,
                              response_handle_t *resp_handle_out,
                              body_handle_t *resp_body_handle_out);

// Module fastly_http_resp
WASM_IMPORT("fastly_http_resp", "new")
fastly_status_t fastly_http_resp_new(response_handle_t *resp_handle);

WASM_IMPORT("fastly_http_resp", "header_names_get")
fastly_status_t fastly_http_resp_header_names_get(response_handle_t resp_handle,
                                                  char *buf, size_t buf_len,
                                                  uint32_t cursor,
                                                  int64_t *ending_cursor,
                                                  size_t *nwritten);

WASM_IMPORT("fastly_http_resp", "header_value_get")
fastly_status_t fastly_http_resp_header_value_get(response_handle_t resp_handle,
                                                  const char *name,
                                                  size_t name_len, char *value,
                                                  size_t value_max_len,
                                                  size_t *nwritten);

WASM_IMPORT("fastly_http_resp", "header_values_get")
fastly_status_t fastly_http_resp_header_values_get(
    response_handle_t resp_handle, const char *name, size_t name_len, char *buf,
    size_t buf_len, uint32_t cursor, int64_t *ending_cursor, size_t *nwritten);

WASM_IMPORT("fastly_http_resp", "header_values_set")
fastly_status_t
fastly_http_resp_header_values_set(response_handle_t resp_handle,
                                   const char *name, size_t name_len,
                                   const char *buf, size_t buf_len);

WASM_IMPORT("fastly_http_resp", "header_insert")
fastly_status_t fastly_http_resp_header_insert(response_handle_t resp_handle,
                                               const char *name,
                                               size_t name_len,
                                               const char *value,
                                               size_t value_len);

WASM_IMPORT("fastly_http_resp", "header_append")
fastly_status_t fastly_http_resp_header_append(response_handle_t resp_handle,
                                               const char *name,
                                               size_t name_len,
                                               const char *value,
                                               size_t value_len);

WASM_IMPORT("fastly_http_resp", "header_remove")
fastly_status_t fastly_http_resp_header_remove(response_handle_t resp_handle,
                                               const char *name,
                                               size_t name_len);

WASM_IMPORT("fastly_http_resp", "version_get")
fastly_status_t fastly_http_resp_version_get(response_handle_t resp_handle,
                                             uint32_t *version_out);

WASM_IMPORT("fastly_http_resp", "version_set")
fastly_status_t fastly_http_resp_version_set(response_handle_t resp_handle,
                                             uint32_t version);

WASM_IMPORT("fastly_http_resp", "send_downstream")
fastly_status_t fastly_http_resp_send_downstream(response_handle_t resp_handle,
                                                 body_handle_t body_handle,
                                                 int32_t streaming);

WASM_IMPORT("fastly_http_resp", "status_get")
fastly_status_t fastly_http_resp_status_get(response_handle_t resp_handle,
                                            uint16_t *status_out);

WASM_IMPORT("fastly_http_resp", "status_set")
fastly_status_t fastly_http_resp_status_set(response_handle_t resp_handle,
                                            uint16_t status);

// Module fastly_dictionary
WASM_IMPORT("fastly_dictionary", "open")
fastly_status_t
fastly_dictionary_open(const char *name, size_t name_len,
                            dictionary_handle_t *dict_handle_out);

WASM_IMPORT("fastly_dictionary", "get")
fastly_status_t fastly_dictionary_get(dictionary_handle_t dict_handle,
                                           const char *key, size_t key_len,
                                           char *value, size_t value_max_len,
                                           size_t *nwritten);

// Module fastly_object_store
WASM_IMPORT("fastly_object_store", "open")
fastly_status_t
fastly_object_store_open(const char *name, size_t name_len,
                         object_store_handle_t *object_store_handle);

WASM_IMPORT("fastly_object_store", "lookup")
fastly_status_t
fastly_object_store_lookup(object_store_handle_t object_store_handle,
                           const char *key, size_t key_len,
                           body_handle_t *body_handle);

WASM_IMPORT("fastly_object_store", "insert")
fastly_status_t
fastly_object_store_insert(object_store_handle_t object_store_handle,
                           const char *key, size_t key_len,
                           body_handle_t body_handle);

// Module fastly_geo
WASM_IMPORT("fastly_geo", "lookup")
int fastly_geo_lookup(const char *ip, size_t ip_len, char *value,
                      size_t value_max_len, size_t *nwritten);

#pragma GCC diagnostic pop

#endif

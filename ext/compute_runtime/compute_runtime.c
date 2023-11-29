#include "compute_runtime.h"
#include "compute_at_edge_abi.h"

RBIMPL_ATTR_NORETURN()
static void _raise_fastly_error(fastly_status_t status, int line, const char *func) {
  rb_raise(rb_eRuntimeError, "Fastly error: %d at %s:%d", status, func, line);
}

#define HANDLE_FASTLY_ERROR(status) if (status != 0) _raise_fastly_error(status, __LINE__, __func__);

static VALUE _fastly_http_body_new(VALUE obj)
{
  body_handle_t body_handle;
  fastly_status_t status = fastly_http_body_new(&body_handle);
  HANDLE_FASTLY_ERROR(status);
  return INT2NUM(body_handle);
}

static VALUE _fastly_http_body_append(VALUE obj, VALUE dest, VALUE src)
{
  fastly_status_t status = fastly_http_body_append(NUM2INT(dest), NUM2INT(src));
  HANDLE_FASTLY_ERROR(status);
  return Qnil;
}

static VALUE _fastly_http_body_close(VALUE obj, VALUE handle)
{
  fastly_status_t status = fastly_http_body_close(NUM2INT(handle));
  HANDLE_FASTLY_ERROR(status);
  return Qnil;
}

static VALUE _fastly_http_body_write(VALUE obj, VALUE body_handle, VALUE buf, VALUE end)
{
  body_handle_t handle = NUM2INT(body_handle);
  const uint8_t *buf_ptr = (const uint8_t *)RSTRING_PTR(buf);
  size_t buf_len = RSTRING_LEN(buf);
  body_write_end_t write_end = NUM2INT(end);
  size_t nwritten;
  fastly_status_t status = fastly_http_body_write(handle, buf_ptr, buf_len, write_end, &nwritten);
  HANDLE_FASTLY_ERROR(status);
  return RB_SIZE2NUM(nwritten);
}

static VALUE _fastly_http_body_read(VALUE obj, VALUE handle, VALUE data, VALUE data_max_len)
{
  body_handle_t body_handle = NUM2INT(handle);
  uint8_t* data_ptr = (uint8_t*)RSTRING_PTR(data);
  rb_str_modify(data);
  size_t data_max_len_val = NUM2INT(data_max_len);
  size_t nwritten;
  fastly_status_t status = fastly_http_body_read(body_handle, data_ptr, data_max_len_val, &nwritten);
  HANDLE_FASTLY_ERROR(status);
  rb_str_set_len(data, nwritten);
  return RB_SIZE2NUM(nwritten);
}

static VALUE _fastly_http_req_downstream_client_ip_addr(VALUE obj)
{
  uint8_t octets[16];
  size_t nwritten;
  int status = fastly_http_req_downstream_client_ip_addr(octets, &nwritten);
  HANDLE_FASTLY_ERROR(status);
  return rb_str_new((const char*)octets, nwritten);
}

static VALUE _fastly_http_resp_new(VALUE obj) {
  response_handle_t resp_handle;
  fastly_status_t status = fastly_http_resp_new(&resp_handle);
  HANDLE_FASTLY_ERROR(status);
  return INT2NUM(resp_handle);
}

static VALUE _fastly_http_resp_send_downstream(VALUE obj, VALUE resp_handle, VALUE body_handle, VALUE streaming) {
  response_handle_t handle = NUM2INT(resp_handle);
  body_handle_t body_handle_handle = NUM2INT(body_handle);
  int32_t is_streaming = NUM2INT(streaming);
  fastly_status_t status = fastly_http_resp_send_downstream(handle, body_handle_handle, is_streaming);
  HANDLE_FASTLY_ERROR(status);
  return Qnil;
}

static VALUE _fastly_object_store_open(VALUE obj, VALUE name) {
  object_store_handle_t object_store_handle;
  const char *name_ptr = RSTRING_PTR(name);
  size_t name_len = RSTRING_LEN(name);
  fastly_status_t status = fastly_object_store_open(name_ptr, name_len, &object_store_handle);
  HANDLE_FASTLY_ERROR(status);
  return INT2NUM(object_store_handle);
}

static VALUE _fastly_object_store_lookup(VALUE obj, VALUE object_store_handle, VALUE key) {
  object_store_handle_t handle = NUM2INT(object_store_handle);
  const char *key_ptr = RSTRING_PTR(key);
  size_t key_len = RSTRING_LEN(key);
  body_handle_t body_handle;
  fastly_status_t status = fastly_object_store_lookup(handle, key_ptr, key_len, &body_handle);
  HANDLE_FASTLY_ERROR(status);
  return INT2NUM(body_handle);
}

static VALUE _fastly_object_store_insert(VALUE obj, VALUE object_store_handle, VALUE key, VALUE body_handle) {
  object_store_handle_t handle = NUM2INT(object_store_handle);
  const char *key_ptr = RSTRING_PTR(key);
  size_t key_len = RSTRING_LEN(key);
  body_handle_t body_handle_handle = NUM2INT(body_handle);
  fastly_status_t status = fastly_object_store_insert(handle, key_ptr, key_len, body_handle_handle);
  HANDLE_FASTLY_ERROR(status);
  return Qnil;
}

static VALUE _fastly_geo_lookup(VALUE obj, VALUE ip, VALUE value, VALUE value_max_len) {
  const char *ip_ptr = RSTRING_PTR(ip);
  size_t ip_len = RSTRING_LEN(ip);
  char *value_ptr = RSTRING_PTR(value);
  rb_str_modify(value);
  size_t nwritten;
  fastly_status_t status = fastly_geo_lookup(ip_ptr, ip_len, value_ptr, value_max_len, &nwritten);
  HANDLE_FASTLY_ERROR(status);
  rb_str_set_len(value, nwritten);
  return RB_SIZE2NUM(nwritten);
}

VALUE rb_mComputeRuntime;

void
Init_compute_runtime(void)
{
  rb_mComputeRuntime = rb_define_module("ComputeRuntime");
  VALUE rb_mABI = rb_define_module_under(rb_mComputeRuntime, "ABI");

  rb_define_module_function(rb_mABI, "fastly_http_resp_new", _fastly_http_resp_new, 0);
  rb_define_module_function(rb_mABI, "fastly_http_resp_send_downstream", _fastly_http_resp_send_downstream, 3);
  rb_define_module_function(rb_mABI, "fastly_http_req_downstream_client_ip_addr", _fastly_http_req_downstream_client_ip_addr, 0);
  rb_define_module_function(rb_mABI, "fastly_http_body_new", _fastly_http_body_new, 0);
  rb_define_module_function(rb_mABI, "fastly_http_body_append", _fastly_http_body_append, 2);
  rb_define_module_function(rb_mABI, "fastly_http_body_close", _fastly_http_body_close, 1);
  rb_define_module_function(rb_mABI, "fastly_http_body_write", _fastly_http_body_write, 3);
  rb_define_module_function(rb_mABI, "fastly_http_body_read", _fastly_http_body_read, 3);
  rb_define_module_function(rb_mABI, "fastly_object_store_open", _fastly_object_store_open, 1);
  rb_define_module_function(rb_mABI, "fastly_object_store_lookup", _fastly_object_store_lookup, 2);
  rb_define_module_function(rb_mABI, "fastly_object_store_insert", _fastly_object_store_insert, 3);
  rb_define_module_function(rb_mABI, "fastly_geo_lookup", _fastly_geo_lookup, 3);
}

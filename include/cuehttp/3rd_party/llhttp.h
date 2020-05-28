#ifndef INCLUDE_LLHTTP_H_
#define INCLUDE_LLHTTP_H_

#define LLHTTP_VERSION_MAJOR 2
#define LLHTTP_VERSION_MINOR 0
#define LLHTTP_VERSION_PATCH 5

#ifndef INCLUDE_LLHTTP_ITSELF_H_
#define INCLUDE_LLHTTP_ITSELF_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct llhttp__internal_s llhttp__internal_t;
struct llhttp__internal_s {
  int32_t _index;
  void* _span_pos0;
  void* _span_cb0;
  int32_t error;
  const char* reason;
  const char* error_pos;
  void* data;
  void* _current;
  uint64_t content_length;
  uint8_t type;
  uint8_t method;
  uint8_t http_major;
  uint8_t http_minor;
  uint8_t header_state;
  uint16_t flags;
  uint8_t upgrade;
  uint16_t status_code;
  uint8_t finish;
  void* settings;
};

int llhttp__internal_init(llhttp__internal_t* s);
int llhttp__internal_execute(llhttp__internal_t* s, const char* p, const char* endp);

#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif  /* INCLUDE_LLHTTP_ITSELF_H_ */

#ifndef LLLLHTTP_C_HEADERS_
#define LLLLHTTP_C_HEADERS_
#ifdef __cplusplus
extern "C" {
#endif

enum llhttp_errno {
  HPE_OK = 0,
  HPE_INTERNAL = 1,
  HPE_STRICT = 2,
  HPE_LF_EXPECTED = 3,
  HPE_UNEXPECTED_CONTENT_LENGTH = 4,
  HPE_CLOSED_CONNECTION = 5,
  HPE_INVALID_METHOD = 6,
  HPE_INVALID_URL = 7,
  HPE_INVALID_CONSTANT = 8,
  HPE_INVALID_VERSION = 9,
  HPE_INVALID_HEADER_TOKEN = 10,
  HPE_INVALID_CONTENT_LENGTH = 11,
  HPE_INVALID_CHUNK_SIZE = 12,
  HPE_INVALID_STATUS = 13,
  HPE_INVALID_EOF_STATE = 14,
  HPE_INVALID_TRANSFER_ENCODING = 15,
  HPE_CB_MESSAGE_BEGIN = 16,
  HPE_CB_HEADERS_COMPLETE = 17,
  HPE_CB_MESSAGE_COMPLETE = 18,
  HPE_CB_CHUNK_HEADER = 19,
  HPE_CB_CHUNK_COMPLETE = 20,
  HPE_PAUSED = 21,
  HPE_PAUSED_UPGRADE = 22,
  HPE_USER = 23
};
typedef enum llhttp_errno llhttp_errno_t;

enum llhttp_flags {
  F_CONNECTION_KEEP_ALIVE = 0x1,
  F_CONNECTION_CLOSE = 0x2,
  F_CONNECTION_UPGRADE = 0x4,
  F_CHUNKED = 0x8,
  F_UPGRADE = 0x10,
  F_CONTENT_LENGTH = 0x20,
  F_SKIPBODY = 0x40,
  F_TRAILING = 0x80,
  F_LENIENT = 0x100,
  F_TRANSFER_ENCODING = 0x200
};
typedef enum llhttp_flags llhttp_flags_t;

enum llhttp_type {
  HTTP_BOTH = 0,
  HTTP_REQUEST = 1,
  HTTP_RESPONSE = 2
};
typedef enum llhttp_type llhttp_type_t;

enum llhttp_finish {
  HTTP_FINISH_SAFE = 0,
  HTTP_FINISH_SAFE_WITH_CB = 1,
  HTTP_FINISH_UNSAFE = 2
};
typedef enum llhttp_finish llhttp_finish_t;

enum llhttp_method {
  HTTP_DELETE = 0,
  HTTP_GET = 1,
  HTTP_HEAD = 2,
  HTTP_POST = 3,
  HTTP_PUT = 4,
  HTTP_CONNECT = 5,
  HTTP_OPTIONS = 6,
  HTTP_TRACE = 7,
  HTTP_COPY = 8,
  HTTP_LOCK = 9,
  HTTP_MKCOL = 10,
  HTTP_MOVE = 11,
  HTTP_PROPFIND = 12,
  HTTP_PROPPATCH = 13,
  HTTP_SEARCH = 14,
  HTTP_UNLOCK = 15,
  HTTP_BIND = 16,
  HTTP_REBIND = 17,
  HTTP_UNBIND = 18,
  HTTP_ACL = 19,
  HTTP_REPORT = 20,
  HTTP_MKACTIVITY = 21,
  HTTP_CHECKOUT = 22,
  HTTP_MERGE = 23,
  HTTP_MSEARCH = 24,
  HTTP_NOTIFY = 25,
  HTTP_SUBSCRIBE = 26,
  HTTP_UNSUBSCRIBE = 27,
  HTTP_PATCH = 28,
  HTTP_PURGE = 29,
  HTTP_MKCALENDAR = 30,
  HTTP_LINK = 31,
  HTTP_UNLINK = 32,
  HTTP_SOURCE = 33,
  HTTP_PRI = 34
};
typedef enum llhttp_method llhttp_method_t;

#define HTTP_ERRNO_MAP(XX) \
  XX(0, OK, OK) \
  XX(1, INTERNAL, INTERNAL) \
  XX(2, STRICT, STRICT) \
  XX(3, LF_EXPECTED, LF_EXPECTED) \
  XX(4, UNEXPECTED_CONTENT_LENGTH, UNEXPECTED_CONTENT_LENGTH) \
  XX(5, CLOSED_CONNECTION, CLOSED_CONNECTION) \
  XX(6, INVALID_METHOD, INVALID_METHOD) \
  XX(7, INVALID_URL, INVALID_URL) \
  XX(8, INVALID_CONSTANT, INVALID_CONSTANT) \
  XX(9, INVALID_VERSION, INVALID_VERSION) \
  XX(10, INVALID_HEADER_TOKEN, INVALID_HEADER_TOKEN) \
  XX(11, INVALID_CONTENT_LENGTH, INVALID_CONTENT_LENGTH) \
  XX(12, INVALID_CHUNK_SIZE, INVALID_CHUNK_SIZE) \
  XX(13, INVALID_STATUS, INVALID_STATUS) \
  XX(14, INVALID_EOF_STATE, INVALID_EOF_STATE) \
  XX(15, INVALID_TRANSFER_ENCODING, INVALID_TRANSFER_ENCODING) \
  XX(16, CB_MESSAGE_BEGIN, CB_MESSAGE_BEGIN) \
  XX(17, CB_HEADERS_COMPLETE, CB_HEADERS_COMPLETE) \
  XX(18, CB_MESSAGE_COMPLETE, CB_MESSAGE_COMPLETE) \
  XX(19, CB_CHUNK_HEADER, CB_CHUNK_HEADER) \
  XX(20, CB_CHUNK_COMPLETE, CB_CHUNK_COMPLETE) \
  XX(21, PAUSED, PAUSED) \
  XX(22, PAUSED_UPGRADE, PAUSED_UPGRADE) \
  XX(23, USER, USER) \


#define HTTP_METHOD_MAP(XX) \
  XX(0, DELETE, DELETE) \
  XX(1, GET, GET) \
  XX(2, HEAD, HEAD) \
  XX(3, POST, POST) \
  XX(4, PUT, PUT) \
  XX(5, CONNECT, CONNECT) \
  XX(6, OPTIONS, OPTIONS) \
  XX(7, TRACE, TRACE) \
  XX(8, COPY, COPY) \
  XX(9, LOCK, LOCK) \
  XX(10, MKCOL, MKCOL) \
  XX(11, MOVE, MOVE) \
  XX(12, PROPFIND, PROPFIND) \
  XX(13, PROPPATCH, PROPPATCH) \
  XX(14, SEARCH, SEARCH) \
  XX(15, UNLOCK, UNLOCK) \
  XX(16, BIND, BIND) \
  XX(17, REBIND, REBIND) \
  XX(18, UNBIND, UNBIND) \
  XX(19, ACL, ACL) \
  XX(20, REPORT, REPORT) \
  XX(21, MKACTIVITY, MKACTIVITY) \
  XX(22, CHECKOUT, CHECKOUT) \
  XX(23, MERGE, MERGE) \
  XX(24, MSEARCH, M-SEARCH) \
  XX(25, NOTIFY, NOTIFY) \
  XX(26, SUBSCRIBE, SUBSCRIBE) \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE) \
  XX(28, PATCH, PATCH) \
  XX(29, PURGE, PURGE) \
  XX(30, MKCALENDAR, MKCALENDAR) \
  XX(31, LINK, LINK) \
  XX(32, UNLINK, UNLINK) \
  XX(33, SOURCE, SOURCE) \
  XX(34, PRI, PRI) \



#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif  /* LLLLHTTP_C_HEADERS_ */

#ifndef INCLUDE_LLHTTP_API_H_
#define INCLUDE_LLHTTP_API_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>

typedef llhttp__internal_t llhttp_t;
typedef struct llhttp_settings_s llhttp_settings_t;

typedef int (*llhttp_data_cb)(llhttp_t*, const char *at, size_t length);
typedef int (*llhttp_cb)(llhttp_t*);

struct llhttp_settings_s {
  /* Possible return values 0, -1, `HPE_PAUSED` */
  llhttp_cb      on_message_begin;

  llhttp_data_cb on_url;
  llhttp_data_cb on_status;
  llhttp_data_cb on_header_field;
  llhttp_data_cb on_header_value;

  /* Possible return values:
   * 0  - Proceed normally
   * 1  - Assume that request/response has no body, and proceed to parsing the
   *      next message
   * 2  - Assume absence of body (as above) and make `llhttp_execute()` return
   *      `HPE_PAUSED_UPGRADE`
   * -1 - Error
   * `HPE_PAUSED`
   */
  llhttp_cb      on_headers_complete;

  llhttp_data_cb on_body;

  /* Possible return values 0, -1, `HPE_PAUSED` */
  llhttp_data_cb on_message_complete;

  /* When on_chunk_header is called, the current chunk length is stored
   * in parser->content_length.
   * Possible return values 0, -1, `HPE_PAUSED`
   */
  llhttp_cb      on_chunk_header;
  llhttp_cb      on_chunk_complete;
};

/* Initialize the parser with specific type and user settings */
void llhttp_init(llhttp_t* parser, llhttp_type_t type,
                 const llhttp_settings_t* settings);

/* Initialize the settings object */
void llhttp_settings_init(llhttp_settings_t* settings);

/* Parse full or partial request/response, invoking user callbacks along the
 * way.
 *
 * If any of `llhttp_data_cb` returns errno not equal to `HPE_OK` - the parsing
 * interrupts, and such errno is returned from `llhttp_execute()`. If
 * `HPE_PAUSED` was used as a errno, the execution can be resumed with
 * `llhttp_resume()` call.
 *
 * In a special case of CONNECT/Upgrade request/response `HPE_PAUSED_UPGRADE`
 * is returned after fully parsing the request/response. If the user wishes to
 * continue parsing, they need to invoke `llhttp_resume_after_upgrade()`.
 *
 * NOTE: if this function ever returns a non-pause type error, it will continue
 * to return the same error upon each successive call up until `llhttp_init()`
 * is called.
 */
llhttp_errno_t llhttp_execute(llhttp_t* parser, const char* data, size_t len);

/* This method should be called when the other side has no further bytes to
 * send (e.g. shutdown of readable side of the TCP connection.)
 *
 * Requests without `Content-Length` and other messages might require treating
 * all incoming bytes as the part of the body, up to the last byte of the
 * connection. This method will invoke `on_message_complete()` callback if the
 * request was terminated safely. Otherwise a error code would be returned.
 */
llhttp_errno_t llhttp_finish(llhttp_t* parser);

/* Returns `1` if the incoming message is parsed until the last byte, and has
 * to be completed by calling `llhttp_finish()` on EOF
 */
int llhttp_message_needs_eof(const llhttp_t* parser);

/* Returns `1` if there might be any other messages following the last that was
 * successfully parsed.
 */
int llhttp_should_keep_alive(const llhttp_t* parser);

/* Make further calls of `llhttp_execute()` return `HPE_PAUSED` and set
 * appropriate error reason.
 *
 * Important: do not call this from user callbacks! User callbacks must return
 * `HPE_PAUSED` if pausing is required.
 */
void llhttp_pause(llhttp_t* parser);

/* Might be called to resume the execution after the pause in user's callback.
 * See `llhttp_execute()` above for details.
 *
 * Call this only if `llhttp_execute()` returns `HPE_PAUSED`.
 */
void llhttp_resume(llhttp_t* parser);

/* Might be called to resume the execution after the pause in user's callback.
 * See `llhttp_execute()` above for details.
 *
 * Call this only if `llhttp_execute()` returns `HPE_PAUSED_UPGRADE`
 */
void llhttp_resume_after_upgrade(llhttp_t* parser);

/* Returns the latest return error */
llhttp_errno_t llhttp_get_errno(const llhttp_t* parser);

/* Returns the verbal explanation of the latest returned error.
 *
 * Note: User callback should set error reason when returning the error. See
 * `llhttp_set_error_reason()` for details.
 */
const char* llhttp_get_error_reason(const llhttp_t* parser);

/* Assign verbal description to the returned error. Must be called in user
 * callbacks right before returning the errno.
 *
 * Note: `HPE_USER` error code might be useful in user callbacks.
 */
void llhttp_set_error_reason(llhttp_t* parser, const char* reason);

/* Returns the pointer to the last parsed byte before the returned error. The
 * pointer is relative to the `data` argument of `llhttp_execute()`.
 *
 * Note: this method might be useful for counting the number of parsed bytes.
 */
const char* llhttp_get_error_pos(const llhttp_t* parser);

/* Returns textual name of error code */
const char* llhttp_errno_name(llhttp_errno_t err);

/* Returns textual name of HTTP method */
const char* llhttp_method_name(llhttp_method_t method);


/* Enables/disables lenient header value parsing (disabled by default).
 *
 * Lenient parsing disables header value token checks, extending llhttp's
 * protocol support to highly non-compliant clients/server. No
 * `HPE_INVALID_HEADER_TOKEN` will be raised for incorrect header values when
 * lenient parsing is "on".
 *
 * **(USE AT YOUR OWN RISK)**
 */
void llhttp_set_lenient(llhttp_t* parser, int enabled);

///////////////////////////////////////////////////////////////////////////////////////////////////
// llhttp.c
#ifdef __SSE4_2__
 #ifdef _MSC_VER
  #include <nmmintrin.h>
 #else  /* !_MSC_VER */
  #include <x86intrin.h>
 #endif  /* _MSC_VER */
#endif  /* __SSE4_2__ */

#ifdef _MSC_VER
 #define ALIGN(n) _declspec(align(n))
#else  /* !_MSC_VER */
 #define ALIGN(n) __attribute__((aligned(n)))
#endif  /* _MSC_VER */

typedef int (*llhttp__internal__span_cb)(
             llhttp__internal_t*, const char*, const char*);

static const unsigned char llparse_blob0[] = {
  'C', 'L'
};
#ifdef __SSE4_2__
static const unsigned char ALIGN(16) llparse_blob1[] = {
  0x9, 0x9, 0xc, 0xc, '!', '"', '$', '>', '@', '~', 0x80,
  0xff, 0x0, 0x0, 0x0, 0x0
};
#endif  /* __SSE4_2__ */
static const unsigned char llparse_blob2[] = {
  'o', 'n'
};
static const unsigned char llparse_blob3[] = {
  'e', 'c', 't', 'i', 'o', 'n'
};
static const unsigned char llparse_blob4[] = {
  'l', 'o', 's', 'e'
};
static const unsigned char llparse_blob5[] = {
  'e', 'e', 'p', '-', 'a', 'l', 'i', 'v', 'e'
};
static const unsigned char llparse_blob6[] = {
  'p', 'g', 'r', 'a', 'd', 'e'
};
#ifdef __SSE4_2__
static const unsigned char ALIGN(16) llparse_blob7[] = {
  0x9, 0x9, ' ', '~', 0x80, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0
};
#endif  /* __SSE4_2__ */
static const unsigned char llparse_blob8[] = {
  'c', 'h', 'u', 'n', 'k', 'e', 'd'
};
#ifdef __SSE4_2__
static const unsigned char ALIGN(16) llparse_blob9[] = {
  ' ', '!', '#', '\'', '*', '+', '-', '.', '0', '9', 'A',
  'Z', '^', 'z', '|', '|'
};
#endif  /* __SSE4_2__ */
#ifdef __SSE4_2__
static const unsigned char ALIGN(16) llparse_blob10[] = {
  '~', '~', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0
};
#endif  /* __SSE4_2__ */
static const unsigned char llparse_blob11[] = {
  'e', 'n', 't', '-', 'l', 'e', 'n', 'g', 't', 'h'
};
static const unsigned char llparse_blob12[] = {
  'r', 'o', 'x', 'y', '-', 'c', 'o', 'n', 'n', 'e', 'c',
  't', 'i', 'o', 'n'
};
static const unsigned char llparse_blob13[] = {
  'r', 'a', 'n', 's', 'f', 'e', 'r', '-', 'e', 'n', 'c',
  'o', 'd', 'i', 'n', 'g'
};
static const unsigned char llparse_blob14[] = {
  'p', 'g', 'r', 'a', 'd', 'e'
};
static const unsigned char llparse_blob15[] = {
  0xd, 0xa
};
static const unsigned char llparse_blob16[] = {
  'T', 'T', 'P', '/'
};
static const unsigned char llparse_blob17[] = {
  'C', 'E', '/'
};
static const unsigned char llparse_blob18[] = {
  'I', 'N', 'D'
};
static const unsigned char llparse_blob19[] = {
  'E', 'C', 'K', 'O', 'U', 'T'
};
static const unsigned char llparse_blob20[] = {
  'N', 'E', 'C', 'T'
};
static const unsigned char llparse_blob21[] = {
  'E', 'L', 'E', 'T', 'E'
};
static const unsigned char llparse_blob22[] = {
  'E', 'T'
};
static const unsigned char llparse_blob23[] = {
  'E', 'A', 'D'
};
static const unsigned char llparse_blob24[] = {
  'N', 'K'
};
static const unsigned char llparse_blob25[] = {
  'C', 'K'
};
static const unsigned char llparse_blob26[] = {
  'S', 'E', 'A', 'R', 'C', 'H'
};
static const unsigned char llparse_blob27[] = {
  'R', 'G', 'E'
};
static const unsigned char llparse_blob28[] = {
  'C', 'T', 'I', 'V', 'I', 'T', 'Y'
};
static const unsigned char llparse_blob29[] = {
  'L', 'E', 'N', 'D', 'A', 'R'
};
static const unsigned char llparse_blob30[] = {
  'V', 'E'
};
static const unsigned char llparse_blob31[] = {
  'O', 'T', 'I', 'F', 'Y'
};
static const unsigned char llparse_blob32[] = {
  'P', 'T', 'I', 'O', 'N', 'S'
};
static const unsigned char llparse_blob33[] = {
  'T', 'C', 'H'
};
static const unsigned char llparse_blob34[] = {
  'S', 'T'
};
static const unsigned char llparse_blob35[] = {
  'I', 'N', 'D'
};
static const unsigned char llparse_blob36[] = {
  'A', 'T', 'C', 'H'
};
static const unsigned char llparse_blob37[] = {
  'G', 'E'
};
static const unsigned char llparse_blob38[] = {
  'I', 'N', 'D'
};
static const unsigned char llparse_blob39[] = {
  'O', 'R', 'T'
};
static const unsigned char llparse_blob40[] = {
  'A', 'R', 'C', 'H'
};
static const unsigned char llparse_blob41[] = {
  'U', 'R', 'C', 'E'
};
static const unsigned char llparse_blob42[] = {
  'B', 'S', 'C', 'R', 'I', 'B', 'E'
};
static const unsigned char llparse_blob43[] = {
  'R', 'A', 'C', 'E'
};
static const unsigned char llparse_blob44[] = {
  'I', 'N', 'D'
};
static const unsigned char llparse_blob45[] = {
  'N', 'K'
};
static const unsigned char llparse_blob46[] = {
  'C', 'K'
};
static const unsigned char llparse_blob47[] = {
  'U', 'B', 'S', 'C', 'R', 'I', 'B', 'E'
};
static const unsigned char llparse_blob48[] = {
  'H', 'T', 'T', 'P', '/'
};
static const unsigned char llparse_blob49[] = {
  'A', 'D'
};
static const unsigned char llparse_blob50[] = {
  'T', 'P', '/'
};

enum llparse_match_status_e {
  kMatchComplete,
  kMatchPause,
  kMatchMismatch
};
typedef enum llparse_match_status_e llparse_match_status_t;

struct llparse_match_s {
  llparse_match_status_t status;
  const unsigned char* current;
};
typedef struct llparse_match_s llparse_match_t;

static llparse_match_t llparse__match_sequence_id(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp,
    const unsigned char* seq, uint32_t seq_len) {
  uint32_t index;
  llparse_match_t res;

  index = s->_index;
  for (; p != endp; p++) {
    unsigned char current;

    current = *p;
    if (current == seq[index]) {
      if (++index == seq_len) {
        res.status = kMatchComplete;
        goto reset;
      }
    } else {
      res.status = kMatchMismatch;
      goto reset;
    }
  }
  s->_index = index;
  res.status = kMatchPause;
  res.current = p;
  return res;
reset:
  s->_index = 0;
  res.current = p;
  return res;
}

static llparse_match_t llparse__match_sequence_to_lower_unsafe(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp,
    const unsigned char* seq, uint32_t seq_len) {
  uint32_t index;
  llparse_match_t res;

  index = s->_index;
  for (; p != endp; p++) {
    unsigned char current;

    current = ((*p) | 0x20);
    if (current == seq[index]) {
      if (++index == seq_len) {
        res.status = kMatchComplete;
        goto reset;
      }
    } else {
      res.status = kMatchMismatch;
      goto reset;
    }
  }
  s->_index = index;
  res.status = kMatchPause;
  res.current = p;
  return res;
reset:
  s->_index = 0;
  res.current = p;
  return res;
}

enum llparse_state_e {
  s_error,
  s_n_llhttp__internal__n_invoke_llhttp__after_message_complete,
  s_n_llhttp__internal__n_pause_1,
  s_n_llhttp__internal__n_invoke_is_equal_upgrade,
  s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2,
  s_n_llhttp__internal__n_chunk_data_almost_done_skip,
  s_n_llhttp__internal__n_chunk_data_almost_done,
  s_n_llhttp__internal__n_consume_content_length,
  s_n_llhttp__internal__n_span_start_llhttp__on_body,
  s_n_llhttp__internal__n_invoke_is_equal_content_length,
  s_n_llhttp__internal__n_chunk_size_almost_done,
  s_n_llhttp__internal__n_chunk_parameters,
  s_n_llhttp__internal__n_chunk_size_otherwise,
  s_n_llhttp__internal__n_chunk_size,
  s_n_llhttp__internal__n_chunk_size_digit,
  s_n_llhttp__internal__n_invoke_update_content_length,
  s_n_llhttp__internal__n_consume_content_length_1,
  s_n_llhttp__internal__n_span_start_llhttp__on_body_1,
  s_n_llhttp__internal__n_eof,
  s_n_llhttp__internal__n_span_start_llhttp__on_body_2,
  s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete,
  s_n_llhttp__internal__n_headers_almost_done,
  s_n_llhttp__internal__n_span_start_llhttp__on_header_value,
  s_n_llhttp__internal__n_header_value_discard_lws,
  s_n_llhttp__internal__n_header_value_discard_ws_almost_done,
  s_n_llhttp__internal__n_header_value_lws,
  s_n_llhttp__internal__n_header_value_almost_done,
  s_n_llhttp__internal__n_header_value_lenient,
  s_n_llhttp__internal__n_header_value_otherwise,
  s_n_llhttp__internal__n_header_value_connection_token,
  s_n_llhttp__internal__n_header_value_connection_ws,
  s_n_llhttp__internal__n_header_value_connection_1,
  s_n_llhttp__internal__n_header_value_connection_2,
  s_n_llhttp__internal__n_header_value_connection_3,
  s_n_llhttp__internal__n_header_value_connection,
  s_n_llhttp__internal__n_error_18,
  s_n_llhttp__internal__n_header_value,
  s_n_llhttp__internal__n_header_value_discard_rws,
  s_n_llhttp__internal__n_error_19,
  s_n_llhttp__internal__n_header_value_content_length_ws,
  s_n_llhttp__internal__n_header_value_content_length,
  s_n_llhttp__internal__n_header_value_te_chunked_last,
  s_n_llhttp__internal__n_header_value_te_token_ows,
  s_n_llhttp__internal__n_header_value_te_token,
  s_n_llhttp__internal__n_header_value_te_chunked,
  s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1,
  s_n_llhttp__internal__n_header_value_discard_ws,
  s_n_llhttp__internal__n_header_field_general_otherwise,
  s_n_llhttp__internal__n_header_field_general,
  s_n_llhttp__internal__n_header_field_colon,
  s_n_llhttp__internal__n_header_field_3,
  s_n_llhttp__internal__n_header_field_4,
  s_n_llhttp__internal__n_header_field_2,
  s_n_llhttp__internal__n_header_field_1,
  s_n_llhttp__internal__n_header_field_5,
  s_n_llhttp__internal__n_header_field_6,
  s_n_llhttp__internal__n_header_field_7,
  s_n_llhttp__internal__n_header_field,
  s_n_llhttp__internal__n_span_start_llhttp__on_header_field,
  s_n_llhttp__internal__n_header_field_start,
  s_n_llhttp__internal__n_url_skip_to_http09,
  s_n_llhttp__internal__n_url_skip_lf_to_http09,
  s_n_llhttp__internal__n_req_http_end_1,
  s_n_llhttp__internal__n_req_http_end,
  s_n_llhttp__internal__n_req_http_minor,
  s_n_llhttp__internal__n_req_http_dot,
  s_n_llhttp__internal__n_req_http_major,
  s_n_llhttp__internal__n_req_http_start_1,
  s_n_llhttp__internal__n_req_http_start_2,
  s_n_llhttp__internal__n_req_http_start,
  s_n_llhttp__internal__n_url_skip_to_http,
  s_n_llhttp__internal__n_url_fragment,
  s_n_llhttp__internal__n_span_end_stub_query_3,
  s_n_llhttp__internal__n_url_query,
  s_n_llhttp__internal__n_url_query_or_fragment,
  s_n_llhttp__internal__n_url_path,
  s_n_llhttp__internal__n_span_start_stub_path_2,
  s_n_llhttp__internal__n_span_start_stub_path,
  s_n_llhttp__internal__n_span_start_stub_path_1,
  s_n_llhttp__internal__n_url_server_with_at,
  s_n_llhttp__internal__n_url_server,
  s_n_llhttp__internal__n_url_schema_delim_1,
  s_n_llhttp__internal__n_url_schema_delim,
  s_n_llhttp__internal__n_span_end_stub_schema,
  s_n_llhttp__internal__n_url_schema,
  s_n_llhttp__internal__n_url_start,
  s_n_llhttp__internal__n_span_start_llhttp__on_url_1,
  s_n_llhttp__internal__n_span_start_llhttp__on_url,
  s_n_llhttp__internal__n_req_spaces_before_url,
  s_n_llhttp__internal__n_req_first_space_before_url,
  s_n_llhttp__internal__n_start_req_1,
  s_n_llhttp__internal__n_start_req_2,
  s_n_llhttp__internal__n_start_req_4,
  s_n_llhttp__internal__n_start_req_6,
  s_n_llhttp__internal__n_start_req_7,
  s_n_llhttp__internal__n_start_req_5,
  s_n_llhttp__internal__n_start_req_3,
  s_n_llhttp__internal__n_start_req_8,
  s_n_llhttp__internal__n_start_req_9,
  s_n_llhttp__internal__n_start_req_10,
  s_n_llhttp__internal__n_start_req_12,
  s_n_llhttp__internal__n_start_req_13,
  s_n_llhttp__internal__n_start_req_11,
  s_n_llhttp__internal__n_start_req_15,
  s_n_llhttp__internal__n_start_req_16,
  s_n_llhttp__internal__n_start_req_18,
  s_n_llhttp__internal__n_start_req_20,
  s_n_llhttp__internal__n_start_req_21,
  s_n_llhttp__internal__n_start_req_19,
  s_n_llhttp__internal__n_start_req_17,
  s_n_llhttp__internal__n_start_req_22,
  s_n_llhttp__internal__n_start_req_14,
  s_n_llhttp__internal__n_start_req_23,
  s_n_llhttp__internal__n_start_req_24,
  s_n_llhttp__internal__n_start_req_26,
  s_n_llhttp__internal__n_start_req_27,
  s_n_llhttp__internal__n_start_req_31,
  s_n_llhttp__internal__n_start_req_32,
  s_n_llhttp__internal__n_start_req_30,
  s_n_llhttp__internal__n_start_req_29,
  s_n_llhttp__internal__n_start_req_28,
  s_n_llhttp__internal__n_start_req_34,
  s_n_llhttp__internal__n_start_req_33,
  s_n_llhttp__internal__n_start_req_25,
  s_n_llhttp__internal__n_start_req_37,
  s_n_llhttp__internal__n_start_req_38,
  s_n_llhttp__internal__n_start_req_36,
  s_n_llhttp__internal__n_start_req_35,
  s_n_llhttp__internal__n_start_req_40,
  s_n_llhttp__internal__n_start_req_41,
  s_n_llhttp__internal__n_start_req_42,
  s_n_llhttp__internal__n_start_req_39,
  s_n_llhttp__internal__n_start_req_43,
  s_n_llhttp__internal__n_start_req_46,
  s_n_llhttp__internal__n_start_req_48,
  s_n_llhttp__internal__n_start_req_49,
  s_n_llhttp__internal__n_start_req_47,
  s_n_llhttp__internal__n_start_req_50,
  s_n_llhttp__internal__n_start_req_45,
  s_n_llhttp__internal__n_start_req_44,
  s_n_llhttp__internal__n_start_req,
  s_n_llhttp__internal__n_res_line_almost_done,
  s_n_llhttp__internal__n_res_status,
  s_n_llhttp__internal__n_span_start_llhttp__on_status,
  s_n_llhttp__internal__n_res_status_start,
  s_n_llhttp__internal__n_res_status_code_otherwise,
  s_n_llhttp__internal__n_res_status_code,
  s_n_llhttp__internal__n_res_http_end,
  s_n_llhttp__internal__n_res_http_minor,
  s_n_llhttp__internal__n_res_http_dot,
  s_n_llhttp__internal__n_res_http_major,
  s_n_llhttp__internal__n_start_res,
  s_n_llhttp__internal__n_req_or_res_method_2,
  s_n_llhttp__internal__n_req_or_res_method_3,
  s_n_llhttp__internal__n_req_or_res_method_1,
  s_n_llhttp__internal__n_req_or_res_method,
  s_n_llhttp__internal__n_start_req_or_res,
  s_n_llhttp__internal__n_invoke_load_type,
  s_n_llhttp__internal__n_start,
};
typedef enum llparse_state_e llparse_state_t;

int llhttp__on_url(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__on_header_field(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__on_header_value(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__on_body(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__on_status(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__internal__c_update_finish(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->finish = 2;
  return 0;
}

int llhttp__on_message_begin(
    llhttp__internal_t* s, const char* p,
    const char* endp);

int llhttp__internal__c_load_type(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return state->type;
}

int llhttp__internal__c_store_method(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  state->method = match;
  return 0;
}

int llhttp__internal__c_is_equal_method(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return state->method == 5;
}

int llhttp__internal__c_update_http_major(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->http_major = 0;
  return 0;
}

int llhttp__internal__c_update_http_minor(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->http_minor = 9;
  return 0;
}

int llhttp__internal__c_test_flags(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return (state->flags & 128) == 128;
}

int llhttp__on_chunk_complete(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__on_message_complete(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__internal__c_is_equal_upgrade(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return state->upgrade == 1;
}

int llhttp__after_message_complete(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__internal__c_update_finish_1(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->finish = 0;
  return 0;
}

int llhttp__internal__c_test_flags_1(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return (state->flags & 544) == 544;
}

int llhttp__internal__c_test_flags_2(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return (state->flags & 256) == 256;
}

int llhttp__internal__c_test_flags_3(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return (state->flags & 40) == 40;
}

int llhttp__before_headers_complete(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__on_headers_complete(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__after_headers_complete(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__internal__c_update_content_length(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->content_length = 0;
  return 0;
}

int llhttp__internal__c_mul_add_content_length(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  /* Multiplication overflow */
  if (state->content_length > 0xffffffffffffffffULL / 16) {
    return 1;
  }

  state->content_length *= 16;

  /* Addition overflow */
  if (match >= 0) {
    if (state->content_length > 0xffffffffffffffffULL - match) {
      return 1;
    }
  } else {
    if (state->content_length < 0ULL - match) {
      return 1;
    }
  }
  state->content_length += match;
  return 0;
}

int llhttp__on_chunk_header(
    llhttp__internal_t* s, const unsigned char* p,
    const unsigned char* endp);

int llhttp__internal__c_is_equal_content_length(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return state->content_length == 0;
}

int llhttp__internal__c_or_flags(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 128;
  return 0;
}

int llhttp__internal__c_update_finish_2(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->finish = 1;
  return 0;
}

int llhttp__internal__c_or_flags_1(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 64;
  return 0;
}

int llhttp__internal__c_update_upgrade(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->upgrade = 1;
  return 0;
}

int llhttp__internal__c_store_header_state(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  state->header_state = match;
  return 0;
}

int llhttp__internal__c_load_header_state(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return state->header_state;
}

int llhttp__internal__c_or_flags_3(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 1;
  return 0;
}

int llhttp__internal__c_update_header_state(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->header_state = 1;
  return 0;
}

int llhttp__internal__c_or_flags_4(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 2;
  return 0;
}

int llhttp__internal__c_or_flags_5(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 4;
  return 0;
}

int llhttp__internal__c_or_flags_6(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 8;
  return 0;
}

int llhttp__internal__c_update_header_state_2(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->header_state = 6;
  return 0;
}

int llhttp__internal__c_update_header_state_4(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->header_state = 0;
  return 0;
}

int llhttp__internal__c_update_header_state_5(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->header_state = 5;
  return 0;
}

int llhttp__internal__c_update_header_state_6(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->header_state = 7;
  return 0;
}

int llhttp__internal__c_test_flags_5(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return (state->flags & 32) == 32;
}

int llhttp__internal__c_mul_add_content_length_1(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  /* Multiplication overflow */
  if (state->content_length > 0xffffffffffffffffULL / 10) {
    return 1;
  }

  state->content_length *= 10;

  /* Addition overflow */
  if (match >= 0) {
    if (state->content_length > 0xffffffffffffffffULL - match) {
      return 1;
    }
  } else {
    if (state->content_length < 0ULL - match) {
      return 1;
    }
  }
  state->content_length += match;
  return 0;
}

int llhttp__internal__c_or_flags_15(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 32;
  return 0;
}

int llhttp__internal__c_or_flags_16(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 512;
  return 0;
}

int llhttp__internal__c_update_header_state_8(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->header_state = 8;
  return 0;
}

int llhttp__internal__c_or_flags_17(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->flags |= 16;
  return 0;
}

int llhttp__internal__c_store_http_major(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  state->http_major = match;
  return 0;
}

int llhttp__internal__c_store_http_minor(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  state->http_minor = match;
  return 0;
}

int llhttp__internal__c_is_equal_method_1(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  return state->method == 33;
}

int llhttp__internal__c_update_status_code(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->status_code = 0;
  return 0;
}

int llhttp__internal__c_mul_add_status_code(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  /* Multiplication overflow */
  if (state->status_code > 0xffff / 10) {
    return 1;
  }

  state->status_code *= 10;

  /* Addition overflow */
  if (match >= 0) {
    if (state->status_code > 0xffff - match) {
      return 1;
    }
  } else {
    if (state->status_code < 0 - match) {
      return 1;
    }
  }
  state->status_code += match;

  /* Enforce maximum */
  if (state->status_code > 999) {
    return 1;
  }
  return 0;
}

int llhttp__internal__c_update_type(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->type = 1;
  return 0;
}

int llhttp__internal__c_update_type_1(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  state->type = 2;
  return 0;
}

int llhttp__internal_init(llhttp__internal_t* state) {
  memset(state, 0, sizeof(*state));
  state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_start;
  return 0;
}

static llparse_state_t llhttp__internal__run(
    llhttp__internal_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  int match;
  switch ((llparse_state_t) (intptr_t) state->_current) {
    case s_n_llhttp__internal__n_invoke_llhttp__after_message_complete:
    s_n_llhttp__internal__n_invoke_llhttp__after_message_complete: {
      switch (llhttp__after_message_complete(state, p, endp)) {
        default:
          goto s_n_llhttp__internal__n_invoke_update_finish_1;
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_pause_1:
    s_n_llhttp__internal__n_pause_1: {
      state->error = 0x16;
      state->reason = "Pause on CONNECT/Upgrade";
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_llhttp__after_message_complete;
      return s_error;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_invoke_is_equal_upgrade:
    s_n_llhttp__internal__n_invoke_is_equal_upgrade: {
      switch (llhttp__internal__c_is_equal_upgrade(state, p, endp)) {
        case 0:
          goto s_n_llhttp__internal__n_invoke_llhttp__after_message_complete;
        default:
          goto s_n_llhttp__internal__n_pause_1;
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2:
    s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2: {
      switch (llhttp__on_message_complete(state, p, endp)) {
        case 0:
          goto s_n_llhttp__internal__n_invoke_is_equal_upgrade;
        case 21:
          goto s_n_llhttp__internal__n_pause_5;
        default:
          goto s_n_llhttp__internal__n_error_9;
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_chunk_data_almost_done_skip:
    s_n_llhttp__internal__n_chunk_data_almost_done_skip: {
      if (p == endp) {
        return s_n_llhttp__internal__n_chunk_data_almost_done_skip;
      }
      p++;
      goto s_n_llhttp__internal__n_invoke_llhttp__on_chunk_complete;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_chunk_data_almost_done:
    s_n_llhttp__internal__n_chunk_data_almost_done: {
      if (p == endp) {
        return s_n_llhttp__internal__n_chunk_data_almost_done;
      }
      p++;
      goto s_n_llhttp__internal__n_chunk_data_almost_done_skip;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_consume_content_length:
    s_n_llhttp__internal__n_consume_content_length: {
      size_t avail;
      size_t need;

      avail = endp - p;
      need = state->content_length;
      if (avail >= need) {
        p += need;
        state->content_length = 0;
        goto s_n_llhttp__internal__n_span_end_llhttp__on_body;
      }

      state->content_length -= avail;
      return s_n_llhttp__internal__n_consume_content_length;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_body:
    s_n_llhttp__internal__n_span_start_llhttp__on_body: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_body;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_body;
      goto s_n_llhttp__internal__n_consume_content_length;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_invoke_is_equal_content_length:
    s_n_llhttp__internal__n_invoke_is_equal_content_length: {
      switch (llhttp__internal__c_is_equal_content_length(state, p, endp)) {
        case 0:
          goto s_n_llhttp__internal__n_span_start_llhttp__on_body;
        default:
          goto s_n_llhttp__internal__n_invoke_or_flags;
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_chunk_size_almost_done:
    s_n_llhttp__internal__n_chunk_size_almost_done: {
      if (p == endp) {
        return s_n_llhttp__internal__n_chunk_size_almost_done;
      }
      p++;
      goto s_n_llhttp__internal__n_invoke_llhttp__on_chunk_header;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_chunk_parameters:
    s_n_llhttp__internal__n_chunk_parameters: {
      if (p == endp) {
        return s_n_llhttp__internal__n_chunk_parameters;
      }
      switch (*p) {
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_chunk_size_almost_done;
        }
        default: {
          p++;
          goto s_n_llhttp__internal__n_chunk_parameters;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_chunk_size_otherwise:
    s_n_llhttp__internal__n_chunk_size_otherwise: {
      if (p == endp) {
        return s_n_llhttp__internal__n_chunk_size_otherwise;
      }
      switch (*p) {
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_chunk_size_almost_done;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_chunk_parameters;
        }
        case ';': {
          p++;
          goto s_n_llhttp__internal__n_chunk_parameters;
        }
        default: {
          goto s_n_llhttp__internal__n_error_6;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_chunk_size:
    s_n_llhttp__internal__n_chunk_size: {
      if (p == endp) {
        return s_n_llhttp__internal__n_chunk_size;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'A': {
          p++;
          match = 10;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'B': {
          p++;
          match = 11;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'C': {
          p++;
          match = 12;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'D': {
          p++;
          match = 13;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'E': {
          p++;
          match = 14;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'F': {
          p++;
          match = 15;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'a': {
          p++;
          match = 10;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'b': {
          p++;
          match = 11;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'c': {
          p++;
          match = 12;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'd': {
          p++;
          match = 13;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'e': {
          p++;
          match = 14;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'f': {
          p++;
          match = 15;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        default: {
          goto s_n_llhttp__internal__n_chunk_size_otherwise;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_chunk_size_digit:
    s_n_llhttp__internal__n_chunk_size_digit: {
      if (p == endp) {
        return s_n_llhttp__internal__n_chunk_size_digit;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'A': {
          p++;
          match = 10;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'B': {
          p++;
          match = 11;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'C': {
          p++;
          match = 12;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'D': {
          p++;
          match = 13;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'E': {
          p++;
          match = 14;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'F': {
          p++;
          match = 15;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'a': {
          p++;
          match = 10;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'b': {
          p++;
          match = 11;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'c': {
          p++;
          match = 12;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'd': {
          p++;
          match = 13;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'e': {
          p++;
          match = 14;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        case 'f': {
          p++;
          match = 15;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length;
        }
        default: {
          goto s_n_llhttp__internal__n_error_8;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_invoke_update_content_length:
    s_n_llhttp__internal__n_invoke_update_content_length: {
      switch (llhttp__internal__c_update_content_length(state, p, endp)) {
        default:
          goto s_n_llhttp__internal__n_chunk_size_digit;
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_consume_content_length_1:
    s_n_llhttp__internal__n_consume_content_length_1: {
      size_t avail;
      size_t need;

      avail = endp - p;
      need = state->content_length;
      if (avail >= need) {
        p += need;
        state->content_length = 0;
        goto s_n_llhttp__internal__n_span_end_llhttp__on_body_1;
      }

      state->content_length -= avail;
      return s_n_llhttp__internal__n_consume_content_length_1;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_body_1:
    s_n_llhttp__internal__n_span_start_llhttp__on_body_1: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_body_1;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_body;
      goto s_n_llhttp__internal__n_consume_content_length_1;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_eof:
    s_n_llhttp__internal__n_eof: {
      if (p == endp) {
        return s_n_llhttp__internal__n_eof;
      }
      p++;
      goto s_n_llhttp__internal__n_eof;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_body_2:
    s_n_llhttp__internal__n_span_start_llhttp__on_body_2: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_body_2;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_body;
      goto s_n_llhttp__internal__n_eof;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete:
    s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete: {
      switch (llhttp__after_headers_complete(state, p, endp)) {
        case 1:
          goto s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_1;
        case 2:
          goto s_n_llhttp__internal__n_invoke_update_content_length;
        case 3:
          goto s_n_llhttp__internal__n_span_start_llhttp__on_body_1;
        case 4:
          goto s_n_llhttp__internal__n_invoke_update_finish_2;
        case 5:
          goto s_n_llhttp__internal__n_error_10;
        default:
          goto s_n_llhttp__internal__n_invoke_llhttp__on_message_complete;
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_headers_almost_done:
    s_n_llhttp__internal__n_headers_almost_done: {
      if (p == endp) {
        return s_n_llhttp__internal__n_headers_almost_done;
      }
      p++;
      goto s_n_llhttp__internal__n_invoke_test_flags;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_header_value:
    s_n_llhttp__internal__n_span_start_llhttp__on_header_value: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_header_value;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_header_value;
      goto s_n_llhttp__internal__n_span_end_llhttp__on_header_value;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_discard_lws:
    s_n_llhttp__internal__n_header_value_discard_lws: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_discard_lws;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_llhttp__internal__n_header_value_discard_ws;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_discard_ws;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_load_header_state;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_discard_ws_almost_done:
    s_n_llhttp__internal__n_header_value_discard_ws_almost_done: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_discard_ws_almost_done;
      }
      p++;
      goto s_n_llhttp__internal__n_header_value_discard_lws;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_lws:
    s_n_llhttp__internal__n_header_value_lws: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_lws;
      }
      switch (*p) {
        case 9: {
          goto s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1;
        }
        case ' ': {
          goto s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_load_header_state_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_almost_done:
    s_n_llhttp__internal__n_header_value_almost_done: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_almost_done;
      }
      switch (*p) {
        case 10: {
          p++;
          goto s_n_llhttp__internal__n_header_value_lws;
        }
        default: {
          goto s_n_llhttp__internal__n_error_15;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_lenient:
    s_n_llhttp__internal__n_header_value_lenient: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_lenient;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_header_value_1;
        }
        case 13: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_header_value_3;
        }
        default: {
          p++;
          goto s_n_llhttp__internal__n_header_value_lenient;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_otherwise:
    s_n_llhttp__internal__n_header_value_otherwise: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_otherwise;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_header_value_1;
        }
        case 13: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_header_value_2;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_test_flags_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_connection_token:
    s_n_llhttp__internal__n_header_value_connection_token: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_connection_token;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection_token;
        }
        case 2: {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection;
        }
        default: {
          goto s_n_llhttp__internal__n_header_value_otherwise;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_connection_ws:
    s_n_llhttp__internal__n_header_value_connection_ws: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_connection_ws;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_header_value_otherwise;
        }
        case 13: {
          goto s_n_llhttp__internal__n_header_value_otherwise;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection_ws;
        }
        case ',': {
          p++;
          goto s_n_llhttp__internal__n_invoke_load_header_state_4;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_connection_1:
    s_n_llhttp__internal__n_header_value_connection_1: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_connection_1;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob4, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_invoke_update_header_state_2;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_value_connection_1;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_header_value_connection_token;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_connection_2:
    s_n_llhttp__internal__n_header_value_connection_2: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_connection_2;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob5, 9);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_invoke_update_header_state_5;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_value_connection_2;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_header_value_connection_token;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_connection_3:
    s_n_llhttp__internal__n_header_value_connection_3: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_connection_3;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob6, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_invoke_update_header_state_6;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_value_connection_3;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_header_value_connection_token;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_connection:
    s_n_llhttp__internal__n_header_value_connection: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_connection;
      }
      switch (((*p) | 0x20)) {
        case 9: {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection;
        }
        case 'c': {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection_1;
        }
        case 'k': {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection_2;
        }
        case 'u': {
          p++;
          goto s_n_llhttp__internal__n_header_value_connection_3;
        }
        default: {
          goto s_n_llhttp__internal__n_header_value_connection_token;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_error_18:
    s_n_llhttp__internal__n_error_18: {
      state->error = 0xb;
      state->reason = "Content-Length overflow";
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_error;
      return s_error;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value:
    s_n_llhttp__internal__n_header_value: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value;
      }
      #ifdef __SSE4_2__
      if (endp - p >= 16) {
        __m128i ranges;
        __m128i input;
        int avail;
        int match_len;

        /* Load input */
        input = _mm_loadu_si128((__m128i const*) p);
        ranges = _mm_loadu_si128((__m128i const*) llparse_blob7);

        /* Find first character that does not match `ranges` */
        match_len = _mm_cmpestri(ranges, 6,
            input, 16,
            _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES |
              _SIDD_NEGATIVE_POLARITY);

        if (match_len != 0) {
          p += match_len;
          goto s_n_llhttp__internal__n_header_value;
        }
        goto s_n_llhttp__internal__n_header_value_otherwise;
      }
      #endif  /* __SSE4_2__ */
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_header_value;
        }
        default: {
          goto s_n_llhttp__internal__n_header_value_otherwise;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_discard_rws:
    s_n_llhttp__internal__n_header_value_discard_rws: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_discard_rws;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_header_value_otherwise;
        }
        case 13: {
          goto s_n_llhttp__internal__n_header_value_otherwise;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_discard_rws;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_7;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_error_19:
    s_n_llhttp__internal__n_error_19: {
      state->error = 0xb;
      state->reason = "Invalid character in Content-Length";
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_error;
      return s_error;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_content_length_ws:
    s_n_llhttp__internal__n_header_value_content_length_ws: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_content_length_ws;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_invoke_or_flags_15;
        }
        case 13: {
          goto s_n_llhttp__internal__n_invoke_or_flags_15;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_content_length_ws;
        }
        default: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_header_value_5;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_content_length:
    s_n_llhttp__internal__n_header_value_content_length: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_content_length;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_mul_add_content_length_1;
        }
        default: {
          goto s_n_llhttp__internal__n_header_value_content_length_ws;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_te_chunked_last:
    s_n_llhttp__internal__n_header_value_te_chunked_last: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_te_chunked_last;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_8;
        }
        case 13: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_8;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_te_chunked_last;
        }
        default: {
          goto s_n_llhttp__internal__n_header_value_te_chunked;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_te_token_ows:
    s_n_llhttp__internal__n_header_value_te_token_ows: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_te_token_ows;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_llhttp__internal__n_header_value_te_token_ows;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_te_token_ows;
        }
        default: {
          goto s_n_llhttp__internal__n_header_value_te_chunked;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_te_token:
    s_n_llhttp__internal__n_header_value_te_token: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_te_token;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_header_value_te_token;
        }
        case 2: {
          p++;
          goto s_n_llhttp__internal__n_header_value_te_token_ows;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_7;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_te_chunked:
    s_n_llhttp__internal__n_header_value_te_chunked: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_te_chunked;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob8, 7);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_header_value_te_chunked_last;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_value_te_chunked;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_header_value_te_token;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1:
    s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_header_value;
      goto s_n_llhttp__internal__n_invoke_load_header_state_2;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_value_discard_ws:
    s_n_llhttp__internal__n_header_value_discard_ws: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_value_discard_ws;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_llhttp__internal__n_header_value_discard_ws;
        }
        case 10: {
          p++;
          goto s_n_llhttp__internal__n_header_value_discard_lws;
        }
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_header_value_discard_ws_almost_done;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_value_discard_ws;
        }
        default: {
          goto s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_general_otherwise:
    s_n_llhttp__internal__n_header_field_general_otherwise: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_general_otherwise;
      }
      switch (*p) {
        case ':': {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_header_field_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_20;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_general:
    s_n_llhttp__internal__n_header_field_general: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_general;
      }
      #ifdef __SSE4_2__
      if (endp - p >= 16) {
        __m128i ranges;
        __m128i input;
        int avail;
        int match_len;

        /* Load input */
        input = _mm_loadu_si128((__m128i const*) p);
        ranges = _mm_loadu_si128((__m128i const*) llparse_blob9);

        /* Find first character that does not match `ranges` */
        match_len = _mm_cmpestri(ranges, 16,
            input, 16,
            _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES |
              _SIDD_NEGATIVE_POLARITY);

        if (match_len != 0) {
          p += match_len;
          goto s_n_llhttp__internal__n_header_field_general;
        }
        ranges = _mm_loadu_si128((__m128i const*) llparse_blob10);

        /* Find first character that does not match `ranges` */
        match_len = _mm_cmpestri(ranges, 2,
            input, 16,
            _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES |
              _SIDD_NEGATIVE_POLARITY);

        if (match_len != 0) {
          p += match_len;
          goto s_n_llhttp__internal__n_header_field_general;
        }
        goto s_n_llhttp__internal__n_header_field_general_otherwise;
      }
      #endif  /* __SSE4_2__ */
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_header_field_general;
        }
        default: {
          goto s_n_llhttp__internal__n_header_field_general_otherwise;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_colon:
    s_n_llhttp__internal__n_header_field_colon: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_colon;
      }
      switch (*p) {
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_header_field_colon;
        }
        case ':': {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_header_field;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_9;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_3:
    s_n_llhttp__internal__n_header_field_3: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_3;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob3, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_store_header_state;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_field_3;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_4:
    s_n_llhttp__internal__n_header_field_4: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_4;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob11, 10);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_store_header_state;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_field_4;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_2:
    s_n_llhttp__internal__n_header_field_2: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_2;
      }
      switch (((*p) | 0x20)) {
        case 'n': {
          p++;
          goto s_n_llhttp__internal__n_header_field_3;
        }
        case 't': {
          p++;
          goto s_n_llhttp__internal__n_header_field_4;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_1:
    s_n_llhttp__internal__n_header_field_1: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_1;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob2, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_header_field_2;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_field_1;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_5:
    s_n_llhttp__internal__n_header_field_5: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_5;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob12, 15);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_store_header_state;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_field_5;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_6:
    s_n_llhttp__internal__n_header_field_6: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_6;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob13, 16);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_store_header_state;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_field_6;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_7:
    s_n_llhttp__internal__n_header_field_7: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_7;
      }
      match_seq = llparse__match_sequence_to_lower_unsafe(state, p, endp, llparse_blob14, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_store_header_state;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_header_field_7;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field:
    s_n_llhttp__internal__n_header_field: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_field;
      }
      switch (((*p) | 0x20)) {
        case 'c': {
          p++;
          goto s_n_llhttp__internal__n_header_field_1;
        }
        case 'p': {
          p++;
          goto s_n_llhttp__internal__n_header_field_5;
        }
        case 't': {
          p++;
          goto s_n_llhttp__internal__n_header_field_6;
        }
        case 'u': {
          p++;
          goto s_n_llhttp__internal__n_header_field_7;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_header_state_10;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_header_field:
    s_n_llhttp__internal__n_span_start_llhttp__on_header_field: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_header_field;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_header_field;
      goto s_n_llhttp__internal__n_header_field;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_header_field_start:
    s_n_llhttp__internal__n_header_field_start: {
      if (p == endp) {
        return s_n_llhttp__internal__n_header_field_start;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_headers_almost_done;
        }
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_headers_almost_done;
        }
        default: {
          goto s_n_llhttp__internal__n_span_start_llhttp__on_header_field;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_skip_to_http09:
    s_n_llhttp__internal__n_url_skip_to_http09: {
      if (p == endp) {
        return s_n_llhttp__internal__n_url_skip_to_http09;
      }
      p++;
      goto s_n_llhttp__internal__n_invoke_update_http_major;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_skip_lf_to_http09:
    s_n_llhttp__internal__n_url_skip_lf_to_http09: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_url_skip_lf_to_http09;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob15, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_invoke_update_http_major;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_url_skip_lf_to_http09;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_21;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_end_1:
    s_n_llhttp__internal__n_req_http_end_1: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_end_1;
      }
      switch (*p) {
        case 10: {
          p++;
          goto s_n_llhttp__internal__n_header_field_start;
        }
        default: {
          goto s_n_llhttp__internal__n_error_22;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_end:
    s_n_llhttp__internal__n_req_http_end: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_end;
      }
      switch (*p) {
        case 10: {
          p++;
          goto s_n_llhttp__internal__n_header_field_start;
        }
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_req_http_end_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_22;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_minor:
    s_n_llhttp__internal__n_req_http_minor: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_minor;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_store_http_minor;
        }
        default: {
          goto s_n_llhttp__internal__n_error_23;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_dot:
    s_n_llhttp__internal__n_req_http_dot: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_dot;
      }
      switch (*p) {
        case '.': {
          p++;
          goto s_n_llhttp__internal__n_req_http_minor;
        }
        default: {
          goto s_n_llhttp__internal__n_error_24;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_major:
    s_n_llhttp__internal__n_req_http_major: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_major;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_store_http_major;
        }
        default: {
          goto s_n_llhttp__internal__n_error_25;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_start_1:
    s_n_llhttp__internal__n_req_http_start_1: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_start_1;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob16, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_req_http_major;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_req_http_start_1;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_27;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_start_2:
    s_n_llhttp__internal__n_req_http_start_2: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_start_2;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob17, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_invoke_is_equal_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_req_http_start_2;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_27;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_http_start:
    s_n_llhttp__internal__n_req_http_start: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_http_start;
      }
      switch (*p) {
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_req_http_start;
        }
        case 'H': {
          p++;
          goto s_n_llhttp__internal__n_req_http_start_1;
        }
        case 'I': {
          p++;
          goto s_n_llhttp__internal__n_req_http_start_2;
        }
        default: {
          goto s_n_llhttp__internal__n_error_27;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_skip_to_http:
    s_n_llhttp__internal__n_url_skip_to_http: {
      if (p == endp) {
        return s_n_llhttp__internal__n_url_skip_to_http;
      }
      p++;
      goto s_n_llhttp__internal__n_req_http_start;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_fragment:
    s_n_llhttp__internal__n_url_fragment: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 1, 3, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_url_fragment;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_url_fragment;
        }
        case 2: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_6;
        }
        case 3: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_7;
        }
        case 4: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_8;
        }
        default: {
          goto s_n_llhttp__internal__n_error_28;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_end_stub_query_3:
    s_n_llhttp__internal__n_span_end_stub_query_3: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_end_stub_query_3;
      }
      p++;
      goto s_n_llhttp__internal__n_url_fragment;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_query:
    s_n_llhttp__internal__n_url_query: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 1, 3, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        4, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_url_query;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_url_query;
        }
        case 2: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_9;
        }
        case 3: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_10;
        }
        case 4: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_11;
        }
        case 5: {
          goto s_n_llhttp__internal__n_span_end_stub_query_3;
        }
        default: {
          goto s_n_llhttp__internal__n_error_29;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_query_or_fragment:
    s_n_llhttp__internal__n_url_query_or_fragment: {
      if (p == endp) {
        return s_n_llhttp__internal__n_url_query_or_fragment;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_3;
        }
        case 13: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_4;
        }
        case ' ': {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_5;
        }
        case '#': {
          p++;
          goto s_n_llhttp__internal__n_url_fragment;
        }
        case '?': {
          p++;
          goto s_n_llhttp__internal__n_url_query;
        }
        default: {
          goto s_n_llhttp__internal__n_error_30;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_path:
    s_n_llhttp__internal__n_url_path: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_url_path;
      }
      #ifdef __SSE4_2__
      if (endp - p >= 16) {
        __m128i ranges;
        __m128i input;
        int avail;
        int match_len;

        /* Load input */
        input = _mm_loadu_si128((__m128i const*) p);
        ranges = _mm_loadu_si128((__m128i const*) llparse_blob1);

        /* Find first character that does not match `ranges` */
        match_len = _mm_cmpestri(ranges, 12,
            input, 16,
            _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES |
              _SIDD_NEGATIVE_POLARITY);

        if (match_len != 0) {
          p += match_len;
          goto s_n_llhttp__internal__n_url_path;
        }
        goto s_n_llhttp__internal__n_url_query_or_fragment;
      }
      #endif  /* __SSE4_2__ */
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_url_path;
        }
        default: {
          goto s_n_llhttp__internal__n_url_query_or_fragment;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_stub_path_2:
    s_n_llhttp__internal__n_span_start_stub_path_2: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_stub_path_2;
      }
      p++;
      goto s_n_llhttp__internal__n_url_path;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_stub_path:
    s_n_llhttp__internal__n_span_start_stub_path: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_stub_path;
      }
      p++;
      goto s_n_llhttp__internal__n_url_path;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_stub_path_1:
    s_n_llhttp__internal__n_span_start_stub_path_1: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_stub_path_1;
      }
      p++;
      goto s_n_llhttp__internal__n_url_path;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_server_with_at:
    s_n_llhttp__internal__n_url_server_with_at: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 6,
        7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4,
        0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 4, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_url_server_with_at;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_12;
        }
        case 2: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_13;
        }
        case 3: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_14;
        }
        case 4: {
          p++;
          goto s_n_llhttp__internal__n_url_server;
        }
        case 5: {
          goto s_n_llhttp__internal__n_span_start_stub_path_1;
        }
        case 6: {
          p++;
          goto s_n_llhttp__internal__n_url_query;
        }
        case 7: {
          p++;
          goto s_n_llhttp__internal__n_error_31;
        }
        default: {
          goto s_n_llhttp__internal__n_error_32;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_server:
    s_n_llhttp__internal__n_url_server: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        3, 4, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 6,
        7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 4, 0, 4,
        0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 4, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_url_server;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url;
        }
        case 2: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_1;
        }
        case 3: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_url_2;
        }
        case 4: {
          p++;
          goto s_n_llhttp__internal__n_url_server;
        }
        case 5: {
          goto s_n_llhttp__internal__n_span_start_stub_path;
        }
        case 6: {
          p++;
          goto s_n_llhttp__internal__n_url_query;
        }
        case 7: {
          p++;
          goto s_n_llhttp__internal__n_url_server_with_at;
        }
        default: {
          goto s_n_llhttp__internal__n_error_33;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_schema_delim_1:
    s_n_llhttp__internal__n_url_schema_delim_1: {
      if (p == endp) {
        return s_n_llhttp__internal__n_url_schema_delim_1;
      }
      switch (*p) {
        case '/': {
          p++;
          goto s_n_llhttp__internal__n_url_server;
        }
        default: {
          goto s_n_llhttp__internal__n_error_35;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_schema_delim:
    s_n_llhttp__internal__n_url_schema_delim: {
      if (p == endp) {
        return s_n_llhttp__internal__n_url_schema_delim;
      }
      switch (*p) {
        case 10: {
          p++;
          goto s_n_llhttp__internal__n_error_34;
        }
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_error_34;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_error_34;
        }
        case '/': {
          p++;
          goto s_n_llhttp__internal__n_url_schema_delim_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_35;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_end_stub_schema:
    s_n_llhttp__internal__n_span_end_stub_schema: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_end_stub_schema;
      }
      p++;
      goto s_n_llhttp__internal__n_url_schema_delim;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_schema:
    s_n_llhttp__internal__n_url_schema: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
        0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0,
        0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_url_schema;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_error_34;
        }
        case 2: {
          goto s_n_llhttp__internal__n_span_end_stub_schema;
        }
        case 3: {
          p++;
          goto s_n_llhttp__internal__n_url_schema;
        }
        default: {
          goto s_n_llhttp__internal__n_error_36;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_url_start:
    s_n_llhttp__internal__n_url_start: {
      static uint8_t lookup_table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0,
        0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      };
      if (p == endp) {
        return s_n_llhttp__internal__n_url_start;
      }
      switch (lookup_table[(uint8_t) *p]) {
        case 1: {
          p++;
          goto s_n_llhttp__internal__n_error_34;
        }
        case 2: {
          goto s_n_llhttp__internal__n_span_start_stub_path_2;
        }
        case 3: {
          goto s_n_llhttp__internal__n_url_schema;
        }
        default: {
          goto s_n_llhttp__internal__n_error_37;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_url_1:
    s_n_llhttp__internal__n_span_start_llhttp__on_url_1: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_url_1;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_url;
      goto s_n_llhttp__internal__n_url_start;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_url:
    s_n_llhttp__internal__n_span_start_llhttp__on_url: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_url;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_url;
      goto s_n_llhttp__internal__n_url_server;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_spaces_before_url:
    s_n_llhttp__internal__n_req_spaces_before_url: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_spaces_before_url;
      }
      switch (*p) {
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_req_spaces_before_url;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_is_equal_method;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_first_space_before_url:
    s_n_llhttp__internal__n_req_first_space_before_url: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_first_space_before_url;
      }
      switch (*p) {
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_req_spaces_before_url;
        }
        default: {
          goto s_n_llhttp__internal__n_error_38;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_1:
    s_n_llhttp__internal__n_start_req_1: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_1;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob0, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 19;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_1;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_2:
    s_n_llhttp__internal__n_start_req_2: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_2;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob18, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 16;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_2;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_4:
    s_n_llhttp__internal__n_start_req_4: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_4;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob19, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 22;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_4;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_6:
    s_n_llhttp__internal__n_start_req_6: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_6;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob20, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_6;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_7:
    s_n_llhttp__internal__n_start_req_7: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_7;
      }
      switch (*p) {
        case 'Y': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_5:
    s_n_llhttp__internal__n_start_req_5: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_5;
      }
      switch (*p) {
        case 'N': {
          p++;
          goto s_n_llhttp__internal__n_start_req_6;
        }
        case 'P': {
          p++;
          goto s_n_llhttp__internal__n_start_req_7;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_3:
    s_n_llhttp__internal__n_start_req_3: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_3;
      }
      switch (*p) {
        case 'H': {
          p++;
          goto s_n_llhttp__internal__n_start_req_4;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_5;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_8:
    s_n_llhttp__internal__n_start_req_8: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_8;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob21, 5);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_8;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_9:
    s_n_llhttp__internal__n_start_req_9: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_9;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob22, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_9;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_10:
    s_n_llhttp__internal__n_start_req_10: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_10;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob23, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_10;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_12:
    s_n_llhttp__internal__n_start_req_12: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_12;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob24, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 31;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_12;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_13:
    s_n_llhttp__internal__n_start_req_13: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_13;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob25, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_13;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_11:
    s_n_llhttp__internal__n_start_req_11: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_11;
      }
      switch (*p) {
        case 'I': {
          p++;
          goto s_n_llhttp__internal__n_start_req_12;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_13;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_15:
    s_n_llhttp__internal__n_start_req_15: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_15;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob26, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 24;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_15;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_16:
    s_n_llhttp__internal__n_start_req_16: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_16;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob27, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 23;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_16;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_18:
    s_n_llhttp__internal__n_start_req_18: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_18;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob28, 7);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 21;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_18;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_20:
    s_n_llhttp__internal__n_start_req_20: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_20;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob29, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 30;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_20;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_21:
    s_n_llhttp__internal__n_start_req_21: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_21;
      }
      switch (*p) {
        case 'L': {
          p++;
          match = 10;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_19:
    s_n_llhttp__internal__n_start_req_19: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_19;
      }
      switch (*p) {
        case 'A': {
          p++;
          goto s_n_llhttp__internal__n_start_req_20;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_21;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_17:
    s_n_llhttp__internal__n_start_req_17: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_17;
      }
      switch (*p) {
        case 'A': {
          p++;
          goto s_n_llhttp__internal__n_start_req_18;
        }
        case 'C': {
          p++;
          goto s_n_llhttp__internal__n_start_req_19;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_22:
    s_n_llhttp__internal__n_start_req_22: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_22;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob30, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 11;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_22;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_14:
    s_n_llhttp__internal__n_start_req_14: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_14;
      }
      switch (*p) {
        case '-': {
          p++;
          goto s_n_llhttp__internal__n_start_req_15;
        }
        case 'E': {
          p++;
          goto s_n_llhttp__internal__n_start_req_16;
        }
        case 'K': {
          p++;
          goto s_n_llhttp__internal__n_start_req_17;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_22;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_23:
    s_n_llhttp__internal__n_start_req_23: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_23;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob31, 5);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 25;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_23;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_24:
    s_n_llhttp__internal__n_start_req_24: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_24;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob32, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_24;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_26:
    s_n_llhttp__internal__n_start_req_26: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_26;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob33, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 28;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_26;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_27:
    s_n_llhttp__internal__n_start_req_27: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_27;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob34, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_27;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_31:
    s_n_llhttp__internal__n_start_req_31: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_31;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob35, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 12;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_31;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_32:
    s_n_llhttp__internal__n_start_req_32: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_32;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob36, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 13;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_32;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_30:
    s_n_llhttp__internal__n_start_req_30: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_30;
      }
      switch (*p) {
        case 'F': {
          p++;
          goto s_n_llhttp__internal__n_start_req_31;
        }
        case 'P': {
          p++;
          goto s_n_llhttp__internal__n_start_req_32;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_29:
    s_n_llhttp__internal__n_start_req_29: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_29;
      }
      switch (*p) {
        case 'P': {
          p++;
          goto s_n_llhttp__internal__n_start_req_30;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_28:
    s_n_llhttp__internal__n_start_req_28: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_28;
      }
      switch (*p) {
        case 'I': {
          p++;
          match = 34;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_29;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_34:
    s_n_llhttp__internal__n_start_req_34: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_34;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob37, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 29;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_34;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_33:
    s_n_llhttp__internal__n_start_req_33: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_33;
      }
      switch (*p) {
        case 'R': {
          p++;
          goto s_n_llhttp__internal__n_start_req_34;
        }
        case 'T': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_25:
    s_n_llhttp__internal__n_start_req_25: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_25;
      }
      switch (*p) {
        case 'A': {
          p++;
          goto s_n_llhttp__internal__n_start_req_26;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_27;
        }
        case 'R': {
          p++;
          goto s_n_llhttp__internal__n_start_req_28;
        }
        case 'U': {
          p++;
          goto s_n_llhttp__internal__n_start_req_33;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_37:
    s_n_llhttp__internal__n_start_req_37: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_37;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob38, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 17;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_37;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_38:
    s_n_llhttp__internal__n_start_req_38: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_38;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob39, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 20;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_38;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_36:
    s_n_llhttp__internal__n_start_req_36: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_36;
      }
      switch (*p) {
        case 'B': {
          p++;
          goto s_n_llhttp__internal__n_start_req_37;
        }
        case 'P': {
          p++;
          goto s_n_llhttp__internal__n_start_req_38;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_35:
    s_n_llhttp__internal__n_start_req_35: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_35;
      }
      switch (*p) {
        case 'E': {
          p++;
          goto s_n_llhttp__internal__n_start_req_36;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_40:
    s_n_llhttp__internal__n_start_req_40: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_40;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob40, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 14;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_40;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_41:
    s_n_llhttp__internal__n_start_req_41: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_41;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob41, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 33;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_41;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_42:
    s_n_llhttp__internal__n_start_req_42: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_42;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob42, 7);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 26;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_42;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_39:
    s_n_llhttp__internal__n_start_req_39: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_39;
      }
      switch (*p) {
        case 'E': {
          p++;
          goto s_n_llhttp__internal__n_start_req_40;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_41;
        }
        case 'U': {
          p++;
          goto s_n_llhttp__internal__n_start_req_42;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_43:
    s_n_llhttp__internal__n_start_req_43: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_43;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob43, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_43;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_46:
    s_n_llhttp__internal__n_start_req_46: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_46;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob44, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 18;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_46;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_48:
    s_n_llhttp__internal__n_start_req_48: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_48;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob45, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 32;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_48;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_49:
    s_n_llhttp__internal__n_start_req_49: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_49;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob46, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 15;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_49;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_47:
    s_n_llhttp__internal__n_start_req_47: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_47;
      }
      switch (*p) {
        case 'I': {
          p++;
          goto s_n_llhttp__internal__n_start_req_48;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_49;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_50:
    s_n_llhttp__internal__n_start_req_50: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_50;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob47, 8);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 27;
          goto s_n_llhttp__internal__n_invoke_store_method_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_req_50;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_45:
    s_n_llhttp__internal__n_start_req_45: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_45;
      }
      switch (*p) {
        case 'B': {
          p++;
          goto s_n_llhttp__internal__n_start_req_46;
        }
        case 'L': {
          p++;
          goto s_n_llhttp__internal__n_start_req_47;
        }
        case 'S': {
          p++;
          goto s_n_llhttp__internal__n_start_req_50;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_44:
    s_n_llhttp__internal__n_start_req_44: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_44;
      }
      switch (*p) {
        case 'N': {
          p++;
          goto s_n_llhttp__internal__n_start_req_45;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req:
    s_n_llhttp__internal__n_start_req: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req;
      }
      switch (*p) {
        case 'A': {
          p++;
          goto s_n_llhttp__internal__n_start_req_1;
        }
        case 'B': {
          p++;
          goto s_n_llhttp__internal__n_start_req_2;
        }
        case 'C': {
          p++;
          goto s_n_llhttp__internal__n_start_req_3;
        }
        case 'D': {
          p++;
          goto s_n_llhttp__internal__n_start_req_8;
        }
        case 'G': {
          p++;
          goto s_n_llhttp__internal__n_start_req_9;
        }
        case 'H': {
          p++;
          goto s_n_llhttp__internal__n_start_req_10;
        }
        case 'L': {
          p++;
          goto s_n_llhttp__internal__n_start_req_11;
        }
        case 'M': {
          p++;
          goto s_n_llhttp__internal__n_start_req_14;
        }
        case 'N': {
          p++;
          goto s_n_llhttp__internal__n_start_req_23;
        }
        case 'O': {
          p++;
          goto s_n_llhttp__internal__n_start_req_24;
        }
        case 'P': {
          p++;
          goto s_n_llhttp__internal__n_start_req_25;
        }
        case 'R': {
          p++;
          goto s_n_llhttp__internal__n_start_req_35;
        }
        case 'S': {
          p++;
          goto s_n_llhttp__internal__n_start_req_39;
        }
        case 'T': {
          p++;
          goto s_n_llhttp__internal__n_start_req_43;
        }
        case 'U': {
          p++;
          goto s_n_llhttp__internal__n_start_req_44;
        }
        default: {
          goto s_n_llhttp__internal__n_error_46;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_line_almost_done:
    s_n_llhttp__internal__n_res_line_almost_done: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_line_almost_done;
      }
      p++;
      goto s_n_llhttp__internal__n_header_field_start;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_status:
    s_n_llhttp__internal__n_res_status: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_status;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_status;
        }
        case 13: {
          goto s_n_llhttp__internal__n_span_end_llhttp__on_status_1;
        }
        default: {
          p++;
          goto s_n_llhttp__internal__n_res_status;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_span_start_llhttp__on_status:
    s_n_llhttp__internal__n_span_start_llhttp__on_status: {
      if (p == endp) {
        return s_n_llhttp__internal__n_span_start_llhttp__on_status;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = (void*)llhttp__on_status;
      goto s_n_llhttp__internal__n_res_status;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_status_start:
    s_n_llhttp__internal__n_res_status_start: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_status_start;
      }
      switch (*p) {
        case 10: {
          p++;
          goto s_n_llhttp__internal__n_header_field_start;
        }
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_res_line_almost_done;
        }
        default: {
          goto s_n_llhttp__internal__n_span_start_llhttp__on_status;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_status_code_otherwise:
    s_n_llhttp__internal__n_res_status_code_otherwise: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_status_code_otherwise;
      }
      switch (*p) {
        case 10: {
          goto s_n_llhttp__internal__n_res_status_start;
        }
        case 13: {
          goto s_n_llhttp__internal__n_res_status_start;
        }
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_res_status_start;
        }
        default: {
          goto s_n_llhttp__internal__n_error_40;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_status_code:
    s_n_llhttp__internal__n_res_status_code: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_status_code;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_mul_add_status_code;
        }
        default: {
          goto s_n_llhttp__internal__n_res_status_code_otherwise;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_http_end:
    s_n_llhttp__internal__n_res_http_end: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_http_end;
      }
      switch (*p) {
        case ' ': {
          p++;
          goto s_n_llhttp__internal__n_invoke_update_status_code;
        }
        default: {
          goto s_n_llhttp__internal__n_error_41;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_http_minor:
    s_n_llhttp__internal__n_res_http_minor: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_http_minor;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_store_http_minor_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_42;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_http_dot:
    s_n_llhttp__internal__n_res_http_dot: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_http_dot;
      }
      switch (*p) {
        case '.': {
          p++;
          goto s_n_llhttp__internal__n_res_http_minor;
        }
        default: {
          goto s_n_llhttp__internal__n_error_43;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_res_http_major:
    s_n_llhttp__internal__n_res_http_major: {
      if (p == endp) {
        return s_n_llhttp__internal__n_res_http_major;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 0;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '2': {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '3': {
          p++;
          match = 3;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '4': {
          p++;
          match = 4;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '5': {
          p++;
          match = 5;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '6': {
          p++;
          match = 6;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '7': {
          p++;
          match = 7;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '8': {
          p++;
          match = 8;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        case '9': {
          p++;
          match = 9;
          goto s_n_llhttp__internal__n_invoke_store_http_major_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_44;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_res:
    s_n_llhttp__internal__n_start_res: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_start_res;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob48, 5);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_res_http_major;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_start_res;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_47;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_or_res_method_2:
    s_n_llhttp__internal__n_req_or_res_method_2: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_req_or_res_method_2;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob49, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 2;
          goto s_n_llhttp__internal__n_invoke_store_method;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_req_or_res_method_2;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_45;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_or_res_method_3:
    s_n_llhttp__internal__n_req_or_res_method_3: {
      llparse_match_t match_seq;

      if (p == endp) {
        return s_n_llhttp__internal__n_req_or_res_method_3;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob50, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_llhttp__internal__n_invoke_update_type_1;
        }
        case kMatchPause: {
          return s_n_llhttp__internal__n_req_or_res_method_3;
        }
        case kMatchMismatch: {
          goto s_n_llhttp__internal__n_error_45;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_or_res_method_1:
    s_n_llhttp__internal__n_req_or_res_method_1: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_or_res_method_1;
      }
      switch (*p) {
        case 'E': {
          p++;
          goto s_n_llhttp__internal__n_req_or_res_method_2;
        }
        case 'T': {
          p++;
          goto s_n_llhttp__internal__n_req_or_res_method_3;
        }
        default: {
          goto s_n_llhttp__internal__n_error_45;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_req_or_res_method:
    s_n_llhttp__internal__n_req_or_res_method: {
      if (p == endp) {
        return s_n_llhttp__internal__n_req_or_res_method;
      }
      switch (*p) {
        case 'H': {
          p++;
          goto s_n_llhttp__internal__n_req_or_res_method_1;
        }
        default: {
          goto s_n_llhttp__internal__n_error_45;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start_req_or_res:
    s_n_llhttp__internal__n_start_req_or_res: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start_req_or_res;
      }
      switch (*p) {
        case 'H': {
          goto s_n_llhttp__internal__n_req_or_res_method;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_type_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_invoke_load_type:
    s_n_llhttp__internal__n_invoke_load_type: {
      switch (llhttp__internal__c_load_type(state, p, endp)) {
        case 1:
          goto s_n_llhttp__internal__n_start_req;
        case 2:
          goto s_n_llhttp__internal__n_start_res;
        default:
          goto s_n_llhttp__internal__n_start_req_or_res;
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_llhttp__internal__n_start:
    s_n_llhttp__internal__n_start: {
      if (p == endp) {
        return s_n_llhttp__internal__n_start;
      }
      switch (*p) {
        case 10: {
          p++;
          goto s_n_llhttp__internal__n_start;
        }
        case 13: {
          p++;
          goto s_n_llhttp__internal__n_start;
        }
        default: {
          goto s_n_llhttp__internal__n_invoke_update_finish;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    default:
      /* UNREACHABLE */
      abort();
  }
  s_n_llhttp__internal__n_error_34: {
    state->error = 0x7;
    state->reason = "Invalid characters in url";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_finish_1: {
    switch (llhttp__internal__c_update_finish_1(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_start;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause_5: {
    state->error = 0x15;
    state->reason = "on_message_complete pause";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_is_equal_upgrade;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_9: {
    state->error = 0x12;
    state->reason = "`on_message_complete` callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause_7: {
    state->error = 0x15;
    state->reason = "on_chunk_complete pause";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_13: {
    state->error = 0x14;
    state->reason = "`on_chunk_complete` callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__on_chunk_complete_1: {
    switch (llhttp__on_chunk_complete(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2;
      case 21:
        goto s_n_llhttp__internal__n_pause_7;
      default:
        goto s_n_llhttp__internal__n_error_13;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_11: {
    state->error = 0x4;
    state->reason = "Content-Length can't be present with Transfer-Encoding";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_12: {
    state->error = 0x4;
    state->reason = "Content-Length can't be present with chunked encoding";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause_2: {
    state->error = 0x15;
    state->reason = "on_message_complete pause";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_pause_1;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_3: {
    state->error = 0x12;
    state->reason = "`on_message_complete` callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_1: {
    switch (llhttp__on_message_complete(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_pause_1;
      case 21:
        goto s_n_llhttp__internal__n_pause_2;
      default:
        goto s_n_llhttp__internal__n_error_3;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_7: {
    state->error = 0xc;
    state->reason = "Chunk size overflow";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause_3: {
    state->error = 0x15;
    state->reason = "on_chunk_complete pause";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_update_content_length;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_5: {
    state->error = 0x14;
    state->reason = "`on_chunk_complete` callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__on_chunk_complete: {
    switch (llhttp__on_chunk_complete(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_invoke_update_content_length;
      case 21:
        goto s_n_llhttp__internal__n_pause_3;
      default:
        goto s_n_llhttp__internal__n_error_5;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_body: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_body(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_chunk_data_almost_done;
      return s_error;
    }
    goto s_n_llhttp__internal__n_chunk_data_almost_done;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags: {
    switch (llhttp__internal__c_or_flags(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_field_start;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause_4: {
    state->error = 0x15;
    state->reason = "on_chunk_header pause";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_is_equal_content_length;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_4: {
    state->error = 0x13;
    state->reason = "`on_chunk_header` callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__on_chunk_header: {
    switch (llhttp__on_chunk_header(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_invoke_is_equal_content_length;
      case 21:
        goto s_n_llhttp__internal__n_pause_4;
      default:
        goto s_n_llhttp__internal__n_error_4;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_6: {
    state->error = 0xc;
    state->reason = "Invalid character in chunk size";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_mul_add_content_length: {
    switch (llhttp__internal__c_mul_add_content_length(state, p, endp, match)) {
      case 1:
        goto s_n_llhttp__internal__n_error_7;
      default:
        goto s_n_llhttp__internal__n_chunk_size;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_8: {
    state->error = 0xc;
    state->reason = "Invalid character in chunk size";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_body_1: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_body(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2;
      return s_error;
    }
    goto s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_finish_2: {
    switch (llhttp__internal__c_update_finish_2(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_span_start_llhttp__on_body_2;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_10: {
    state->error = 0xf;
    state->reason = "Request has invalid `Transfer-Encoding`";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause: {
    state->error = 0x15;
    state->reason = "on_message_complete pause";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_llhttp__after_message_complete;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_2: {
    state->error = 0x12;
    state->reason = "`on_message_complete` callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__on_message_complete: {
    switch (llhttp__on_message_complete(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_invoke_llhttp__after_message_complete;
      case 21:
        goto s_n_llhttp__internal__n_pause;
      default:
        goto s_n_llhttp__internal__n_error_2;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_1: {
    switch (llhttp__internal__c_or_flags_1(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_2: {
    switch (llhttp__internal__c_or_flags_1(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_upgrade: {
    switch (llhttp__internal__c_update_upgrade(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_or_flags_2;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause_6: {
    state->error = 0x15;
    state->reason = "Paused by on_headers_complete";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_1: {
    state->error = 0x11;
    state->reason = "User callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__on_headers_complete: {
    switch (llhttp__on_headers_complete(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete;
      case 1:
        goto s_n_llhttp__internal__n_invoke_or_flags_1;
      case 2:
        goto s_n_llhttp__internal__n_invoke_update_upgrade;
      case 21:
        goto s_n_llhttp__internal__n_pause_6;
      default:
        goto s_n_llhttp__internal__n_error_1;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__before_headers_complete: {
    switch (llhttp__before_headers_complete(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_llhttp__on_headers_complete;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_test_flags_3: {
    switch (llhttp__internal__c_test_flags_3(state, p, endp)) {
      case 1:
        goto s_n_llhttp__internal__n_error_12;
      default:
        goto s_n_llhttp__internal__n_invoke_llhttp__before_headers_complete;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_test_flags_2: {
    switch (llhttp__internal__c_test_flags_2(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_error_11;
      case 1:
        goto s_n_llhttp__internal__n_invoke_test_flags_3;
      default:
        goto s_n_llhttp__internal__n_invoke_llhttp__before_headers_complete;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_test_flags_1: {
    switch (llhttp__internal__c_test_flags_1(state, p, endp)) {
      case 1:
        goto s_n_llhttp__internal__n_invoke_test_flags_2;
      default:
        goto s_n_llhttp__internal__n_invoke_llhttp__before_headers_complete;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_test_flags: {
    switch (llhttp__internal__c_test_flags(state, p, endp)) {
      case 1:
        goto s_n_llhttp__internal__n_invoke_llhttp__on_chunk_complete_1;
      default:
        goto s_n_llhttp__internal__n_invoke_test_flags_1;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_14: {
    state->error = 0xb;
    state->reason = "Empty Content-Length";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_value: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_value(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_header_field_start;
      return s_error;
    }
    goto s_n_llhttp__internal__n_header_field_start;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state: {
    switch (llhttp__internal__c_update_header_state(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_span_start_llhttp__on_header_value;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_3: {
    switch (llhttp__internal__c_or_flags_3(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_4: {
    switch (llhttp__internal__c_or_flags_4(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_5: {
    switch (llhttp__internal__c_or_flags_5(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_6: {
    switch (llhttp__internal__c_or_flags_6(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_span_start_llhttp__on_header_value;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_load_header_state_1: {
    switch (llhttp__internal__c_load_header_state(state, p, endp)) {
      case 5:
        goto s_n_llhttp__internal__n_invoke_or_flags_3;
      case 6:
        goto s_n_llhttp__internal__n_invoke_or_flags_4;
      case 7:
        goto s_n_llhttp__internal__n_invoke_or_flags_5;
      case 8:
        goto s_n_llhttp__internal__n_invoke_or_flags_6;
      default:
        goto s_n_llhttp__internal__n_span_start_llhttp__on_header_value;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_load_header_state: {
    switch (llhttp__internal__c_load_header_state(state, p, endp)) {
      case 2:
        goto s_n_llhttp__internal__n_error_14;
      default:
        goto s_n_llhttp__internal__n_invoke_load_header_state_1;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_1: {
    switch (llhttp__internal__c_update_header_state(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_field_start;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_7: {
    switch (llhttp__internal__c_or_flags_3(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state_1;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_8: {
    switch (llhttp__internal__c_or_flags_4(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state_1;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_9: {
    switch (llhttp__internal__c_or_flags_5(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state_1;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_10: {
    switch (llhttp__internal__c_or_flags_6(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_field_start;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_load_header_state_3: {
    switch (llhttp__internal__c_load_header_state(state, p, endp)) {
      case 5:
        goto s_n_llhttp__internal__n_invoke_or_flags_7;
      case 6:
        goto s_n_llhttp__internal__n_invoke_or_flags_8;
      case 7:
        goto s_n_llhttp__internal__n_invoke_or_flags_9;
      case 8:
        goto s_n_llhttp__internal__n_invoke_or_flags_10;
      default:
        goto s_n_llhttp__internal__n_header_field_start;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_15: {
    state->error = 0x3;
    state->reason = "Missing expected LF after header value";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_value_1: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_value(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_header_value_almost_done;
      return s_error;
    }
    goto s_n_llhttp__internal__n_header_value_almost_done;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_value_2: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_value(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) (p + 1);
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_header_value_almost_done;
      return s_error;
    }
    p++;
    goto s_n_llhttp__internal__n_header_value_almost_done;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_value_3: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_value(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) (p + 1);
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_header_value_almost_done;
      return s_error;
    }
    p++;
    goto s_n_llhttp__internal__n_header_value_almost_done;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_16: {
    state->error = 0xa;
    state->reason = "Invalid header value char";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_test_flags_4: {
    switch (llhttp__internal__c_test_flags_2(state, p, endp)) {
      case 1:
        goto s_n_llhttp__internal__n_header_value_lenient;
      default:
        goto s_n_llhttp__internal__n_error_16;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_3: {
    switch (llhttp__internal__c_update_header_state(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_connection;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_11: {
    switch (llhttp__internal__c_or_flags_3(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state_3;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_12: {
    switch (llhttp__internal__c_or_flags_4(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state_3;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_13: {
    switch (llhttp__internal__c_or_flags_5(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state_3;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_14: {
    switch (llhttp__internal__c_or_flags_6(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_connection;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_load_header_state_4: {
    switch (llhttp__internal__c_load_header_state(state, p, endp)) {
      case 5:
        goto s_n_llhttp__internal__n_invoke_or_flags_11;
      case 6:
        goto s_n_llhttp__internal__n_invoke_or_flags_12;
      case 7:
        goto s_n_llhttp__internal__n_invoke_or_flags_13;
      case 8:
        goto s_n_llhttp__internal__n_invoke_or_flags_14;
      default:
        goto s_n_llhttp__internal__n_header_value_connection;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_4: {
    switch (llhttp__internal__c_update_header_state_4(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_connection_token;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_2: {
    switch (llhttp__internal__c_update_header_state_2(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_connection_ws;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_5: {
    switch (llhttp__internal__c_update_header_state_5(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_connection_ws;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_6: {
    switch (llhttp__internal__c_update_header_state_6(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_connection_ws;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_value_4: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_value(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_error_18;
      return s_error;
    }
    goto s_n_llhttp__internal__n_error_18;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_mul_add_content_length_1: {
    switch (llhttp__internal__c_mul_add_content_length_1(state, p, endp, match)) {
      case 1:
        goto s_n_llhttp__internal__n_span_end_llhttp__on_header_value_4;
      default:
        goto s_n_llhttp__internal__n_header_value_content_length;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_7: {
    switch (llhttp__internal__c_update_header_state_4(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_15: {
    switch (llhttp__internal__c_or_flags_15(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_discard_rws;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_value_5: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_value(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_error_19;
      return s_error;
    }
    goto s_n_llhttp__internal__n_error_19;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_17: {
    state->error = 0x4;
    state->reason = "Duplicate Content-Length";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_test_flags_5: {
    switch (llhttp__internal__c_test_flags_5(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_header_value_content_length;
      default:
        goto s_n_llhttp__internal__n_error_17;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_8: {
    switch (llhttp__internal__c_update_header_state_8(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_otherwise;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_16: {
    switch (llhttp__internal__c_or_flags_16(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_value_te_chunked;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_or_flags_17: {
    switch (llhttp__internal__c_or_flags_17(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_header_state_7;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_load_header_state_2: {
    switch (llhttp__internal__c_load_header_state(state, p, endp)) {
      case 1:
        goto s_n_llhttp__internal__n_header_value_connection;
      case 2:
        goto s_n_llhttp__internal__n_invoke_test_flags_5;
      case 3:
        goto s_n_llhttp__internal__n_invoke_or_flags_16;
      case 4:
        goto s_n_llhttp__internal__n_invoke_or_flags_17;
      default:
        goto s_n_llhttp__internal__n_header_value;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_field: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_field(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) (p + 1);
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_header_value_discard_ws;
      return s_error;
    }
    p++;
    goto s_n_llhttp__internal__n_header_value_discard_ws;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_header_field_1: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_header_field(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) (p + 1);
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_header_value_discard_ws;
      return s_error;
    }
    p++;
    goto s_n_llhttp__internal__n_header_value_discard_ws;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_20: {
    state->error = 0xa;
    state->reason = "Invalid header token";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_9: {
    switch (llhttp__internal__c_update_header_state_4(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_field_general;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_store_header_state: {
    switch (llhttp__internal__c_store_header_state(state, p, endp, match)) {
      default:
        goto s_n_llhttp__internal__n_header_field_colon;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_header_state_10: {
    switch (llhttp__internal__c_update_header_state_4(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_field_general;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_http_minor: {
    switch (llhttp__internal__c_update_http_minor(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_header_field_start;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_http_major: {
    switch (llhttp__internal__c_update_http_major(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_http_minor;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_3: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_21: {
    state->error = 0x7;
    state->reason = "Expected CRLF";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_4: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_lf_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_lf_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_22: {
    state->error = 0x9;
    state->reason = "Expected CRLF after version";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_store_http_minor: {
    switch (llhttp__internal__c_store_http_minor(state, p, endp, match)) {
      default:
        goto s_n_llhttp__internal__n_req_http_end;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_23: {
    state->error = 0x9;
    state->reason = "Invalid minor version";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_24: {
    state->error = 0x9;
    state->reason = "Expected dot";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_store_http_major: {
    switch (llhttp__internal__c_store_http_major(state, p, endp, match)) {
      default:
        goto s_n_llhttp__internal__n_req_http_dot;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_25: {
    state->error = 0x9;
    state->reason = "Invalid major version";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_27: {
    state->error = 0x8;
    state->reason = "Expected HTTP/";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_26: {
    state->error = 0x8;
    state->reason = "Expected SOURCE method for ICE/x.x request";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_is_equal_method_1: {
    switch (llhttp__internal__c_is_equal_method_1(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_error_26;
      default:
        goto s_n_llhttp__internal__n_req_http_major;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_5: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_6: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_7: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_lf_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_lf_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_8: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_28: {
    state->error = 0x7;
    state->reason = "Invalid char in url fragment start";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_9: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_10: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_lf_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_lf_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_11: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_29: {
    state->error = 0x7;
    state->reason = "Invalid char in url query";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_30: {
    state->error = 0x7;
    state->reason = "Invalid char in url path";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_1: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_lf_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_lf_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_2: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_12: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_13: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_lf_to_http09;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_lf_to_http09;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_url_14: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_url(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_url_skip_to_http;
      return s_error;
    }
    goto s_n_llhttp__internal__n_url_skip_to_http;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_31: {
    state->error = 0x7;
    state->reason = "Double @ in url";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_32: {
    state->error = 0x7;
    state->reason = "Unexpected char in url server";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_33: {
    state->error = 0x7;
    state->reason = "Unexpected char in url server";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_35: {
    state->error = 0x7;
    state->reason = "Unexpected char in url schema";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_36: {
    state->error = 0x7;
    state->reason = "Unexpected char in url schema";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_37: {
    state->error = 0x7;
    state->reason = "Unexpected start char in url";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_is_equal_method: {
    switch (llhttp__internal__c_is_equal_method(state, p, endp)) {
      case 0:
        goto s_n_llhttp__internal__n_span_start_llhttp__on_url_1;
      default:
        goto s_n_llhttp__internal__n_span_start_llhttp__on_url;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_38: {
    state->error = 0x6;
    state->reason = "Expected space after method";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_store_method_1: {
    switch (llhttp__internal__c_store_method(state, p, endp, match)) {
      default:
        goto s_n_llhttp__internal__n_req_first_space_before_url;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_46: {
    state->error = 0x6;
    state->reason = "Invalid method encountered";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_39: {
    state->error = 0xd;
    state->reason = "Response overflow";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_mul_add_status_code: {
    switch (llhttp__internal__c_mul_add_status_code(state, p, endp, match)) {
      case 1:
        goto s_n_llhttp__internal__n_error_39;
      default:
        goto s_n_llhttp__internal__n_res_status_code;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_status: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_status(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) (p + 1);
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_header_field_start;
      return s_error;
    }
    p++;
    goto s_n_llhttp__internal__n_header_field_start;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_span_end_llhttp__on_status_1: {
    const unsigned char* start;
    int err;

    start = (const unsigned char*)state->_span_pos0;
    state->_span_pos0 = NULL;
    err = llhttp__on_status(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) (p + 1);
      state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_res_line_almost_done;
      return s_error;
    }
    p++;
    goto s_n_llhttp__internal__n_res_line_almost_done;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_40: {
    state->error = 0xd;
    state->reason = "Invalid response status";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_status_code: {
    switch (llhttp__internal__c_update_status_code(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_res_status_code;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_41: {
    state->error = 0x9;
    state->reason = "Expected space after version";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_store_http_minor_1: {
    switch (llhttp__internal__c_store_http_minor(state, p, endp, match)) {
      default:
        goto s_n_llhttp__internal__n_res_http_end;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_42: {
    state->error = 0x9;
    state->reason = "Invalid minor version";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_43: {
    state->error = 0x9;
    state->reason = "Expected dot";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_store_http_major_1: {
    switch (llhttp__internal__c_store_http_major(state, p, endp, match)) {
      default:
        goto s_n_llhttp__internal__n_res_http_dot;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_44: {
    state->error = 0x9;
    state->reason = "Invalid major version";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_47: {
    state->error = 0x8;
    state->reason = "Expected HTTP/";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_type: {
    switch (llhttp__internal__c_update_type(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_req_first_space_before_url;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_store_method: {
    switch (llhttp__internal__c_store_method(state, p, endp, match)) {
      default:
        goto s_n_llhttp__internal__n_invoke_update_type;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error_45: {
    state->error = 0x8;
    state->reason = "Invalid word encountered";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_type_1: {
    switch (llhttp__internal__c_update_type_1(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_res_http_major;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_type_2: {
    switch (llhttp__internal__c_update_type(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_start_req;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_pause_8: {
    state->error = 0x15;
    state->reason = "on_message_begin pause";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_n_llhttp__internal__n_invoke_load_type;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_error: {
    state->error = 0x10;
    state->reason = "`on_message_begin` callback error";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_llhttp__on_message_begin: {
    switch (llhttp__on_message_begin(state, (const char*)p, (const char*)endp)) {
      case 0:
        goto s_n_llhttp__internal__n_invoke_load_type;
      case 21:
        goto s_n_llhttp__internal__n_pause_8;
      default:
        goto s_n_llhttp__internal__n_error;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_llhttp__internal__n_invoke_update_finish: {
    switch (llhttp__internal__c_update_finish(state, p, endp)) {
      default:
        goto s_n_llhttp__internal__n_invoke_llhttp__on_message_begin;
    }
    /* UNREACHABLE */;
    abort();
  }
}

int llhttp__internal_execute(llhttp__internal_t* state, const char* p, const char* endp) {
  llparse_state_t next;

  /* check lingering errors */
  if (state->error != 0) {
    return state->error;
  }

  /* restart spans */
  if (state->_span_pos0 != NULL) {
    state->_span_pos0 = (void*) p;
  }

  next = llhttp__internal__run(state, (const unsigned char*) p, (const unsigned char*) endp);
  if (next == s_error) {
    return state->error;
  }
  state->_current = (void*) (intptr_t) next;

  /* execute spans */
  if (state->_span_pos0 != NULL) {
    int error;

    error = ((llhttp__internal__span_cb) state->_span_cb0)(state, (const char*)state->_span_pos0, (const char*) endp);
    if (error != 0) {
      state->error = error;
      state->error_pos = endp;
      return error;
    }
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// api.c
#define CALLBACK_MAYBE(PARSER, NAME, ...)                                     \
  do {                                                                        \
    llhttp_settings_t* settings;                                              \
    settings = (llhttp_settings_t*) (PARSER)->settings;                       \
    if (settings == NULL || settings->NAME == NULL) {                         \
      err = HPE_OK;                                                                \
      break;                                                                  \
    }                                                                         \
    err = (llhttp_errno_t)settings->NAME(__VA_ARGS__);                                        \
  } while (0)

void llhttp_init(llhttp_t* parser, llhttp_type_t type,
                 const llhttp_settings_t* settings) {
  llhttp__internal_init(parser);

  parser->type = type;
  parser->settings = (void*) settings;
}


llhttp_errno_t llhttp_execute(llhttp_t* parser, const char* data, size_t len) {
  return (llhttp_errno_t)llhttp__internal_execute(parser, data, data + len);
}


void llhttp_settings_init(llhttp_settings_t* settings) {
  memset(settings, 0, sizeof(*settings));
}


llhttp_errno_t llhttp_finish(llhttp_t* parser) {
  llhttp_errno_t err;

  /* We're in an error state. Don't bother doing anything. */
  if (parser->error != 0) {
    return HPE_OK;
  }

  switch (parser->finish) {
    case HTTP_FINISH_SAFE_WITH_CB:
      CALLBACK_MAYBE(parser, on_message_complete, parser, nullptr, 0);
      if (err != HPE_OK) return err;

    /* FALLTHROUGH */
    case HTTP_FINISH_SAFE:
      return HPE_OK;
    case HTTP_FINISH_UNSAFE:
      parser->reason = "Invalid EOF state";
      return HPE_INVALID_EOF_STATE;
    default:
      abort();
  }
}


void llhttp_pause(llhttp_t* parser) {
  if (parser->error != HPE_OK) {
    return;
  }

  parser->error = HPE_PAUSED;
  parser->reason = "Paused";
}


void llhttp_resume(llhttp_t* parser) {
  if (parser->error != HPE_PAUSED) {
    return;
  }

  parser->error = 0;
}


void llhttp_resume_after_upgrade(llhttp_t* parser) {
  if (parser->error != HPE_PAUSED_UPGRADE) {
    return;
  }

  parser->error = 0;
}


llhttp_errno_t llhttp_get_errno(const llhttp_t* parser) {
  return (llhttp_errno_t)parser->error;
}


const char* llhttp_get_error_reason(const llhttp_t* parser) {
  return parser->reason;
}


void llhttp_set_error_reason(llhttp_t* parser, const char* reason) {
  parser->reason = reason;
}


const char* llhttp_get_error_pos(const llhttp_t* parser) {
  return parser->error_pos;
}


const char* llhttp_errno_name(llhttp_errno_t err) {
#define HTTP_ERRNO_GEN(CODE, NAME, _) case HPE_##NAME: return "HPE_" #NAME;
  switch (err) {
    HTTP_ERRNO_MAP(HTTP_ERRNO_GEN)
    default: abort();
  }
#undef HTTP_ERRNO_GEN
}


const char* llhttp_method_name(llhttp_method_t method) {
#define HTTP_METHOD_GEN(NUM, NAME, STRING) case HTTP_##NAME: return #STRING;
  switch (method) {
    HTTP_METHOD_MAP(HTTP_METHOD_GEN)
    default: abort();
  }
#undef HTTP_METHOD_GEN
}


void llhttp_set_lenient(llhttp_t* parser, int enabled) {
  if (enabled) {
    parser->flags |= F_LENIENT;
  } else {
    parser->flags &= ~F_LENIENT;
  }
}


/* Callbacks */


int llhttp__on_message_begin(llhttp_t* s, const char* p, const char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_message_begin, s);
  return err;
}


int llhttp__on_url(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_url, s, (const char*)p, endp - p);
  return err;
}


int llhttp__on_status(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_status, s, (const char*)p, endp - p);
  return err;
}


int llhttp__on_header_field(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_header_field, s, (const char*)p, endp - p);
  return err;
}


int llhttp__on_header_value(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_header_value, s, (const char*)p, endp - p);
  return err;
}


int llhttp__on_headers_complete(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_headers_complete, s);
  return err;
}


int llhttp__on_message_complete(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_message_complete, s, (const char*)p, endp - p);
  return err;
}


int llhttp__on_body(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_body, s, (const char*)p, endp - p);
  return err;
}


int llhttp__on_chunk_header(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_chunk_header, s);
  return err;
}


int llhttp__on_chunk_complete(llhttp_t* s, const unsigned char* p, const unsigned char* endp) {
  int err;
  CALLBACK_MAYBE(s, on_chunk_complete, s);
  return err;
}


/* Private */


void llhttp__debug(llhttp_t* s, const char* p, const char* endp,
                   const char* msg) {
  if (p == endp) {
    fprintf(stderr, "p=%p type=%d flags=%02x next=null debug=%s\n", s, s->type,
            s->flags, msg);
  } else {
    fprintf(stderr, "p=%p type=%d flags=%02x next=%02x   debug=%s\n", s,
            s->type, s->flags, *p, msg);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// http.c

int llhttp_message_needs_eof(const llhttp_t* parser);
int llhttp_should_keep_alive(const llhttp_t* parser);

int llhttp__before_headers_complete(llhttp_t* parser, const unsigned char* p,
                                    const unsigned char* endp) {
  /* Set this here so that on_headers_complete() callbacks can see it */
  if ((parser->flags & F_UPGRADE) &&
      (parser->flags & F_CONNECTION_UPGRADE)) {
    /* For responses, "Upgrade: foo" and "Connection: upgrade" are
     * mandatory only when it is a 101 Switching Protocols response,
     * otherwise it is purely informational, to announce support.
     */
    parser->upgrade =
        (parser->type == HTTP_REQUEST || parser->status_code == 101);
  } else {
    parser->upgrade = (parser->method == HTTP_CONNECT);
  }
  return 0;
}


/* Return values:
 * 0 - No body, `restart`, message_complete
 * 1 - CONNECT request, `restart`, message_complete, and pause
 * 2 - chunk_size_start
 * 3 - body_identity
 * 4 - body_identity_eof
 * 5 - invalid transfer-encoding for request
 */
int llhttp__after_headers_complete(llhttp_t* parser, const unsigned char* p,
                                   const unsigned char* endp) {
  int hasBody;

  hasBody = parser->flags & F_CHUNKED || parser->content_length > 0;
  if (parser->upgrade && (parser->method == HTTP_CONNECT ||
                          (parser->flags & F_SKIPBODY) || !hasBody)) {
    /* Exit, the rest of the message is in a different protocol. */
    return 1;
  }

  if (parser->flags & F_SKIPBODY) {
    return 0;
  } else if (parser->flags & F_CHUNKED) {
    /* chunked encoding - ignore Content-Length header, prepare for a chunk */
    return 2;
  } else if (parser->flags & F_TRANSFER_ENCODING) {
    if (parser->type == HTTP_REQUEST && (parser->flags & F_LENIENT) == 0) {
      /* RFC 7230 3.3.3 */

      /* If a Transfer-Encoding header field
       * is present in a request and the chunked transfer coding is not
       * the final encoding, the message body length cannot be determined
       * reliably; the server MUST respond with the 400 (Bad Request)
       * status code and then close the connection.
       */
      return 5;
    } else {
      /* RFC 7230 3.3.3 */

      /* If a Transfer-Encoding header field is present in a response and
       * the chunked transfer coding is not the final encoding, the
       * message body length is determined by reading the connection until
       * it is closed by the server.
       */
      return 4;
    }
  } else {
    if (!(parser->flags & F_CONTENT_LENGTH)) {
      if (!llhttp_message_needs_eof(parser)) {
        /* Assume content-length 0 - read the next */
        return 0;
      } else {
        /* Read body until EOF */
        return 4;
      }
    } else if (parser->content_length == 0) {
      /* Content-Length header given but zero: Content-Length: 0\r\n */
      return 0;
    } else {
      /* Content-Length header given and non-zero */
      return 3;
    }
  }
}


int llhttp__after_message_complete(llhttp_t* parser, const unsigned char* p,
                                   const unsigned char* endp) {
  int should_keep_alive;

  should_keep_alive = llhttp_should_keep_alive(parser);
  parser->finish = HTTP_FINISH_SAFE;

  /* Keep `F_LENIENT` flag between messages, but reset every other flag */
  parser->flags &= F_LENIENT;

  /* NOTE: this is ignored in loose parsing mode */
  return should_keep_alive;
}


int llhttp_message_needs_eof(const llhttp_t* parser) {
  if (parser->type == HTTP_REQUEST) {
    return 0;
  }

  /* See RFC 2616 section 4.4 */
  if (parser->status_code / 100 == 1 || /* 1xx e.g. Continue */
      parser->status_code == 204 ||     /* No Content */
      parser->status_code == 304 ||     /* Not Modified */
      (parser->flags & F_SKIPBODY)) {     /* response to a HEAD request */
    return 0;
  }

  /* RFC 7230 3.3.3, see `llhttp__after_headers_complete` */
  if ((parser->flags & F_TRANSFER_ENCODING) &&
      (parser->flags & F_CHUNKED) == 0) {
    return 1;
  }

  if (parser->flags & (F_CHUNKED | F_CONTENT_LENGTH)) {
    return 0;
  }

  return 1;
}


int llhttp_should_keep_alive(const llhttp_t* parser) {
  if (parser->http_major > 0 && parser->http_minor > 0) {
    /* HTTP/1.1 */
    if (parser->flags & F_CONNECTION_CLOSE) {
      return 0;
    }
  } else {
    /* HTTP/1.0 or earlier */
    if (!(parser->flags & F_CONNECTION_KEEP_ALIVE)) {
      return 0;
    }
  }

  return !llhttp_message_needs_eof(parser);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// http-parser parse_url

#ifndef BIT_AT
# define BIT_AT(a, i)                                                \
  (!!((unsigned int) (a)[(unsigned int) (i) >> 3] &                  \
   (1 << ((unsigned int) (i) & 7))))
#endif

#define CR                  '\r'
#define LF                  '\n'
#define LOWER(c)            (unsigned char)(c | 0x20)
#define IS_ALPHA(c)         (LOWER(c) >= 'a' && LOWER(c) <= 'z')
#define IS_NUM(c)           ((c) >= '0' && (c) <= '9')
#define IS_ALPHANUM(c)      (IS_ALPHA(c) || IS_NUM(c))
#define IS_HEX(c)           (IS_NUM(c) || (LOWER(c) >= 'a' && LOWER(c) <= 'f'))
#define IS_MARK(c)          ((c) == '-' || (c) == '_' || (c) == '.' || \
  (c) == '!' || (c) == '~' || (c) == '*' || (c) == '\'' || (c) == '(' || \
  (c) == ')')
#define IS_USERINFO_CHAR(c) (IS_ALPHANUM(c) || IS_MARK(c) || (c) == '%' || \
  (c) == ';' || (c) == ':' || (c) == '&' || (c) == '=' || (c) == '+' || \
  (c) == '$' || (c) == ',')

  #define STRICT_TOKEN(c)     ((c == ' ') ? 0 : tokens[(unsigned char)c])

#if HTTP_PARSER_STRICT
#define TOKEN(c)            STRICT_TOKEN(c)
#define IS_URL_CHAR(c)      (BIT_AT(normal_url_char, (unsigned char)c))
#define IS_HOST_CHAR(c)     (IS_ALPHANUM(c) || (c) == '.' || (c) == '-')
#else
#define TOKEN(c)            tokens[(unsigned char)c]
#define IS_URL_CHAR(c)                                                         \
  (BIT_AT(normal_url_char, (unsigned char)c) || ((c) & 0x80))
#define IS_HOST_CHAR(c)                                                        \
  (IS_ALPHANUM(c) || (c) == '.' || (c) == '-' || (c) == '_')
#endif

#if HTTP_PARSER_STRICT
# define T(v) 0
#else
# define T(v) v
#endif


static const uint8_t normal_url_char[32] = {
/*   0 nul    1 soh    2 stx    3 etx    4 eot    5 enq    6 ack    7 bel  */
        0    |   0    |   0    |   0    |   0    |   0    |   0    |   0,
/*   8 bs     9 ht    10 nl    11 vt    12 np    13 cr    14 so    15 si   */
        0    | T(2)   |   0    |   0    | T(16)  |   0    |   0    |   0,
/*  16 dle   17 dc1   18 dc2   19 dc3   20 dc4   21 nak   22 syn   23 etb */
        0    |   0    |   0    |   0    |   0    |   0    |   0    |   0,
/*  24 can   25 em    26 sub   27 esc   28 fs    29 gs    30 rs    31 us  */
        0    |   0    |   0    |   0    |   0    |   0    |   0    |   0,
/*  32 sp    33  !    34  "    35  #    36  $    37  %    38  &    39  '  */
        0    |   2    |   4    |   0    |   16   |   32   |   64   |  128,
/*  40  (    41  )    42  *    43  +    44  ,    45  -    46  .    47  /  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/*  48  0    49  1    50  2    51  3    52  4    53  5    54  6    55  7  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/*  56  8    57  9    58  :    59  ;    60  <    61  =    62  >    63  ?  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |   0,
/*  64  @    65  A    66  B    67  C    68  D    69  E    70  F    71  G  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/*  72  H    73  I    74  J    75  K    76  L    77  M    78  N    79  O  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/*  80  P    81  Q    82  R    83  S    84  T    85  U    86  V    87  W  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/*  88  X    89  Y    90  Z    91  [    92  \    93  ]    94  ^    95  _  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/*  96  `    97  a    98  b    99  c   100  d   101  e   102  f   103  g  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/* 104  h   105  i   106  j   107  k   108  l   109  m   110  n   111  o  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/* 112  p   113  q   114  r   115  s   116  t   117  u   118  v   119  w  */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |  128,
/* 120  x   121  y   122  z   123  {   124  |   125  }   126  ~   127 del */
        1    |   2    |   4    |   8    |   16   |   32   |   64   |   0, };

#undef T

enum state
  { s_dead = 1 /* important that this is > 0 */

  , s_start_req_or_res
  , s_res_or_resp_H
  , s_start_res
  , s_res_H
  , s_res_HT
  , s_res_HTT
  , s_res_HTTP
  , s_res_http_major
  , s_res_http_dot
  , s_res_http_minor
  , s_res_http_end
  , s_res_first_status_code
  , s_res_status_code
  , s_res_status_start
  , s_res_status
  , s_res_line_almost_done

  , s_start_req

  , s_req_method
  , s_req_spaces_before_url
  , s_req_schema
  , s_req_schema_slash
  , s_req_schema_slash_slash
  , s_req_server_start
  , s_req_server
  , s_req_server_with_at
  , s_req_path
  , s_req_query_string_start
  , s_req_query_string
  , s_req_fragment_start
  , s_req_fragment
  , s_req_http_start
  , s_req_http_H
  , s_req_http_HT
  , s_req_http_HTT
  , s_req_http_HTTP
  , s_req_http_I
  , s_req_http_IC
  , s_req_http_major
  , s_req_http_dot
  , s_req_http_minor
  , s_req_http_end
  , s_req_line_almost_done

  , s_header_field_start
  , s_header_field
  , s_header_value_discard_ws
  , s_header_value_discard_ws_almost_done
  , s_header_value_discard_lws
  , s_header_value_start
  , s_header_value
  , s_header_value_lws

  , s_header_almost_done

  , s_chunk_size_start
  , s_chunk_size
  , s_chunk_parameters
  , s_chunk_size_almost_done

  , s_headers_almost_done
  , s_headers_done

  /* Important: 's_headers_done' must be the last 'header' state. All
   * states beyond this must be 'body' states. It is used for overflow
   * checking. See the PARSING_HEADER() macro.
   */

  , s_chunk_data
  , s_chunk_data_almost_done
  , s_chunk_data_done

  , s_body_identity
  , s_body_identity_eof

  , s_message_done
  };

enum http_host_state
  {
    s_http_host_dead = 1
  , s_http_userinfo_start
  , s_http_userinfo
  , s_http_host_start
  , s_http_host_v6_start
  , s_http_host
  , s_http_host_v6
  , s_http_host_v6_end
  , s_http_host_v6_zone_start
  , s_http_host_v6_zone
  , s_http_host_port_start
  , s_http_host_port
};

enum http_parser_url_fields
  { UF_SCHEMA           = 0
  , UF_HOST             = 1
  , UF_PORT             = 2
  , UF_PATH             = 3
  , UF_QUERY            = 4
  , UF_FRAGMENT         = 5
  , UF_USERINFO         = 6
  , UF_MAX              = 7
  };


/* Result structure for http_parser_parse_url().
 *
 * Callers should index into field_data[] with UF_* values iff field_set
 * has the relevant (1 << UF_*) bit set. As a courtesy to clients (and
 * because we probably have padding left over), we convert any port to
 * a uint16_t.
 */
struct http_parser_url {
  uint16_t field_set;           /* Bitmask of (1 << UF_*) values */
  uint16_t port;                /* Converted UF_PORT string */

  struct {
    uint16_t off;               /* Offset into buffer in which field starts */
    uint16_t len;               /* Length of run in buffer */
  } field_data[UF_MAX];
};

static enum http_host_state
http_parse_host_char(enum http_host_state s, const char ch) {
  switch(s) {
    case s_http_userinfo:
    case s_http_userinfo_start:
      if (ch == '@') {
        return s_http_host_start;
      }

      if (IS_USERINFO_CHAR(ch)) {
        return s_http_userinfo;
      }
      break;

    case s_http_host_start:
      if (ch == '[') {
        return s_http_host_v6_start;
      }

      if (IS_HOST_CHAR(ch)) {
        return s_http_host;
      }

      break;

    case s_http_host:
      if (IS_HOST_CHAR(ch)) {
        return s_http_host;
      }

    /* fall through */
    case s_http_host_v6_end:
      if (ch == ':') {
        return s_http_host_port_start;
      }

      break;

    case s_http_host_v6:
      if (ch == ']') {
        return s_http_host_v6_end;
      }

    /* fall through */
    case s_http_host_v6_start:
      if (IS_HEX(ch) || ch == ':' || ch == '.') {
        return s_http_host_v6;
      }

      if (s == s_http_host_v6 && ch == '%') {
        return s_http_host_v6_zone_start;
      }
      break;

    case s_http_host_v6_zone:
      if (ch == ']') {
        return s_http_host_v6_end;
      }

    /* fall through */
    case s_http_host_v6_zone_start:
      /* RFC 6874 Zone ID consists of 1*( unreserved / pct-encoded) */
      if (IS_ALPHANUM(ch) || ch == '%' || ch == '.' || ch == '-' || ch == '_' ||
          ch == '~') {
        return s_http_host_v6_zone;
      }
      break;

    case s_http_host_port:
    case s_http_host_port_start:
      if (IS_NUM(ch)) {
        return s_http_host_port;
      }

      break;

    default:
      break;
  }
  return s_http_host_dead;
}

static int
http_parse_host(const char * buf, struct http_parser_url *u, int found_at) {
  enum http_host_state s;

  const char *p;
  size_t buflen = u->field_data[UF_HOST].off + u->field_data[UF_HOST].len;

  assert(u->field_set & (1 << UF_HOST));

  u->field_data[UF_HOST].len = 0;

  s = found_at ? s_http_userinfo_start : s_http_host_start;

  for (p = buf + u->field_data[UF_HOST].off; p < buf + buflen; p++) {
    enum http_host_state new_s = http_parse_host_char(s, *p);

    if (new_s == s_http_host_dead) {
      return 1;
    }

    switch(new_s) {
      case s_http_host:
        if (s != s_http_host) {
          u->field_data[UF_HOST].off = (uint16_t)(p - buf);
        }
        u->field_data[UF_HOST].len++;
        break;

      case s_http_host_v6:
        if (s != s_http_host_v6) {
          u->field_data[UF_HOST].off = (uint16_t)(p - buf);
        }
        u->field_data[UF_HOST].len++;
        break;

      case s_http_host_v6_zone_start:
      case s_http_host_v6_zone:
        u->field_data[UF_HOST].len++;
        break;

      case s_http_host_port:
        if (s != s_http_host_port) {
          u->field_data[UF_PORT].off = (uint16_t)(p - buf);
          u->field_data[UF_PORT].len = 0;
          u->field_set |= (1 << UF_PORT);
        }
        u->field_data[UF_PORT].len++;
        break;

      case s_http_userinfo:
        if (s != s_http_userinfo) {
          u->field_data[UF_USERINFO].off = (uint16_t)(p - buf);
          u->field_data[UF_USERINFO].len = 0;
          u->field_set |= (1 << UF_USERINFO);
        }
        u->field_data[UF_USERINFO].len++;
        break;

      default:
        break;
    }
    s = new_s;
  }

  /* Make sure we don't end somewhere unexpected */
  switch (s) {
    case s_http_host_start:
    case s_http_host_v6_start:
    case s_http_host_v6:
    case s_http_host_v6_zone_start:
    case s_http_host_v6_zone:
    case s_http_host_port_start:
    case s_http_userinfo:
    case s_http_userinfo_start:
      return 1;
    default:
      break;
  }

  return 0;
}

/* Our URL parser.
 *
 * This is designed to be shared by http_parser_execute() for URL validation,
 * hence it has a state transition + byte-for-byte interface. In addition, it
 * is meant to be embedded in http_parser_parse_url(), which does the dirty
 * work of turning state transitions URL components for its API.
 *
 * This function should only be invoked with non-space characters. It is
 * assumed that the caller cares about (and can detect) the transition between
 * URL and non-URL states by looking for these.
 */
static enum state
parse_url_char(enum state s, const char ch)
{
  if (ch == ' ' || ch == '\r' || ch == '\n') {
    return s_dead;
  }

#if HTTP_PARSER_STRICT
  if (ch == '\t' || ch == '\f') {
    return s_dead;
  }
#endif

  switch (s) {
    case s_req_spaces_before_url:
      /* Proxied requests are followed by scheme of an absolute URI (alpha).
       * All methods except CONNECT are followed by '/' or '*'.
       */

      if (ch == '/' || ch == '*') {
        return s_req_path;
      }

      if (IS_ALPHA(ch)) {
        return s_req_schema;
      }

      break;

    case s_req_schema:
      if (IS_ALPHA(ch)) {
        return s;
      }

      if (ch == ':') {
        return s_req_schema_slash;
      }

      break;

    case s_req_schema_slash:
      if (ch == '/') {
        return s_req_schema_slash_slash;
      }

      break;

    case s_req_schema_slash_slash:
      if (ch == '/') {
        return s_req_server_start;
      }

      break;

    case s_req_server_with_at:
      if (ch == '@') {
        return s_dead;
      }

    /* fall through */
    case s_req_server_start:
    case s_req_server:
      if (ch == '/') {
        return s_req_path;
      }

      if (ch == '?') {
        return s_req_query_string_start;
      }

      if (ch == '@') {
        return s_req_server_with_at;
      }

      if (IS_USERINFO_CHAR(ch) || ch == '[' || ch == ']') {
        return s_req_server;
      }

      break;

    case s_req_path:
      if (IS_URL_CHAR(ch)) {
        return s;
      }

      switch (ch) {
        case '?':
          return s_req_query_string_start;

        case '#':
          return s_req_fragment_start;
      }

      break;

    case s_req_query_string_start:
    case s_req_query_string:
      if (IS_URL_CHAR(ch)) {
        return s_req_query_string;
      }

      switch (ch) {
        case '?':
          /* allow extra '?' in query string */
          return s_req_query_string;

        case '#':
          return s_req_fragment_start;
      }

      break;

    case s_req_fragment_start:
      if (IS_URL_CHAR(ch)) {
        return s_req_fragment;
      }

      switch (ch) {
        case '?':
          return s_req_fragment;

        case '#':
          return s;
      }

      break;

    case s_req_fragment:
      if (IS_URL_CHAR(ch)) {
        return s;
      }

      switch (ch) {
        case '?':
        case '#':
          return s;
      }

      break;

    default:
      break;
  }

  /* We should never fall out of the switch above unless there's an error */
  return s_dead;
}

void
http_parser_url_init(struct http_parser_url *u) {
  memset(u, 0, sizeof(*u));
}

int
http_parser_parse_url(const char *buf, size_t buflen, int is_connect,
                      struct http_parser_url *u)
{
  enum state s;
  const char *p;
  enum http_parser_url_fields uf, old_uf;
  int found_at = 0;

  if (buflen == 0) {
    return 1;
  }

  u->port = u->field_set = 0;
  s = is_connect ? s_req_server_start : s_req_spaces_before_url;
  old_uf = UF_MAX;

  for (p = buf; p < buf + buflen; p++) {
    s = parse_url_char(s, *p);

    /* Figure out the next field that we're operating on */
    switch (s) {
      case s_dead:
        return 1;

      /* Skip delimeters */
      case s_req_schema_slash:
      case s_req_schema_slash_slash:
      case s_req_server_start:
      case s_req_query_string_start:
      case s_req_fragment_start:
        continue;

      case s_req_schema:
        uf = UF_SCHEMA;
        break;

      case s_req_server_with_at:
        found_at = 1;

      /* fall through */
      case s_req_server:
        uf = UF_HOST;
        break;

      case s_req_path:
        uf = UF_PATH;
        break;

      case s_req_query_string:
        uf = UF_QUERY;
        break;

      case s_req_fragment:
        uf = UF_FRAGMENT;
        break;

      default:
        assert(!"Unexpected state");
        return 1;
    }

    /* Nothing's changed; soldier on */
    if (uf == old_uf) {
      u->field_data[uf].len++;
      continue;
    }

    u->field_data[uf].off = (uint16_t)(p - buf);
    u->field_data[uf].len = 1;

    u->field_set |= (1 << uf);
    old_uf = uf;
  }

  /* host must be present if there is a schema */
  /* parsing http:///toto will fail */
  if ((u->field_set & (1 << UF_SCHEMA)) &&
      (u->field_set & (1 << UF_HOST)) == 0) {
    return 1;
  }

  if (u->field_set & (1 << UF_HOST)) {
    if (http_parse_host(buf, u, found_at) != 0) {
      return 1;
    }
  }

  /* CONNECT requests can only contain "hostname:port" */
  if (is_connect && u->field_set != ((1 << UF_HOST)|(1 << UF_PORT))) {
    return 1;
  }

  if (u->field_set & (1 << UF_PORT)) {
    uint16_t off;
    uint16_t len;
    const char* p;
    const char* end;
    unsigned long v;

    off = u->field_data[UF_PORT].off;
    len = u->field_data[UF_PORT].len;
    end = buf + off + len;

    /* NOTE: The characters are already validated and are in the [0-9] range */
    assert(off + len <= buflen && "Port number overflow");
    v = 0;
    for (p = buf + off; p < end; p++) {
      v *= 10;
      v += *p - '0';

      /* Ports have a max value of 2^16 */
      if (v > 0xffff) {
        return 1;
      }
    }

    u->port = (uint16_t) v;
  }

  return 0;
}

#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif  /* INCLUDE_LLHTTP_API_H_ */

#endif  /* INCLUDE_LLHTTP_H_ */

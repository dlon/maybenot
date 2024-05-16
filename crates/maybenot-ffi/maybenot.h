/* Generated with cbindgen:0.26.0 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum MaybenotEventType {
  /**
   * We sent a normal packet.
   */
  MaybenotEventType_NonpaddingSent = 0,
  /**
   * We received a normal packet.
   */
  MaybenotEventType_NonpaddingReceived = 1,
  /**
   * We send a padding packet.
   */
  MaybenotEventType_PaddingSent = 2,
  /**
   * We received a padding packet.
   */
  MaybenotEventType_PaddingReceived = 3,
};
typedef uint32_t MaybenotEventType;

/**
 * An FFI friendly result error code type.
 */
enum MaybenotResult {
  /**
   * Operation completed successfully
   */
  MaybenotResult_Ok = 0,
  /**
   * The machine string wasn't valid UTF-8
   */
  MaybenotResult_MachineStringNotUtf8 = 1,
  /**
   * Failed to parse machine string
   */
  MaybenotResult_InvalidMachineString = 2,
  /**
   * Failed to start framework
   */
  MaybenotResult_StartFramework = 3,
  /**
   * A null pointer was encountered
   */
  MaybenotResult_NullPointer = 4,
};
typedef uint32_t MaybenotResult;

/**
 * A running Maybenot instance.
 *
 * - Create it: [maybenot_start].
 * - Feed it actions: [maybenot_on_event].
 * - Stop it: [maybenot_stop].
 */
typedef struct MaybenotFramework MaybenotFramework;

typedef struct MaybenotEvent {
  MaybenotEventType event_type;
  /**
   * The number of bytes that was sent or received.
   */
  uint16_t xmit_bytes;
  /**
   * The ID of the machine that triggered the event, if any.
   */
  uintptr_t machine;
} MaybenotEvent;

typedef struct MaybenotDuration {
  /**
   * Number of whole seconds
   */
  uint64_t secs;
  /**
   * A nanosecond fraction of a second.
   */
  uint32_t nanos;
} MaybenotDuration;

enum MaybenotAction_Tag {
  MaybenotAction_Cancel = 0,
  /**
   * Send a padding packet.
   */
  MaybenotAction_InjectPadding = 1,
  MaybenotAction_BlockOutgoing = 2,
};
typedef uint32_t MaybenotAction_Tag;

typedef struct MaybenotAction_Cancel_Body {
  /**
   * The machine that generated the action.
   */
  uintptr_t machine;
} MaybenotAction_Cancel_Body;

typedef struct MaybenotAction_InjectPadding_Body {
  /**
   * The machine that generated the action.
   */
  uintptr_t machine;
  /**
   * The time to wait before injecting a padding packet.
   */
  struct MaybenotDuration timeout;
  bool replace;
  bool bypass;
  /**
   * The size of the padding packet.
   */
  uint16_t size;
} MaybenotAction_InjectPadding_Body;

typedef struct MaybenotAction_BlockOutgoing_Body {
  /**
   * The machine that generated the action.
   */
  uintptr_t machine;
  /**
   * The time to wait before blocking.
   */
  struct MaybenotDuration timeout;
  bool replace;
  bool bypass;
  /**
   * How long to block.
   */
  struct MaybenotDuration duration;
} MaybenotAction_BlockOutgoing_Body;

typedef struct MaybenotAction {
  MaybenotAction_Tag tag;
  union {
    MaybenotAction_Cancel_Body cancel;
    MaybenotAction_InjectPadding_Body inject_padding;
    MaybenotAction_BlockOutgoing_Body block_outgoing;
  };
} MaybenotAction;

/**
 * Get the version of maybenot-ffi as a null terminated UTF-8-string.
 *
 * Example: `maybenot-ffi/1.0.1`
 */
const char *maybenot_version(void);

/**
 * Start a new [`MaybenotFramework`] instance.
 *
 * # Safety
 * - `machines_str` must be a null-terminated UTF-8 string, containing LF-separated machines.
 * - `out` must be a valid pointer to some valid and aligned pointer-sized memory.
 * - The pointer written to `out` is NOT safe to be used concurrently.
 */
MaybenotResult maybenot_start(const char *machines_str,
                              double max_padding_bytes,
                              double max_blocking_bytes,
                              uint16_t mtu,
                              struct MaybenotFramework **out);

/**
 * Get the number of machines running in the [`MaybenotFramework`] instance.
 *
 * # Safety
 * - `this` must have been created by [`maybenot_start`].
 */
uintptr_t maybenot_num_machines(struct MaybenotFramework *this_);

/**
 * Stop a running [`MaybenotFramework`] instance. This will free the maybenot pointer.
 *
 * # Safety
 * - `this` MUST have been created by [`maybenot_start`].
 * - `this` MUST NOT be used after it has been passed to [`maybenot_stop`].
 */
void maybenot_stop(struct MaybenotFramework *this_);

/**
 * Feed events to the [`MaybenotFramework`] instance.
 *
 * This may generate [super::MaybenotAction]s that will be written to `actions_out`.
 * The number of actions will be written to `num_actions_out`.
 *
 * # Safety
 * - `this` MUST have been created by [`maybenot_start`].
 * - `events` MUST be a valid pointer to an array of size `num_events`.
 * - `actions_out` MUST have capacity for [`maybenot_num_machines`] items of size
 *   `sizeof(MaybenotAction)` bytes.
 * - `num_actions_out` MUST be a valid pointer where a 64bit int can be written.
 */
MaybenotResult maybenot_on_events(struct MaybenotFramework *this_,
                                  const struct MaybenotEvent *events,
                                  uintptr_t num_events,
                                  struct MaybenotAction *actions_out,
                                  uintptr_t *num_actions_out);

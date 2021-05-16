#define FSEMU_INTERNAL

#include "fsemu-recordingui.h"

#include "fsemu-recording.h"
#include "fsemu-widget.h"

static struct {
    bool initialized;
    fsemu_widget_t *layer_w;
    fsemu_widget_t *frame_number_w;

    fsemu_widget_t *playing_w;
    fsemu_widget_t *recording_w;
    fsemu_widget_t *stopped_w;

    fsemu_widget_t *sync_w;
    fsemu_widget_t *desync_w;

    fsemu_widget_t *writable_w;
    fsemu_widget_t *notwritable_w;

} fsemu_recordingui;

// ---------------------------------------------------------------------------
// Module init
// ---------------------------------------------------------------------------

void fsemu_recordingui_init(void)
{
    if (fsemu_recordingui.initialized) {
        return;
    }
    fsemu_recordingui.initialized = true;
    fsemu_recording_log("Initializing recording UI module\n");

    fsemu_widget_t *w;

    w = fsemu_recordingui.layer_w =
        fsemu_widget_new_with_name("fsemu_recordingui_layer");
    fsemu_widget_set_visible(w, false);
    fsemu_widget_set_z_index(w, 20000);  // FIXME
    fsemu_gui_add_item(w);

    w = fsemu_recordingui.frame_number_w =
        fsemu_widget_new_with_name("frame_number_w");
    fsemu_widget_set_font_size(w, 26);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0xAAAAAA));
    fsemu_widget_set_text(w, "0000");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 300);
    fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);

    w = fsemu_recordingui.writable_w =
        fsemu_widget_new_with_name("writable_w");
    fsemu_widget_set_font_size(w, 24);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0xCC9900));
    fsemu_widget_set_text(w, "WRITABLE");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 380);
    // fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);

    w = fsemu_recordingui.notwritable_w = fsemu_widget_new_with_name("notwritable_w");
    fsemu_widget_set_font_size(w, 24);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0x008800));
    fsemu_widget_set_text(w, "READONLY");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 380);
    // fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);

    w = fsemu_recordingui.recording_w =
        fsemu_widget_new_with_name("recording_w");
    fsemu_widget_set_font_size(w, 32);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0xFF4400));
    fsemu_widget_set_text(w, "RECORD");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 600);
    // fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);

    w = fsemu_recordingui.playing_w = fsemu_widget_new_with_name("playing_w");
    fsemu_widget_set_font_size(w, 32);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0x00FF00));
    fsemu_widget_set_text(w, "PLAY");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 600);
    fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);

    w = fsemu_recordingui.stopped_w = fsemu_widget_new_with_name("end_w");
    fsemu_widget_set_font_size(w, 32);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0xCC9900));
    fsemu_widget_set_text(w, "END");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 600);
    fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);


    w = fsemu_recordingui.sync_w = fsemu_widget_new_with_name("sync_w");
    fsemu_widget_set_font_size(w, 32);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0x008800));
    fsemu_widget_set_text(w, "SYNC");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 700);
    // fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);

    w = fsemu_recordingui.desync_w = fsemu_widget_new_with_name("desync_w");
    fsemu_widget_set_font_size(w, 32);
    fsemu_widget_set_text_color(w, FSEMU_COLOR_RGB(0xFF0000));
    fsemu_widget_set_text(w, "DESYNC");
    fsemu_widget_set_left(w, 60);
    fsemu_widget_set_top(w, 700);
    // fsemu_widget_set_visible(w, true);
    fsemu_widget_add_child(fsemu_recordingui.layer_w, w);

}

// ---------------------------------------------------------------------------
// Module update
// ---------------------------------------------------------------------------

void fsemu_recordingui_update(void)
{
    if (fsemu_recordingui.initialized == false) {
        return;
    }
    bool enabled = fsemu_recording_enabled();
    fsemu_widget_set_visible(fsemu_recordingui.layer_w, enabled);

    bool playing = fsemu_recording_is_playing();
    fsemu_widget_set_visible(fsemu_recordingui.playing_w, playing);
    bool recording = fsemu_recording_is_recording();
    fsemu_widget_set_visible(fsemu_recordingui.recording_w, recording);
    bool stopped = fsemu_recording_is_stopped();
    fsemu_widget_set_visible(fsemu_recordingui.stopped_w, stopped);

    bool desync = fsemu_recording_is_desync();
    fsemu_widget_set_visible(fsemu_recordingui.sync_w, playing && !desync);
    fsemu_widget_set_visible(fsemu_recordingui.desync_w, playing && desync);

    bool writable = fsemu_recording_is_writable();
    fsemu_widget_set_visible(fsemu_recordingui.writable_w, writable);
    fsemu_widget_set_visible(fsemu_recordingui.notwritable_w, !writable);

    char buffer[64];
    sprintf(buffer, "%d", fsemu_recording_frame_number());
    // fsemu_recording_frame_number()
    fsemu_widget_set_text(fsemu_recordingui.frame_number_w, buffer);
}

// ---------------------------------------------------------------------------
// Module shutdown
// ---------------------------------------------------------------------------

void fsemu_recordingui_shutdown(void)
{
}
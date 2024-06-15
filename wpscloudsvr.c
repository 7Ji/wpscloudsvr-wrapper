#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

#include <gtk/gtk.h>
#include <glib.h>
#include <libappindicator/app-indicator.h>

#define REALPATH "/opt/kingsoft/wps-office/office6/wpscloudsvr.real"

#define println(format, arg...) printf(format"\n", ##arg)
#define println_with_errno(format, arg...) \
    printf(format", errno: %d (%s)\n", ##arg, errno, strerror(errno))

pid_t wpscloudsvr = 0;
int main_r = 0;

int kill_wpscloudsvr() {
    int status;
    pid_t waited;
    const int signals[] = {SIGINT, SIGTERM, SIGKILL};

    if (wpscloudsvr == 0) {
        println("WPS cloud srv was not started, this is impossible");
        return -1;
    }
    for (unsigned short i = 0; i < 4; ++i) {
        // Non-blocking wait first in case it already exited
        for (unsigned short j = 0; j < 3; ++j) { // Non-blocking wait, 3 tries
            waited = waitpid(wpscloudsvr, &status, WNOHANG);
            if (waited > 0) {
                if (waited != wpscloudsvr) {
                    println("Waited WPS cloud srv pid (%d) != expected (%d)", waited, wpscloudsvr);
                    return -1;
                }
                if (status) {
                    println("WPS cloud srv bad return: %d", status);
                    return -1;
                }
                println("WPS cloud srv quit cleanly");
                return 0;
            } else if (waited < 0) {
                println_with_errno("Failed to wait for WPS cloud srv");
                return -1;
            } else {
                usleep(333333);
            }
        }
        if (i == 3) {
            break;
        }
        if (kill(wpscloudsvr, signals[i]) < 0) {
            println_with_errno("Failed to kill process %d with signal %d", wpscloudsvr, signals[i]);
            return -1;
        }
    }
    println("Failed to kill WPS cloudsvr in any means");
    return -1;
}

static void kill_wpscloudsvr_callback(GtkAction *action) {
    (void) action;
    gtk_main_quit();
    if (kill_wpscloudsvr()) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}

void killer(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *indicator_menu;
    GtkActionGroup *action_group;
    GtkUIManager *uim;
    AppIndicator *indicator;
    GError *error = NULL;

    GtkActionEntry const entries[] = {
    { "Quit",     "application-exit", "_退出", "<control>Q",
        "Exit the application", G_CALLBACK (kill_wpscloudsvr_callback) },
    };
    guint const n_entries = G_N_ELEMENTS (entries);
    gchar const *const restrict ui_info =
    "<ui>"
    "  <popup name='IndicatorPopup'>"
    "    <menuitem action='Quit' />"
    "  </popup>"
    "</ui>";

    gtk_init (&argc, &argv);

    /* main window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    action_group = gtk_action_group_new ("AppActions");
    gtk_action_group_add_actions (action_group,
                                    entries, n_entries,
                                    window);

    uim = gtk_ui_manager_new ();
    gtk_ui_manager_insert_action_group (uim, action_group, 0);
    if (!gtk_ui_manager_add_ui_from_string (uim, ui_info, -1, &error))
        {
        g_message ("Failed to build menus: %s\n", error->message);
        g_error_free (error);
        error = NULL;
        }

    /* Indicator */
    indicator = app_indicator_new ("example-simple-client",
                                    "indicator-messages",
                                    APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

    indicator_menu = gtk_ui_manager_get_widget (uim, "/ui/IndicatorPopup");

    app_indicator_set_status (indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon (indicator, "indicator-messages-new");
    app_indicator_set_title(indicator, "WPS 云同步服务");
    app_indicator_set_icon(indicator, "cloud-upload-symbolic");
    app_indicator_set_menu (indicator, GTK_MENU (indicator_menu));
    gtk_main ();
}

int main (int argc, char **argv) {
    bool is_daemon = false;
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "/start_from=qingipc", 19) == 0) {
            is_daemon = true;
            break;
        }
    }
    if (is_daemon) {
        wpscloudsvr = fork();
        if (wpscloudsvr > 0) { // parent
            killer(argc, argv);
            return main_r;
        } else if (wpscloudsvr == 0) { // child
            execv(REALPATH, argv);
            println_with_errno("Failed to exec wps cloudsrv (daemon)");
        } else {
            println_with_errno("Failed to fork");
        }
    } else {
        execv(REALPATH, argv);
        println_with_errno("Failed to exec wps cloudsrv (non-daemon)");
    }
    return -1;
    
}
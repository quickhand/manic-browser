/***************************************************************************
 *   Copyright (C) 2009 by Marc Lajoie                                     *
 *   quickhand@openinkpot.org                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <string.h>

#include <Evas.h>
#include <Ecore.h>
#include <Edje.h>
#include <Elementary.h>
#include <EWebKit.h>
#include <Etk.h>
#define REL_THEME "themes/themes_oitheme.edj"

using namespace std;


Evas *evas;
Evas_Object *win, *bg, *pagelayout,*webkitobj,*addressentry_embed;
Etk_Widget *addressentry,*embed;
/* return value of app - keep it here so we can modify it */
static int retval = -1;
static unsigned char mouseclicked=0;
static unsigned char cancelmouseup=0;
int winwidth=600;
int winheight=800;

char *get_theme_file()
{
 	//char *cwd = get_current_dir_name();
	char *rel_theme;
	asprintf(&rel_theme, "%s/%s", "/usr/share/manic-browser", REL_THEME);
    //asprintf(&rel_theme, "%s/%s",cwd, REL_THEME);
	//free(cwd);
	return rel_theme;
}

const char *get_cur_url()
{
    Evas_Object *webpage=ewk_webview_object_webpage_get(webkitobj);
    if(webpage==NULL)
        return NULL;
    
    Evas_Object *frame=ewk_webpage_object_mainframe_get(webpage);
    if(frame==NULL)
    {
        return NULL;
    }
    
    return ewk_webframe_object_url_get(frame);
    
    
    
}


/* if someone presses the close button on our window - exit nicely */
static void
win_del(void *data, Evas_Object *obj, void *event_info)
{
    retval = -1;
    /* cleanly exit */
    elm_exit();
}

static void addressentry_activated(void *data)
{
    const char *entrytxt=etk_entry_text_get(ETK_ENTRY(addressentry));
    ewk_webview_object_load_url(webkitobj,entrytxt);
    
}
static void back_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
    if(ewk_webview_object_navigation_can_go_back(webkitobj))
        ewk_webview_object_navigation_back(webkitobj);
}
static void forward_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
    if(ewk_webview_object_navigation_can_go_forward(webkitobj))
        ewk_webview_object_navigation_forward(webkitobj);
}
static void reload_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
    ewk_webview_object_navigation_reload(webkitobj);
}
static void stop_bt_clicked(void *data, Evas_Object *obj, void *event_info)
{
    ewk_webview_object_navigation_stop(webkitobj);
}


static void page_loading(void *data, Evas_Object *obj, void *event_info)
{
    

}

static void page_url_changed(void *data, Evas_Object *obj, void *event_info)
{
    etk_entry_text_set(ETK_ENTRY(addressentry),get_cur_url());
}

static void  webkitobj_mouse_down(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
    evas_object_focus_set(webkitobj,TRUE);
}

EAPI int
elm_main(int argc, char **argv)
{
    etk_init(argc,argv);
    Evas_Object *bx, *bx2, *bt,*icon;
    
    /* new window - do the usual and give it a name, title and delete handler */
    win = elm_win_add(NULL, "dialog", ELM_WIN_BASIC);
    elm_win_title_set(win, "Dialog");
    evas_object_smart_callback_add(win, "delete-request", win_del, NULL);
    evas_object_resize(win,winwidth,winheight);
    evas_object_size_hint_max_set(win,winwidth,winheight);
    /*get evas*/
    evas=evas_object_evas_get(win);
    ewk_init(evas);
    char *themefile;
    themefile=get_theme_file();
    ewk_theme_set(themefile);
    /* add a standard bg */
    bg = elm_bg_add(win);
    /* not not allow bg to expand. let's limit dialog size to contents */
    evas_object_size_hint_weight_set(bg, 1.0, 1.0);
    elm_win_resize_object_add(win, bg);
    evas_object_show(bg);
    
    /* add a box object - default is vertical. a box holds children in a row,
    * either horizontally or vertically. nothing more. */
    bx = elm_box_add(win);
    evas_object_size_hint_weight_set(bx, 1.0, 1.0);
    evas_object_size_hint_align_set(bt, -1.0, -1.0);
    elm_win_resize_object_add(win, bx);
    evas_object_show(bx);
    
    /* add controls box */
    bx2 = elm_box_add(win);
    elm_box_horizontal_set(bx2,TRUE);
    evas_object_size_hint_weight_set(bx2, 1.0, 0.0);
    elm_box_homogenous_set(bx2,FALSE);
    elm_box_pack_end(bx,bx2);
    evas_object_show(bx2);
    
    /* add buttons to control box */
    
    icon=elm_icon_add(win);
    elm_icon_standard_set(icon,"arrow_left");
    evas_object_show(icon);
    
    bt = elm_button_add(win);
    elm_button_icon_set(bt,icon);
    evas_object_size_hint_weight_set(bt,0.0,0.0);
    //evas_object_size_hint_align_set(bt, -1.0, -1.0);
    elm_box_pack_end(bx2, bt);
    evas_object_smart_callback_add(bt,"clicked",back_bt_clicked, NULL);
    evas_object_show(bt);
    
    icon=elm_icon_add(win);
    //elm_icon_file_set(icon,themefile,"elm/icon/arrow_left/default");
    elm_icon_standard_set(icon,"arrow_right");
    evas_object_show(icon);
    
    bt = elm_button_add(win);
    elm_button_icon_set(bt,icon);
    evas_object_size_hint_weight_set(bt,0.0,0.0);
    //evas_object_size_hint_align_set(bt, -1.0, -1.0);
    elm_box_pack_end(bx2, bt);
    evas_object_smart_callback_add(bt,"clicked",forward_bt_clicked, NULL);
    evas_object_show(bt);
    
    icon=elm_icon_add(win);
    //elm_icon_file_set(icon,themefile,"elm/icon/arrow_left/default");
    elm_icon_standard_set(icon,"clock");
    evas_object_show(icon);
    
    bt = elm_button_add(win);
    elm_button_icon_set(bt,icon);
    evas_object_size_hint_weight_set(bt, 0.0, 0.0);
    //evas_object_size_hint_align_set(bt, -1.0, -1.0);
    elm_box_pack_end(bx2, bt);
    evas_object_smart_callback_add(bt,"clicked",reload_bt_clicked, NULL);
    evas_object_show(bt);
    
    icon=elm_icon_add(win);
    //elm_icon_file_set(icon,themefile,"elm/icon/arrow_left/default");
    elm_icon_standard_set(icon,"close");
    evas_object_show(icon);
    
    bt = elm_button_add(win);
    elm_button_icon_set(bt,icon);
    evas_object_size_hint_weight_set(bt, 0.0, 0.0);
    //evas_object_size_hint_align_set(bt, -1.0, -1.0);
    elm_box_pack_end(bx2, bt);
    evas_object_smart_callback_add(bt,"clicked",stop_bt_clicked, NULL);
    evas_object_show(bt);
    
    //Set up address bar
    Evas_Object *addlayout=elm_layout_add(win);
    elm_layout_file_set(addlayout,themefile,"addlayout");
    evas_object_size_hint_weight_set(addlayout, 1.0, 1.0);
    elm_box_pack_end(bx2,addlayout);
    evas_object_show(addlayout);
    
    
    
    //Create the embed widget
    
    embed = etk_embed_new(evas);
    
    //Pack an entry widget into the embed widget
    addressentry = etk_entry_new();
    etk_entry_text_set(ETK_ENTRY(addressentry),"http://openinkpot.org");
    etk_container_add(ETK_CONTAINER(embed),addressentry);
    etk_signal_connect_by_code(ETK_ENTRY_TEXT_ACTIVATED_SIGNAL,ETK_OBJECT(addressentry),ETK_CALLBACK(addressentry_activated),NULL);
    etk_widget_show_all(embed);
    
    
    addressentry_embed = etk_embed_object_get(ETK_EMBED(embed));
    elm_layout_content_set(addlayout,"addlayout/swallow",addressentry_embed);
    evas_object_show(addressentry_embed);
    
    
    /* set up layout */
    
    pagelayout=elm_layout_add(win);
    elm_layout_file_set(pagelayout,themefile,"pagelayout");
    evas_object_size_hint_weight_set(pagelayout, 1.0, 1.0);
    evas_object_size_hint_align_set(pagelayout, -1.0, -1.0);
    free(themefile);
    elm_box_pack_end(bx,pagelayout);
    
    evas_object_show(pagelayout);
    
    /* add webkit component to layout */
    
    webkitobj = ewk_webview_object_add(evas);
    
    elm_layout_content_set(pagelayout,"pagelayout/swallow",webkitobj);
    
    ewk_webview_object_load_url(webkitobj, "http://www.openinkpot.org");
    
    ewk_callback_load_progress_add(webkitobj,page_loading,NULL);
    ewk_callback_url_changed_add(webkitobj,page_url_changed,NULL);
    evas_object_event_callback_add(webkitobj,EVAS_CALLBACK_MOUSE_DOWN,webkitobj_mouse_down,NULL);
    evas_object_show(webkitobj);
    
    
    evas_object_show(win);
    
    
    
    elm_run();
    elm_shutdown();
    ewk_shutdown();
    etk_shutdown();
    return 0;
    }
ELM_MAIN()

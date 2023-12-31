/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * gdl-dock-item-button-image.c
 *
 * Author: Joel Holdsworth <joel@airwebreathe.org.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "gdl-dock-item-button-image.h"

#include <math.h>

/**
 * SECTION:gdl-dock-item-button-image
 * @title: GdlDockItemButtonImage
 * @short_description: A widget displaying an image used in dock grip
 * @stability: Private
 * @see_also: GtlDockItemGrip
 *
 * The #GdlDockItemButtonImage displays a small image which could be a cross
 * (close) or a triangle (iconify). These images are used for buttons in the
 * #GdlDockItemGrip object.
 */


#define ICON_SIZE 16

G_DEFINE_TYPE (GdlDockItemButtonImage,
               gdl_dock_item_button_image,
               GTK_TYPE_WIDGET);

static gboolean
gdl_dock_item_button_image_draw (GtkWidget      *widget,
                                 cairo_t *cr)
{
    GdlDockItemButtonImage *button_image;
    GtkStyleContext *context;
    GdkRGBA color;
    int offset_x, offset_y;

    g_return_val_if_fail (widget != NULL, 0);
    button_image = GDL_DOCK_ITEM_BUTTON_IMAGE (widget);

    /* Center the icon inside the widget */
    offset_x = (gtk_widget_get_allocated_width (widget) - ICON_SIZE) / 2;
    offset_y = (gtk_widget_get_allocated_height (widget) - ICON_SIZE) / 2;
    cairo_translate (cr, offset_x, offset_y);

    /* Set up the pen */
    cairo_set_line_width(cr, 1.0);

    context = gtk_widget_get_style_context (widget);
    gtk_style_context_get_color (context, GTK_STATE_FLAG_NORMAL, &color);
    color.alpha = 0.55;
    gdk_cairo_set_source_rgba(cr, &color);

    /* Draw the icon border */
    cairo_move_to (cr, 10.5, 2.5);
    cairo_arc (cr, 10.5, 4.5, 2, -0.5 * M_PI, 0);
    cairo_line_to (cr, 12.5, 10.5);
    cairo_arc (cr, 10.5, 10.5, 2, 0, 0.5 * M_PI);
    cairo_line_to (cr, 4.5, 12.5);
    cairo_arc (cr, 4.5, 10.5, 2, 0.5 * M_PI, M_PI);
    cairo_line_to (cr, 2.5, 4.5);
    cairo_arc (cr, 4.5, 4.5, 2, M_PI, 1.5 * M_PI);
    cairo_close_path (cr);

    cairo_stroke (cr);

    /* Draw the icon */
    cairo_new_path (cr);

    switch(button_image->image_type) {
    case GDL_DOCK_ITEM_BUTTON_IMAGE_CLOSE:
        cairo_move_to (cr, 4.0, 5.5);
        cairo_line_to (cr, 4.0, 5.5);
        cairo_line_to (cr, 6.0, 7.5);
        cairo_line_to (cr, 4.0, 9.5);
        cairo_line_to (cr, 5.5, 11.0);
        cairo_line_to (cr, 7.5, 9.0);
        cairo_line_to (cr, 9.5, 11.0);
        cairo_line_to (cr, 11.0, 9.5);
        cairo_line_to (cr, 9.0, 7.5);
        cairo_line_to (cr, 11.0, 5.5);
        cairo_line_to (cr, 9.5, 4.0);
        cairo_line_to (cr, 7.5, 6.0);
        cairo_line_to (cr, 5.5, 4.0);
        cairo_close_path (cr);
        break;

    case GDL_DOCK_ITEM_BUTTON_IMAGE_ICONIFY:
        if (gtk_widget_get_direction (widget) != GTK_TEXT_DIR_RTL) {
            cairo_move_to (cr, 4.5, 7.5);
            cairo_line_to (cr, 10.0, 4.75);
            cairo_line_to (cr, 10.0, 10.25);
            cairo_close_path (cr);
        } else {
            cairo_move_to (cr, 10.5, 7.5);
            cairo_line_to (cr, 5, 4.75);
            cairo_line_to (cr, 5, 10.25);
            cairo_close_path (cr);
        }
        break;

    default:
        break;
    }

    cairo_fill (cr);

    return FALSE;
}

static void
gdl_dock_item_button_image_init (
    GdlDockItemButtonImage *button_image)
{
    gtk_widget_set_has_window (GTK_WIDGET (button_image), FALSE);
}

static void
gdl_dock_item_button_image_get_preferred_width (GtkWidget *widget,
                                                gint      *minimum,
                                                gint      *natural)
{
   *minimum = *natural = ICON_SIZE;
}

static void
gdl_dock_item_button_image_get_preferred_height (GtkWidget *widget,
                                                 gint      *minimum,
                                                 gint      *natural)
{
   *minimum = *natural = ICON_SIZE;
}

static void
gdl_dock_item_button_image_class_init (
    GdlDockItemButtonImageClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    widget_class->draw =
        gdl_dock_item_button_image_draw;
    widget_class->get_preferred_width =
        gdl_dock_item_button_image_get_preferred_width;
    widget_class->get_preferred_height =
        gdl_dock_item_button_image_get_preferred_height;
}

/* ----- Public interface ----- */

/**
 * gdl_dock_item_button_image_new:
 * @image_type: Specifies what type of image the widget should display
 *
 * Creates a new GDL dock button image object.
 *
 * Returns: The newly created dock item button image widget
 */
GtkWidget*
gdl_dock_item_button_image_new (GdlDockItemButtonImageType image_type)
{
    GdlDockItemButtonImage *button_image = g_object_new (
        GDL_TYPE_DOCK_ITEM_BUTTON_IMAGE, NULL);
    button_image->image_type = image_type;

    return GTK_WIDGET (button_image);
}

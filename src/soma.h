#ifndef SOMA_H
#define SOMA_H

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <time.h>

#define VARSHAM_SIZE 60
#define MASAM_SIZE 12
#define RUTHUVU_SIZE 6
#define THITHI 30

// Declarations for arrays
extern const char *varsham[VARSHAM_SIZE];
extern const char *masam[MASAM_SIZE];
extern const char *ruthuvu[RUTHUVU_SIZE];
extern const char *thithulu[THITHI];
extern const gchar *image_files[THITHI];

// External variables
extern char prasthutha_varsham[20];
extern char enaka_varsham[20];
extern char mundu_varsham[20];
extern int prasthutha_varsha_soochika;

extern char prasthutha_masam[16];
extern char enaka_masam[16];
extern char mundu_masam[16];
extern int prasthutha_masa_soochika;

extern char prasthutha_ruthuvu[16];
extern char enaka_ruthuvu[16];
extern char mundu_ruthuvu[16];
extern int prasthutha_ruthu_soochika;

extern GdkPixbuf *pixbuf[THITHI];

// Function prototypes
void get_prasthutha_varsham(void);
void move_year_up(void);
void move_year_down(void);

void get_prasthutha_masam(void);
void move_masa_up(void);
void move_masa_down(void);

void get_prasthutha_ruthuvu(void);
void move_ruthu_up(void);
void move_ruthu_down(void);

gboolean load_moon_phase_images(void);
void samvathsaralu();
void masalu();
void ruthuvulu();

#endif
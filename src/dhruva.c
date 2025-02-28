#include "soma.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdio.h>

// samvathsaralu
const char *varsham[VARSHAM_SIZE] = {
    "Prabhava", "Vibhava", "Shukla", "Pramodoota", "Prajotpati",
    "Angeerasa", "Sreemukha", "Bhaava", "Yuva", "Dhaatu",
    "Eeswara", "Bahudhanya", "Pramadi", "Vikrama", "Vrusha",
    "Chitrabhanu", "Svabhanu", "Tarana", "Pardhiva", "Vyaya",
    "Sarvajittu", "Sarvadhari", "Virodhi", "Vikruti", "Khara",
    "Nandana", "Vijaya", "Jaya", "Manmadha", "Durmukhi",
    "Hevilambi", "Vilambi", "Vikari", "Sharvari", "Plava",
    "Shubhakrutu", "Shobhakrutu", "Krodhi", "Vishvavasu", "Parabhava",
    "Plavanga", "Keelaka", "Soumya", "Sadharana", "Virodhikrutu",
    "Pareedhani", "Pramadeecha", "Ananda", "Rakshasa", "Nala",
    "Pingala", "KaLayukti", "Siddhaardhi", "Raudri", "Durmati",
    "Dundubhi", "Rudhirodgari", "Raktakshi", "Krodhana", "Akshaya"
};

// masalu
const char *masam[MASAM_SIZE] = {
    "chaithram", "vaishakham", "jyeshtam", "ashadam",
    "shravanam", "bhadrapadam", "ashvayujam", "karthikam",
    "margashiram", "pushyam", "magham", "phalgunam"
};

// ruthuvuly
const char *ruthuvu[RUTHUVU_SIZE] = {
    "vasantham", "greeshmam", "varsham",
    "sharad", "hemantham", "shishira"
};

// thithulu
const char *thithulu[THITHI] = {
    "amasa", "shukla padyami", "shukla vidiya", "shukla thadiya",
    "shukla chavithi", "shukla panchami", "shukla shashti", "shukla sapthami",
    "shukla ashtami", "shukla navami", "shukla dashami", "shukla ekadashi",
    "shukla dwadashi", "shukla thrayodashi", "shukla chathurdashi", "punnami",
    "krushna padyami", "krushna vidiya", "krushna thadiya", "krushna chavithi",
    "krushna panchami", "krushna shashti", "krushna sapthami", "krushna ashtami",
    "krushna navami", "krushna dashami", "krushna ekadashi", "krushna dwadashi",
    "krushna thrayodashi", "krushna chathurdashi"
};

// thithulu
const gchar *image_files[THITHI] = {
    "resource/images/amasa.png", "resource/images/shukla_padyami.png", 
    "resource/images/shukla_vidiya.png", "resource/images/shukla_thadiya.png", 
    "resource/images/shukla_chavithi.png", "resource/images/shukla_panchami.png", 
    "resource/images/shukla_shashti.png", "resource/images/shukla_sapthami.png", 
    "resource/images/shukla_ashtami.png", "resource/images/shukla_navami.png", 
    "resource/images/shukla_dashami.png", "resource/images/shukla_ekadashi.png", 
    "resource/images/shukla_dwadashi.png", "resource/images/shukla_thrayodashi.png", 
    "resource/images/shukla_chathurdashi.png", "resource/images/punnami.png", 
    "resource/images/krushna_padyami.png", "resource/images/krushna_vidiya.png", 
    "resource/images/krushna_thadiya.png", "resource/images/krushna_chavithi.png", 
    "resource/images/krushna_panchami.png", "resource/images/krushna_shashti.png", 
    "resource/images/krushna_sapthami.png", "resource/images/krushna_ashtami.png", 
    "resource/images/krushna_navami.png", "resource/images/krushna_dashami.png", 
    "resource/images/krushna_ekadashi.png", "resource/images/krushna_dwadashi.png", 
    "resource/images/krushna_thrayodashi.png", "resource/images/krushna_chathurdashi.png"
};

// Variable definitions
char prasthutha_varsham[20];
char enaka_varsham[20];
char mundu_varsham[20];
int prasthutha_varsha_soochika = 0;

char prasthutha_masam[16];
char enaka_masam[16];
char mundu_masam[16];
int prasthutha_masa_soochika = 0;

char prasthutha_ruthuvu[16];
char enaka_ruthuvu[16];
char mundu_ruthuvu[16];
int prasthutha_ruthu_soochika = 0;

GdkPixbuf *pixbuf[THITHI];

// Function implementations
void get_prasthutha_varsham(void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900;

    int base_year = 1867;
    int cycle_position = (year - base_year) % 60;
    if (cycle_position < 0) cycle_position += 60;

    prasthutha_varsha_soochika = cycle_position;
    snprintf(enaka_varsham, sizeof(enaka_varsham), "%s", varsham[(cycle_position - 1 + VARSHAM_SIZE) % VARSHAM_SIZE]);
    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[cycle_position]);
    snprintf(mundu_varsham, sizeof(mundu_varsham), "%s", varsham[(cycle_position + 1) % VARSHAM_SIZE]);
}

void move_year_up(void) {
    prasthutha_varsha_soochika = (prasthutha_varsha_soochika + 1) % VARSHAM_SIZE;
    snprintf(enaka_varsham, sizeof(enaka_varsham), "%s", varsham[(prasthutha_varsha_soochika - 1 + VARSHAM_SIZE) % VARSHAM_SIZE]);
    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[prasthutha_varsha_soochika]);
    snprintf(mundu_varsham, sizeof(mundu_varsham), "%s", varsham[(prasthutha_varsha_soochika + 1) % VARSHAM_SIZE]);
}

void move_year_down(void) {
    prasthutha_varsha_soochika = (prasthutha_varsha_soochika - 1 + VARSHAM_SIZE) % VARSHAM_SIZE;
    snprintf(enaka_varsham, sizeof(enaka_varsham), "%s", varsham[(prasthutha_varsha_soochika - 1 + VARSHAM_SIZE) % VARSHAM_SIZE]);
    snprintf(prasthutha_varsham, sizeof(prasthutha_varsham), "%s", varsham[prasthutha_varsha_soochika]);
    snprintf(mundu_varsham, sizeof(mundu_varsham), "%s", varsham[(prasthutha_varsha_soochika + 1) % VARSHAM_SIZE]);
}

void get_prasthutha_masam(void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int month = tm.tm_mon;

    // int base_masam = 0;
    int cycle_position = (month + 9) % 12;
    if (cycle_position < 0) cycle_position += 12;

    prasthutha_masa_soochika = cycle_position;
    snprintf(enaka_masam, sizeof(enaka_masam), "%s", masam[(cycle_position - 1 + MASAM_SIZE) % MASAM_SIZE]);
    snprintf(prasthutha_masam, sizeof(prasthutha_masam), "%s", masam[cycle_position]);
    snprintf(mundu_masam, sizeof(mundu_masam), "%s", masam[(cycle_position + 1) % MASAM_SIZE]);
}

void move_masa_up(void) {
    prasthutha_masa_soochika = (prasthutha_masa_soochika + 1) % MASAM_SIZE;
    snprintf(enaka_masam, sizeof(enaka_masam), "%s", masam[(prasthutha_masa_soochika - 1 + MASAM_SIZE) % MASAM_SIZE]);
    snprintf(prasthutha_masam, sizeof(prasthutha_masam), "%s", masam[prasthutha_masa_soochika]);
    snprintf(mundu_masam, sizeof(mundu_masam), "%s", masam[(prasthutha_masa_soochika + 1) % MASAM_SIZE]);
}

void move_masa_down(void) {
    prasthutha_masa_soochika = (prasthutha_masa_soochika - 1 + MASAM_SIZE) % MASAM_SIZE;
    snprintf(enaka_masam, sizeof(enaka_masam), "%s", masam[(prasthutha_masa_soochika - 1 + MASAM_SIZE) % MASAM_SIZE]);
    snprintf(prasthutha_masam, sizeof(prasthutha_masam), "%s", masam[prasthutha_masa_soochika]);
    snprintf(mundu_masam, sizeof(mundu_masam), "%s", masam[(prasthutha_masa_soochika + 1) % MASAM_SIZE]);
}

void get_prasthutha_ruthuvu(void) {
    prasthutha_ruthu_soochika = prasthutha_masa_soochika / 2;
    snprintf(enaka_ruthuvu, sizeof(enaka_ruthuvu), "%s", ruthuvu[(prasthutha_ruthu_soochika - 1 + RUTHUVU_SIZE) % RUTHUVU_SIZE]);
    snprintf(prasthutha_ruthuvu, sizeof(prasthutha_ruthuvu), "%s", ruthuvu[prasthutha_ruthu_soochika]);
    snprintf(mundu_ruthuvu, sizeof(mundu_ruthuvu), "%s", ruthuvu[(prasthutha_ruthu_soochika + 1) % RUTHUVU_SIZE]);
}

void move_ruthu_up(void) {
    prasthutha_ruthu_soochika = (prasthutha_ruthu_soochika + 1) % RUTHUVU_SIZE;
    snprintf(enaka_ruthuvu, sizeof(enaka_ruthuvu), "%s", ruthuvu[(prasthutha_ruthu_soochika - 1 + RUTHUVU_SIZE) % RUTHUVU_SIZE]);
    snprintf(prasthutha_ruthuvu, sizeof(prasthutha_ruthuvu), "%s", ruthuvu[prasthutha_ruthu_soochika]);
    snprintf(mundu_ruthuvu, sizeof(mundu_ruthuvu), "%s", ruthuvu[(prasthutha_ruthu_soochika + 1) % RUTHUVU_SIZE]);
}

void move_ruthu_down(void) {
    prasthutha_ruthu_soochika = (prasthutha_ruthu_soochika - 1 + RUTHUVU_SIZE) % RUTHUVU_SIZE;
    snprintf(enaka_ruthuvu, sizeof(enaka_ruthuvu), "%s", ruthuvu[(prasthutha_ruthu_soochika - 1 + RUTHUVU_SIZE) % RUTHUVU_SIZE]);
    snprintf(prasthutha_ruthuvu, sizeof(prasthutha_ruthuvu), "%s", ruthuvu[prasthutha_ruthu_soochika]);
    snprintf(mundu_ruthuvu, sizeof(mundu_ruthuvu), "%s", ruthuvu[(prasthutha_ruthu_soochika + 1) % RUTHUVU_SIZE]);
}

gboolean load_moon_phase_images(void) {    
    for (int i = 0; i < THITHI; i++) {
        pixbuf[i] = gdk_pixbuf_new_from_file(image_files[i], NULL);
        if (pixbuf[i] == NULL) {
            g_printerr("Failed to load image: %s\n", image_files[i]);
            return FALSE;
        }
    }
    return TRUE;
}
